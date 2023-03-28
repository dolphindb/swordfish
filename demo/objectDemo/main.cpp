#include "Swordfish.h"

#include <memory>

using std::make_shared;
using std::shared_ptr;

using oltp::Connection;
using oltp::DB;

void testTable(DB &db);
void testVector(DB &db);
void testMatrix(DB &db);
void testSet(DB &db);
void testDictionary(DB &db);

int main()
{
  // MUST inintialize runtime first
  DolphinDBLib::initializeRuntime();

  // create (or open) a database
  oltp::DBOption option;
  shared_ptr<DB> db = make_shared<DB>("test_db", option);

  testTable(*db);
  testVector(*db);
  testMatrix(*db);
  testSet(*db);
  testDictionary(*db);

  DolphinDBLib::finalizeRuntime();
  return 0;
}

void testTable(DB &db)
{
  Connection conn(db);

  // create a table
  std::string colName1 = "id";
  std::string colName2 = "val";
  std::vector<std::string> colNames = {colName1, colName2};
  std::vector<DATA_TYPE> colTypes = {DT_INT, DT_DOUBLE};
  TableSP tb1 = Util::createTable(colNames, colTypes, 0, 0);

  // insert rows
  std::string errMsg;
  ConstantSP id = Util::createConstant(DT_INT);
  ConstantSP val = Util::createConstant(DT_DOUBLE);
  for (auto i = 0; i < 10; ++i)
  {
    id->setInt(i);
    val->setDouble(i * 1.1);
    std::vector<ConstantSP> rowValues = {id, val};
    INDEX rowIdx = i;
    tb1->append(rowValues, rowIdx, errMsg);
  }
  if (errMsg.empty())
  {
    std::cout << tb1->getString() << std::endl;
  }
  else
  {
    std::cout << "insert rows failed! " << errMsg << std::endl;
  }

  // delete row
  errMsg.clear();
  ConstantSP rowIdx1 = Util::createConstant(DT_INT);
  rowIdx1->setInt(0);
  VectorSP rowIdxVec = Util::createVector(DT_INT, 0, 1);
  rowIdxVec->append(rowIdx1);
  tb1->remove(rowIdxVec, errMsg);
  if (errMsg.empty())
  {
    std::cout << tb1->getString() << std::endl;
  }
  else
  {
    std::cout << "delete row " << rowIdx1->getInt() << " failed! " << errMsg << std::endl;
  }

  // select row
  INDEX rowIdx2 = 3;
  ConstantSP row = tb1->getRow(rowIdx2);
  std::cout << "row index: " << rowIdx2;
  std::cout << " id: " << row->getMember(colName1)->getInt();
  std::cout << " val: " << row->getMember(colName2)->getDouble() << std::endl;

  // update row
  errMsg.clear();
  id->setInt(111);
  val->setDouble(111.111);
  std::vector<ConstantSP> rowValues = {id, val};
  ConstantSP rowIdx3 = Util::createConstant(DT_INT);
  rowIdx3->setInt(0);
  tb1->update(rowValues, rowIdx3, colNames, errMsg);
  if (errMsg.empty())
  {
    std::cout << tb1->getString() << std::endl;
  }
  else
  {
    std::cout << "update row failed! " << errMsg << std::endl;
  }

  // receive a table from database
  TableSP tb2 = conn.execute("table(1 2 3 as id, 11 22 33 as val)");
  std::cout << tb2->getString() << std::endl;

  return;
}

void testVector(DB &db)
{
  Connection conn(db);

  // create a vector
  VectorSP v1 = Util::createVector(DT_DOUBLE, 0);

  // add elements
  std::vector<double> newData = {1.1, 2.2, 3.3};
  v1->appendDouble(newData.data(), newData.size());

  // read elements' value
  for (int i = 0; i < v1->size(); ++i)
  {
    std::cout << v1->getDouble(i) << std::endl;
  }

  // remove element
  ConstantSP idx = Util::createConstant(DT_INT);
  idx->setInt(0);
  VectorSP idxVec = Util::createVector(DT_INT, 0, 1);
  idxVec->append(idx);
  v1->remove(idxVec);
  std::cout << v1->getString() << std::endl;

  // receive a vector from database
  VectorSP v2 = conn.execute("1..10");
  std::cout << v2->getString() << std::endl;

  return;
}

void testMatrix(DB &db)
{
  Connection conn(db);

  // create a matrix
  int *rawData = new int[12]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  VectorSP m1 = Util::createMatrix(DT_INT, 4, 3, 12, 0, rawData);

  // set labels
  VectorSP rowLabelVec = Util::createVector(DT_STRING, 0, 3);
  VectorSP colLabelVec = Util::createVector(DT_STRING, 0, 4);
  std::vector<std::string> rowLabels = {"row1", "row2", "row3"};
  std::vector<std::string> colLabels = {"col1", "col2", "col3", "col4"};
  rowLabelVec->appendString(rowLabels.data(), rowLabels.size());
  colLabelVec->appendString(colLabels.data(), colLabels.size());
  m1->setRowLabel(rowLabelVec);
  m1->setColumnLabel(colLabelVec);
  std::cout << m1->getString() << std::endl;

  // read elements' value
  for (int i = 0; i < m1->columns(); ++i)
  {
    auto row = m1->getColumn(i);
    for (int j = 0; j < row->size(); ++j)
    {
      std::cout << row->getInt(j) << std::endl;
    }
  }

  // receive a matrix from database
  VectorSP m2 = conn.execute("1..6$2:3");
  std::cout << m2->getString() << std::endl;

  return;
}

void testSet(DB &db)
{
  Connection conn(db);

  // create a set
  SetSP s1 = Util::createSet(DT_INT, nullptr, 3);

  // add new element
  ConstantSP val = Util::createConstant(DT_INT);
  val->setInt(1);
  s1->append(val);
  val->setInt(2);
  s1->append(val);
  std::cout << s1->getString() << std::endl;

  // check if set contains element
  ConstantSP res = Util::createConstant(DT_BOOL);
  s1->contain(val, res);
  if (res->getBool())
  {
    std::cout << "s1 contains " << val->getInt() << std::endl;
  }

  // remove element
  s1->remove(val);
  s1->contain(val, res);
  if (!res->getBool())
  {
    std::cout << "s1 does not contain " << val->getInt() << std::endl;
  }

  // clear the set
  s1->clear();

  // receive a set from database
  SetSP s2 = conn.execute("set(1 3 4 5 5 6 6 7)");
  std::cout << s2->getString() << std::endl;

  return;
}

void testDictionary(DB &db)
{
  Connection conn(db);

  // create a dictionary
  DictionarySP d1 = Util::createDictionary(DT_INT, nullptr, DT_DOUBLE, nullptr);

  // add new key-value pair
  ConstantSP k = Util::createConstant(DT_INT);
  k->setInt(1);
  ConstantSP v = Util::createConstant(DT_DOUBLE);
  v->setDouble(1.1);
  d1->set(k, v);
  k->setInt(2);
  v->setDouble(2.2);
  d1->set(k, v);
  std::cout << d1->getString() << std::endl;

  // check key exists
  ConstantSP res = Util::createConstant(DT_BOOL);
  d1->contain(k, res);
  if (res->getBool())
  {
    std::cout << "d1 contains " << k->getDouble() << std::endl;
  }

  // read value of specific key
  std::cout << "key " << k->getInt() << " 's value is" << d1->getMember(k)->getDouble() << std::endl;

  // remove key-value pair
  d1->remove(k);
  d1->contain(k, res);
  if (!res->getBool())
  {
    std::cout << "d1 does not contain " << k->getDouble() << std::endl;
  }

  // clear the dictionary
  d1->clear();

  // receive a dictionary from database
  DictionarySP d2 = conn.execute("dict(1 2 3, 10 20 30)");
  std::cout << d2->getString() << std::endl;

  return;
}