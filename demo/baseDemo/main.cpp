#include "EmbeddedOLTP.h"

#include <memory>

using std::shared_ptr;
using std::make_shared;

using oltp::DB;
using oltp::Connection;
using oltp::RawTableBuffer;
using oltp::RawTableBufferGenerator;
using oltp::RawTupleBufferReader;
using oltp::RawTupleBufferWriter;
using oltp::RawTableBufferWriter;

void test_low_level_query_api(DB &db);

int main()
{
  // MUST inintialize runtime first
  DolphinDBLib::initializeRuntime();

  // create (or open) a database
  oltp::DBOption option;
  shared_ptr<DB> db = make_shared<DB>("test_db", option);

  // connect to the database
  Connection conn(*db);

  {
    std::vector<ColumnDesc> colDesc;
    colDesc.emplace_back("a", DT_INT, 1);
    colDesc.emplace_back("e", DT_STRING, 1);
    colDesc.emplace_back("f", DT_STRING, 1);
    colDesc.emplace_back("b", DT_INT, 2);
    colDesc.emplace_back("c", DT_BOOL, 3);
    colDesc.emplace_back("d", DT_LONG, 3);
    colDesc.emplace_back("g", DT_STRING, 1);

    // create table
    std::vector<std::string> pk{"a"};
    std::vector<std::pair<bool, std::vector<std::string>>> secondaryKeys{
      // {true, {"b", "c", "d"}},
      // {true, {"d"}}
    };
    conn.createTable("table1", colDesc, pk, secondaryKeys);

    {
      std::cout << "\n====== test Low Level Insert API ======\n";

      const int rows = 5;
      const size_t bytes_per_row = oltp::computeBytes(colDesc, /*max_len_for_varlen*/20);
      RawTableBufferGenerator gen(rows, bytes_per_row);

      const RawTableBuffer &table = gen.get();
      RawTableBufferWriter writer(table, colDesc);
      writer.append(1, "e_1", "f_11",  5, false,  2, "g_111")
            .append(2, "e_2", "f_22",  4, false, 13, "g_222")
            .append(3, "e_3", "f_33", 42,  true, 37, "g_333")
            .append(4, "e_4", "f_44", 18, false, 63, "g_444")
            .append(5, "e_5", "g_55", 49,  true, 28, "g_555");

      // use low-level api insert tuples to table
      conn.insert("table1", table);

      auto obj = conn.execute("select * from table1 order by a");
      std::cout << obj->getString() << "\n";
    }

    {
      std::cout << "\n====== test SQL query ======\n";

      auto obj = conn.execute("select b,d,A from table1 where a = 5");
      std::cout << obj->getString() << "\n";
    }

    {
      std::cout << "\n====== test API ======\n";

      std::cout << "before update: \n";
      // select from table1 where a = 5, d = 1 or d / 2 = 14
      auto result = conn.query("table1", {}, conn.makeFilters("a = 5, d = 1 or d / 2 = 14"));
      std::cout << result->getString() << "\n";

      // update table1 set d = 7 where a = 5
      conn.update("table1", conn.makeColumnUpdateDefs("d = 7"), conn.makeFilters("a = 5"));
      
      std::cout << "after update: \n";
      // select from table1 where a = 5
      result = conn.query("table1", {}, conn.makeFilters("a = 5"));
      std::cout << result->getString() << "\n";
    }
  }

  {
    std::cout << "\n====== test DolphinDB Script ======\n";
    ConstantSP obj;
    obj = conn.execute(R"(
      a = 1
      b = 2
      a + b
    )");
    std::cout << obj->getString() << '\n';                // output: 3
    std::cout << conn.execute("a")->getString() << '\n';  // output: 1

    // no output
    std::cout << conn.execute(R"(print("hello world"))")->getString() << '\n';
  }

  // test low level query api
  test_low_level_query_api(*db);

  // drop table
  conn.dropTable("table1");

  DolphinDBLib::finalizeRuntime();
  return 0;
}

void test_low_level_query_api(DB &db)
{
  Connection conn(db);

  std::cout << "\n====== test Low Level Query API ======\n";

  int max_rows = 3;
  std::vector<DATA_TYPE> types{DT_INT, DT_STRING, DT_STRING, DT_INT, DT_BOOL, DT_LONG, DT_STRING};
  size_t bytes_per_row = oltp::computeBytes(types, /*max_len_for_varlen*/20);

  RawTableBufferGenerator gen(max_rows, bytes_per_row);
  RawTableBuffer result = gen.get();

  // select * from table1 where a < 6
  size_t rows = conn.query("table1", {}, conn.makeFilters("a < 6"), result);
  std::cout << "get " << rows << " rows query result \n";

  RawTupleBufferReader reader;
  for (size_t i = 0; i < rows; ++i) {
    std::cout << "row #" << i << ": ";
    reader.reset(result.tuples[i]);
    std::cout << reader.readInt() << ' ';
    std::cout << reader.readString().toString() << ' ';
    std::cout << reader.readString().toString() << ' ';
    std::cout << reader.readInt() << ' ';
    std::cout << reader.readBool() << ' ';
    std::cout << reader.readLong() << ' ';
    std::cout << reader.readString().toString() << ' ';
    std::cout << '\n';
  }
}
