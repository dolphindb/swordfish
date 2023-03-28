#include "Swordfish.h"

#include <memory>
#include <string>
#include <vector>

using std::shared_ptr;
using std::make_shared;
using std::string;
using std::vector;

using oltp::DB;
using oltp::Connection;
using oltp::RawTableBuffer;
using oltp::RawTableBufferGenerator;
using oltp::RawTupleBufferReader;
using oltp::RawTupleBufferWriter;
using oltp::RawTableBufferWriter;
using oltp::TableDescriptionSP;

vector<ColumnDesc> create_table(DB &db, const string &table_name);
void drop_table(DB &db, const string &table_name);
void insert_data(DB &db, const string &table_name, const vector<ColumnDesc> &colDesc);
void query_data(DB &db, const string &table_name);
void update_data(DB &db, const string &table_name);

int main()
{
  // MUST inintialize DolphinDB runtime first
  std::cout << "Inintialize DolphinDB runtime ... \n";
  DolphinDBLib::initializeRuntime();

  {
    const string db_name = "test_db";
    const string table_name = "table1";

    // create (or open) a database (use default option)
    oltp::DBOption option;
    shared_ptr<DB> db = make_shared<DB>(db_name, option);

    auto colDesc = create_table(*db, table_name);
    insert_data(*db, table_name, colDesc);
    query_data(*db, table_name); 
    update_data(*db, table_name);
    drop_table(*db, table_name);

    // close database
    db.reset();

    // delete database
    string err;
    bool ret = Util::removeDirectoryRecursive(db_name, err);
    if (!ret) {
      std::cout << "Delete database [" << db_name << "] failed: " << err << '\n';
      exit(-1);
    }
  }

  // MUST finalize DolphinDB runtime at last
  std::cout << "\nFinalize DolphinDB runtime ... \n";
  DolphinDBLib::finalizeRuntime();
  return 0;
}

//=======================================================================

vector<ColumnDesc> create_table(DB &db, const string &table_name) {
  // connect to the database
  Connection conn(db);
  std::cout << "\n------------------- Create table -------------------\n";

  vector<ColumnDesc> colDesc;
  // `extra` is meaningless for these data type
  colDesc.emplace_back("a", DT_INT,    /*extra*/0);
  colDesc.emplace_back("e", DT_STRING, /*extra*/0);
  colDesc.emplace_back("f", DT_STRING, /*extra*/0);
  colDesc.emplace_back("b", DT_INT,    /*extra*/0);
  colDesc.emplace_back("c", DT_BOOL,   /*extra*/0);
  colDesc.emplace_back("d", DT_LONG,   /*extra*/0);
  colDesc.emplace_back("g", DT_STRING, /*extra*/0);

  // create table
  std::vector<std::string> pk{"a"};
  std::vector<std::pair<bool, std::vector<std::string>>> secondaryKeys{
    {/*unique*/true, {"b", "c", "d"}},
    {/*unique*/false, {"d"}}
  };
  conn.createTable("table1", colDesc, pk, secondaryKeys);

  // get the table info
  TableDescriptionSP info = conn.getTableInfo(table_name);
  if (info.isNull()) {
    std::cout << "Create table [" << table_name << "] failed! \n";
    exit(-1);
  }

  TableSP schemaTable = Util::createTable({"column_name", "column_type"}, {DT_STRING, DT_STRING},
                                          info->columns.size(), info->columns.size());
  for (size_t i = 0; i < info->columns.size(); ++i) {
    const auto &col = info->columns[i];
    schemaTable->getColumn(0)->setString(i, col.getName());
    schemaTable->getColumn(1)->setString(i, Util::getDataTypeString(col.getType()));
  }
  std::cout << schemaTable->getString() << '\n';

  for (const auto &index : info->indexes) {
    if (index.primary) {
      std::cout << "primary index ";
    } else if (index.unique) {
      std::cout << "unique secondary index ";
    } else {
      std::cout << "non-unique secondary index ";
    }
    std::cout << "[" << index.name << "]: key => [ ";
    for (const auto &col : index.key) {
      std::cout << col.getName() << ' ';
    }
    std::cout << "]" << '\n';
  }

  return info->columns;
}

void drop_table(DB &db, const string &table_name) {
  // connect to the database
  Connection conn(db);
  // std::cout << "\n------------------- Drop table -------------------\n";

  conn.dropTable(table_name);
}

void insert_data(DB &db, const string &table_name, const vector<ColumnDesc> &colDesc) {
  // connect to the database
  Connection conn(db);
  std::cout << "\n------------------- Insert data -------------------\n";

  {
    std::cout << "use low-level api... ";
    // insert 5 rows to the table
    const int rows = 5;
    // compute how many bytes are enough to store a tuple
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
    const size_t rows_inserted = conn.insert("table1", table);
    if (rows_inserted != rows) {
      std::cout << "Insert data failed! \n";
      exit(-1);
    }
    std::cout << rows_inserted << " rows were inserted. \n";
  }
}

void query_data(DB &db, const string &table_name) {
  Connection conn(db);
  std::cout << "\n------------------- Query data -------------------\n";

  {
    string sql = "select * from " + table_name;
    std::cout << "1. use SQL api: " << sql << '\n';
    auto result = conn.execute(sql);
    std::cout << result->getString() << '\n';
  }
  {
    string sql = "select * from " + table_name + " where a>0,a<=3";
    std::cout << "2. use high-level api: " << sql << '\n';
    auto result = conn.query(table_name, {}, conn.makeFilters("a>0,a<=3"));
    std::cout << result->getString() << '\n';
  }
  {
    string sql = "select a,G,D,b from " + table_name + " where a>3";
    std::cout << "3. use low-level api: " << sql << '\n';

    const int max_rows = 10;  // only 10 rows at most
    std::vector<DATA_TYPE> types{/*a*/DT_INT, /*g*/DT_STRING, /*d*/DT_LONG, /*b*/DT_INT};
    size_t bytes_per_row = oltp::computeBytes(types, /*max_len_for_varlen*/20);

    RawTableBufferGenerator gen(max_rows, bytes_per_row);
    RawTableBuffer result = gen.get();

    // select a,G,D,b from table1 where a>3
    size_t rows = conn.query("table1", {"a","g","d","b"}, conn.makeFilters("a>3"), result);
    std::cout << "result contains " << rows << " rows: \n";

    RawTupleBufferReader reader;
    for (size_t i = 0; i < rows; ++i) {
      std::cout << "  row #" << i << ": ";
      reader.reset(result.tuples[i]);
      std::cout << reader.readInt() << ' ';
      std::cout << reader.readString().toString() << ' ';
      std::cout << reader.readLong() << ' ';
      std::cout << reader.readInt() << ' ';
      std::cout << '\n';
    }
    std::cout << '\n';
  }
  {
    string sql = "select * from " + table_name + " order by a";
    std::cout << "4. use SQL api: " << sql << '\n';
    auto result = conn.execute(sql);
    std::cout << result->getString() << '\n';
  }
}

void update_data(DB &db, const string &table_name) {
  Connection conn(db);
  std::cout << "\n------------------- Update data -------------------\n";

  {
    string sql = "update " + table_name + " set b=b+1,d=d-1 where a=2";
    std::cout << "1. use SQL api: " << sql << '\n';

    std::cout << "before update: \n";
    std::cout << conn.execute("select * from " + table_name + " where a=2")->getString() << '\n';

    conn.execute(sql);

    std::cout << "after update: \n";
    std::cout << conn.execute("select * from " + table_name + " where a=2")->getString() << '\n';
  }
  {
    string sql = "update " + table_name + " set f='hello',g='world' where a=3";
    std::cout << "2. use high-level api: " << sql << '\n';

    std::cout << "before update: \n";
    std::cout << conn.execute("select * from " + table_name + " where a=3")->getString() << '\n';

    conn.update(table_name, conn.makeColumnUpdateDefs("f='hello',g='world'"), conn.makeFilters("a=3"));

    std::cout << "after update: \n";
    std::cout << conn.execute("select * from " + table_name + " where a=3")->getString() << '\n';
  }
}
