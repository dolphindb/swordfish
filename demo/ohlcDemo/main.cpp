// 导入Swordfish库
#include "Swordfish.h" 

// 导入C++标准库
#include <iostream>
#include <string>
#include <thread>

using std::shared_ptr;
using std::make_shared;

using oltp::DB;
using oltp::Connection;


int main() {
    // 初始化运行时环境
    DolphinDBLib::initializeRuntime();
    
    // 打开/创建名为test_db的数据库，并获取连接会话对象
    oltp::DBOption option;
    shared_ptr<DB> db = make_shared<DB>("test_db", option);
    Connection conn(*db);
    auto session = conn.getCurrentSession();
    
    const string inputTableName = "trade";
    const string outputTableName= "aggrStream";
    const string resultTableName = "ohlcStream";
    
    // 在当前连接运行streamFunc.dos脚本文件，定义之后会用到的函数
    bool success = session->run("../../demo/ohlcDemo/streamFunc.dos");
    if(!success) {
        throw RuntimeException(session->getLastErrorMessage());
    }
    
    // 在当前连接运行定义好的函数，创建内存表并加载数据，创建流表，创建时序聚合流数据引擎并订阅流表
    conn.execute("loadInputTable(tableName=`"+ inputTableName +")");
    conn.execute("createEngineOutputStream(tableName=`"+ outputTableName +")");
    conn.execute("createResultStream(tableName=`" + resultTableName + ")");
    conn.execute("createTsEngine(outputTableName=`" + outputTableName + ", inputTableName=`" + inputTableName + ")");
    conn.execute("subscribeStreamTable(outputTableName=`"+ resultTableName + ", inputTableName=`" + outputTableName + ")");
    
    std::cout << "Initializing..." << std::endl;
    
    // 向流引擎写数据的线程函数
    auto writeData = [ &inputTableName, &db]() {
        // 在线程上创建新的数据库连接，并运行streamFunc.dos脚本定义函数
        Connection conn1(*db);
        auto session = conn1.getCurrentSession();
        session->run("../../demo/ohlcDemo/streamFunc.dos");
        
        // 每隔一秒从内存表取1000条数据推入流引擎计算K线
        std::string code = "exec count(*) from objByName(`"+ inputTableName +")";
        int inputSize = conn1.execute(code)->getInt();
        int BLK_SIZE = 1000;
        for(int start = 0; start < inputSize; start += BLK_SIZE) {
            std::string cmd = "insertToEngine("+ std::to_string(start) + ", " + std::to_string(BLK_SIZE) +")";
            conn1.execute(cmd);
            sleep(1);
        }
    };
    
    // 读取流引擎输出的线程函数
    auto readData = [ &resultTableName, &db]() {
        // 在线程上创建新的数据库连接
        Connection conn2(*db);
        
        // 每隔两秒，从结果流表中读10条流引擎输出推送过来的数据
        int start = 0;
        int end = 2300;
        int currentLen = 0;
        while(start <= end) {
            currentLen = conn2.execute("exec count(*) from objByName(`"+ resultTableName +", true)")->getInt();
            if(currentLen >= start+10) {
                auto data = conn2.execute("select * from objByName(`"+ resultTableName +", true) limit " + std::to_string(start) + ", 10");
                std::cout << data->getString() << std::endl;
                start += 10;
            } 
            sleep(2);
        }
    };
    
    // 启动读写线程，并等待线程运行结束
    std::thread t1(writeData);
    std::thread t2(readData);
    t1.join();
    t2.join();
    
    // 清理环境，结束运行时环境
    conn.execute("cleanEnv()");
    DolphinDBLib::finalizeRuntime();
    
    return 0;
}