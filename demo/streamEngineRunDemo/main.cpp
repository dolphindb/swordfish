#include "EmbeddedOLTP.h"
#include <iostream>
#include <string>

using std::shared_ptr;
using std::make_shared;

using oltp::DB;
using oltp::Connection;

int main() {
    DolphinDBLib::initializeRuntime();
    oltp::DBOption option;
    shared_ptr<DB> db = make_shared<DB>("test_db", option);
    Connection conn(*db);
    
    std::cout << "Initialize calculation" << std::endl;
    
    conn.execute(R"(
        // 算子
        def sum_diff(x, y){
            return (x-y)/(x+y)
        }

        @state
        def factor1(price) {
            a = ema(price, 20)
            b = ema(price, 40)
            c = 1000 * sum_diff(a, b)
            return  ema(c, 10) - ema(c, 20)
        }

        // 创建引擎、订阅流表
        share streamTable(1:0, `time`sym`price, [TIMESTAMP,STRING,DOUBLE]) as tickStream
        result = table(1000:0, `sym`time`factor1, [STRING,TIMESTAMP,DOUBLE])
        rse = createReactiveStateEngine(name="reactiveDemo", metrics=[<time>,<factor1(price)>], dummyTable=tickStream, outputTable=result, keyColumn="sym")
        subscribeTable(tableName=`tickStream, actionName="factors", handler=tableInsert{rse})

        def produceData(n, mutable tickStream) {
            tickStream.append!(table((now() + (1..n)) as time, take("000001.SH", n) as sym, rand(10.0, n) as price))
        }
    )");
    
    std::cout << "Start calculation" << std::endl;
    
    int ROW_NUM_LIMIT = 200;
    int chunk_size = 20;
    int start = 0;
    while(start < ROW_NUM_LIMIT) {
        conn.execute("produceData(" + std::to_string(chunk_size) + ", tickStream)");
        sleep(1);
        std::string delta_result_query = "select * from result limit "+ std::to_string(start) + ", " + std::to_string(chunk_size);
        auto delta_result = conn.execute(delta_result_query);
        std::cout << delta_result_query << std::endl;
        std::cout << delta_result->getString() << std::endl;
        start += chunk_size;
    }
    
    std::cout << "Clean environment" << std::endl;
    
    conn.execute(R"(
        // 清理环境
        unsubscribeTable(tableName=`tickStream, actionName="factors")
        dropStreamEngine("reactiveDemo")
        undef(`tickStream, SHARED)
        undef(`result`rse)
    )");
    
    DolphinDBLib::finalizeRuntime();
}