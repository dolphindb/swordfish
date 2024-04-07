#include "Swordfish.h"

#include <memory>
#include <string>
#include <vector>

void test_olap();
void test_tsdb();

int main()
{
  // MUST initialize DolphinDB runtime first
  std::cout << "Initialize DolphinDB runtime ... \n";
  DolphinDBLib::initializeRuntime();

  std::cout << "Swordfish " << DolphinDBLib::getVersionString() << '\n';

  test_olap();
  test_tsdb();

  // MUST finalize DolphinDB runtime at last
  std::cout << "\nFinalize DolphinDB runtime ... \n";
  DolphinDBLib::finalizeRuntime();
  return 0;
}

void test_olap()
{
  std::cout << "\n------------------- Test OLAP -------------------\n";

  std::cout << DolphinDBLib::execute(R"(
    dbName = "dfs://test_olap"
    tableName = "xxx"

    if (!existsTable(dbName, tableName)) {
      n = 20
      t = table(2020.01.01T00:00:00 + 0..(n-1) as timestamp, rand(`IBM`MS`APPL`AMZN,n) as symbol, rand(10.0, n) as value)
      db = database(dbName, VALUE, `IBM`MS`APPL`AMZN)
      pt = db.createPartitionedTable(table=t, tableName=tableName, partitionColumns="symbol", compressMethods={timestamp:"delta"});
      pt.append!(t)
    } else {
      db = database(dbName)
      pt = loadTable(db, tableName)
    }

    select * from pt
  )")->getString();
}

void test_tsdb()
{
  std::cout << "\n------------------- Test TSDB -------------------\n";

  auto res = DolphinDBLib::execute(R"(
    // dbName = "dfs://test_tsdb"
    // tableName = "yyy"

    // if (!existsTable(dbName, tableName)) {
    //   n = 20
    //   SecurityID = rand(`st0001`st0002`st0003`st0004`st0005, n)
    //   sym = rand(`A`B, n)
    //   TradeDate = 2022.01.01 + rand(100,n)
    //   TotalVolumeTrade = rand(1000..3000, n)
    //   TotalValueTrade = rand(100.0, n)
    //   schemaTable = table(SecurityID, TradeDate, TotalVolumeTrade, TotalValueTrade).sortBy!(`SecurityID`TradeDate)

    //   db = database(dbName, RANGE, [2022.01.01, 2022.02.01, 2022.03.01, 2022.04.01, 2022.05.01], engine='TSDB')
    //   pt = createPartitionedTable(dbHandle=db, table=schemaTable, tableName=tableName, partitionColumns=`TradeDate,
    //       sortColumns=`SecurityID`TradeDate, keepDuplicates=ALL, sortKeyMappingFunction=[hashBucket{,5}])
    //   pt.append!(schemaTable)
    //   flushTSDBCache()
    // } else {
    //   db = database(dbName)
    //   pt = loadTable(db, tableName)
    // }

    // select * from pt
    getNodeType()
  )");
  // for (auto i=0;i<res->keys()->size();i++){
  //   string key = res->keys()->get(i)->getString();
  //   std::cout << key << ": " << res->getMember(key)->getString() << '\n';
  // }
  std::cout << res->getString();
}
