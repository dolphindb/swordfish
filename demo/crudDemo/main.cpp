#include "Swordfish.h"
#include "Util.h"
#include <memory>
#include <thread>
#include <vector>

using namespace oltp;

int main(int argc, char* argv[])
{
    // initialize Runtime Environment
    DolphinDBLib::initializeRuntime();

    // connect database
    DBOption option;
    std::shared_ptr<DB> db = std::make_shared<DB>("test_db2", option);
    Connection conn(*db);
    const string tableName = "test_table";

    // create Table
    std::vector<ColumnDesc> colDesc;
    colDesc.emplace_back("col_int", DT_INT, 1);
    colDesc.emplace_back("col_bool", DT_BOOL, 2);
    colDesc.emplace_back("col_long", DT_LONG, 3);

    std::vector<std::string> primaryKey { "col_int" };
    std::vector<std::pair<bool, std::vector<std::string>>> secondaryKeys;
    conn.createTable(tableName, colDesc, primaryKey, secondaryKeys);

    auto obj = conn.execute("select * from test_table");
    std::cout << "================== create table ==================\n";
    std::cout << obj->getString() << "\n";


    // low level insert data
    {
        const int rows = 5;
        const size_t bytes_per_row = computeBytes(colDesc, 100);
        RawTableBufferGenerator gen(rows, bytes_per_row);

        const RawTableBuffer& table = gen.get();
        RawTupleBufferWriter writer;
        writer.reset(table.tuples[0]);
        writer.writeInt(5).writeBool(false).writeLong(200);
        writer.reset(table.tuples[1]);
        writer.writeInt(4).writeBool(true).writeLong(211);
        writer.reset(table.tuples[2]);
        writer.writeInt(3).writeBool(false).writeLong(211);
        writer.reset(table.tuples[3]);
        writer.writeInt(2).writeBool(true).writeLong(213);
        writer.reset(table.tuples[4]);
        writer.writeInt(1).writeBool(false).writeLong(215);

        conn.insert(tableName, table);
        auto obj = conn.execute("select * from test_table");
        std::cout << "============= low level insert data =============\n";
        std::cout << obj->getString() << "\n";
    }


    // insert data
    {
        std::vector<int> colIntValue { 17, 37, 67 };
        std::vector<char> colBoolValue { 0, 0, 0 };
        std::vector<long long> colLongValue { 199, 224, 238 };
        VectorSP colInt = Util::createVector(DT_INT, 0, 3);
        VectorSP colBool = Util::createVector(DT_BOOL, 0, 3);
        VectorSP colLong = Util::createVector(DT_LONG, 0, 3);
        colInt->appendInt((int*)(colIntValue.data()), 3);
        colBool->appendBool((char*)(colBoolValue.data()), 3);
        colLong->appendLong((long long*)(colLongValue.data()), 3);
        std::vector<string> colNames { "col_int", "col_bool", "col_long" };
        std::vector<ConstantSP> cols { colInt, colBool, colLong };
        TableSP insertTable = Util::createTable(colNames, cols);
        conn.insert(tableName, insertTable);

        auto obj = conn.execute("select * from test_table");
        std::cout << "================== insert data ==================\n";
        std::cout << obj->getString() << "\n";
    }


    // low level query
    {
        string filterTxt = "col_long >= 211, col_bool = false";
        vector<ObjectSP> filter = conn.makeFilters(filterTxt);
        vector<string> queryCols { "col_int", "col_bool", "col_long" };

        const int queryRowsMax = 5;
        const size_t bytes_per_row = computeBytes(colDesc, 100);
        RawTableBufferGenerator queryGen(queryRowsMax, bytes_per_row);
        const RawTableBuffer& resultBuffer = queryGen.get();

        size_t queryRows = conn.query(tableName, queryCols, filter, resultBuffer);
        std::cout << "============= low level query data ==============\n";
        RawTupleBufferReader reader;
        cout << "queried rows count: " << queryRows << "\n";
        std::cout << "col_int col_bool col_long\n";
        std::cout << "------- -------- --------\n";
        for (size_t i = 0; i < queryRows; ++i) {
            reader.reset(resultBuffer.tuples[i]);
            std::cout << reader.readInt() << "\t";
            std::cout << int(reader.readBool()) << "\t";
            std::cout << reader.readLong() << "\n";
        }
        std::cout << std::endl;
    }

    // query data
    {
        string filterTxt = "col_long >= 211, col_bool = false";
        vector<ObjectSP> filter = conn.makeFilters(filterTxt);
        vector<string> queryCols { "col_int", "col_bool", "col_long" };
        TableSP tableResult = conn.query(tableName, queryCols, filter);
        std::cout << "================== query data ===================\n";
        std::cout << tableResult->getString() << "\n";
    }


    // update data
    {
        string filterTxt = "col_long >= 211, col_bool = false";
        string updateDefsTxt = "col_int = col_int*10, col_bool = true";
        vector<ObjectSP> filter = conn.makeFilters(filterTxt);
        vector<ColumnDefSP> columnUpdateDefs = conn.makeColumnUpdateDefs(updateDefsTxt);
        size_t changeRows = conn.update(tableName, columnUpdateDefs, filter);
        auto obj = conn.execute("select * from test_table");
        std::cout << "================== update data ==================\n";
        std::cout << "changed rows count: " << changeRows << "\n";
        std::cout << obj->getString() << "\n";
    }

    // delete data
    {
        string filterTxt = "col_long >= 213, col_int > 5";
        vector<ObjectSP> filter = conn.makeFilters(filterTxt);
        size_t removeRows = conn.remove(tableName, filter);
        auto obj = conn.execute("select * from test_table");
        std::cout << "================== delete data ==================\n";
        std::cout << "removed rows count: " << removeRows << "\n";
        std::cout << obj->getString() << "\n";
    }

    // use transaction
    // only one of following transactions would be committed
    {
        auto transaction1 = [&conn, &tableName]() {
            if (conn.isInActiveTransactionContext()) {
                return;
            }
            conn.beginTransaction();
            string filterTxt = "col_long <= 500, col_bool = true";
            string updateDefsTxt = "col_long = col_long+1";
            vector<ObjectSP> filter = conn.makeFilters(filterTxt);
            vector<ColumnDefSP> columnUpdateDefs = conn.makeColumnUpdateDefs(updateDefsTxt);

            size_t changeRows;
            do {
                changeRows = conn.update(tableName, columnUpdateDefs, filter);
            } while (changeRows > 0);
            conn.commit();
        };

        auto transaction2 = [&conn, &tableName]() {
            if (conn.isInActiveTransactionContext()) {
                return;
            }
            conn.beginTransaction();
            string filterTxt = "col_int < 500, col_bool = true";
            string updateDefsTxt = "col_int = col_int+10";
            vector<ObjectSP> filter = conn.makeFilters(filterTxt);
            vector<ColumnDefSP> columnUpdateDefs = conn.makeColumnUpdateDefs(updateDefsTxt);

            size_t changeRows;
            do {
                changeRows = conn.update(tableName, columnUpdateDefs, filter);
            } while (changeRows > 0);
            conn.commit();
        };

        std::thread t1(transaction1);
        std::thread t2(transaction2);

        t1.join();
        t2.join();

        auto obj = conn.execute("select * from test_table");
        std::cout << "================ use transaction ================\n";
        std::cout << obj->getString() << "\n";
    }

    // transaction rollback
    {
        conn.beginTransaction();
        string filterTxt = "col_long <= 500, col_bool = false";
        string updateDefsTxt = "col_long = col_long+1000";
        vector<ObjectSP> filter = conn.makeFilters(filterTxt);
        vector<ColumnDefSP> columnUpdateDefs = conn.makeColumnUpdateDefs(updateDefsTxt);
        conn.update(tableName, columnUpdateDefs, filter);

        auto obj = conn.execute("select * from test_table");
        std::cout << "================= before rollback =================\n";
        std::cout << obj->getString() << "\n";

        conn.rollback();

        obj = conn.execute("select * from test_table");
        std::cout << "================= after rollback =================\n";
        std::cout << obj->getString() << "\n";
    }

    // drop existed table
    conn.dropTable(tableName);
    DolphinDBLib::finalizeRuntime();
}