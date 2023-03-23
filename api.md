# API 参考

**目录**
- [类 (class) 说明](#类-class-说明)
  - [类：oltp::Connection](#类oltpconnection)
    - [公开成员函数](#公开成员函数)
    - [详细说明](#详细说明)
    - [成员函数](#成员函数)
      - [void oltp::Connection::beginTransaction ()](#void-oltpconnectionbegintransaction-)
      - [void oltp::Connection::createTable ()](#void-oltpconnectioncreatetable-)
      - [void oltp::Connection::dropTable ()](#void-oltpconnectiondroptable-)
      - [ConstantSP oltp::Connection::execute ()](#constantsp-oltpconnectionexecute-)
      - [TableDescriptionSP oltp::Connection::getTableInfo ()](#tabledescriptionsp-oltpconnectiongettableinfo-)
      - [size\_t oltp::Connection::insert ()](#size_t-oltpconnectioninsert-)
      - [size\_t oltp::Connection::insert ()](#size_t-oltpconnectioninsert--1)
      - [bool oltp::Connection::isInActiveTransactionContext ()](#bool-oltpconnectionisinactivetransactioncontext-)
      - [vector\< string \> oltp::Connection::listAllTable ()](#vector-string--oltpconnectionlistalltable-)
      - [vector\< ColumnDefSP \> oltp::Connection::makeColumnUpdateDefs ()](#vector-columndefsp--oltpconnectionmakecolumnupdatedefs-)
      - [vector\< ObjectSP \> oltp::Connection::makeFilters ()](#vector-objectsp--oltpconnectionmakefilters-)
      - [TableSP oltp::Connection::query ()](#tablesp-oltpconnectionquery-)
      - [size\_t oltp::Connection::query ()](#size_t-oltpconnectionquery-)
      - [size\_t oltp::Connection::remove ()](#size_t-oltpconnectionremove-)
      - [void oltp::Connection::requestCheckpoint ()](#void-oltpconnectionrequestcheckpoint-)
      - [template`<typename Code >` void oltp::Connection::transaction ()](#templatetypename-code--void-oltpconnectiontransaction-)
      - [size\_t oltp::Connection::update ()](#size_t-oltpconnectionupdate-)
  - [类：oltp::DB](#类oltpdb)
    - [公有成员函数](#公有成员函数)
    - [友元类](#友元类)
    - [详细说明](#详细说明-1)
    - [构造函数和析构函数](#构造函数和析构函数)
      - [oltp::DB::DB ()\[explicit\]](#oltpdbdb-explicit)
  - [类：oltp::DBOption](#类oltpdboption)
    - [公共属性](#公共属性)
    - [成员](#成员)
  - [类：DolphinDBLib](#类dolphindblib)
    - [静态公有成员函数](#静态公有成员函数)
      - [成员函数](#成员函数-1)
  - [类：oltp::NeedRetryException](#类oltpneedretryexception)
    - [公有成员函数](#公有成员函数-1)
    - [详细说明](#详细说明-2)
  - [类：oltp::NotRetryException](#类oltpnotretryexception)
    - [公有成员函数](#公有成员函数-2)
    - [详细说明](#详细说明-3)
  - [类：oltp::RawTableBufferGenerator](#类oltprawtablebuffergenerator)
    - [公有成员函数](#公有成员函数-3)
  - [类：oltp::RawTableBufferWriter](#类oltprawtablebufferwriter)
    - [公有成员函数](#公有成员函数-4)
      - [成员函数](#成员函数-2)
  - [类：oltp::RawTupleBufferReader](#类oltprawtuplebufferreader)
    - [公有成员函数](#公有成员函数-5)
  - [类：oltp::StringView](#类oltpstringview)
    - [公有成员函数](#公有成员函数-6)
- [结构体 (struct) 说明](#结构体-struct-说明)
  - [结构体：oltp::RawTableBuffer](#结构体oltprawtablebuffer)
    - [公共属性](#公共属性-1)
    - [详细说明](#详细说明-4)
    - [成员](#成员-1)
  - [结构体：oltp::RawTupleBuffer](#结构体oltprawtuplebuffer)
    - [公共属性](#公共属性-2)
    - [详细说明](#详细说明-5)
  - [结构体：oltp::IndexDescription](#结构体oltpindexdescription)
    - [公共属性](#公共属性-3)
    - [成员](#成员-2)
  - [结构体：oltp::TableDescription](#结构体oltptabledescription)
    - [公共属性](#公共属性-4)
    - [成员](#成员-3)
- [参考](#参考)



# 类 (class) 说明

## 类：oltp::Connection

\#include <Swordfish.h>

### 公开成员函数

- `Connection (DB &db)`
- `Connection (const Connection &)=delete`
- `Connection & operator= (const Connection &)=delete`
- `ConstantSP execute (const string &script)`
- `void beginTransaction ()`
- `void commit ()`
- `void rollback ()`
- `template<typename Code> void transaction (Code code)`
- `bool isInActiveTransactionContext ()`
- `void createTable (const string &tableName, const vector< ColumnDesc > &colDesc, const vector< string > &primaryKey, const vector< pair< bool, vector< string > > > &secondaryKeys)`: 在当前数据库中创建一个表。
- `void dropTable (const string &tableName)`: 删除当前数据库中的一个表。
- `TableSP query (const string &tableName, const vector< string > &columnNames, const vector< ObjectSP > &filters)`: 从表中查询数据。
- `size_t insert (const string &tableName, const TableSP &data)`: 向表中插入数据。
- `size_t remove (const string &tableName, const vector< ObjectSP > &filters)`: 从表中删除数据。
- `size_t update (const string &tableName, const vector< ColumnDefSP > &updateExpr, const vector< ObjectSP > &filters)`: 更新表中数据。
- `size_t query (const string &tableName, const vector< string > &columnNames, const vector< ObjectSP > &filters, const RawTableBuffer &result)`: 查询表内数据
- `size_t insert (const string &tableName, const RawTableBuffer &data)`: 向表内插入数据
- `void requestCheckpoint (bool force, bool wait)`: 要求做检查点。
- `Session * getCurrentSession ()`
- `vector< ObjectSP > makeFilters (const string &text)`
- `vector< ColumnDefSP > makeColumnUpdateDefs (const string &text)`
- `vector< string > listAllTable ()`
- `TableDescriptionSP getTableInfo (const string &tableName)`

### 详细说明

一个连接对应一个客户端，连接到相应的数据库。

**警告**

> 请勿在多个线程中使用同一个连接对象。

**注意**

> 确保连接处于活动状态时，**数据库**对象是有效的。

### 成员函数

#### void oltp::Connection::beginTransaction ()

开始一个新事务，然后可以保证多个 DML 操作（查询/插入/删除/更新）原子性。事务中所有的 DML 要么全成功（如果提交），要么全失败，因为（如果回滚）什么都没有发生。

**警告**

> 不可在事务中执行任何 DDL 操作（即创建/删除表）。

如果已经在一个活跃的事务中，请勿开始一个新的事务，否则将抛出 RuntimeException。

#### void oltp::Connection::createTable ()

在当前数据库中创建一个表。

用法：

```cpp
void oltp::Connection::createTable (const string & *tableName*, const vector< ColumnDesc > & *colDesc*, const vector< string > &  *primaryKey*, const vector< pair< bool, vector< string > > > & *secondaryKeys*)
```

**参数**

- *tableName*：表名
- *colDesc*: 列描述
- *primaryKey*: 主键，用于主键索引的列
- *secondaryKeys*: 副键，二级索引键

**注意**

> 主键索引为必需，二级索引为可选，可有多个二级索引。

二级索引键值对 `<bool, vector<string>>` 的描述：

- 第一，是否是唯一索引。
- 第二，用作索引键的列。

#### void oltp::Connection::dropTable ()

删除当前数据库中的一个表。

用法：

```cpp
void oltp::Connection::dropTable (const string & *tableName*)
```

**参数**

*tableName*: 要删除的表名

#### ConstantSP oltp::Connection::execute ()

执行一个SQL或DolphinDB脚本。

用法：

```cpp
ConstantSP oltp::Connection::execute (const string & *script*)
```

例子:

```sql
execute("select * from table order by a"), execute("a = 1; b = 2; a + b")。
```

**警告**

> 请勿使用该API来创建/删除表，而要使用 DDL API。

#### TableDescriptionSP oltp::Connection::getTableInfo ()

获取一个特定表的信息，若找不到相关信息，则返回NULL-SP。

用法：

```cpp
TableDescriptionSP oltp::Connection::getTableInfo (const string & *tableName*)
```

#### size_t oltp::Connection::insert ()

用于低级别插入的API。调用时**必须**确保传入数据与表的 schema 相匹配。

用法：

```cpp
size_t oltp::Connection::insert (const string & *tableName*, const RawTableBuffer & *data*)
```

**警告**

> 与高级别的API相比，该API并无性能改进。若没有必要，不建议使用该API。

**异常**

- **NeedRetryException**: 由于并发事务而导致的读写冲突，不是错误。
- **NotRetryException**: 是一种无法处理的错误。异常原因包括写入的表不存在、写入的数据和表的 schema 不匹配等。

#### size_t oltp::Connection::insert ()

向表中插入数据。

用法：

```cpp
size_t oltp::Connection::insert (const string & *tableName*, const TableSP & *data*)
```

**参数**

- *tableName*: 要插入数据的表名
- *data*: 一个表对象代表想要插入的数据。

**返回值**

返回插入数据的行数

**异常**

- **NeedRetryException**: 由于并发事务而导致的读写冲突，不是错误。
- **NotRetryException**: 是一种无法处理的错误。异常原因包括写入的表不存在、写入的数据和表的 schema 不匹配等。

#### bool oltp::Connection::isInActiveTransactionContext ()

检查是否在一个活跃的事务中。

#### vector< string > oltp::Connection::listAllTable ()

获取当前数据库中所有表的名称。

#### vector< ColumnDefSP > oltp::Connection::makeColumnUpdateDefs ()

生成符合 SQL 语法中的 update 语句，例如"a = 1"，"a = a + 1, b = b - 1"。

用法：

```cpp
vector< ColumnDefSP > oltp::Connection::makeColumnUpdateDefs (const string & *text*)
```

#### vector< ObjectSP > oltp::Connection::makeFilters ()

生成符合 SQL where 子句中的过滤条件，例如，"a = 1"，"a > 3, b = 2"。

用法：

```cpp
vector< ObjectSP > oltp::Connection::makeFilters (const string & *text*)
```

**参数**

*text*: 字符串文本。若为空，则查询全部。

**注意**

> 请勿使用 "a>3和b=2"，而应使用 "a>3，b=2"。

#### TableSP oltp::Connection::query ()

从表中查询数据。

用法：

```cpp
TableSP oltp::Connection::query (const string & *tableName*, const vector< string > & *columnNames*, const vector< ObjectSP > & *filters*)
```

**参数**

- *tableName*: 要查询的表名
- *columnNames*: 要查询的列（若为空，则查询所有列）。
- *filters*: 在  SQL 中的 Where 子句中，若为空则查询全部数据。使用 makeFilters生成过滤条件，例如，makeFilters("a = 1") , makeFilters("a  = 1, b > 2") 。

**返回值**

返回一个表对象代表查询结果。

**异常**

- **NeedRetryException**: 由于并发事务而导致的读写冲突，不是错误。
- **NotRetryException**: 是一种无法处理的错误。异常原因包括写入的表不存在、写入的数据和表的 schema 不匹配等。

**注意**

> 该 API 相当于一个简单的 SQL，如 "select xxx form table where a = 2"。若想执行复杂的 SQL，例如有 "order by"、"group by "等的操作时，请使用 execute API。

#### size_t oltp::Connection::query ()

这是一个用于低级别查询的API。调用时需提前分配内存（见 **RawTableBuffer**），并将结果写入该内存区域。**RawTableBuffer::rows** 表示要返回的最大行数。调用时必须确保 **RawTableBuffer** 中的元组足够大，能够存储查询结果的一行。

用法：

```cpp
size_t oltp::Connection::query (const string & *tableName*, const vector< string > & *columnNames*, const vector< ObjectSP > & *filters*, const RawTableBuffer & *result*)
```

**注意**

> 与高级别API相比，这个API性能改进仅达~20%。若无必要，不建议使用该API。

**参数**

- *columnNames*: 要查询的列。若为空，则查询全部列。
- *result*: 用来存储查询结果的缓冲区。

**返回值**

存入缓冲区的行数

**异常**

- **NeedRetryException**: 由于并发事务而导致的读写冲突，不是错误。
- **NotRetryException**: 是一种无法处理的错误。异常原因包括写入的表不存在、写入的数据和表的 schema 不匹配等。

**注意**

> 该 API 相当于一个简单的 SQL，如 "select xxx form table where a = 2"。若想执行复杂的 SQL，例如有 "order by"、"group by "等的操作时，请使用 execute API。

#### size_t oltp::Connection::remove ()

从表中删除数据。

用法：

```cpp
size_t oltp::Connection::remove (const string & *tableName*, const vector< ObjectSP > & *filters*)
```

**参数**

- *tableName*: 要删除的表名
- *filters*: 在  SQL 中的 Where子句中，若为空则查询全部数据。使用 makeFilters 来生成过滤条件。

**返回值**

被删除数据的行数

**异常**

- **NeedRetryException**: 由于并发事务而导致的读写冲突，不是错误。
- **NotRetryException**: 是一种无法处理的错误。异常原因包括写入的表不存在、写入的数据和表的 schema 不匹配等。

#### void oltp::Connection::requestCheckpoint ()

要求做检查点。

用法：

```cpp
void oltp::Connection::requestCheckpoint (bool *force*, bool *wait*)
```

**参数**

- *force*: 强制检查点，即使没有做任何改变。
- *wait*: 是否等待检查点完成

#### template`<typename Code >` void oltp::Connection::transaction ()

用于在同一事务中执行多个DML操作的方法。

用法：

```cpp
template<typename Code > void oltp::Connection::transaction (Code  code)
```

例子：

```cpp
conn.transaction([&]() {
    conn.insert(...);
    conn.update(...);
    conn.update(...);
});
```

**参数**

- 模板参数 *Code*: 可调用的类型。例如，lambda
- 方法参数 *code*: 用于DML操作执行使用

#### size_t oltp::Connection::update ()

```cpp
size_t oltp::Connection::update (const string & *tableName*, const vector< ColumnDefSP > & *updateExpr*, const vector< ObjectSP > & *filters*)
```

更新表中数据。

**参数**

- *tableName*: 要更新的表名
- *updateExpr*: 如何更新该表。使用  makeColumnUpdateDefs 来生成 update 表达式，例如 makeColumnUpdateDefs("a  = a +1") 。
- *filters*: 在  SQL 中的 Where子句中，若为空则查询全部数据。使用 makeFilters 来生成过滤条件。

**返回值**

更新数据的行数。

**异常**

- **NeedRetryException**: 由于并发事务而导致的读写冲突，不是错误。
- **NotRetryException**: 是一种无法处理的错误。异常原因包括写入的表不存在、写入的数据和表的 schema 不匹配等。

## 类：oltp::DB

\#include <Swordfish.h>

### 公有成员函数

- `DB (const string &path, const DBOption &option)`： 以指定方式（option）打开数据库
- `DB (const DB &)=delete`
- `DB & operator= (const DB &)=delete`

### 友元类

`class Connection`

### 详细说明

数据库对象。打开（或创建）一个数据库，然后连接到数据库对其进行操作。

**注意**

> 调用 destructor 关闭**数据库**前，应确保连接到这个数据库的客户端（**连接**）均处于非活跃状态。

### 构造函数和析构函数

#### oltp::DB::DB ()[explicit]

```cpp
oltp::DB::DB (const string & *path*, const DBOption & *option*)[explicit]
```

用特定的选项打开某一数据库。

**参数**

*path*: 数据库的目录。

## 类：oltp::DBOption

### 公共属性

- bool **readOnly** = false
- bool **enableWAL** = true
- bool **syncOnTxnCommit** = false
- bool **enableCheckpoint** = true
- size_t **checkpointThreshold** = 100
- size_t **checkpointInterval** = 60

### 成员

- `size_t oltp::DBOption::checkpointInterval = 60`: 强制每隔#秒做一次检查点。
- `size_t oltp::DBOption::checkpointThreshold = 100`: 当预写日志的大小超过# MiB时，强制做检查点。
- `bool oltp::DBOption::enableCheckpoint = true`: 若显示ture，将根据checkpointThreshold 和 checkpointInterval自动进行检查。
- `bool oltp::DBOption::enableWAL = true`: 若显示 false，所有的数据都会存储在内存中，一旦数据库关闭这些数据就会丢失。若显示 true，便开启预写日志以保证数据库的持久性。

**注意**

> 若不想丢失任何数据，**必须**启用WAL。

- `bool oltp::DBOption::readOnly = false`: 若显示 true，将以只读模式打开数据库，该操作只支持查询数据，不支持写数据（插入/删除/更新）或创建/删除表。一个数据库可以同时以只读模式打开多次（在同一进程或不同进程中）。但是一旦以写模式打开，其他人就无法同时打开该数据库。

**注意**

> 若无必要，请勿在同一进程中同时打开数据库（只读模式），因为每次打开数据库，所有的数据都会被加载到内存中。

- `bool oltp::DBOption::syncOnTxnCommit = false`: 该选项只有在 enableWAL 为 true 时才有意义。
  - 若显示 true，在写事务提交之前，必须等待该事务的所有预写日志持久化完成。
  - 若显示 false，数据库可以在进程崩溃时存活，但无法在操作系统崩溃时存活。在这种模式下，性能通常会更好。

**注意**

> 如想避免在操作系统崩溃时丢失任何数据，请将此选项设为 "true"。而若想获得更好的性能，且不怕操作系统崩溃，请将此选项设为 “false”。

## 类：DolphinDBLib

### 静态公有成员函数

- `static void initializeRuntime ()`: 初始化 DolphinDB 的运行时。
- `static void finalizeRuntime ()`: 关闭 DolphinDB 的运行时。
- `static SessionSP createSession ()`: 建立一个session

#### 成员函数

- `static SessionSP DolphinDBLib::createSession ()[static]`: 创建一个会话，然后可以用它来运行 DolphinDB 的内置函数。
- `static void DolphinDBLib::finalizeRuntime ()[static]`: 关闭 DolphinDB 的运行时。

**警告**

> 若将 DolphinDB 运行时进行初始化操作，必须调用该函数来销毁运行时环境。一旦调用该函数，则不可调用其他 API 函数（包括 initializeRuntime）。

`static void DolphinDBLib::initializeRuntime ()[static]`: 初始化 DolphinDB 运行时。

**警告**

> 请务必先调用该函数，方可使用其他 API 函数。

## 类：oltp::NeedRetryException

\#include <Swordfish.h>

### 公有成员函数

- `NeedRetryException (const string &err)`
- `virtual const char * what () const noexcept override`

### 详细说明

操作（查询/插入/删除/更新）可能会抛出 **NeedRetryException**。若捕捉到该异常，应该回滚当前事务并开始一个新的事务进行重试。

## 类：oltp::NotRetryException

\#include <Swordfish.h>

### 公有成员函数

- `NotRetryException (const string &err)`
- `virtual const char * what () const noexcept override`

### 详细说明

DML 操作（查询/插入/删除/更新）可能会抛出 **NotRetryException**。若捕捉到该异常，只需回滚当前事务，**请勿**重试。

## 类：oltp::RawTableBufferGenerator

### 公有成员函数

- `RawTableBufferGenerator (int rows, size_t bytes_per_row)`
- `RawTableBufferGenerator (const RawTableBufferGenerator &)=delete`
- `RawTableBufferGenerator & operator= (const RawTableBufferGenerator &)=delete`
- `const RawTableBuffer & get () const`

## 类：oltp::RawTableBufferWriter

### 公有成员函数

- `RawTableBufferWriter (const RawTableBuffer &table, const vector< ColumnDesc > &schema)`
- `void reset (const RawTableBuffer &table)`
- `void reset (const RawTableBuffer &table, const vector< ColumnDesc > &schema)`
- `template<typename ... Args> RawTableBufferWriter & append (Args... args)`

#### 成员函数

`template<typename ... Args> RawTableBufferWriter & oltp::RawTableBufferWriter::append (Args... *args*)[inline]`

**用法**:

```cpp
RawTableBufferWriter writer(table, schema);

writer.append(1, false, 1.0, "a")

   .append(2, true, 2.0, "b")

   .append(3, false, 3.0, "c");
```

## 类：oltp::RawTupleBufferReader

### 公有成员函数

- `RawTupleBufferReader (const RawTupleBuffer &tuple)`
- `void reset (const RawTupleBuffer &tuple)`
- `char readBool ()`
- `char readChar ()`
- `short readShort ()`
- `int readInt ()`
- `long long readLong ()`
- `float readFloat ()`
- `double readDouble ()`
- `StringView readString ()`

## 类：oltp::StringView

### 公有成员函数

- `StringView (const char *str, int size)`
- `const char * str () const`
- `int size () const`
- `std::string toString () const`
- `bool empty () const`

# 结构体 (struct) 说明

## 结构体：oltp::RawTableBuffer

\#include <Swordfish.h>

### 公共属性

- `int rows`
- `RawTupleBuffer * tuples`

### 详细说明

**注意**

> 使用 **RawTableBufferGenerator** 来生成一个 **RawTableBuffer**。

### 成员

`int oltp::RawTableBuffer::rows`: 此表可存储的最大元组。

## 结构体：oltp::RawTupleBuffer

\#include <Swordfish.h>

### 公共属性

`char * buffer`

### 详细说明

**注意**

> 使用 RawTupleBufferGenerator 来生成一个 **RawTupleBuffer**。

## 结构体：oltp::IndexDescription

### 公共属性

- `string name`
- `bool primary`
- `bool unique`
- `vector< ColumnDesc > key`

### 成员

- `vector<ColumnDesc> oltp::IndexDescription::key`: 用作索引键的列。
- `string oltp::IndexDescription::name`: 索引名称。
- `bool oltp::IndexDescription::primary`: 是否为主键索引。
- `bool oltp::IndexDescription::unique`: 是否是唯一索引。

## 结构体：oltp::TableDescription

### 公共属性

- `string name`
- `vector< ColumnDesc > columns`
- `vector< IndexDescription > indexes`

### 成员

- `vector<ColumnDesc> oltp::TableDescription::columns`: 列描述
- `vector<IndexDescription> oltp::TableDescription::indexes`: 该表的索引
- `string oltp::TableDescription::name`: 表名

# 参考

- 包含上述类、结构体和成员函数的头文件：[Swordfish.h](include/Swordfish.h)
- [DolphinDB 用户手册](https://www.dolphindb.cn/cn/help/index.html)
