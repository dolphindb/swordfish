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
    auto session = conn.getCurrentSession();
    
    // run .dos on local path
    bool success = session->run("../src/streamSubDemo/subscribeTest.dos");
    if(!success) {
        throw RuntimeException(session->getLastErrorMessage());
    }
    
    std::cout << "================= create oltp table ================="<< std::endl;
    const string tableName = "result";
    auto tableNameList = conn.listAllTable();
    if(std::count(tableNameList.begin(), tableNameList.end(), tableName) != 0) {
        conn.dropTable(tableName);
    }
    std::vector<ColumnDesc> colDesc;
    colDesc.emplace_back("sym", DT_STRING, 1);
    colDesc.emplace_back("time", DT_TIMESTAMP, 2);
    colDesc.emplace_back("factor1", DT_DOUBLE, 3);
    std::vector<std::string> primaryKey { "time", "sym" };
    std::vector<std::pair<bool, std::vector<std::string>>> secondaryKeys;
    conn.createTable(tableName, colDesc, primaryKey, secondaryKeys);
    auto resultTable = conn.execute("select * from objByName(`"+tableName +")");
    std::cout << resultTable->getString() << std::endl;
    
    // create stream table
    // by calling the function defined in the .dos file
    conn.execute("createStreamTable()");
    
    // create stream engine
    // by calling the function defined in the .dos file
    conn.execute("createEngine()");
    
    // oltp table subscribes to stream table
    // by calling the function defined in the .dos file
    conn.execute("subscribeStreamTable(`"+tableName +")");
    
    std::cout << "================= intput to stream engine ================="<< std::endl;
    int BLK_SIZE = 20;
    int WT_LIMIT = 200;
    for(int start = 0; start < WT_LIMIT; start += BLK_SIZE) {
        std::string code = "produceData("+ std::to_string(BLK_SIZE) +")";
        conn.execute(code);
        std::cout << code << std::endl;
        
        code = "exec count(*) from outStream";
        auto tableSize = conn.execute(code);
        std::cout << code << std::endl;
        std::cout << tableSize->getString() << std::endl;
        
        sleep(1);
        
        code = "exec count(*) from objByName(`"+tableName +")";
        tableSize = conn.execute(code);
        std::cout << code << std::endl;
        std::cout << tableSize->getString() << std::endl;
        
        code = "select * from objByName(`"+tableName +") order by time limit " + std::to_string(start) + ", " + std::to_string(BLK_SIZE);
        resultTable = conn.execute(code);
        std::cout << code << std::endl;
        std::cout << resultTable->getString() << std::endl;
        
        sleep(1);
    }
    
    std::cout << "================= clean environment ================="<< std::endl;
    conn.execute("cleanEnvironment()");
    conn.dropTable(tableName);
    
    DolphinDBLib::finalizeRuntime();
    
    return 0;
}
