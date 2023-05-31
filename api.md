# API 参考

**目录**

- [类 (class) 说明](#类-class-说明)
  - [类：oltp::Connection](#类oltpconnection)
    - [公开成员函数](#公开成员函数)
    - [详细说明](#详细说明)
    - [成员函数](#成员函数)
  - [类：oltp::DB](#类oltpdb)
    - [公有成员函数](#公有成员函数)
    - [友元类](#友元类)
    - [详细说明](#详细说明-1)
    - [构造函数和析构函数](#构造函数和析构函数)
  - [类：oltp::DBOption](#类oltpdboption)
    - [公共属性](#公共属性)
    - [成员](#成员)
  - [类：DolphinDBLib](#类dolphindblib)
    - [静态公有成员函数](#静态公有成员函数)
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
  - [类：oltp::RawTupleBufferReader](#类oltprawtuplebufferreader)
    - [公有成员函数](#公有成员函数-5)
  - [类：oltp::StringView](#类oltpstringview)
    - [公有成员函数](#公有成员函数-6)
  - [类：Constant](#类constant)
    - [公有成员函数](#公有成员函数-7)
    - [静态 Public 属性](#静态-public-属性)
    - [Protected 成员函数](#protected-成员函数)
    - [成员函数说明](#成员函数说明)
  - [类：Table](#类table)
    - [Public 成员函数](#public-成员函数)
    - [额外继承的成员函数](#额外继承的成员函数)
    - [成员函数说明](#成员函数说明-1)
  - [类：Vector](#类vector)
    - [Public 成员函数](#public-成员函数-1)
    - [额外继承的成员函数](#额外继承的成员函数-1)
    - [成员函数说明](#成员函数说明-2)
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

#### template `<typename Code >` void oltp::Connection::transaction ()

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

## 类：Constant

继承自 Object .

被 **Table** 以及 **Vector** 继承.

\#include <CoreConcept.h>

### 公有成员函数

- **Constant** (unsigned short flag)
- **Constant** (DATA_FORM df, DATA_TYPE dt, DATA_CATEGORY dc)
- bool **isTemporary** () const
- void **setTemporary** (bool val)

*设置常量是否为临时的。*

- bool **isIndependent** () const

*返回这个常量是否独立。 非独立常量表示包含在其他常量中，例如表包含的向量。 常量默认设置为独立。*

- void **setIndependent** (bool val)

*设置常量是否独立。*

- bool **isReadOnly** () const

*返回此常量是否为只读。 只读常量的值不会改变。 常量默认不是只读的。*

- void **setReadOnly** (bool val)

*设置常量是否只读。*

- bool **isReadOnlyArgument** () const

*返回此常量是否为只读参数。 只读参数不能应用于可变函数。 常量默认不设置为只读参数*

- void **setReadOnlyArgument** (bool val)

*设置常量是否为只读参数。*

- bool **isNothing** () const

*返回这个常量是否为空。 在大多数情况下，nothing 常量用作函数的默认参数。 默认情况下，只有 void 标量会被设置为 nothing。*

- void **setNothing** (bool val)

*设置常量是否为空。*

- bool **isStatic** () const

*返回这个常量是否是静态的。 static是指在sql中用常量初始化，所以它的值是不能改变的。 常量默认不是静态的*

- void **setStatic** (bool val)

*设置静态标志。*

- bool **transferAsString** () const

*返回 transferAsString 标志。 TransferAsString 标志用于在将数据附加到远程块时提高序列化的性能。*

- void **transferAsString** (bool val)

*设置 transferAsString 标志。*

- bool **isSynchronized** () const

*返回这个常量是否可以被多个线程访问。 必须在持有锁的同时访问同步常量。 常量默认不同步。*

- void **setSynchronized** (bool val)

*设置同步标志。*

- bool **isOOInstance** () const

*返回这个常量是否是一个面向对象的实例。 默认情况下，常量不是面向对象的实例。*

- void **setOOInstance** (bool val)

*设置面向对象的实例标志。*

- bool **isIndexed** () const

*返回这个常量是否有索引。 常量默认没有索引。*

- void **setIndexed** (bool val)

*设置索引标志。*

- bool **isSeries** () const

*返回这个常量是否有索引。 Series 表示索引单列矩阵。 常量默认不是系列。*

- void **setSeries** (bool val)

*设置系列标志。*

- bool **isTransient** () const

*返回这个常量是否是瞬态的。 必须通过 **getValue()** 获取副本才能使用瞬态常量。 默认情况下常量不是瞬态的。*

- void **setTransient** (bool val)

*等临时标志。*

- DATA_FORM **getForm** () const

*返回此常量的 DATA_FORM。*

- void **setForm** (DATA_FORM df)

*设置此常量的 DATA_FORM。*

- DATA_TYPE **getType** () const

*返回此常量的 DATA_TYPE。*

- DATA_CATEGORY **getCategory** () const

*返回此常量的 DATA_CATEGORY。*

- bool **isScalar** () const

*返回这个常量是否是标量。*

- bool **isArray** () const

*返回这个常量是否是数组。*

- bool **isPair** () const

*返回这个常量是否是对。*

- bool **isMatrix** () const

*返回这个常量是否是矩阵。*

- bool **isVector** () const

*返回这个常量是否是向量。 向量可以是数组、对或矩阵。*

- bool **isTable** () const

*返回这个常量是否是表。*

- bool **isSet** () const

*返回是否设置了这个常量。*

- bool **isDictionary** () const

*返回这个常量是否是字典。*

- bool **isChunk** () const

*返回这个常量是否是块。*

- bool **isSysObj** () const

*返回这个常量是否是系统对象。*

- bool **isTuple** () const

*返回这个常量是否是元组。 元组必须是任意向量。*

- bool **isNumber** () const

*返回这个常量是否是数字。 数字可以是整数、浮点数或十进制。*

- virtual ConstantSP **getIterator** (const ConstantSP &self) const

*获取这个常量本身的迭代器。*

- virtual ConstantSP **next** ()

*返回下一个项目。如果迭代器已经到达末尾，它返回一个空指针，即 item.isNull() 为真。*

- virtual uint64_t **hash** () const
- virtual bool **equal** (const ConstantSP &other) const
- virtual bool **isDatabase** () const

*返回此常量是否为数据库句柄。*

- virtual char **getBool** () const

*返回这个常量的布尔值。*

- virtual char **getChar** () const

*返回这个常量的字符值。*

- virtual short **getShort** () const

*返回这个常量的短值。*

- virtual int **getInt** () const

*返回此常量的 int 值。*

- virtual long long **getLong** () const

*返回这个常量的长值。*

- virtual INDEX **getIndex** () const

*返回这个常量的索引值。*

- virtual float **getFloat** () const

*返回这个常量的浮点值。*

- virtual double **getDouble** () const

*返回这个常量的双精度值。*

- virtual string **getString** () const

*获取此常量的字符串版本。 这个函数对调试很有帮助。*

- virtual string **getString** (Heap \*heap) const

*根据堆中的会话类型获取此常量的字符串版本。 这个函数对调试很有帮助。*

- virtual string **getScript** () const

*返回这个常量的描述。*

- virtual string **getScript** (Heap \*heap) const

*根据堆中的会话类型返回这个常量的描述。*

- virtual const DolphinString & **getStringRef** () const

*返回此常量的 dolphinString 引用*

- virtual const Guid **getInt128** () const

*返回此常量的 guid 值。*

- virtual const unsigned char \* **getBinary** () const

*获取 int128 标量的二进制数据。*

- virtual bool **isNull** () const

*返回这个常量是否为空。*

- virtual void **setBool** (char val)

*将 bool 值设置为此常量。*

- virtual void **setChar** (char val)

*将 char 值设置为此常量。*

- virtual void **setShort** (short val)

*将短值设置为此常量。*

- virtual void **setInt** (int val)

*将 int 值设置为此常量。*

- virtual void **setLong** (long long val)

*将 long 值设置为此常量。*

- virtual void **setIndex** (INDEX val)

*将索引值设置为此常量。*

- virtual void **setFloat** (float val)

*将浮点值设置为此常量。*

- virtual void **setDouble** (double val)

*将双精度值设置为此常量。*

- virtual void **setString** (const DolphinString &val)

*将 DolphinString 值设置为此常量。*

- virtual void **setBinary** (const unsigned char \*val, int unitLength)

*将二进制数据设置为常量。 请注意，此接口仅在 int128 标量中实现。*

- virtual void **setNull** ()

*将空值设置为此常量。*

- virtual char **getBool** (INDEX index) const

*获取此常量中第 index 个元素的 bool 值。 注意索引要有效，否则会出现越界访问。*

- virtual char **getChar** (INDEX index) const

*获取此常量中第 index 个元素的 char 值。 注意索引要有效，否则会出现越界访问。*

- virtual short **getShort** (INDEX index) const

*获取此常量中第 index 个元素的短值。 注意索引要有效，否则会出现越界访问。*

- virtual int **getInt** (INDEX index) const

*获取此常量中第 index 个元素的 int 值。 注意索引要有效，否则会出现越界访问。*

- virtual long long **getLong** (INDEX index) const

*获取此常量中第 index 个元素的 long 值。 注意索引要有效，否则会出现越界访问。*

- virtual INDEX **getIndex** (INDEX index) const

*获取该常量中第 index 个元素的索引值。 注意索引要有效，否则会出现越界访问。*

- virtual float **getFloat** (INDEX index) const

*获取此常量中第 index 个元素的浮点值。 注意索引要有效，否则会出现越界访问。*

- virtual double **getDouble** (INDEX index) const

*获取此常量中第 index 个元素的双精度值。 注意索引要有效，否则会出现越界访问。*

- virtual string **getString** (INDEX index) const

*获取此常量中第 index 个元素的字符串值。 注意索引要有效，否则会出现越界访问。*

- virtual const DolphinString & **getStringRef** (INDEX index) const

*获取此常量中第 index 个元素的 dolphinString 引用。 注意索引要有效，否则会出现越界访问。*

- virtual const Guid **getInt128** (INDEX index) const

*获取此常量中第 index 个元素的 guid 值。 注意索引要有效，否则会出现越界访问。*

- virtual const unsigned char \* **getBinary** (INDEX index) const

*获取此常量中第 index 个元素的二进制数据。 注意索引要有效，否则会出现越界访问。*

- virtual bool **isNull** (INDEX index) const

*返回第 index 个元素是否为 null。*

- virtual int **getDecimal32** (INDEX index, int scale) const

*获取此常量中第 index 个元素的特定比例的 decimal32 数据。 注意索引要有效，否则会出现越界访问。*

- virtual long long **getDecimal64** (INDEX index, int scale) const

*获取此常量中第 index 个元素的特定比例的 decimal64 数据。 注意索引要有效，否则会出现越界访问。*

- virtual ConstantSP **get** (INDEX index) const

*获取该常量中第 index 个元素的数据。 注意索引要有效，否则会出现越界访问。*

- virtual ConstantSP **get** (INDEX column, INDEX row) const

*根据表或矩阵中的行和列索引获取数据。 注意列索引和行索引都要有效，否则会出现越界访问。*

- virtual ConstantSP **get** (const ConstantSP &index) const

*根据索引获取数据。*

- virtual ConstantSP **get** (INDEX offset, const ConstantSP &index) const

*根据索引获取数据。*

- virtual ConstantSP **getColumn** (INDEX index) const

*根据索引获取表或矩阵中指定列的数据。 注意索引要有效，否则会出现越界访问。*

- virtual ConstantSP **getRow** (INDEX index) const

*根据索引获取矩阵中指定行的数据。 注意索引要有效，否则会出现越界访问。*

- virtual ConstantSP **getItem** (INDEX index) const

*根据索引获取指定项的数据。 请注意，第 index 列将在表或矩阵中返回，并且 index 应该有效。*

- virtual ConstantSP **getItems** (const ConstantSP &index) const

*根据索引获取指定item的数据。 如果索引超出范围，则对应返回空值。 请注意，索引列将在表或矩阵中返回。*

- virtual ConstantSP **getWindow** (INDEX colStart, int colLength, INDEX rowStart, int rowLength) const

*从这个常量中获取一个子表。 注意，子表是原表的副本。*

- virtual ConstantSP **getSlice** (const ConstantSP &rowIndex, const ConstantSP &colIndex) const

*根据rowIndex 和colIndex 获取切片数据。 如果索引超出范围，则对应返回空值。*

- virtual ConstantSP **getRowLabel** () const
- virtual ConstantSP **getColumnLabel** () const
- virtual bool **isNull** (INDEX start, int len, char \*buf) const

*判断从start到(start + len -1)的数据是否为null*

- virtual bool **isValid** (INDEX start, int len, char \*buf) const

*判断从start到(start + len -1)的数据是否有效。 所有的param和return都和isNull一样。*

- virtual bool **getBool** (INDEX start, int len, char \*buf) const

*获取从 start 到 (start + len -1) 的布尔数据。*

- virtual bool **getChar** (INDEX start, int len, char \*buf) const

*获取从start到(start + len -1)的字符数据。*

- virtual bool **getShort** (INDEX start, int len, short \*buf) const

*获取从start到(start + len -1)的数据的短值。*

- virtual bool **getInt** (INDEX start, int len, int \*buf) const

*获取从start到(start + len -1)的数据的int值。*

- virtual bool **getLong** (INDEX start, int len, long long \*buf) const

*获取数据从start到(start + len -1)的long long值。*

- virtual bool **getIndex** (INDEX start, int len, INDEX \*buf) const

*获取数据从start到(start + len -1)的索引值。*

- virtual bool **getFloat** (INDEX start, int len, float \*buf) const

*获取数据从start到(start + len -1)的浮点值。*

- virtual bool **getDouble** (INDEX start, int len, double \*buf) const

*获取从 start 到 (start + len -1) 的数据的双精度值。*

- virtual bool **getSymbol** (INDEX start, int len, int \*buf, SymbolBase \*symBase, bool insertIfNotThere) const

*获取从 start 到 (start + len -1) 的符号数据。*

- virtual bool **getString** (INDEX start, int len, DolphinString \*\*buf) const

*获取从 start 到 (start + len -1) 的 DolphinString 数据。*

- virtual bool **getString** (INDEX start, int len, char \*\*buf) const

*获取从start到(start + len -1)的字符串数据。*

- virtual bool **getBinary** (INDEX start, int len, int unitLength, unsigned char \*buf) const

*获取从 start 到 (start + len -1) 的二进制数据。*

- virtual bool **getDecimal32** (INDEX start, int len, int scale, int \*buf) const

*获取从 start 到 (start + len -1) 的特定比例的 decimal32 数据。*

- virtual bool **getDecimal64** (INDEX start, int len, int scale, long long \*buf) const

*获取从 start 到 (start + len -1) 的特定比例的 decimal64 数据。*

- virtual bool **isNull** (INDEX \*indices, int len, char \*buf) const

*根据索引判断数据是否为空。*

- virtual bool **isValid** (INDEX \*indices, int len, char \*buf) const

*根据索引判断数据是否有效。*

- virtual bool **getBool** (INDEX \*indices, int len, char \*buf) const

*根据索引获取布尔数据。*

- virtual bool **getChar** (INDEX \*indices, int len, char \*buf) const

*根据索引获取字符数据。*

- virtual bool **getShort** (INDEX \*indices, int len, short \*buf) const

*根据索引获取数据的短值。*

- virtual bool **getInt** (INDEX \*indices, int len, int \*buf) const

*根据索引获取数据的int值。*

- virtual bool **getLong** (INDEX \*indices, int len, long long \*buf) const

*根据索引获取数据的 long long 值。*

- virtual bool **getIndex** (INDEX \*indices, int len, INDEX \*buf) const

*根据索引获取数据的索引值。*

- virtual bool **getFloat** (INDEX \*indices, int len, float \*buf) const

*根据索引获取数据的浮点值。*

- virtual bool **getDouble** (INDEX \*indices, int len, double \*buf) const

*根据索引获取数据的双精度值。*

- virtual bool **getSymbol** (INDEX \*indices, int len, int \*buf, SymbolBase \*symBase, bool insertIfNotThere) const

*根据索引获取符号数据。*

- virtual bool **getString** (INDEX \*indices, int len, DolphinString \*\*buf) const

*根据索引获取 DolphinString 数据。*

- virtual bool **getString** (INDEX \*indices, int len, char \*\*buf) const

*根据索引获取字符串数据。*

- virtual bool **getBinary** (INDEX \*indices, int len, int unitLength, unsigned char \*buf) const

*根据索引获取二进制数据。*

- virtual bool **getDecimal32** (INDEX \*indices, int len, int scale, int \*buf) const

*根据索引获取特定scale的decimal32数据。*

- virtual bool **getDecimal64** (INDEX \*indices, int len, int scale, long long \*buf) const

*根据索引获取特定比例的decimal64数据。*

- virtual const char \* **getBoolConst** (INDEX start, int len, char \*buf) const

*获取从 start 到 (start + len -1) 的布尔数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const char \* **getCharConst** (INDEX start, int len, char \*buf) const

*获取从 start 到 (start + len -1) 的字符值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const short \* **getShortConst** (INDEX start, int len, short \*buf) const

*获取从 start 到 (start + len -1) 的短值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const int \* **getIntConst** (INDEX start, int len, int \*buf) const

*获取从 start 到 (start + len -1) 的 int 值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const long long \* **getLongConst** (INDEX start, int len, long long \*buf) const

*获取从 start 到 (start + len -1) 的 long long 值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const INDEX \* **getIndexConst** (INDEX start, int len, INDEX \*buf) const

*获取从start到(start + len -1)的索引值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const float \* **getFloatConst** (INDEX start, int len, float \*buf) const

*获取浮点值开始到 (start + len -1)。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const double \* **getDoubleConst** (INDEX start, int len, double \*buf) const

*获取从 start 到 (start + len -1) 的双精度值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const int \* **getSymbolConst** (INDEX start, int len, int \*buf, SymbolBase \*symBase, bool insertIfNotThere) const

*获取从 start 到 (start + len -1) 的符号数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual DolphinString \*\* **getStringConst** (INDEX start, int len, DolphinString \*\*buf) const

*获取从 start 到 (start + len -1) 的 DolphinString 数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual char \*\* **getStringConst** (INDEX start, int len, char \*\*buf) const

*获取从start到(start + len -1)的字符串数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const unsigned char \* **getBinaryConst** (INDEX start, int len, int unitLength, unsigned char \*buf) const

*获取从 start 到 (start + len -1) 的二进制数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const int \* **getDecimal32Const** (INDEX start, int len, int scale, int \*buf) const

*获取从 start 到 (start + len -1) 的特定刻度的 decimal32 数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const long long \* **getDecimal64Const** (INDEX start, int len, int scale, long long \*buf) const

*获取从 start 到 (start + len -1) 的特定比例的 decimal64 数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual char \* **getBoolBuffer** (INDEX start, int len, char \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setBool(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual char \* **getCharBuffer** (INDEX start, int len, char \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setChar(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual short \* **getShortBuffer** (INDEX start, int len, short \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setShort(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual int \* **getIntBuffer** (INDEX start, int len, int \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setInt(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual long long \* **getLongBuffer** (INDEX start, int len, long long \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setLong(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual INDEX \* **getIndexBuffer** (INDEX start, int len, INDEX \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setIndex(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual float \* **getFloatBuffer** (INDEX start, int len, float \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setFloat(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual double \* **getDoubleBuffer** (INDEX start, int len, double \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setDouble(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual unsigned char \* **getBinaryBuffer** (INDEX start, int len, int unitLength, unsigned char \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setBinary(start, len, unitLength, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual void \* **getDataBuffer** (INDEX start, int len, void \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setData(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual int \* **getDecimal32Buffer** (INDEX start, int len, int scale, int \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setDecimal32(start, len, scale, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual long long \* **getDecimal64Buffer** (INDEX start, int len, int scale, long long \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setDecimal64(start, len, scale, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual IO_ERR **serialize** (Heap \*pHeap, const ByteArrayCodeBufferSP &buffer) const

*使用类型代码或数据源序列化常量以进行缓冲。*

- virtual IO_ERR **serialize** (const ByteArrayCodeBufferSP &buffer) const

*将常量序列化到缓冲区。*

- virtual int **serialize** (char \*buf, int bufSize, INDEX indexStart, int offset, int &numElement, int &partial) const

*将常量序列化到缓冲区。 请注意，文字常量可能会被部分序列化。*

- virtual int **serialize** (char \*buf, int bufSize, INDEX indexStart, int offset, int targetNumElement, int &numElement, int &partial) const

*将常量序列化到缓冲区。 请注意，文字常量可能会被部分序列化。*

- virtual IO_ERR **deserialize** (DataInputStream \*in, INDEX indexStart, int offset, INDEX targetNumElement, INDEX &numElement, int &partial)

*从 DataInputStream 中读取数据并将其反序列化为常量。 请注意，文字常量可能会被部分反序列化。*

- virtual void **nullFill** (const ConstantSP &val)

*用val填充空值。*

- virtual void **setBool** (INDEX index, char val)

*将第 index 个元素设置为 val。*

- virtual void **setChar** (INDEX index, char val)

*将第 index 个元素设置为 val。*

- virtual void **setShort** (INDEX index, short val)

*将第 index 个元素设置为 val。*

- virtual void **setInt** (INDEX index, int val)

*将第 index 个元素设置为 val。*

- virtual void **setLong** (INDEX index, long long val)

*将第 index 个元素设置为 val。*

- virtual void **setIndex** (INDEX index, INDEX val)

*将第 index 个元素设置为 val。*

- virtual void **setFloat** (INDEX index, float val)

*将第 index 个元素设置为 val。*

- virtual void **setDouble** (INDEX index, double val)

*将第 index 个元素设置为 val。*

- virtual void **setString** (INDEX index, const DolphinString &val)

*将第 index 个元素设置为 val。*

- virtual void **setBinary** (INDEX index, int unitLength, const unsigned char \*val)

*将第 index 个元素设置为 val。*

- virtual void **setNull** (INDEX index)

*将第 index 个元素设置为 null。*

- virtual void **setDecimal32** (INDEX index, int scale, int val)

*将第 index 个元素设置为特定比例的 val。*

- virtual void **setDecimal64** (INDEX index, int scale, long long val)

*将第 index 个元素设置为特定比例的 val。*

- virtual bool **set** (INDEX index, const ConstantSP &value, INDEX valueIndex)

*将索引指定的单元格值替换为 valueIndex 指定的新值。*

- virtual bool **set** (INDEX index, const ConstantSP &value)

*用新值替换索引指定的单元格值。*

- virtual bool **set** (INDEX column, INDEX row, const ConstantSP &value)

*用新值替换由列和行索引指定的单元格值。*

- virtual bool **set** (const ConstantSP &index, const ConstantSP &value)

*用值替换索引指定的单元格值。*

- virtual bool **set** (const ConstantSP &index, const ConstantSP &value, const ConstantSP &valueIndex)

*将索引指定的单元格值替换为 valueIndex 指定的新值。*

- virtual bool **setNonNull** (const ConstantSP &index, const ConstantSP &value)

*用值替换索引指定的单元格值。 请注意，setNonNull 在替换数据时将忽略空值。*

- virtual bool **setItem** (INDEX index, const ConstantSP &value)

*用新值替换索引项。 请注意，第 index 列将替换为矩阵中的新值。*

- virtual bool **setColumn** (INDEX index, const ConstantSP &value)

*用新值替换索引列。*

- virtual void **setRowLabel** (const ConstantSP &label)
- virtual void **setColumnLabel** (const ConstantSP &label)
- virtual bool **reshape** (INDEX cols, INDEX **rows**)

*用新的列和行重塑矩阵。 请注意，如果列或行小于当前大小，则矩阵将被截断， 否则附加 void。*

- virtual bool **assign** (const ConstantSP &value)

*将基础数据替换为值。*

- virtual bool **setBool** (INDEX start, int len, const char \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getBoolBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setChar** (INDEX start, int len, const char \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getCharBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setShort** (INDEX start, int len, const short \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getShortBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setInt** (INDEX start, int len, const int \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getIntBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setLong** (INDEX start, int len, const long long \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getLongBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setIndex** (INDEX start, int len, const INDEX \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getIndexBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setFloat** (INDEX start, int len, const float \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getFloatBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setDouble** (INDEX start, int len, const double \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getDoubleBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setString** (INDEX start, int len, const string \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getStringBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setString** (INDEX start, int len, char \*\*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setString** (INDEX start, int len, const DolphinString \*\*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setBinary** (INDEX start, int len, int unitLength, const unsigned char \*buf)

*用buf设置从start到(start + len -1)的二进制数据。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setData** (INDEX start, int len, void \*buf)

*用buf设置从start到(start + len -1)的数据。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setDecimal32** (INDEX start, int len, int scale, const int \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getDecimal32Buffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setDecimal64** (INDEX start, int len, int scale, const long long \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getDecimal64Buffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **add** (INDEX start, INDEX length, long long inc)

*将 inc 添加到从 start 到 (start + length -1) 的基础数据中。*

- virtual bool **add** (INDEX start, INDEX length, double inc)

*将 inc 添加到从 start 到 (start + length -1) 的基础数据中。*

- virtual void **validate** ()

*检查类型时间、分钟、秒、纳米时间的验证， 并且无效的元素将被替换为 null。*

- virtual int **compare** (INDEX index, const ConstantSP &target) const

*将第 index 个单元格与常量目标进行比较*

- virtual bool **getNullFlag** () const

*获取空标志。*

- virtual void **setNullFlag** (bool containNull)

*设置空标志。*

- virtual bool **hasNull** ()

*返回这个常量是否有空值。*

- virtual bool **hasNull** (INDEX start, INDEX length)

*返回这个常量从start到(start + length -1)是否有空值。*

- virtual bool **sizeable** () const

*返回此常量是否可以调整大小。*

- virtual bool **copyable** () const

*返回这个常量是否可以被复制。*

- virtual INDEX **size** () const

*返回此常量的大小。*

- virtual INDEX **itemCount** () const

*返回此常量的项目数。 请注意，矩阵将返回列数。*

- virtual INDEX **rows** () const

*返回行数。*

- virtual INDEX **uncompressedRows** () const

*返回未压缩的行数。*

- virtual INDEX **columns** () const

*返回列数。*

- virtual ConstantSP **getMember** (const ConstantSP &key) const

*根据键从字典中获取数据，空值将用于填充任何缺失的数据。*

- virtual ConstantSP **keys** () const

*从字典中获取所有键。*

- virtual ConstantSP **values** () const

*从字典中获取所有值。*

- virtual bool **hasMethod** (const string &name) const

*返回指定的方法是否在当前对象所属的类中。*

- virtual bool **hasOperator** (const string &name) const

*返回指定的操作符是否在当前对象所属的类中。 运算符可以是对象的方法或属性。*

- virtual FunctionDefSP **getMethod** (const string &name) const

*返回当前对象所属类的指定方法。*

- virtual FunctionDefSP **getOperator** (const string &name) const

*返回当前对象所属类的指定运算符。 运算符可以是对象的方法或属性。*

- virtual ConstantSP **getMember** (const string &key) const

*根据key从字典中获取数据。*

- virtual long long **releaseMemory** (long long target, bool &satisfied)

*如果使用的内存不小于目标，则释放此常量的内存。*

- virtual long long **getAllocatedMemory** () const

*获取此常量的分配内存（以字节为单位）。*

- virtual DATA_TYPE **getRawType** () const =0

*获取此常量的原始数据类型。*

- virtual int **getExtraParamForType** () const

*返回这个常量的额外属性。*

- virtual ConstantSP **getInstance** () const =0

*获取此常量的副本，实际数据除外。*

- virtual ConstantSP **getValue** () const =0

*获取此常量的副本。*

- virtual ConstantSP **getValue** (Heap \*pHeap)

*从指定的堆中获取此常量的副本。*

- virtual ConstantSP **getReference** (Heap \*pHeap)

*如果此常量是临时的，则返回其本身，否则返回此常量的副本。*

- virtual OBJECT_TYPE **getObjectType** () const

*返回此常量的对象类型。*

- virtual SymbolBaseSP **getSymbolBase** () const

*从具有符号类型的常量中获取 SymbolBase。*

- virtual void **contain** (const ConstantSP &target, const ConstantSP &resultSP) const

*返回目标常量是否存在于当前集合或字典键中。 并将返回的布尔值设置为resultSP。*

- virtual bool **isFastMode** () const

*返回此向量是否为规则向量，即所有数据是否连续。*

- virtual void \* **getDataArray** () const

*获取这个常量的底层数据数组。如果数据不连续，则返回 0。*

- virtual void \*\* **getDataSegment** () const

*获取该常量的底层数据数组段。如果数据是连续的，则返回 0。*

- virtual bool **isIndexArray** () const

*返回这个常量是否是索引数组。*

- virtual INDEX \* **getIndexArray** () const

*获取该常量的底层索引数据数组。如果数据不连续，则返回 0。*

- virtual bool **isHugeIndexArray** () const

*返回这个向量是否是一个巨大的索引向量，即所有数据都不连续。*

- virtual INDEX \*\* **getHugeIndexArray** () const

*获取该常量的底层索引数据数组段。如果数据是连续的，则返回 0。*

- virtual int **getSegmentSize** () const

*获取此常量的基础数据数组段的大小。*

- virtual int **getSegmentSizeInBit** () const

*获取数据段的大小（以位为单位）。*

- virtual bool **containNotMarshallableObject** () const

*返回此常量是否包含不可编组的对象。*

### 静态 Public 属性

- static DolphinString **DEMPTY**

*一个空的 DolphinString。*

- static string **EMPTY**

*一个空字符串。*

- static string **NULL_STR**

*此字符串的值为“NULL”。*

### Protected 成员函数

- void **setType** (DATA_TYPE dt)
- void **setCategory** (DATA_CATEGORY dc)
- void **setTypeAndCategory** (DATA_TYPE dt, DATA_CATEGORY dc)

### 成员函数说明

#### virtual bool Constant::add (INDEX *start*, INDEX *length*, double *inc*)\[inline\], \[virtual\]

将 inc 添加到从 start 到 (start + length -1) 的基础数据中。

##### 返回

> 成功则为真，否则为假。

#### virtual bool Constant::add (INDEX *start*, INDEX *length*, long long *inc*)\[inline\], \[virtual\]

将 inc 添加到从 start 到 (start + length -1) 的基础数据中。

##### 返回

> 成功则为真，否则为假。

#### virtual bool Constant::assign (const ConstantSP & *value*)\[inline\], \[virtual\]

将基础数据替换为值。

##### 参数

|           |                                                       |
| --------- | ----------------------------------------------------- |
| *value* | 要赋值的数据。请注意，值的大小必须与 这个常量的大小。 |

##### 返回

> 如果赋值成功则为真，否则为假。

#### virtual INDEX Constant::columns () const\[inline\], \[virtual\]

返回列数。

被 **Table** 重载.

#### virtual int Constant::compare (INDEX *index*, const ConstantSP & *target*) const\[inline\], \[virtual\]

将第 index 个单元格与常量目标进行比较

> @参数索引

##### 参数

|          |  |
| -------- | - |
| *目标* |  |

##### 返回

> 0: 如果索引单元等于目标 1: 如果索引单元格大于目标 -1: 如果索引单元格小于目标

#### virtual void Constant::contain (const ConstantSP & *target*, const ConstantSP & *resultSP*) const\[inline\], \[virtual\]

返回目标常量是否存在于当前集合或字典键中。 并将返回的布尔值设置为resultSP。

##### 参数

|                                                                |                              |
| -------------------------------------------------------------- | ---------------------------- |
| *target*                                                     | 目标常量，可以是标量或向量。 |
| *resultSP：将设置结果布尔值的结果常量，应至少与目标一样长。* |                              |

#### virtual IO_ERR Constant::deserialize (DataInputStream \* *in*, INDEX *indexStart*, int *offset*, INDEX *targetNumElement*, INDEX & *numElement*, int & *partial*)\[inline\], \[virtual\]

从 DataInputStream 中读取数据并将其反序列化为常量。 请注意，文字常量可能会被部分反序列化。

##### 参数

|                                                          |                                     |
| -------------------------------------------------------- | ----------------------------------- |
| *in*                                                   | 一个用于读取数据的DataInputStream。 |
| *indexStart：要开始反序列化的元素的索引。*             |                                     |
| *offset：第indexStart元素中已经部分反序列化的字节数。* |                                     |
| *targetNumElement：表示要反序列化的元素个数。*         |                                     |
| *numElement*                                           | 返回已经完全反序列化的元素个数。    |
| *partial：返回已部分反序列化的元素的字节数。*          |                                     |

##### 返回

> 反序列化的结果。

#### virtual ConstantSP Constant::get (const ConstantSP & *index*) const\[inline\], \[virtual\]

根据索引获取数据。

##### 参数

|           |                                                                                   |
| --------- | --------------------------------------------------------------------------------- |
| *index* | 索引给出了要获取的数据的索引。如果索引超出范围， 即负数或大于大小，对应返回空值。 |

##### 返回

> ConstantSP：数据。

被 **Vector**, **Table**, 以及 **Table** 重载.

#### virtual ConstantSP Constant::get (INDEX *column*, INDEX *row*) const\[inline\], \[virtual\]

根据表或矩阵中的行和列索引获取数据。 注意列索引和行索引都要有效，否则会出现越界访问。

##### 参数

|                      |          |
| -------------------- | -------- |
| *column：列索引。* |          |
| *row*              | 行索引。 |

##### 返回

> ConstantSP：数据。

被 **Vector** 以及 **Table** 重载.

#### virtual ConstantSP Constant::get (INDEX *index*) const\[inline\], \[virtual\]

获取该常量中第 index 个元素的数据。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |

##### 返回

> ConstantSP：数据。

被 **Vector**   , **Table**   , **Table** , 以及 **Vector** 重载.

#### virtual ConstantSP Constant::get (INDEX *offset*, const ConstantSP & *index*) const\[inline\], \[virtual\]

根据索引获取数据。

##### 参数

|                              |                                |
| ---------------------------- | ------------------------------ |
| *offset：索引偏移offset。* |                                |
| *index*                    | 索引给出了要获取的数据的索引。 |

##### 返回

> ConstantSP：数据。

被 **Vector** , 以及 **Table** 重载.

#### virtual long long Constant::getAllocatedMemory () const\[inline\], \[virtual\]

获取此常量的分配内存（以字节为单位）。

##### 返回

> 以字节为单位分配的内存。

被 **Table** 重载.

#### virtual const unsigned char \* Constant::getBinary () const\[inline\], \[virtual\]

获取 int128 标量的二进制数据。

##### 返回

> 指向二进制数据的指针。

#### virtual bool Constant::getBinary (INDEX \* *indices*, int *len*, int *unitLength*, unsigned char \* *buf*) const\[inline\], \[virtual\]

根据索引获取二进制数据。

##### 参数

|                                        |                      |
| -------------------------------------- | -------------------- |
| *indices：要检索的可能乱序的索引。*  |                      |
| *len*                                | 要检索的数据的长度。 |
| *unitLength：该常量数据类型的长度。* |                      |
| *buf*                                | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual const unsigned char \* Constant::getBinary (INDEX *index*) const\[inline\], \[virtual\]

获取此常量中第 index 个元素的二进制数据。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |

##### 返回

> 指向第 index 个元素的二进制数据的指针。

#### virtual bool Constant::getBinary (INDEX *start*, int *len*, int *unitLength*, unsigned char \* *buf*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的二进制数据。

##### 参数

|                                        |                      |
| -------------------------------------- | -------------------- |
| *start*                              | 起始索引。           |
| *len*                                | 要检索的数据的长度。 |
| *unitLength：该常量数据类型的长度。* |                      |
| *buf*                                | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual unsigned char \* Constant::getBinaryBuffer (INDEX *start*, int *len*, int *unitLength*, unsigned char \* *buf*) const\[inline\], \[virtual\]

获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setBinary(start, len, unitLength, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。

##### 参数

|                                        |                                       |
| -------------------------------------- | ------------------------------------- |
| *start*                              | 起始索引。                            |
| *len*                                | 要获取的数据长度。                    |
| *unitLength：该常量数据类型的长度。* |                                       |
| *buf*                                | 至少长度为 len\*unitLength 的缓冲区。 |

##### 返回

> 写入数据的缓冲区。

#### virtual const unsigned char \* Constant::getBinaryConst (INDEX *start*, int *len*, int *unitLength*, unsigned char \* *buf*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的二进制数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。

##### 参数

|                                        |                                       |
| -------------------------------------- | ------------------------------------- |
| *start*                              | 起始索引。                            |
| *len*                                | 要获取的数据长度。                    |
| *unitLength：该常量数据类型的长度。* |                                       |
| *buf*                                | 至少长度为 len\*unitLength 的缓冲区。 |

##### 返回

> 包含所需数据的缓冲区。

#### virtual bool Constant::getBool (INDEX \* *indices*, int *len*, char \* *buf*) const\[inline\], \[virtual\]

根据索引获取布尔数据。

##### 参数

|                                       |                      |
| ------------------------------------- | -------------------- |
| *indices：要检索的可能乱序的索引。* |                      |
| *len*                               | 要检索的数据的长度。 |
| *buf*                               | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual char Constant::getBool (INDEX *index*) const\[inline\], \[virtual\]

获取此常量中第 index 个元素的 bool 值。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |

#### virtual bool Constant::getBool (INDEX *start*, int *len*, char \* *buf*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的布尔数据。

##### 参数

|           |                      |
| --------- | -------------------- |
| *start* | 起始索引。           |
| *len*   | 要检索的数据的长度。 |
| *buf*   | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual char \* Constant::getBoolBuffer (INDEX *start*, int *len*, char \* *buf*) const\[inline\], \[virtual\]

获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setBool(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 写入数据的缓冲区。

#### virtual const char \* Constant::getBoolConst (INDEX *start*, int *len*, char \* *buf*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的布尔数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 包含所需数据的缓冲区。

#### virtual bool Constant::getChar (INDEX \* *indices*, int *len*, char \* *buf*) const\[inline\], \[virtual\]

根据索引获取字符数据。

##### 参数

|                                       |                      |
| ------------------------------------- | -------------------- |
| *indices：要检索的可能乱序的索引。* |                      |
| *len*                               | 要检索的数据的长度。 |
| *buf*                               | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual char Constant::getChar (INDEX *index*) const\[inline\], \[virtual\]

获取此常量中第 index 个元素的 char 值。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |

#### virtual bool Constant::getChar (INDEX *start*, int *len*, char \* *buf*) const\[inline\], \[virtual\]

获取从start到(start + len -1)的字符数据。

##### 参数

|           |                      |
| --------- | -------------------- |
| *start* | 起始索引。           |
| *len*   | 要检索的数据的长度。 |
| *buf*   | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual char \* Constant::getCharBuffer (INDEX *start*, int *len*, char \* *buf*) const\[inline\], \[virtual\]

获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setChar(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 写入数据的缓冲区。

#### virtual const char \* Constant::getCharConst (INDEX *start*, int *len*, char \* *buf*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的字符值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 包含所需数据的缓冲区。

#### virtual ConstantSP Constant::getColumn (INDEX *index*) const\[inline\], \[virtual\]

根据索引获取表或矩阵中指定列的数据。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |          |
| --------- | -------- |
| *index* | 列索引。 |

##### 返回

> ConstantSP：数据。

被 **Table** 重载.

#### virtual void \* Constant::getDataBuffer (INDEX *start*, int *len*, void \* *buf*) const\[inline\], \[virtual\]

获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setData(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 写入数据的缓冲区。

#### virtual bool Constant::getDecimal32 (INDEX \* *indices*, int *len*, int *scale*, int \* *buf*) const\[inline\], \[virtual\]

根据索引获取特定scale的decimal32数据。

##### 参数

|                                       |                      |
| ------------------------------------- | -------------------- |
| *indices：要检索的可能乱序的索引。* |                      |
| *len*                               | 要检索的数据的长度。 |
| *scale*                             | 小数位数。           |
| *buf*                               | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual int Constant::getDecimal32 (INDEX *index*, int *scale*) const\[inline\], \[virtual\]

获取此常量中第 index 个元素的特定比例的 decimal32 数据。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |
| *scale* | 小数位数。                |

##### 返回

> decimal32 的整数表示。

#### virtual bool Constant::getDecimal32 (INDEX *start*, int *len*, int *scale*, int \* *buf*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的特定比例的 decimal32 数据。

##### 参数

|           |                      |
| --------- | -------------------- |
| *start* | 起始索引。           |
| *len*   | 要检索的数据的长度。 |
| *scale* | 小数位数。           |
| *buf*   | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual int \* Constant::getDecimal32Buffer (INDEX *start*, int *len*, int *scale*, int \* *buf*) const\[inline\], \[virtual\]

获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setDecimal32(start, len, scale, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *scale* | 小数位数。                |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 写入数据的缓冲区。

#### virtual const int \* Constant::getDecimal32Const (INDEX *start*, int *len*, int *scale*, int \* *buf*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的特定刻度的 decimal32 数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *scale* | 小数位数。                |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 包含所需数据的缓冲区。

#### virtual bool Constant::getDecimal64 (INDEX \* *indices*, int *len*, int *scale*, long long \* *buf*) const\[inline\], \[virtual\]

根据索引获取特定比例的decimal64数据。

##### 参数

|                                       |                      |
| ------------------------------------- | -------------------- |
| *indices：要检索的可能乱序的索引。* |                      |
| *len*                               | 要检索的数据的长度。 |
| *scale*                             | 小数位数。           |
| *buf*                               | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual long long Constant::getDecimal64 (INDEX *index*, int *scale*) const\[inline\], \[virtual\]

获取此常量中第 index 个元素的特定比例的 decimal64 数据。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |
| *scale* | 小数位数。                |

##### 返回

> decimal64 的整数表示。

#### virtual bool Constant::getDecimal64 (INDEX *start*, int *len*, int *scale*, long long \* *buf*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的特定比例的 decimal64 数据。

##### 参数

|           |                      |
| --------- | -------------------- |
| *start* | 起始索引。           |
| *len*   | 要检索的数据的长度。 |
| *scale* | 小数位数。           |
| *buf*   | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual long long \* Constant::getDecimal64Buffer (INDEX *start*, int *len*, int *scale*, long long \* *buf*) const\[inline\], \[virtual\]

获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setDecimal64(start, len, scale, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *scale* | 小数位数。                |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 写入数据的缓冲区。

#### virtual const long long \* Constant::getDecimal64Const (INDEX *start*, int *len*, int *scale*, long long \* *buf*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的特定比例的 decimal64 数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *scale* | 小数位数。                |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 包含所需数据的缓冲区。

#### virtual bool Constant::getDouble (INDEX \* *indices*, int *len*, double \* *buf*) const\[inline\], \[virtual\]

根据索引获取数据的双精度值。

##### 参数

|                                       |                      |
| ------------------------------------- | -------------------- |
| *indices：要检索的可能乱序的索引。* |                      |
| *len*                               | 要检索的数据的长度。 |
| *buf*                               | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual double Constant::getDouble (INDEX *index*) const\[inline\], \[virtual\]

获取此常量中第 index 个元素的双精度值。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |

#### virtual bool Constant::getDouble (INDEX *start*, int *len*, double \* *buf*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的数据的双精度值。

##### 参数

|           |                      |
| --------- | -------------------- |
| *start* | 起始索引。           |
| *len*   | 要检索的数据的长度。 |
| *buf*   | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual double \* Constant::getDoubleBuffer (INDEX *start*, int *len*, double \* *buf*) const\[inline\], \[virtual\]

获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setDouble(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 写入数据的缓冲区。

#### virtual const double \* Constant::getDoubleConst (INDEX *start*, int *len*, double \* *buf*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的双精度值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 包含所需数据的缓冲区。

#### virtual bool Constant::getFloat (INDEX \* *indices*, int *len*, float \* *buf*) const\[inline\], \[virtual\]

根据索引获取数据的浮点值。

##### 参数

|                                       |                      |
| ------------------------------------- | -------------------- |
| *indices：要检索的可能乱序的索引。* |                      |
| *len*                               | 要检索的数据的长度。 |
| *buf*                               | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual float Constant::getFloat (INDEX *index*) const\[inline\], \[virtual\]

获取此常量中第 index 个元素的浮点值。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |

#### virtual bool Constant::getFloat (INDEX *start*, int *len*, float \* *buf*) const\[inline\], \[virtual\]

获取数据从start到(start + len -1)的浮点值。

##### 参数

|           |                      |
| --------- | -------------------- |
| *start* | 起始索引。           |
| *len*   | 要检索的数据的长度。 |
| *buf*   | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual float \* Constant::getFloatBuffer (INDEX *start*, int *len*, float \* *buf*) const\[inline\], \[virtual\]

获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setFloat(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 写入数据的缓冲区。

#### virtual const float \* Constant::getFloatConst (INDEX *start*, int *len*, float \* *buf*) const\[inline\], \[virtual\]

获取浮点值开始到 (start + len -1)。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 包含所需数据的缓冲区。

#### virtual bool Constant::getIndex (INDEX \* *indices*, int *len*, INDEX \* *buf*) const\[inline\], \[virtual\]

根据索引获取数据的索引值。

##### 参数

|                                       |                      |
| ------------------------------------- | -------------------- |
| *indices：要检索的可能乱序的索引。* |                      |
| *len*                               | 要检索的数据的长度。 |
| *buf*                               | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual INDEX Constant::getIndex (INDEX *index*) const\[inline\], \[virtual\]

获取该常量中第 index 个元素的索引值。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |

#### virtual bool Constant::getIndex (INDEX *start*, int *len*, INDEX \* *buf*) const\[inline\], \[virtual\]

获取数据从start到(start + len -1)的索引值。

##### 参数

|           |                      |
| --------- | -------------------- |
| *start* | 起始索引。           |
| *len*   | 要检索的数据的长度。 |
| *buf*   | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual INDEX \* Constant::getIndexBuffer (INDEX *start*, int *len*, INDEX \* *buf*) const\[inline\], \[virtual\]

获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setIndex(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 写入数据的缓冲区。

#### virtual const INDEX \* Constant::getIndexConst (INDEX *start*, int *len*, INDEX \* *buf*) const\[inline\], \[virtual\]

获取从start到(start + len -1)的索引值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 包含所需数据的缓冲区。

#### virtual ConstantSP Constant::getInstance () const\[pure virtual\]

获取此常量的副本，实际数据除外。

在 **Vector** 内被实现.

#### virtual bool Constant::getInt (INDEX \* *indices*, int *len*, int \* *buf*) const\[inline\], \[virtual\]

根据索引获取数据的int值。

##### 参数

|                                       |                      |
| ------------------------------------- | -------------------- |
| *indices：要检索的可能乱序的索引。* |                      |
| *len*                               | 要检索的数据的长度。 |
| *buf*                               | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual int Constant::getInt (INDEX *index*) const\[inline\], \[virtual\]

获取此常量中第 index 个元素的 int 值。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |

#### virtual bool Constant::getInt (INDEX *start*, int *len*, int \* *buf*) const\[inline\], \[virtual\]

获取从start到(start + len -1)的数据的int值。

##### 参数

|           |                      |
| --------- | -------------------- |
| *start* | 起始索引。           |
| *len*   | 要检索的数据的长度。 |
| *buf*   | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual const Guid Constant::getInt128 (INDEX *index*) const\[inline\], \[virtual\]

获取此常量中第 index 个元素的 guid 值。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |

#### virtual int \* Constant::getIntBuffer (INDEX *start*, int *len*, int \* *buf*) const\[inline\], \[virtual\]

获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setInt(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 写入数据的缓冲区。

#### virtual const int \* Constant::getIntConst (INDEX *start*, int *len*, int \* *buf*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的 int 值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 包含所需数据的缓冲区。

#### virtual ConstantSP Constant::getItem (INDEX *index*) const\[inline\], \[virtual\]

根据索引获取指定项的数据。 请注意，第 index 列将在表或矩阵中返回，并且 index 应该有效。

##### 参数

|           |            |
| --------- | ---------- |
| *index* | 项目索引。 |

##### 返回

> ConstantSP：数据。

#### virtual ConstantSP Constant::getItems (const ConstantSP & *index*) const\[inline\], \[virtual\]

根据索引获取指定item的数据。 如果索引超出范围，则对应返回空值。 请注意，索引列将在表或矩阵中返回。

##### 参数

|           |                                |
| --------- | ------------------------------ |
| *index* | 索引给出了要获取的数据的索引。 |

##### 返回

> ConstantSP：数据。

#### virtual ConstantSP Constant::getIterator (const ConstantSP & *self*) const\[inline\], \[virtual\]

获取这个常量本身的迭代器。

被 **Vector** , 以及 **Table** 重载.

#### virtual bool Constant::getLong (INDEX \* *indices*, int *len*, long long \* *buf*) const\[inline\], \[virtual\]

根据索引获取数据的 long long 值。

##### 参数

|                                       |                      |
| ------------------------------------- | -------------------- |
| *indices：要检索的可能乱序的索引。* |                      |
| *len*                               | 要检索的数据的长度。 |
| *buf*                               | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual long long Constant::getLong (INDEX *index*) const\[inline\], \[virtual\]

获取此常量中第 index 个元素的 long 值。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |

#### virtual bool Constant::getLong (INDEX *start*, int *len*, long long \* *buf*) const\[inline\], \[virtual\]

获取数据从start到(start + len -1)的long long值。

##### 参数

|           |                      |
| --------- | -------------------- |
| *start* | 起始索引。           |
| *len*   | 要检索的数据的长度。 |
| *buf*   | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual long long \* Constant::getLongBuffer (INDEX *start*, int *len*, long long \* *buf*) const\[inline\], \[virtual\]

获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setLong(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 写入数据的缓冲区。

#### virtual const long long \* Constant::getLongConst (INDEX *start*, int *len*, long long \* *buf*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的 long long 值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 包含所需数据的缓冲区。

#### virtual ConstantSP Constant::getMember (const ConstantSP & *key*) const\[inline\], \[virtual\]

根据键从字典中获取数据，空值将用于填充任何缺失的数据。

##### 参数

|                                               |  |
| --------------------------------------------- | - |
| *key：应该检索数据的键，可以是标量或向量。* |  |

##### 返回

> 如果键是标量，则值将是标量。 如果键是一个向量，那么值将是一个向量。

被 **Table** 重载.

#### virtual ConstantSP Constant::getMember (const string & *key*) const\[inline\], \[virtual\]

根据key从字典中获取数据。

##### 返回

> ConstantSP: 数据

#### virtual FunctionDefSP Constant::getMethod (const string & *name*) const\[inline\], \[virtual\]

返回当前对象所属类的指定方法。

##### 参数

|          |              |
| -------- | ------------ |
| *name* | 方法的名称。 |

##### 返回

> 函数定义。调用返回方法时第一个参数必须是对象本身。

#### virtual bool Constant::getNullFlag () const\[inline\], \[virtual\]

获取空标志。

##### 返回

> true: 这个常量\*\*可能\*\*包含空值
>
> false: 这个常量不能包含空值

#### virtual FunctionDefSP Constant::getOperator (const string & *name*) const\[inline\], \[virtual\]

返回当前对象所属类的指定运算符。 运算符可以是对象的方法或属性。

##### 参数

|          |                |
| -------- | -------------- |
| *name* | 操作者的名字。 |

##### 返回

> 函数定义。调用返回的运算符时，第一个参数必须是对象本身。

#### virtual DATA_TYPE Constant::getRawType () const\[pure virtual\]

获取此常量的原始数据类型。

##### 返回

> 原始类型。

在 **Table** 内被实现.

#### virtual ConstantSP Constant::getReference (Heap \* *pHeap*)\[inline\], \[virtual\]

如果此常量是临时的，则返回其本身，否则返回此常量的副本。

##### 参数

|           |  |
| --------- | - |
| *pHeap* |  |

#### virtual ConstantSP Constant::getRow (INDEX *index*) const\[inline\], \[virtual\]

根据索引获取矩阵中指定行的数据。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |          |
| --------- | -------- |
| *index* | 行索引。 |

##### 返回

> ConstantSP：数据。

#### virtual string Constant::getScript () const\[inline\], \[virtual\]

返回这个常量的描述。

被 **Vector** , 以及 **Table** 重载.

#### virtual string Constant::getScript (Heap \* *heap*) const\[inline\], \[virtual\]

根据堆中的会话类型返回这个常量的描述。

##### 参数

|          |                        |
| -------- | ---------------------- |
| *heap* | 堆表示不同形式的结果。 |

#### virtual bool Constant::getShort (INDEX \* *indices*, int *len*, short \* *buf*) const\[inline\], \[virtual\]

根据索引获取数据的短值。

##### 参数

|                                       |                      |
| ------------------------------------- | -------------------- |
| *indices：要检索的可能乱序的索引。* |                      |
| *len*                               | 要检索的数据的长度。 |
| *buf*                               | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual short Constant::getShort (INDEX *index*) const\[inline\], \[virtual\]

获取此常量中第 index 个元素的短值。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |

#### virtual bool Constant::getShort (INDEX *start*, int *len*, short \* *buf*) const\[inline\], \[virtual\]

获取从start到(start + len -1)的数据的短值。

##### 参数

|           |                      |
| --------- | -------------------- |
| *start* | 起始索引。           |
| *len*   | 要检索的数据的长度。 |
| *buf*   | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual short \* Constant::getShortBuffer (INDEX *start*, int *len*, short \* *buf*) const\[inline\], \[virtual\]

获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setShort(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 写入数据的缓冲区。

#### virtual const short \* Constant::getShortConst (INDEX *start*, int *len*, short \* *buf*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的短值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 包含所需数据的缓冲区。

#### virtual ConstantSP Constant::getSlice (const ConstantSP & *rowIndex*, const ConstantSP & *colIndex*) const\[inline\], \[virtual\]

根据rowIndex 和colIndex 获取切片数据。 如果索引超出范围，则对应返回空值。

##### 参数

|                        |                                 |
| ---------------------- | ------------------------------- |
| *rowIndex：rowIndex* | 给出要获取的行的索引。          |
| *colIndex*           | colIndex 给出要获取的列的索引。 |

##### 返回

> ConstantSP：切片数据。

#### virtual string Constant::getString () const\[inline\], \[virtual\]

获取此常量的字符串版本。 这个函数对调试很有帮助。

被 **Vector** , 以及 **Table** 重载.

#### virtual string Constant::getString (Heap \* *heap*) const\[inline\], \[virtual\]

根据堆中的会话类型获取此常量的字符串版本。 这个函数对调试很有帮助。

##### 参数

|          |                        |
| -------- | ---------------------- |
| *heap* | 堆表示不同形式的结果。 |

被 **Vector** 重载.

#### virtual bool Constant::getString (INDEX \* *indices*, int *len*, char \*\* *buf*) const\[inline\], \[virtual\]

根据索引获取字符串数据。

##### 参数

|                                       |                      |
| ------------------------------------- | -------------------- |
| *indices：要检索的可能乱序的索引。* |                      |
| *len*                               | 要检索的数据的长度。 |
| *buf*                               | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual bool Constant::getString (INDEX \* *indices*, int *len*, DolphinString \*\* *buf*) const\[inline\], \[virtual\]

根据索引获取 DolphinString 数据。

##### 参数

|                                       |                      |
| ------------------------------------- | -------------------- |
| *indices：要检索的可能乱序的索引。* |                      |
| *len*                               | 要检索的数据的长度。 |
| *buf*                               | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual string Constant::getString (INDEX *index*) const\[inline\], \[virtual\]

获取此常量中第 index 个元素的字符串值。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |

被 **Vector** , 以及 **Table** 重载.

#### virtual bool Constant::getString (INDEX *start*, int *len*, char \*\* *buf*) const\[inline\], \[virtual\]

获取从start到(start + len -1)的字符串数据。

##### 参数

|           |                      |
| --------- | -------------------- |
| *start* | 起始索引。           |
| *len*   | 要检索的数据的长度。 |
| *buf*   | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual bool Constant::getString (INDEX *start*, int *len*, DolphinString \*\* *buf*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的 DolphinString 数据。

##### 参数

|           |                      |
| --------- | -------------------- |
| *start* | 起始索引。           |
| *len*   | 要检索的数据的长度。 |
| *buf*   | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual char \*\* Constant::getStringConst (INDEX *start*, int *len*, char \*\* *buf*) const\[inline\], \[virtual\]

获取从start到(start + len -1)的字符串数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 包含所需数据的缓冲区。

#### virtual DolphinString \*\* Constant::getStringConst (INDEX *start*, int *len*, DolphinString \*\* *buf*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的 DolphinString 数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要获取的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 包含所需数据的缓冲区。

#### virtual const DolphinString & Constant::getStringRef (INDEX *index*) const\[inline\], \[virtual\]

获取此常量中第 index 个元素的 dolphinString 引用。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |

#### virtual bool Constant::getSymbol (INDEX \* *indices*, int *len*, int \* *buf*, SymbolBase \* *symBase*, bool *insertIfNotThere*) const\[inline\], \[virtual\]

根据索引获取符号数据。

##### 参数

|                      |                                                          |
| -------------------- | -------------------------------------------------------- |
| *start*            | 起始索引。                                               |
| *len*              | 要检索的数据的长度。                                     |
| *buf*              | 结果存储在buf中。                                        |
| *symBase*          | SymbolBase 是一个从字符串到整数的映射。                  |
| *insertIfNotThere* | 是否应该将在 Symbase 中找不到的字符串插入到 Symbase 中。 |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual bool Constant::getSymbol (INDEX *start*, int *len*, int \* *buf*, SymbolBase \* *symBase*, bool *insertIfNotThere*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的符号数据。

##### 参数

|                      |                                                          |
| -------------------- | -------------------------------------------------------- |
| *start*            | 起始索引。                                               |
| *len*              | 要检索的数据的长度。                                     |
| *buf*              | 结果存储在buf中。                                        |
| *symBase*          | symbolBase 是一个从字符串到整数的映射。                  |
| *insertIfNotThere* | 是否应该将在 Symbase 中找不到的字符串插入到 Symbase 中。 |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual SymbolBaseSP Constant::getSymbolBase () const\[inline\], \[virtual\]

从具有符号类型的常量中获取 SymbolBase。

##### 返回

> 符号库。

#### virtual const int \* Constant::getSymbolConst (INDEX *start*, int *len*, int \* *buf*, SymbolBase \* *symBase*, bool *insertIfNotThere*) const\[inline\], \[virtual\]

获取从 start 到 (start + len -1) 的符号数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。

##### 参数

|                      |                                                          |
| -------------------- | -------------------------------------------------------- |
| *start*            | 起始索引。                                               |
| *len*              | 要获取的数据长度。                                       |
| *buf*              | 至少长度为 len 的缓冲区。                                |
| *symBase*          | SymbolBase 是一个从字符串到整数的映射。                  |
| *insertIfNotThere* | 是否应该将在 symbase 中找不到的字符串插入到 symbase 中。 |

##### 返回

> 包含所需数据的缓冲区。

#### virtual ConstantSP Constant::getValue () const\[pure virtual\]

获取此常量的副本。

在 **Table** 内被实现.

#### virtual ConstantSP Constant::getValue (Heap \* *pHeap*)\[inline\], \[virtual\]

从指定的堆中获取此常量的副本。

##### 参数

|           |  |
| --------- | - |
| *pHeap* |  |

#### virtual ConstantSP Constant::getWindow (INDEX *colStart*, int *colLength*, INDEX *rowStart*, int *rowLength*) const\[inline\], \[virtual\]

从这个常量中获取一个子表。 注意，子表是原表的副本。

##### 参数

|                                            |                                    |
| ------------------------------------------ | ---------------------------------- |
| *colStart：开始列的索引，应该是有效的。* |                                    |
| *colLength*                              | 列长度。可以为负数，表示反向长度。 |
| *rowStart：开始行的索引，应该是有效的。* |                                    |
| *rowLength*                              | 行长度。可以为负数，表示反向长度。 |

##### 返回

> ConstantSP: 子表。

被 **Vector** , 以及 **Table** 重载.

#### virtual bool Constant::hasMethod (const string & *name*) const\[inline\], \[virtual\]

返回指定的方法是否在当前对象所属的类中。

##### 参数

|          |              |
| -------- | ------------ |
| *name* | 方法的名称。 |

#### virtual bool Constant::hasOperator (const string & *name*) const\[inline\], \[virtual\]

返回指定的操作符是否在当前对象所属的类中。 运算符可以是对象的方法或属性。

##### 参数

|          |                |
| -------- | -------------- |
| *name* | 操作者的名字。 |

#### virtual bool Constant::isNull (INDEX \* *indices*, int *len*, char \* *buf*) const\[inline\], \[virtual\]

根据索引判断数据是否为空。

##### 参数

|             |                      |
| ----------- | -------------------- |
| *indices* | 判断可能乱序的索引。 |
| *len*     | 需要判断的数据长度。 |
| *buf*     | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual bool Constant::isNull (INDEX *start*, int *len*, char \* *buf*) const\[inline\], \[virtual\]

判断从start到(start + len -1)的数据是否为null

##### 参数

|           |                      |
| --------- | -------------------- |
| *start* | 起始索引。           |
| *len*   | 需要判断的数据长度。 |
| *buf*   | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### bool Constant::isTemporary () const\[inline\]

@brief 返回此常量是否是临时的。

> 临时常量的值可能会被其他函数改变。 常量默认设置为临时的。

#### virtual bool Constant::isValid (INDEX \* *indices*, int *len*, char \* *buf*) const\[inline\], \[virtual\]

根据索引判断数据是否有效。

##### 参数

|             |                      |
| ----------- | -------------------- |
| *indices* | 判断可能乱序的索引。 |
| *len*     | 需要判断的数据长度。 |
| *buf*     | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual bool Constant::isValid (INDEX *start*, int *len*, char \* *buf*) const\[inline\], \[virtual\]

判断从start到(start + len -1)的数据是否有效。 所有的param和return都和isNull一样。

##### 参数

|           |                      |
| --------- | -------------------- |
| *start* | 起始索引。           |
| *len*   | 需要判断的数据长度。 |
| *buf*   | 结果存储在buf中。    |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual ConstantSP Constant::keys () const\[inline\], \[virtual\]

从字典中获取所有键。

##### 返回

> 所有的钥匙。

被 **Table** 重载.

#### virtual void Constant::nullFill (const ConstantSP & *val*)\[inline\], \[virtual\]

用val填充空值。

> @参数值

#### virtual long long Constant::releaseMemory (long long *target*, bool & *satisfied*)\[inline\], \[virtual\]

如果使用的内存不小于目标，则释放此常量的内存。

##### 参数

|                        |                                    |
| ---------------------- | ---------------------------------- |
| *target：目标内存。* |                                    |
| *satisfied*          | 如果内存释放则返回真，否则返回假。 |

##### 返回

> 实际释放的内存大小。

#### virtual bool Constant::reshape (INDEX *cols*, INDEX *rows*)\[inline\], \[virtual\]

用新的列和行重塑矩阵。 请注意，如果列或行小于当前大小，则矩阵将被截断， 否则附加 void。

##### 参数

|                |        |
| -------------- | ------ |
| *cols*       | 新列。 |
| *行：新行。* |        |

##### 返回

> 如果重塑成功则为真，否则为假。

#### virtual int Constant::serialize (char \* *buf*, int *bufSize*, INDEX *indexStart*, int *offset*, int & *numElement*, int & *partial*) const\[inline\], \[virtual\]

将常量序列化到缓冲区。 请注意，文字常量可能会被部分序列化。

##### 参数

|                |                                            |
| -------------- | ------------------------------------------ |
| *buf*        | 序列化后的数据存放在buf中。                |
| *bufSize*    | 缓冲区的长度。                             |
| *indexStart* | 开始序列化的元素的索引                     |
| *offset*     | 第indexStart元素中已经部分序列化的字节数。 |
| *numElement* | 返回已经完全序列化的元素个数。             |
| *partial*    | 返回已部分序列化的元素的字节数。           |

##### 返回

> 实际序列化到buf中的字节数。

被 **Vector** 重载.

#### virtual int Constant::serialize (char \* *buf*, int *bufSize*, INDEX *indexStart*, int *offset*, int *targetNumElement*, int & *numElement*, int & *partial*) const\[virtual\]

将常量序列化到缓冲区。 请注意，文字常量可能会被部分序列化。

##### 参数

|                                                |                                            |
| ---------------------------------------------- | ------------------------------------------ |
| *buf*                                        | 序列化后的数据存放在buf中。                |
| *bufSize*                                    | 缓冲区的长度。                             |
| *indexStart：开始序列化的元素的索引。*       |                                            |
| *offset*                                     | 第indexStart元素中已经部分序列化的字节数。 |
| *targetNumElement：表示要序列化的元素个数。* |                                            |
| *numElement*                                 | 返回已经完全序列化的元素个数。             |
| *partial*                                    | 返回已部分序列化的元素的字节数。           |

##### 返回

> 实际序列化到buf中的字节数。

被 **Vector** 重载.

#### virtual IO_ERR Constant::serialize (const ByteArrayCodeBufferSP & *buffer*) const\[inline\], \[virtual\]

将常量序列化到缓冲区。

##### 参数

|            |                                |
| ---------- | ------------------------------ |
| *buffer* | 序列化后的数据存放在buffer中。 |

##### 返回

> 序列化结果。

#### virtual IO_ERR Constant::serialize (Heap \* *pHeap*, const ByteArrayCodeBufferSP & *buffer*) const\[inline\], \[virtual\]

使用类型代码或数据源序列化常量以进行缓冲。

##### 参数

|            |                                |
| ---------- | ------------------------------ |
| *pHeap*  | 堆表示不同的序列化方法。       |
| *buffer* | 序列化后的数据存放在buffer中。 |

##### 返回

> 序列化结果。

#### virtual bool Constant::set (const ConstantSP & *index*, const ConstantSP & *value*)\[inline\], \[virtual\]

用值替换索引指定的单元格值。

##### 参数

|                                             |                            |
| ------------------------------------------- | -------------------------- |
| *index：标量或向量。确保所有索引都有效，* | 即不小于零且小于值的大小。 |
| *value*                                   | 要设置的值。               |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual bool Constant::set (const ConstantSP & *index*, const ConstantSP & *value*, const ConstantSP & *valueIndex*)\[inline\], \[virtual\]

将索引指定的单元格值替换为 valueIndex 指定的新值。

##### 参数

|                                                                |                            |
| -------------------------------------------------------------- | -------------------------- |
| *index：标量或向量。确保所有索引都有效，*                    | 即不小于零且小于值的大小。 |
| *值：向量。*                                                 |                            |
| *valueIndex：过滤值的索引向量。确保向量中的所有索引都有效，* | 即不小于零且小于值的大小。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual bool Constant::set (INDEX *column*, INDEX *row*, const ConstantSP & *value*)\[inline\], \[virtual\]

用新值替换由列和行索引指定的单元格值。

##### 参数

|            |              |
| ---------- | ------------ |
| *column* | 列索引。     |
| *row*    | 行索引。     |
| *value*  | 要设置的值。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual bool Constant::set (INDEX *index*, const ConstantSP & *value*)\[inline\], \[virtual\]

用新值替换索引指定的单元格值。

##### 参数

|           |                                            |
| --------- | ------------------------------------------ |
| *index* | 确保索引有效，即不小于零且小于对象的大小。 |
| *value* | 要设置的值。                               |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual bool Constant::set (INDEX *index*, const ConstantSP & *value*, INDEX *valueIndex*)\[inline\], \[virtual\]

将索引指定的单元格值替换为 valueIndex 指定的新值。

##### 参数

|                      |                                             |
| -------------------- | ------------------------------------------- |
| *index*            | 确保索引有效，即不小于零且小于对象的大小。  |
| *value*            | 要设置的值。                                |
| *valueIndex：确保* | valueIndex 有效，即不小于零且小于值的大小。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual void Constant::setBinary (const unsigned char \* *val*, int *unitLength*)\[inline\], \[virtual\]

将二进制数据设置为常量。 请注意，此接口仅在 int128 标量中实现。

##### 参数

|                                                            |  |
| ---------------------------------------------------------- | - |
| *val：指向二进制数据的指针。*                            |  |
| *unitLength：这个常量的数据类型的长度，但实际没有使用。* |  |

#### virtual void Constant::setBinary (INDEX *index*, int *unitLength*, const unsigned char \* *val*)\[inline\], \[virtual\]

将第 index 个元素设置为 val。

##### 参数

|                                        |                    |
| -------------------------------------- | ------------------ |
| *index*                              | 要设置的元素索引。 |
| *unitLength：该常量数据类型的长度。* |                    |
| *val*                                | 要设置的值。       |

#### virtual bool Constant::setBinary (INDEX *start*, int *len*, int *unitLength*, const unsigned char \* *buf*)\[inline\], \[virtual\]

用buf设置从start到(start + len -1)的二进制数据。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。

##### 参数

|                                        |                                       |
| -------------------------------------- | ------------------------------------- |
| *start*                              | 起始索引。                            |
| *len*                                | 要设置的数据长度。                    |
| *unitLength：该常量数据类型的长度。* |                                       |
| *buf*                                | 至少长度为 len\*unitLength 的缓冲区。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual void Constant::setBool (char *val*)\[inline\], \[virtual\]

将 bool 值设置为此常量。

##### 参数

|         |              |
| ------- | ------------ |
| *val* | 要设置的值。 |

#### virtual void Constant::setBool (INDEX *index*, char *val*)\[inline\], \[virtual\]

将第 index 个元素设置为 val。

##### 参数

|           |                    |
| --------- | ------------------ |
| *index* | 要设置的元素索引。 |
| *val*   | 要设置的值。       |

#### virtual bool Constant::setBool (INDEX *start*, int *len*, const char \* *buf*)\[inline\], \[virtual\]

用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getBoolBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要设置的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual void Constant::setChar (char *val*)\[inline\], \[virtual\]

将 char 值设置为此常量。

##### 参数

|         |              |
| ------- | ------------ |
| *val* | 要设置的值。 |

#### virtual void Constant::setChar (INDEX *index*, char *val*)\[inline\], \[virtual\]

将第 index 个元素设置为 val。

##### 参数

|           |                    |
| --------- | ------------------ |
| *index* | 要设置的元素索引。 |
| *val*   | 要设置的值。       |

#### virtual bool Constant::setChar (INDEX *start*, int *len*, const char \* *buf*)\[inline\], \[virtual\]

用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getCharBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要设置的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual bool Constant::setColumn (INDEX *index*, const ConstantSP & *value*)\[inline\], \[virtual\]

用新值替换索引列。

##### 参数

|                                                     |            |
| --------------------------------------------------- | ---------- |
| *index：确保索引有效，即不小于零且小于列的大小。* |            |
| *value*                                           | 新列的值。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual bool Constant::setData (INDEX *start*, int *len*, void \* *buf*)\[inline\], \[virtual\]

用buf设置从start到(start + len -1)的数据。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。

##### 参数

|           |                                                    |
| --------- | -------------------------------------------------- |
| *start* | 起始索引。                                         |
| *len*   | 要设置的元素的长度。                               |
| *buf*   | 至少长度为 len\*（该常量数据类型的长度）的缓冲区。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual void Constant::setDecimal32 (INDEX *index*, int *scale*, int *val*)\[inline\], \[virtual\]

将第 index 个元素设置为特定比例的 val。

##### 参数

|           |                    |
| --------- | ------------------ |
| *index* | 要设置的元素索引。 |
| *scale* | 小数位数。         |
| *val*   | 要设置的值。       |

#### virtual bool Constant::setDecimal32 (INDEX *start*, int *len*, int *scale*, const int \* *buf*)\[inline\], \[virtual\]

用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getDecimal32Buffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要设置的数据长度。        |
| *scale* | 小数位数。                |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual void Constant::setDecimal64 (INDEX *index*, int *scale*, long long *val*)\[inline\], \[virtual\]

将第 index 个元素设置为特定比例的 val。

##### 参数

|           |                    |
| --------- | ------------------ |
| *index* | 要设置的元素索引。 |
| *scale* | 小数位数。         |
| *val*   | 要设置的值。       |

#### virtual bool Constant::setDecimal64 (INDEX *start*, int *len*, int *scale*, const long long \* *buf*)\[inline\], \[virtual\]

用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getDecimal64Buffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要设置的数据长度。        |
| *scale* | 小数位数。                |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual void Constant::setDouble (double *val*)\[inline\], \[virtual\]

将双精度值设置为此常量。

##### 参数

|         |              |
| ------- | ------------ |
| *val* | 要设置的值。 |

#### virtual void Constant::setDouble (INDEX *index*, double *val*)\[inline\], \[virtual\]

将第 index 个元素设置为 val。

##### 参数

|           |                    |
| --------- | ------------------ |
| *index* | 要设置的元素索引。 |
| *val*   | 要设置的值。       |

#### virtual bool Constant::setDouble (INDEX *start*, int *len*, const double \* *buf*)\[inline\], \[virtual\]

用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getDoubleBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要设置的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual void Constant::setFloat (float *val*)\[inline\], \[virtual\]

将浮点值设置为此常量。

##### 参数

|         |              |
| ------- | ------------ |
| *val* | 要设置的值。 |

#### virtual void Constant::setFloat (INDEX *index*, float *val*)\[inline\], \[virtual\]

将第 index 个元素设置为 val。

##### 参数

|           |                    |
| --------- | ------------------ |
| *index* | 要设置的元素索引。 |
| *val*   | 要设置的值。       |

#### virtual bool Constant::setFloat (INDEX *start*, int *len*, const float \* *buf*)\[inline\], \[virtual\]

用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getFloatBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要设置的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual void Constant::setIndex (INDEX *index*, INDEX *val*)\[inline\], \[virtual\]

将第 index 个元素设置为 val。

##### 参数

|           |                    |
| --------- | ------------------ |
| *index* | 要设置的元素索引。 |
| *val*   | 要设置的值。       |

#### virtual bool Constant::setIndex (INDEX *start*, int *len*, const INDEX \* *buf*)\[inline\], \[virtual\]

用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getIndexBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要设置的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual void Constant::setIndex (INDEX *val*)\[inline\], \[virtual\]

将索引值设置为此常量。

##### 参数

|         |              |
| ------- | ------------ |
| *val* | 要设置的值。 |

#### virtual void Constant::setInt (INDEX *index*, int *val*)\[inline\], \[virtual\]

将第 index 个元素设置为 val。

##### 参数

|           |                    |
| --------- | ------------------ |
| *index* | 要设置的元素索引。 |
| *val*   | 要设置的值。       |

#### virtual bool Constant::setInt (INDEX *start*, int *len*, const int \* *buf*)\[inline\], \[virtual\]

用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getIntBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要设置的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual void Constant::setInt (int *val*)\[inline\], \[virtual\]

将 int 值设置为此常量。

##### 参数

|         |              |
| ------- | ------------ |
| *val* | 要设置的值。 |

#### virtual bool Constant::setItem (INDEX *index*, const ConstantSP & *value*)\[inline\], \[virtual\]

用新值替换索引项。 请注意，第 index 列将替换为矩阵中的新值。

##### 参数

|           |                                            |
| --------- | ------------------------------------------ |
| *index* | 确保索引有效，即不小于零且小于对象的大小。 |
| *value* | 要设置的值。                               |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual void Constant::setLong (INDEX *index*, long long *val*)\[inline\], \[virtual\]

将第 index 个元素设置为 val。

##### 参数

|           |                    |
| --------- | ------------------ |
| *index* | 要设置的元素索引。 |
| *val*   | 要设置的值。       |

#### virtual bool Constant::setLong (INDEX *start*, int *len*, const long long \* *buf*)\[inline\], \[virtual\]

用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getLongBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要设置的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual void Constant::setLong (long long *val*)\[inline\], \[virtual\]

将 long 值设置为此常量。

##### 参数

|         |              |
| ------- | ------------ |
| *val* | 要设置的值。 |

#### virtual bool Constant::setNonNull (const ConstantSP & *index*, const ConstantSP & *value*)\[inline\], \[virtual\]

用值替换索引指定的单元格值。 请注意，setNonNull 在替换数据时将忽略空值。

##### 参数

|                                             |                            |
| ------------------------------------------- | -------------------------- |
| *index：标量或向量。确保所有索引都有效，* | 即不小于零且小于值的大小。 |
| *value*                                   | 要设置的值。               |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual void Constant::setNull (INDEX *index*)\[inline\], \[virtual\]

将第 index 个元素设置为 null。

##### 参数

|           |                    |
| --------- | ------------------ |
| *index* | 要设置的元素索引。 |

#### virtual void Constant::setNullFlag (bool *containNull*)\[inline\], \[virtual\]

设置空标志。

##### 参数

|                                                           |                                  |
| --------------------------------------------------------- | -------------------------------- |
| *containNull：如果此常量\*\*可能\*\*包含空值，则为真，* | 如果此常量不能包含空值则为 false |

#### virtual void Constant::setShort (INDEX *index*, short *val*)\[inline\], \[virtual\]

将第 index 个元素设置为 val。

##### 参数

|           |                    |
| --------- | ------------------ |
| *index* | 要设置的元素索引。 |
| *val*   | 要设置的值。       |

#### virtual bool Constant::setShort (INDEX *start*, int *len*, const short \* *buf*)\[inline\], \[virtual\]

用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getShortBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要设置的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual void Constant::setShort (short *val*)\[inline\], \[virtual\]

将短值设置为此常量。

##### 参数

|         |              |
| ------- | ------------ |
| *val* | 要设置的值。 |

#### virtual void Constant::setString (const DolphinString & *val*)\[inline\], \[virtual\]

将 DolphinString 值设置为此常量。

##### 参数

|         |              |
| ------- | ------------ |
| *val* | 要设置的值。 |

#### virtual void Constant::setString (INDEX *index*, const DolphinString & *val*)\[inline\], \[virtual\]

将第 index 个元素设置为 val。

##### 参数

|           |                    |
| --------- | ------------------ |
| *index* | 要设置的元素索引。 |
| *val*   | 要设置的值。       |

#### virtual bool Constant::setString (INDEX *start*, int *len*, char \*\* *buf*)\[inline\], \[virtual\]

用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要设置的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual bool Constant::setString (INDEX *start*, int *len*, const DolphinString \*\* *buf*)\[inline\], \[virtual\]

用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要设置的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual bool Constant::setString (INDEX *start*, int *len*, const string \* *buf*)\[inline\], \[virtual\]

用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getStringBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *start* | 起始索引。                |
| *len*   | 要设置的数据长度。        |
| *buf*   | 至少长度为 len 的缓冲区。 |

##### 返回

> 如果设置成功则为真，否则为假。

#### virtual INDEX Constant::size () const\[inline\], \[virtual\]

返回此常量的大小。

被 **Table** 重载.

#### virtual bool Constant::sizeable () const\[inline\], \[virtual\]

返回此常量是否可以调整大小。

被 **Table** 重载.

#### virtual ConstantSP Constant::values () const\[inline\], \[virtual\]

从字典中获取所有值。

##### 返回

> 所有值。

被 **Table** 重载.

## 类：Table

继承自 **Constant** .

\#include <CoreConcept.h>

### Public 成员函数

- virtual ConstantSP **getIterator** (const ConstantSP &self) const

*获取这个常量本身的迭代器。*

- virtual string **getScript** () const

*返回这个常量的描述。*

- virtual ConstantSP **getColumn** (const string &name) const =0

*根据列名获取指定列。*

- virtual ConstantSP **getColumn** (const string &qualifier, const string &name) const =0

*根据列名获取指定列。*

- virtual ConstantSP **getColumn** (INDEX index) const =0

*根据索引获取指定列。*

- virtual ConstantSP **getColumn** (const string &name, const ConstantSP &rowFilter) const =0

*根据列名和rowFilter获取指定的列和行。*

- virtual ConstantSP **getColumn** (const string &qualifier, const string &name, const ConstantSP &rowFilter) const =0

*根据列名和rowFilter获取指定的列和行。*

- virtual ConstantSP **getColumn** (INDEX index, const ConstantSP &rowFilter) const =0

*根据列索引和rowFilter获取指定的列和行。*

- virtual INDEX **columns** () const =0

*返回列数。*

- virtual const string & **getColumnName** (int index) const =0

*返回指定列的名称。*

- virtual const string & **getColumnQualifier** (int index) const =0

*根据列索引获取源表的限定符。 通常用在连接表中。*

- virtual void **setColumnName** (int index, const string &name)=0

*设置指定列的名称。*

- virtual int **getColumnIndex** (const string &name) const =0

*返回指定列的索引。*

- virtual DATA_TYPE **getColumnType** (int index) const =0

*返回指定列的数据类型。*

- virtual int **getColumnExtraParam** (int index) const

*返回指定列的额外属性。*

- virtual bool **contain** (const string &name) const =0

*返回此表是否包含具有指定名称的列。*

- virtual bool **contain** (const string &qualifier, const string &name) const =0

*当限定符等于此表的名称时返回 true 并且此表包含具有指定名称的列。*

- virtual bool **contain** (const ColumnRef \*col) const =0

*从 col 中获取限定符和名称。 当限定符等于该表的名称时返回真 并且此表包含具有指定名称的列。*

- virtual bool **contain** (const ColumnRefSP &col) const =0

*从 col 中获取限定符和名称。 当限定符等于该表的名称时返回真 并且此表包含具有指定名称的列。*

- virtual bool **containAll** (const vector\< ColumnRefSP \> &cols) const =0

*从每个 ColumnRef 中获取限定符和名称。 当每个限定符等于此表的名称时返回 true 并且此表包含具有指定名称的列。*

- virtual void **setName** (const string &name)=0

*设置此表的名称。*

- virtual const string & **getName** () const =0

*返回此表的名称。*

- virtual ConstantSP **get** (INDEX index) const

*获取该常量中第 index 个元素的数据。 注意索引要有效，否则会出现越界访问。*

- virtual ConstantSP **get** (const ConstantSP &index) const =0

*根据索引获取数据。*

- virtual ConstantSP **getValue** (INDEX capacity) const =0
- virtual ConstantSP **getValue** () const =0

*获取此常量的副本。*

- virtual ConstantSP **getInstance** (INDEX **size**) const =0
- virtual INDEX **size** () const =0

*返回此常量的大小。*

- virtual bool **sizeable** () const =0

*返回此常量是否可以调整大小。*

- virtual string **getString** (INDEX index) const =0

*获取此常量中第 index 个元素的字符串值。 注意索引要有效，否则会出现越界访问。*

- virtual string **getString** () const =0

*获取此常量的字符串版本。 这个函数对调试很有帮助。*

- virtual ConstantSP **getWindow** (INDEX colStart, int colLength, INDEX rowStart, int rowLength) const =0

*从这个常量中获取一个子表。 注意，子表是原表的副本。*

- virtual ConstantSP **getMember** (const ConstantSP &key) const =0

*根据键从字典中获取数据，空值将用于填充任何缺失的数据。*

- virtual ConstantSP **values** () const =0

*从字典中获取所有值。*

- virtual ConstantSP **keys** () const =0

*从字典中获取所有键。*

- virtual TABLE_TYPE **getTableType** () const =0

*返回此表的表类型。*

- virtual bool **join** (vector\< ConstantSP \> &**columns**)

*向该表中添加列。 请注意，每个列的名称都应与该表不同。*

- virtual bool **clear** ()
- virtual bool **reorderColumns** (const vector\< int \> &newOrders)

*使用给定的新订单对表的列重新排序。*

- virtual bool **replaceColumn** (int index, const ConstantSP &col)

*用新列替换此表的指定列。*

- virtual bool **drop** (vector\< int \> &**columns**)

*删除指定的列。*

- virtual void **remove** (Heap \*heap, const SQLContextSP &context, const ConstantSP &filterExprs)

*根据 filterExprs 从该表中删除行。*

- virtual void **sortBy** (Heap \*heap, const ObjectSP &sortExpr, const ConstantSP &sortOrder)

*对该表的某些列进行排序。*

- virtual void **update** (Heap \*heap, const SQLContextSP &context, const ConstantSP &updateColNames, const ObjectSP &updateExpr, const ConstantSP &filterExprs)

*根据 updateExpr 和 filterExprs 更新这个表的一些列。*

- virtual bool **update** (vector\< ConstantSP \> &**values**, const ConstantSP &indexSP, vector\< string \> &colNames, string &errMsg)=0

*根据索引用新值更新此表的某些列。*

- virtual bool **append** (vector\< ConstantSP \> &**values**, INDEX &insertedRows, string &errMsg)=0

*将值附加到此表。*

- virtual bool **append** (Heap \*heap, vector\< ConstantSP \> &**values**, INDEX &insertedRows, string &errMsg)

*将值附加到此表。*

- virtual bool **remove** (const ConstantSP &indexSP, string &errMsg)=0

*从此表中删除行。*

- virtual bool **upsert** (vector\< ConstantSP \> &**values**, bool ignoreNull, INDEX &insertedRows, string &errMsg)

*将行插入到键控表或索引表中，如果 主键尚不存在，如果存在则更新它们。*

- virtual DATA_TYPE **getRawType** () const

*获取此常量的原始数据类型。*

- virtual bool **isDistributedTable** () const

*返回此表是否为分布式表。*

- virtual bool **isSegmentedTable** () const

*返回此表是否为分段表。*

- virtual bool **isDimensionalTable** () const

*返回此表是否为维度表。*

- virtual bool **isBasicTable** () const

*返回此表是否为基本表。*

- virtual bool **isDFSTable** () const

*返回此表是否为 dfs 表。*

- virtual bool **isAppendable** () const

*返回这个表是否可以追加。*

- virtual bool **isEditable** () const

*返回这个表是否可以追加。*

- virtual bool **isSchemaEditable** () const

*返回是否可以编辑此表的模式。*

- virtual int **getSortKeyCount** () const

*返回此表的排序键的计数。*

- virtual int **getSortKeyColumnIndex** (int index)

*返回此表中第 index 个排序键的列索引。*

- virtual int **isAscendingKey** (int index)

*返回第 index 个排序键是否按升序排列。*

- virtual DomainSP **getGlobalDomain** () const

*返回共享 dfs 表的域。*

- virtual DomainSP **getLocalDomain** () const

*返回此表的域。*

- virtual int **getGlobalPartitionColumnIndex** () const

*返回该表的分区列索引。*

- virtual int **getLocalPartitionColumnIndex** (int dim) const

*根据域维度返回该表的分区列索引。*

- virtual void **setGlobalPartition** (const DomainSP &domain, const string &partitionColumn)

*设置此表的全局域。*

- virtual bool **isLargeConstant** () const
- virtual bool **addSubscriber** (const TableUpdateQueueSP &queue, const string &topic, bool local, long long offset=-1, const ObjectSP &**filter**=0)

*将订阅者添加到此流表。*

- virtual bool **removeSubscriber** (const TableUpdateQueueSP &queue, const string &topic)

*根据主题名称从此流表中删除订阅者。*

- virtual bool **subscriberExists** (const TableUpdateQueueSP &queue, const string &topic) const

*根据订阅任务名称返回订阅是否存在。*

- virtual void **release** () const

*释放这个表的使用计数。*

- virtual void **checkout** () const

*添加该表的使用次数。*

- virtual TableSP **getSegment** (Heap \*heap, const DomainPartitionSP &partition, PartitionGuard \*guard=0)

*从这个 dfs 表中检索分区的数据。*

- virtual TableSP **getSegment** (Heap \*heap, const DomainPartitionSP &partition, vector\< ObjectSP \> &filters, const vector\< string \> &colNames, PartitionGuard \*guard=0, INDEX limit=0, bool byKey=false)

*从该表中检索带有过滤器的分区的数据。*

- virtual const TableSP & **getEmptySegment** () const

*返回一个与该表具有相同架构的空表。*

- virtual bool **segmentExists** (const DomainPartitionSP &partition) const

*返回指定分区是否存在于该表中。*

- virtual int **getPartitionCount** () const

*返回当前表包含多少个分区。*

- virtual long long **getAllocatedMemory** () const =0

*获取此常量的分配内存（以字节为单位）。*

- virtual ConstantSP **retrieveMessage** (long long offset, int length, bool msgAsTable, const ObjectSP &**filter**, long long &messageId)

*从此表中检索消息。*

- virtual bool **snapshotIsolate** () const

*返回这个表是否是快照隔离。*

- virtual void **getSnapshot** (TableSP &copy) const

*获取该表的快照。*

- virtual bool **readPermitted** (const AuthenticatedUserSP &user) const

*返回用户是否对该表有读权限。*

- virtual bool **isExpired** () const

*返回这个表是否过期。*

- virtual void **transferAsString** (bool option)
- virtual int **getKeyColumnCount** () const

*返回此表的键列的计数。*

- virtual int **getKeyColumnIndex** (int index) const

*返回此表中第 index 个键列的列索引。*

- virtual int **getKeyTimeColumnIndex** () const

*返回此表的时间键列的索引。*

- virtual string **getChunkPath** () const

*返回该表的块路径。*

- virtual void **share** ()

*将此表设置为共享。*

- virtual ConstantSP **filter** (vector\< ObjectSP \> &filterExprs, INDEX limit=0, bool byKey=true) const

*按一组列和值关系过滤表。*

- virtual bool **supportBlockFilter** () const

*返回此表是否支持块过滤器。*

- virtual bool **prepareData** (const ConstantSP &**rows**, const vector\< int \> &cols)

*提前准备数据。*

- virtual void **groupBySortKeys** (vector\< string \> &groupBy, const ConstantSP &**filter**, vector\< TableSP \> &tablets, vector\< string \> &removedGroupBys, vector\< ConstantSP \> &groupedFilters)
- virtual DUPLICATE_POLICY **getRowDuplicatePolicy** () const

*返回该表的行重复策略。*

- bool **isSharedTable** () const

*返回此表是否为共享表。*

- bool **isRealtimeTable** () const

*返回此表是否为流表。*

- bool **isAliasTable** () const

*返回这个表是否是别名表。*

- const string & **getOwner** () const

*返回这个表的所有者的名字。*

- void **setOwner** (const string &owner)

*设置此表的所有者的名称。*

- DBENGINE_TYPE **getEngineType** () const

*返回这个表的存储引擎类型。*

- void **setEngineType** (DBENGINE_TYPE engineType)

*设置该表的存储引擎类型。*

- void **setSharedTable** (bool option)

*设置该表的共享属性。*

- void **setRealtimeTable** (bool option)

*设置该表的表类型标志。*

- void **setAliasTable** (bool option)

*设置该表的表类型标志。*

- void **setExpired** (bool option)

*设置该表的过期标志。*

- Mutex \* **getLock** () const

*返回这个字典的锁。*

- virtual bool **writePermitted** (const AuthenticatedUserSP &user) const

*返回用户是否对该表有写权限。*

- virtual const string & **getPhysicalName** () const

*返回此表的物理名称。*

- virtual void **setAccessControl** (bool option)

*设置该表的访问控制标志。*

- virtual bool **isAccessControl** () const

*返回对该表的操作是否需要相应的权限。*

- virtual ConstantSP **get** (INDEX index) const
- virtual ConstantSP **get** (INDEX column, INDEX row) const
- virtual ConstantSP **get** (const ConstantSP &index) const
- virtual ConstantSP **get** (INDEX offset, const ConstantSP &index) const

#### Public 成员函数 继承自 Constant

- **Constant** (unsigned short flag)
- **Constant** (DATA_FORM df, DATA_TYPE dt, DATA_CATEGORY dc)
- bool **isTemporary** () const
- void **setTemporary** (bool val)

*设置常量是否为临时的。*

- bool **isIndependent** () const

*返回这个常量是否独立。 非独立常量表示包含在其他常量中，例如表包含的向量。 常量默认设置为独立。*

- void **setIndependent** (bool val)

*设置常量是否独立。*

- bool **isReadOnly** () const

*返回此常量是否为只读。 只读常量的值不会改变。 常量默认不是只读的。*

- void **setReadOnly** (bool val)

*设置常量是否只读。*

- bool **isReadOnlyArgument** () const

*返回此常量是否为只读参数。 只读参数不能应用于可变函数。 常量默认不设置为只读参数*

- void **setReadOnlyArgument** (bool val)

*设置常量是否为只读参数。*

- bool **isNothing** () const

*返回这个常量是否为空。 在大多数情况下，nothing 常量用作函数的默认参数。 默认情况下，只有 void 标量会被设置为 nothing。*

- void **setNothing** (bool val)

*设置常量是否为空。*

- bool **isStatic** () const

*返回这个常量是否是静态的。 static是指在sql中用常量初始化，所以它的值是不能改变的。 常量默认不是静态的*

- void **setStatic** (bool val)

*设置静态标志。*

- bool **transferAsString** () const

*返回 transferAsString 标志。 TransferAsString 标志用于在将数据附加到远程块时提高序列化的性能。*

- void **transferAsString** (bool val)

*设置 transferAsString 标志。*

- bool **isSynchronized** () const

*返回这个常量是否可以被多个线程访问。 必须在持有锁的同时访问同步常量。 常量默认不同步。*

- void **setSynchronized** (bool val)

*设置同步标志。*

- bool **isOOInstance** () const

*返回这个常量是否是一个面向对象的实例。 默认情况下，常量不是面向对象的实例。*

- void **setOOInstance** (bool val)

*设置面向对象的实例标志。*

- bool **isIndexed** () const

*返回这个常量是否有索引。 常量默认没有索引。*

- void **setIndexed** (bool val)

*设置索引标志。*

- bool **isSeries** () const

*返回这个常量是否有索引。 Series 表示索引单列矩阵。 常量默认不是系列。*

- void **setSeries** (bool val)

*设置系列标志。*

- bool **isTransient** () const

*返回这个常量是否是瞬态的。 必须通过 **getValue()** 获取副本才能使用瞬态常量。 默认情况下常量不是瞬态的。*

- void **setTransient** (bool val)

*等临时标志。*

- DATA_FORM **getForm** () const

*返回此常量的 DATA_FORM。*

- void **setForm** (DATA_FORM df)

*设置此常量的 DATA_FORM。*

- DATA_TYPE **getType** () const

*返回此常量的 DATA_TYPE。*

- DATA_CATEGORY **getCategory** () const

*返回此常量的 DATA_CATEGORY。*

- bool **isScalar** () const

*返回这个常量是否是标量。*

- bool **isArray** () const

*返回这个常量是否是数组。*

- bool **isPair** () const

*返回这个常量是否是对。*

- bool **isMatrix** () const

*返回这个常量是否是矩阵。*

- bool **isVector** () const

*返回这个常量是否是向量。 向量可以是数组、对或矩阵。*

- bool **isTable** () const

*返回这个常量是否是表。*

- bool **isSet** () const

*返回是否设置了这个常量。*

- bool **isDictionary** () const

*返回这个常量是否是字典。*

- bool **isChunk** () const

*返回这个常量是否是块。*

- bool **isSysObj** () const

*返回这个常量是否是系统对象。*

- bool **isTuple** () const

*返回这个常量是否是元组。 元组必须是任意向量。*

- bool **isNumber** () const

*返回这个常量是否是数字。 数字可以是整数、浮点数或十进制。*

- virtual ConstantSP **getIterator** (const ConstantSP &self) const

*获取这个常量本身的迭代器。*

- virtual ConstantSP **next** ()

*返回下一个项目。如果迭代器已经到达末尾，它返回一个空指针，即 item.isNull() 为真。*

- virtual uint64_t **hash** () const
- virtual bool **equal** (const ConstantSP &other) const
- virtual bool **isDatabase** () const

*返回此常量是否为数据库句柄。*

- virtual char **getBool** () const

*返回这个常量的布尔值。*

- virtual char **getChar** () const

*返回这个常量的字符值。*

- virtual short **getShort** () const

*返回这个常量的短值。*

- virtual int **getInt** () const

*返回此常量的 int 值。*

- virtual long long **getLong** () const

*返回这个常量的长值。*

- virtual INDEX **getIndex** () const

*返回这个常量的索引值。*

- virtual float **getFloat** () const

*返回这个常量的浮点值。*

- virtual double **getDouble** () const

*返回这个常量的双精度值。*

- virtual string **getString** () const

*获取此常量的字符串版本。 这个函数对调试很有帮助。*

- virtual string **getString** (Heap \*heap) const

*根据堆中的会话类型获取此常量的字符串版本。 这个函数对调试很有帮助。*

- virtual string **getScript** () const

*返回这个常量的描述。*

- virtual string **getScript** (Heap \*heap) const

*根据堆中的会话类型返回这个常量的描述。*

- virtual const DolphinString & **getStringRef** () const

*返回此常量的 dolphinString 引用*

- virtual const Guid **getInt128** () const

*返回此常量的 guid 值。*

- virtual const unsigned char \* **getBinary** () const

*获取 int128 标量的二进制数据。*

- virtual bool **isNull** () const

*返回这个常量是否为空。*

- virtual void **setBool** (char val)

*将 bool 值设置为此常量。*

- virtual void **setChar** (char val)

*将 char 值设置为此常量。*

- virtual void **setShort** (short val)

*将短值设置为此常量。*

- virtual void **setInt** (int val)

*将 int 值设置为此常量。*

- virtual void **setLong** (long long val)

*将 long 值设置为此常量。*

- virtual void **setIndex** (INDEX val)

*将索引值设置为此常量。*

- virtual void **setFloat** (float val)

*将浮点值设置为此常量。*

- virtual void **setDouble** (double val)

*将双精度值设置为此常量。*

- virtual void **setString** (const DolphinString &val)

*将 DolphinString 值设置为此常量。*

- virtual void **setBinary** (const unsigned char \*val, int unitLength)

*将二进制数据设置为常量。 请注意，此接口仅在 int128 标量中实现。*

- virtual void **setNull** ()

*将空值设置为此常量。*

- virtual char **getBool** (INDEX index) const

*获取此常量中第 index 个元素的 bool 值。 注意索引要有效，否则会出现越界访问。*

- virtual char **getChar** (INDEX index) const

*获取此常量中第 index 个元素的 char 值。 注意索引要有效，否则会出现越界访问。*

- virtual short **getShort** (INDEX index) const

*获取此常量中第 index 个元素的短值。 注意索引要有效，否则会出现越界访问。*

- virtual int **getInt** (INDEX index) const

*获取此常量中第 index 个元素的 int 值。 注意索引要有效，否则会出现越界访问。*

- virtual long long **getLong** (INDEX index) const

*获取此常量中第 index 个元素的 long 值。 注意索引要有效，否则会出现越界访问。*

- virtual INDEX **getIndex** (INDEX index) const

*获取该常量中第 index 个元素的索引值。 注意索引要有效，否则会出现越界访问。*

- virtual float **getFloat** (INDEX index) const

*获取此常量中第 index 个元素的浮点值。 注意索引要有效，否则会出现越界访问。*

- virtual double **getDouble** (INDEX index) const

*获取此常量中第 index 个元素的双精度值。 注意索引要有效，否则会出现越界访问。*

- virtual string **getString** (INDEX index) const

*获取此常量中第 index 个元素的字符串值。 注意索引要有效，否则会出现越界访问。*

- virtual const DolphinString & **getStringRef** (INDEX index) const

*获取此常量中第 index 个元素的 dolphinString 引用。 注意索引要有效，否则会出现越界访问。*

- virtual const Guid **getInt128** (INDEX index) const

*获取此常量中第 index 个元素的 guid 值。 注意索引要有效，否则会出现越界访问。*

- virtual const unsigned char \* **getBinary** (INDEX index) const

*获取此常量中第 index 个元素的二进制数据。 注意索引要有效，否则会出现越界访问。*

- virtual bool **isNull** (INDEX index) const

*返回第 index 个元素是否为 null。*

- virtual int **getDecimal32** (INDEX index, int scale) const

*获取此常量中第 index 个元素的特定比例的 decimal32 数据。 注意索引要有效，否则会出现越界访问。*

- virtual long long **getDecimal64** (INDEX index, int scale) const

*获取此常量中第 index 个元素的特定比例的 decimal64 数据。 注意索引要有效，否则会出现越界访问。*

- virtual ConstantSP **get** (INDEX index) const

*获取该常量中第 index 个元素的数据。 注意索引要有效，否则会出现越界访问。*

- virtual ConstantSP **get** (INDEX column, INDEX row) const

*根据表或矩阵中的行和列索引获取数据。 注意列索引和行索引都要有效，否则会出现越界访问。*

- virtual ConstantSP **get** (const ConstantSP &index) const

*根据索引获取数据。*

- virtual ConstantSP **get** (INDEX offset, const ConstantSP &index) const

*根据索引获取数据。*

- virtual ConstantSP **getColumn** (INDEX index) const

*根据索引获取表或矩阵中指定列的数据。 注意索引要有效，否则会出现越界访问。*

- virtual ConstantSP **getRow** (INDEX index) const

*根据索引获取矩阵中指定行的数据。 注意索引要有效，否则会出现越界访问。*

- virtual ConstantSP **getItem** (INDEX index) const

*根据索引获取指定项的数据。 请注意，第 index 列将在表或矩阵中返回，并且 index 应该有效。*

- virtual ConstantSP **getItems** (const ConstantSP &index) const

*根据索引获取指定item的数据。 如果索引超出范围，则对应返回空值。 请注意，索引列将在表或矩阵中返回。*

- virtual ConstantSP **getWindow** (INDEX colStart, int colLength, INDEX rowStart, int rowLength) const

*从这个常量中获取一个子表。 注意，子表是原表的副本。*

- virtual ConstantSP **getSlice** (const ConstantSP &rowIndex, const ConstantSP &colIndex) const

*根据rowIndex 和colIndex 获取切片数据。 如果索引超出范围，则对应返回空值。*

- virtual ConstantSP **getRowLabel** () const
- virtual ConstantSP **getColumnLabel** () const
- virtual bool **isNull** (INDEX start, int len, char \*buf) const

*判断从start到(start + len -1)的数据是否为null*

- virtual bool **isValid** (INDEX start, int len, char \*buf) const

*判断从start到(start + len -1)的数据是否有效。 所有的param和return都和isNull一样。*

- virtual bool **getBool** (INDEX start, int len, char \*buf) const

*获取从 start 到 (start + len -1) 的布尔数据。*

- virtual bool **getChar** (INDEX start, int len, char \*buf) const

*获取从start到(start + len -1)的字符数据。*

- virtual bool **getShort** (INDEX start, int len, short \*buf) const

*获取从start到(start + len -1)的数据的短值。*

- virtual bool **getInt** (INDEX start, int len, int \*buf) const

*获取从start到(start + len -1)的数据的int值。*

- virtual bool **getLong** (INDEX start, int len, long long \*buf) const

*获取数据从start到(start + len -1)的long long值。*

- virtual bool **getIndex** (INDEX start, int len, INDEX \*buf) const

*获取数据从start到(start + len -1)的索引值。*

- virtual bool **getFloat** (INDEX start, int len, float \*buf) const

*获取数据从start到(start + len -1)的浮点值。*

- virtual bool **getDouble** (INDEX start, int len, double \*buf) const

*获取从 start 到 (start + len -1) 的数据的双精度值。*

- virtual bool **getSymbol** (INDEX start, int len, int \*buf, SymbolBase \*symBase, bool insertIfNotThere) const

*获取从 start 到 (start + len -1) 的符号数据。*

- virtual bool **getString** (INDEX start, int len, DolphinString \*\*buf) const

*获取从 start 到 (start + len -1) 的 DolphinString 数据。*

- virtual bool **getString** (INDEX start, int len, char \*\*buf) const

*获取从start到(start + len -1)的字符串数据。*

- virtual bool **getBinary** (INDEX start, int len, int unitLength, unsigned char \*buf) const

*获取从 start 到 (start + len -1) 的二进制数据。*

- virtual bool **getDecimal32** (INDEX start, int len, int scale, int \*buf) const

*获取从 start 到 (start + len -1) 的特定比例的 decimal32 数据。*

- virtual bool **getDecimal64** (INDEX start, int len, int scale, long long \*buf) const

*获取从 start 到 (start + len -1) 的特定比例的 decimal64 数据。*

- virtual bool **isNull** (INDEX \*indices, int len, char \*buf) const

*根据索引判断数据是否为空。*

- virtual bool **isValid** (INDEX \*indices, int len, char \*buf) const

*根据索引判断数据是否有效。*

- virtual bool **getBool** (INDEX \*indices, int len, char \*buf) const

*根据索引获取布尔数据。*

- virtual bool **getChar** (INDEX \*indices, int len, char \*buf) const

*根据索引获取字符数据。*

- virtual bool **getShort** (INDEX \*indices, int len, short \*buf) const

*根据索引获取数据的短值。*

- virtual bool **getInt** (INDEX \*indices, int len, int \*buf) const

*根据索引获取数据的int值。*

- virtual bool **getLong** (INDEX \*indices, int len, long long \*buf) const

*根据索引获取数据的 long long 值。*

- virtual bool **getIndex** (INDEX \*indices, int len, INDEX \*buf) const

*根据索引获取数据的索引值。*

- virtual bool **getFloat** (INDEX \*indices, int len, float \*buf) const

*根据索引获取数据的浮点值。*

- virtual bool **getDouble** (INDEX \*indices, int len, double \*buf) const

*根据索引获取数据的双精度值。*

- virtual bool **getSymbol** (INDEX \*indices, int len, int \*buf, SymbolBase \*symBase, bool insertIfNotThere) const

*根据索引获取符号数据。*

- virtual bool **getString** (INDEX \*indices, int len, DolphinString \*\*buf) const

*根据索引获取 DolphinString 数据。*

- virtual bool **getString** (INDEX \*indices, int len, char \*\*buf) const

*根据索引获取字符串数据。*

- virtual bool **getBinary** (INDEX \*indices, int len, int unitLength, unsigned char \*buf) const

*根据索引获取二进制数据。*

- virtual bool **getDecimal32** (INDEX \*indices, int len, int scale, int \*buf) const

*根据索引获取特定scale的decimal32数据。*

- virtual bool **getDecimal64** (INDEX \*indices, int len, int scale, long long \*buf) const

*根据索引获取特定比例的decimal64数据。*

- virtual const char \* **getBoolConst** (INDEX start, int len, char \*buf) const

*获取从 start 到 (start + len -1) 的布尔数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const char \* **getCharConst** (INDEX start, int len, char \*buf) const

*获取从 start 到 (start + len -1) 的字符值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const short \* **getShortConst** (INDEX start, int len, short \*buf) const

*获取从 start 到 (start + len -1) 的短值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const int \* **getIntConst** (INDEX start, int len, int \*buf) const

*获取从 start 到 (start + len -1) 的 int 值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const long long \* **getLongConst** (INDEX start, int len, long long \*buf) const

*获取从 start 到 (start + len -1) 的 long long 值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const INDEX \* **getIndexConst** (INDEX start, int len, INDEX \*buf) const

*获取从start到(start + len -1)的索引值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const float \* **getFloatConst** (INDEX start, int len, float \*buf) const

*获取浮点值开始到 (start + len -1)。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const double \* **getDoubleConst** (INDEX start, int len, double \*buf) const

*获取从 start 到 (start + len -1) 的双精度值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const int \* **getSymbolConst** (INDEX start, int len, int \*buf, SymbolBase \*symBase, bool insertIfNotThere) const

*获取从 start 到 (start + len -1) 的符号数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual DolphinString \*\* **getStringConst** (INDEX start, int len, DolphinString \*\*buf) const

*获取从 start 到 (start + len -1) 的 DolphinString 数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual char \*\* **getStringConst** (INDEX start, int len, char \*\*buf) const

*获取从start到(start + len -1)的字符串数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const unsigned char \* **getBinaryConst** (INDEX start, int len, int unitLength, unsigned char \*buf) const

*获取从 start 到 (start + len -1) 的二进制数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const int \* **getDecimal32Const** (INDEX start, int len, int scale, int \*buf) const

*获取从 start 到 (start + len -1) 的特定刻度的 decimal32 数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const long long \* **getDecimal64Const** (INDEX start, int len, int scale, long long \*buf) const

*获取从 start 到 (start + len -1) 的特定比例的 decimal64 数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual char \* **getBoolBuffer** (INDEX start, int len, char \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setBool(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual char \* **getCharBuffer** (INDEX start, int len, char \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setChar(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual short \* **getShortBuffer** (INDEX start, int len, short \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setShort(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual int \* **getIntBuffer** (INDEX start, int len, int \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setInt(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual long long \* **getLongBuffer** (INDEX start, int len, long long \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setLong(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual INDEX \* **getIndexBuffer** (INDEX start, int len, INDEX \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setIndex(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual float \* **getFloatBuffer** (INDEX start, int len, float \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setFloat(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual double \* **getDoubleBuffer** (INDEX start, int len, double \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setDouble(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual unsigned char \* **getBinaryBuffer** (INDEX start, int len, int unitLength, unsigned char \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setBinary(start, len, unitLength, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual void \* **getDataBuffer** (INDEX start, int len, void \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setData(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual int \* **getDecimal32Buffer** (INDEX start, int len, int scale, int \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setDecimal32(start, len, scale, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual long long \* **getDecimal64Buffer** (INDEX start, int len, int scale, long long \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setDecimal64(start, len, scale, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual IO_ERR **serialize** (Heap \*pHeap, const ByteArrayCodeBufferSP &buffer) const

*使用类型代码或数据源序列化常量以进行缓冲。*

- virtual IO_ERR **serialize** (const ByteArrayCodeBufferSP &buffer) const

*将常量序列化到缓冲区。*

- virtual int **serialize** (char \*buf, int bufSize, INDEX indexStart, int offset, int &numElement, int &partial) const

*将常量序列化到缓冲区。 请注意，文字常量可能会被部分序列化。*

- virtual int **serialize** (char \*buf, int bufSize, INDEX indexStart, int offset, int targetNumElement, int &numElement, int &partial) const

*将常量序列化到缓冲区。 请注意，文字常量可能会被部分序列化。*

- virtual IO_ERR **deserialize** (DataInputStream \*in, INDEX indexStart, int offset, INDEX targetNumElement, INDEX &numElement, int &partial)

*从 DataInputStream 中读取数据并将其反序列化为常量。 请注意，文字常量可能会被部分反序列化。*

- virtual void **nullFill** (const ConstantSP &val)

*用val填充空值。*

- virtual void **setBool** (INDEX index, char val)

*将第 index 个元素设置为 val。*

- virtual void **setChar** (INDEX index, char val)

*将第 index 个元素设置为 val。*

- virtual void **setShort** (INDEX index, short val)

*将第 index 个元素设置为 val。*

- virtual void **setInt** (INDEX index, int val)

*将第 index 个元素设置为 val。*

- virtual void **setLong** (INDEX index, long long val)

*将第 index 个元素设置为 val。*

- virtual void **setIndex** (INDEX index, INDEX val)

*将第 index 个元素设置为 val。*

- virtual void **setFloat** (INDEX index, float val)

*将第 index 个元素设置为 val。*

- virtual void **setDouble** (INDEX index, double val)

*将第 index 个元素设置为 val。*

- virtual void **setString** (INDEX index, const DolphinString &val)

*将第 index 个元素设置为 val。*

- virtual void **setBinary** (INDEX index, int unitLength, const unsigned char \*val)

*将第 index 个元素设置为 val。*

- virtual void **setNull** (INDEX index)

*将第 index 个元素设置为 null。*

- virtual void **setDecimal32** (INDEX index, int scale, int val)

*将第 index 个元素设置为特定比例的 val。*

- virtual void **setDecimal64** (INDEX index, int scale, long long val)

*将第 index 个元素设置为特定比例的 val。*

- virtual bool **set** (INDEX index, const ConstantSP &value, INDEX valueIndex)

*将索引指定的单元格值替换为 valueIndex 指定的新值。*

- virtual bool **set** (INDEX index, const ConstantSP &value)

*用新值替换索引指定的单元格值。*

- virtual bool **set** (INDEX column, INDEX row, const ConstantSP &value)

*用新值替换由列和行索引指定的单元格值。*

- virtual bool **set** (const ConstantSP &index, const ConstantSP &value)

*用值替换索引指定的单元格值。*

- virtual bool **set** (const ConstantSP &index, const ConstantSP &value, const ConstantSP &valueIndex)

*将索引指定的单元格值替换为 valueIndex 指定的新值。*

- virtual bool **setNonNull** (const ConstantSP &index, const ConstantSP &value)

*用值替换索引指定的单元格值。 请注意，setNonNull 在替换数据时将忽略空值。*

- virtual bool **setItem** (INDEX index, const ConstantSP &value)

*用新值替换索引项。 请注意，第 index 列将替换为矩阵中的新值。*

- virtual bool **setColumn** (INDEX index, const ConstantSP &value)

*用新值替换索引列。*

- virtual void **setRowLabel** (const ConstantSP &label)
- virtual void **setColumnLabel** (const ConstantSP &label)
- virtual bool **reshape** (INDEX cols, INDEX **rows**)

*用新的列和行重塑矩阵。 请注意，如果列或行小于当前大小，则矩阵将被截断， 否则附加 void。*

- virtual bool **assign** (const ConstantSP &value)

*将基础数据替换为值。*

- virtual bool **setBool** (INDEX start, int len, const char \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getBoolBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setChar** (INDEX start, int len, const char \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getCharBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setShort** (INDEX start, int len, const short \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getShortBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setInt** (INDEX start, int len, const int \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getIntBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setLong** (INDEX start, int len, const long long \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getLongBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setIndex** (INDEX start, int len, const INDEX \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getIndexBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setFloat** (INDEX start, int len, const float \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getFloatBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setDouble** (INDEX start, int len, const double \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getDoubleBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setString** (INDEX start, int len, const string \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getStringBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setString** (INDEX start, int len, char \*\*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setString** (INDEX start, int len, const DolphinString \*\*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setBinary** (INDEX start, int len, int unitLength, const unsigned char \*buf)

*用buf设置从start到(start + len -1)的二进制数据。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setData** (INDEX start, int len, void \*buf)

*用buf设置从start到(start + len -1)的数据。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setDecimal32** (INDEX start, int len, int scale, const int \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getDecimal32Buffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setDecimal64** (INDEX start, int len, int scale, const long long \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getDecimal64Buffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **add** (INDEX start, INDEX length, long long inc)

*将 inc 添加到从 start 到 (start + length -1) 的基础数据中。*

- virtual bool **add** (INDEX start, INDEX length, double inc)

*将 inc 添加到从 start 到 (start + length -1) 的基础数据中。*

- virtual void **validate** ()

*检查类型时间、分钟、秒、纳米时间的验证， 并且无效的元素将被替换为 null。*

- virtual int **compare** (INDEX index, const ConstantSP &target) const

*将第 index 个单元格与常量目标进行比较*

- virtual bool **getNullFlag** () const

*获取空标志。*

- virtual void **setNullFlag** (bool containNull)

*设置空标志。*

- virtual bool **hasNull** ()

*返回这个常量是否有空值。*

- virtual bool **hasNull** (INDEX start, INDEX length)

*返回这个常量从start到(start + length -1)是否有空值。*

- virtual bool **sizeable** () const

*返回此常量是否可以调整大小。*

- virtual bool **copyable** () const

*返回这个常量是否可以被复制。*

- virtual INDEX **size** () const

*返回此常量的大小。*

- virtual INDEX **itemCount** () const

*返回此常量的项目数。 请注意，矩阵将返回列数。*

- virtual INDEX **rows** () const

*返回行数。*

- virtual INDEX **uncompressedRows** () const

*返回未压缩的行数。*

- virtual INDEX **columns** () const

*返回列数。*

- virtual ConstantSP **getMember** (const ConstantSP &key) const

*根据键从字典中获取数据，空值将用于填充任何缺失的数据。*

- virtual ConstantSP **keys** () const

*从字典中获取所有键。*

- virtual ConstantSP **values** () const

*从字典中获取所有值。*

- virtual bool **hasMethod** (const string &name) const

*返回指定的方法是否在当前对象所属的类中。*

- virtual bool **hasOperator** (const string &name) const

*返回指定的操作符是否在当前对象所属的类中。 运算符可以是对象的方法或属性。*

- virtual FunctionDefSP **getMethod** (const string &name) const

*返回当前对象所属类的指定方法。*

- virtual FunctionDefSP **getOperator** (const string &name) const

*返回当前对象所属类的指定运算符。 运算符可以是对象的方法或属性。*

- virtual ConstantSP **getMember** (const string &key) const

*根据key从字典中获取数据。*

- virtual long long **releaseMemory** (long long target, bool &satisfied)

*如果使用的内存不小于目标，则释放此常量的内存。*

- virtual long long **getAllocatedMemory** () const

*获取此常量的分配内存（以字节为单位）。*

- virtual DATA_TYPE **getRawType** () const =0

*获取此常量的原始数据类型。*

- virtual int **getExtraParamForType** () const

*返回这个常量的额外属性。*

- virtual ConstantSP **getInstance** () const =0

*获取此常量的副本，实际数据除外。*

- virtual ConstantSP **getValue** () const =0

*获取此常量的副本。*

- virtual ConstantSP **getValue** (Heap \*pHeap)

*从指定的堆中获取此常量的副本。*

- virtual ConstantSP **getReference** (Heap \*pHeap)

*如果此常量是临时的，则返回其本身，否则返回此常量的副本。*

- virtual OBJECT_TYPE **getObjectType** () const

*返回此常量的对象类型。*

- virtual SymbolBaseSP **getSymbolBase** () const

*从具有符号类型的常量中获取 SymbolBase。*

- virtual void **contain** (const ConstantSP &target, const ConstantSP &resultSP) const

*返回目标常量是否存在于当前集合或字典键中。 并将返回的布尔值设置为resultSP。*

- virtual bool **isFastMode** () const

*返回此向量是否为规则向量，即所有数据是否连续。*

- virtual void \* **getDataArray** () const

*获取这个常量的底层数据数组。如果数据不连续，则返回 0。*

- virtual void \*\* **getDataSegment** () const

*获取该常量的底层数据数组段。如果数据是连续的，则返回 0。*

- virtual bool **isIndexArray** () const

*返回这个常量是否是索引数组。*

- virtual INDEX \* **getIndexArray** () const

*获取该常量的底层索引数据数组。如果数据不连续，则返回 0。*

- virtual bool **isHugeIndexArray** () const

*返回这个向量是否是一个巨大的索引向量，即所有数据都不连续。*

- virtual INDEX \*\* **getHugeIndexArray** () const

*获取该常量的底层索引数据数组段。如果数据是连续的，则返回 0。*

- virtual int **getSegmentSize** () const

*获取此常量的基础数据数组段的大小。*

- virtual int **getSegmentSizeInBit** () const

*获取数据段的大小（以位为单位）。*

- virtual bool **containNotMarshallableObject** () const

*返回此常量是否包含不可编组的对象。*

### 额外继承的成员函数

#### 静态 Public 属性 继承自 Constant

- static DolphinString **DEMPTY**

*一个空的 DolphinString。*

- static string **EMPTY**

*一个空字符串。*

- static string **NULL_STR**

*此字符串的值为“NULL”。*

#### Protected 成员函数 继承自 Constant

- void **setType** (DATA_TYPE dt)
- void **setCategory** (DATA_CATEGORY dc)
- void **setTypeAndCategory** (DATA_TYPE dt, DATA_CATEGORY dc)

### 成员函数说明

#### virtual bool Table::addSubscriber (const TableUpdateQueueSP & *queue*, const string & *topic*, bool *local*, long long *offset* = -1, const ObjectSP & *filter* = 0)\[inline\], \[virtual\]

将订阅者添加到此流表。

##### 参数

|            |                              |
| ---------- | ---------------------------- |
| *队列：* |                              |
| *topic*  | 订阅任务名称。               |
| *local*  | 订阅是否来自本地主机。       |
| *offset* | 订阅开始的第一条消息的位置。 |
| *filter* | 过滤列中的选定值。           |

##### 返回

> 如果订阅成功则为真，否则为假。

#### virtual bool Table::append (Heap \* *heap*, vector\< ConstantSP \> & *values*, INDEX & *insertedRows*, string & *errMsg*)\[inline\], \[virtual\]

将值附加到此表。

##### 参数

|                                                  |                                        |
| ------------------------------------------------ | -------------------------------------- |
| *堆：*                                         |                                        |
| *values：要追加的值。*                         |                                        |
| *insertedRows:out参数，表示实际插入了多少行。* |                                        |
| *errMsg*                                       | 如果追加失败，错误信息存储在errMsg中。 |

##### 返回

> 如果追加成功则为真，否则为假。

#### virtual bool Table::append (vector\< ConstantSP \> & *values*, INDEX & *insertedRows*, string & *errMsg*)\[pure virtual\]

将值附加到此表。

##### 参数

|                                                  |                                        |
| ------------------------------------------------ | -------------------------------------- |
| *values：要追加的值。*                         |                                        |
| *insertedRows:out参数，表示实际插入了多少行。* |                                        |
| *errMsg*                                       | 如果追加失败，错误信息存储在errMsg中。 |

##### 返回

> 如果追加成功则为真，否则为假。

#### virtual INDEX Table::columns () const\[pure virtual\]

返回列数。

重载 **Constant** .

#### virtual bool Table::drop (vector\< int \> & *columns*)\[inline\], \[virtual\]

删除指定的列。

##### 参数

|             |                  |
| ----------- | ---------------- |
| *columns* | 要删除的列的索引 |

##### 返回

> 如果删除成功则为真，否则为假

#### virtual ConstantSP Table::filter (vector\< ObjectSP \> & *filterExprs*, INDEX *limit* = 0, bool *byKey* = true) const\[inline\], \[virtual\]

按一组列和值关系过滤表。

##### 参数

|                                                                      |                                                         |
| -------------------------------------------------------------------- | ------------------------------------------------------- |
| *filterExprs：输入/输出参数。如果应用过滤器，它将从过滤器中删除。* |                                                         |
| *limit*                                                            | : 0: 每个键没有限制， 正数：前 n 行， 负数：最后 n 行。 |
| *byKey*                                                            | true: 按键限制, false: 全局限制。                       |

##### 返回

> 一个索引向量。如果没有应用过滤器或应用的过滤器满足表的所有行，则返回空指针。

#### virtual ConstantSP Constant::get (const ConstantSP & *index*) const\[inline\], \[virtual\]

> 一旦你从基类重载了一个函数（虚函数或普通函数） 在派生类中，基类中具有相同名称的所有函数都隐藏在 派生类。 参考：https://stackoverflow.com/questions/8816794/overloading-a-virtual-function-in-a-child-class

重载 **Constant** .

#### virtual ConstantSP Table::get (const ConstantSP & *index*) const\[pure virtual\]

根据索引获取数据。

##### 参数

|           |                                                                                   |
| --------- | --------------------------------------------------------------------------------- |
| *index* | 索引给出了要获取的数据的索引。如果索引超出范围， 即负数或大于大小，对应返回空值。 |

##### 返回

> ConstantSP：数据。

重载 **Constant** .

#### virtual ConstantSP Constant::get (INDEX *column*, INDEX *row*) const\[inline\], \[virtual\]

> 一旦你从基类重载了一个函数（虚函数或普通函数） 在派生类中，基类中具有相同名称的所有函数都隐藏在 派生类。 参考：https://stackoverflow.com/questions/8816794/overloading-a-virtual-function-in-a-child-class

重载 **Constant** .

#### virtual ConstantSP Constant::get (INDEX *index*) const\[inline\], \[virtual\]

> 一旦你从基类重载了一个函数（虚函数或普通函数） 在派生类中，基类中具有相同名称的所有函数都隐藏在 派生类。 参考：https://stackoverflow.com/questions/8816794/overloading-a-virtual-function-in-a-child-class

重载 **Constant** .

#### virtual ConstantSP Table::get (INDEX *index*) const\[inline\], \[virtual\]

获取该常量中第 index 个元素的数据。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |

##### 返回

> ConstantSP：数据。

重载 **Constant** .

#### virtual ConstantSP Constant::get (INDEX *offset*, const ConstantSP & *index*) const\[inline\], \[virtual\]

> 一旦你从基类重载了一个函数（虚函数或普通函数） 在派生类中，基类中具有相同名称的所有函数都隐藏在 派生类。 参考：https://stackoverflow.com/questions/8816794/overloading-a-virtual-function-in-a-child-class

重载 **Constant** .

#### virtual long long Table::getAllocatedMemory () const\[pure virtual\]

获取此常量的分配内存（以字节为单位）。

##### 返回

> 以字节为单位分配的内存。

重载 **Constant** .

#### virtual ConstantSP Table::getColumn (const string & *name*) const\[pure virtual\]

根据列名获取指定列。

##### 参数

|          |        |
| -------- | ------ |
| *name* | 列名。 |

##### 返回

> ConstantSP: 指定的列。

#### virtual ConstantSP Table::getColumn (const string & *name*, const ConstantSP & *rowFilter*) const\[pure virtual\]

根据列名和rowFilter获取指定的列和行。

##### 参数

|               |                                                                           |
| ------------- | ------------------------------------------------------------------------- |
| *name*      | 列名。                                                                    |
| *rowFilter* | 如果是pair，表示要获取的行的范围。 如果是索引向量，表示要获取的行的索引。 |

##### 返回

> ConstantSP: 指定的列。

#### virtual ConstantSP Table::getColumn (const string & *qualifier*, const string & *name*) const\[pure virtual\]

根据列名获取指定列。

##### 参数

|                          |        |
| ------------------------ | ------ |
| *限定符：该表的名称。* |        |
| *name*                 | 列名。 |

##### 返回

> ConstantSP: 指定的列。

#### virtual ConstantSP Table::getColumn (const string & *qualifier*, const string & *name*, const ConstantSP & *rowFilter*) const\[pure virtual\]

根据列名和rowFilter获取指定的列和行。

##### 参数

|                          |                                                                           |
| ------------------------ | ------------------------------------------------------------------------- |
| *限定符：该表的名称。* |                                                                           |
| *name*                 | 列名。                                                                    |
| *rowFilter*            | 如果是pair，表示要获取的行的范围。 如果是索引向量，表示要获取的行的索引。 |

##### 返回

> ConstantSP: 指定的列。

#### virtual ConstantSP Table::getColumn (INDEX *index*) const\[pure virtual\]

根据索引获取指定列。

##### 参数

|           |          |
| --------- | -------- |
| *index* | 列索引。 |

##### 返回

> ConstantSP: 指定的列。

重载 **Constant** .

#### virtual ConstantSP Table::getColumn (INDEX *index*, const ConstantSP & *rowFilter*) const\[pure virtual\]

根据列索引和rowFilter获取指定的列和行。

##### 参数

|               |                                                                           |
| ------------- | ------------------------------------------------------------------------- |
| *index*     | 列索引。                                                                  |
| *rowFilter* | 如果是pair，表示要获取的行的范围。 如果是索引向量，表示要获取的行的索引。 |

##### 返回

> ConstantSP: 指定的列。

#### virtual const string & Table::getColumnQualifier (int *index*) const\[pure virtual\]

根据列索引获取源表的限定符。 通常用在连接表中。

##### 参数

|           |          |
| --------- | -------- |
| *index* | 列索引。 |

#### virtual DomainSP Table::getGlobalDomain () const\[inline\], \[virtual\]

返回共享 dfs 表的域。

##### 参数

|           |                |
| --------- | -------------- |
| *index* | 排序键的索引。 |

#### virtual ConstantSP Table::getIterator (const ConstantSP & *self*) const\[virtual\]

获取这个常量本身的迭代器。

重载 **Constant** .

#### virtual int Table::getKeyColumnIndex (int *index*) const\[inline\], \[virtual\]

返回此表中第 index 个键列的列索引。

##### 参数

|           |              |
| --------- | ------------ |
| *index* | 键列的索引。 |

##### 返回

> 该表的列索引。

#### virtual int Table::getLocalPartitionColumnIndex (int *dim*) const\[inline\], \[virtual\]

根据域维度返回该表的分区列索引。

##### 参数

|         |                        |
| ------- | ---------------------- |
| *dim* | 这个表的复合域的维度。 |

#### Mutex \* Table::getLock () const\[inline\]

返回这个字典的锁。

##### 返回

> 指向锁的指针。

#### virtual ConstantSP Table::getMember (const ConstantSP & *key*) const\[pure virtual\]

根据键从字典中获取数据，空值将用于填充任何缺失的数据。

##### 参数

|                                               |  |
| --------------------------------------------- | - |
| *key：应该检索数据的键，可以是标量或向量。* |  |

##### 返回

> 如果键是标量，则值将是标量。 如果键是一个向量，那么值将是一个向量。

重载 **Constant** .

#### virtual DATA_TYPE Table::getRawType () const\[inline\], \[virtual\]

获取此常量的原始数据类型。

##### 返回

> 原始类型。

实现了 **Constant**   .

#### virtual string Table::getScript () const\[inline\], \[virtual\]

返回这个常量的描述。

重载 **Constant** .

#### virtual TableSP Table::getSegment (Heap \* *heap*, const DomainPartitionSP & *partition*, PartitionGuard \* *guard* = 0)\[inline\], \[virtual\]

从这个 dfs 表中检索分区的数据。

##### 参数

|               |                                         |
| ------------- | --------------------------------------- |
| *堆：*      |                                         |
| *partition* | 分区信息。                              |
| *guard*     | 返回表的内存要等到guard销毁后才会释放。 |

##### 返回

> 包含指定分区数据的内存表。

#### virtual TableSP Table::getSegment (Heap \* *heap*, const DomainPartitionSP & *partition*, vector\< ObjectSP \> & *filters*, const vector\< string \> & *colNames*, PartitionGuard \* *guard* = 0, INDEX *limit* = 0, bool *byKey* = false)\[inline\], \[virtual\]

从该表中检索带有过滤器的分区的数据。

##### 参数

|                                                                                            |                                                                   |
| ------------------------------------------------------------------------------------------ | ----------------------------------------------------------------- |
| *堆：*                                                                                   |                                                                   |
| *partition*                                                                              | : 分区信息。                                                      |
| *filters：输入/输出参数。如果所有加载的行都满足一个过滤器，则该过滤器将从过滤器中删除。* |                                                                   |
| *colNames：如果*                                                                         | colNames 为空，则可能会使用数位板的所有列。否则，将使用指定的列。 |
| *guard*                                                                                  | 返回表的内存要等到guard销毁后才会释放。                           |
| *limit*                                                                                  | 要读取的行数，正数表示从头开始，负数表示从尾数，0 表示全部。      |
| *byKey*                                                                                  | 是否按键限制。                                                    |

##### 返回

> 包含指定数据的内存表。

#### virtual void Table::getSnapshot (TableSP & *copy*) const\[inline\], \[virtual\]

获取该表的快照。

##### 参数

|          |                                 |
| -------- | ------------------------------- |
| *copy* | 一个out参数，返回这个表的快照。 |

#### virtual int Table::getSortKeyColumnIndex (int *index*)\[inline\], \[virtual\]

返回此表中第 index 个排序键的列索引。

##### 参数

|           |                |
| --------- | -------------- |
| *index* | 排序键的索引。 |

##### 返回

> 该表的列索引。

#### virtual string Table::getString () const\[pure virtual\]

获取此常量的字符串版本。 这个函数对调试很有帮助。

重载 **Constant** .

#### virtual string Table::getString (INDEX *index*) const\[pure virtual\]

获取此常量中第 index 个元素的字符串值。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |

重载 **Constant** .

#### virtual ConstantSP Table::getValue () const\[pure virtual\]

获取此常量的副本。

实现了 **Constant**   .

#### virtual ConstantSP Table::getWindow (INDEX *colStart*, int *colLength*, INDEX *rowStart*, int *rowLength*) const\[pure virtual\]

从这个常量中获取一个子表。 注意，子表是原表的副本。

##### 参数

|                                            |                                    |
| ------------------------------------------ | ---------------------------------- |
| *colStart：开始列的索引，应该是有效的。* |                                    |
| *colLength*                              | 列长度。可以为负数，表示反向长度。 |
| *rowStart：开始行的索引，应该是有效的。* |                                    |
| *rowLength*                              | 行长度。可以为负数，表示反向长度。 |

##### 返回

> ConstantSP: 子表。

重载 **Constant** .

#### virtual void Table::groupBySortKeys (vector\< string \> & *groupBy*, const ConstantSP & *filter*, vector\< TableSP \> & *tablets*, vector\< string \> & *removedGroupBys*, vector\< ConstantSP \> & *groupedFilters*)\[inline\], \[virtual\]

@brief 根据排序键对数据进行分组。

@param groupBy：输入输出参数。如果 groupBy 列中有 sortKeys，那么我们按 sortkeys 对数据进行分组，并删除

@param sortKey：来自 groupBy 的列。

@param filter: 一个输入参数。整个表的过滤器。如果指定了过滤器，则它必须严格按升序排列。

##### 参数

|                                                          |                                                                                                                                                                       |
| -------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| *tablets*                                              | 一个输出参数。如果groupBy列中有sortKeys，那么我们将数据分组保存到tablets中， 即 tablets 中的每个 tablet 都是一组。如果 groupBy 中没有 sortKey 列，则 tablets 将为空。 |
| *removedGroupBys：一个输出参数。排序键的*              | groupBy 对象。这些对象从输入 groupBy 中移除。                                                                                                                         |
| *groupedFilters：一个输出参数。每个平板电脑的过滤器。* |                                                                                                                                                                       |

#### virtual int Table::isAscendingKey (int *index*)\[inline\], \[virtual\]

返回第 index 个排序键是否按升序排列。

##### 参数

|           |                |
| --------- | -------------- |
| *index* | 排序键的索引。 |

#### virtual bool Table::join (vector\< ConstantSP \> & *columns*)\[inline\], \[virtual\]

向该表中添加列。 请注意，每个列的名称都应与该表不同。

##### 返回

> 如果加入成功则为真，否则为假。

#### virtual ConstantSP Table::keys () const\[pure virtual\]

从字典中获取所有键。

##### 返回

> 所有的钥匙。

重载 **Constant** .

#### virtual bool Table::prepareData (const ConstantSP & *rows*, const vector\< int \> & *cols*)\[inline\], \[virtual\]

提前准备数据。

##### 参数

|          |                                                            |
| -------- | ---------------------------------------------------------- |
| *rows* | 行索引。如果 rows 是空指针，则加载所有行。指数按升序排列。 |
| *cols* | 列索引。如果为空，则加载所有列。指数按升序排列。           |

##### 返回

> ：如果此表支持该功能，则为真。

#### virtual bool Table::remove (const ConstantSP & *indexSP*, string & *errMsg*)\[pure virtual\]

从此表中删除行。

##### 参数

|                                 |                                          |
| ------------------------------- | ---------------------------------------- |
| *indexSP：要删除的行的索引。* |                                          |
| *errMsg*                      | 如果romove失败，错误信息存储在errMsg中。 |

##### 返回

> 如果 romove 成功则为真，否则为假。

#### virtual void Table::remove (Heap \* *heap*, const SQLContextSP & *context*, const ConstantSP & *filterExprs*)\[inline\], \[virtual\]

根据 filterExprs 从该表中删除行。

##### 参数

|                       |                                     |
| --------------------- | ----------------------------------- |
| *堆：*              |                                     |
| *上下文*            |                                     |
| *filterExprs：一个* | MetaCode 向量，指示哪些行需要删除。 |

#### virtual bool Table::removeSubscriber (const TableUpdateQueueSP & *queue*, const string & *topic*)\[inline\], \[virtual\]

根据主题名称从此流表中删除订阅者。

##### 参数

|            |                |
| ---------- | -------------- |
| *队列：* |                |
| *topic*  | 订阅任务名称。 |

##### 返回

> True 如果删除成功，eles false。

#### virtual bool Table::reorderColumns (const vector\< int \> & *newOrders*)\[inline\], \[virtual\]

使用给定的新订单对表的列重新排序。

##### 参数

|                                 |  |
| ------------------------------- | - |
| *newOrders：新订单列的索引。* |  |

##### 返回

> 如果重新排序成功则为真，否则为假。

#### virtual bool Table::replaceColumn (int *index*, const ConstantSP & *col*)\[inline\], \[virtual\]

用新列替换此表的指定列。

##### 参数

|           |                  |
| --------- | ---------------- |
| *index* | 被替换列的索引。 |
| *col*   | 新列。           |

##### 返回

> 如果替换成功则为真，否则为假。

#### virtual ConstantSP Table::retrieveMessage (long long *offset*, int *length*, bool *msgAsTable*, const ObjectSP & *filter*, long long & *messageId*)\[inline\], \[virtual\]

从此表中检索消息。

##### 参数

|                                          |                                                                               |
| ---------------------------------------- | ----------------------------------------------------------------------------- |
| *offset：检索开始的第一条消息的位置。* |                                                                               |
| *length*                               | 返回消息的最大长度。                                                          |
| *msgAsTable*                           | 如果为真，则消息以表格的形式返回。 如果为 false，则消息以任何向量的形式返回。 |
| *filter*                               | 返回的消息会被filter过滤。                                                    |
| *messageId*                            | 一个输出参数，返回该表中最后一条消息的索引。                                  |

##### 返回

> 指定的消息。

#### virtual bool Table::segmentExists (const DomainPartitionSP & *partition*) const\[inline\], \[virtual\]

返回指定分区是否存在于该表中。

##### 参数

|               |              |
| ------------- | ------------ |
| *partition* | : 分区信息。 |

#### virtual void Table::setAccessControl (bool *option*)\[inline\], \[virtual\]

设置该表的访问控制标志。

##### 参数

|                                                              |  |
| ------------------------------------------------------------ | - |
| *选项：如果对该表的操作需要相应的权限，则为真，否则为假。* |  |

#### void Table::setAliasTable (bool *option*)

设置该表的表类型标志。

##### 参数

|                                              |  |
| -------------------------------------------- | - |
| *选项：如果设置为别名表则为真，否则为假。* |  |

#### void Table::setExpired (bool *option*)

设置该表的过期标志。

##### 参数

|                                            |  |
| ------------------------------------------ | - |
| *选项：如果设置为过期则为真，否则为假。* |  |

#### virtual void Table::setGlobalPartition (const DomainSP & *domain*, const string & *partitionColumn*)\[inline\], \[virtual\]

设置此表的全局域。

##### 参数

|                     |                    |
| ------------------- | ------------------ |
| *域：全局域。*    |                    |
| *partitionColumn* | 该表的分区列名称。 |

#### void Table::setRealtimeTable (bool *option*)

设置该表的表类型标志。

##### 参数

|                                            |  |
| ------------------------------------------ | - |
| *选项：如果设置为流表则为真，否则为假。* |  |

#### void Table::setSharedTable (bool *option*)

设置该表的共享属性。

##### 参数

|                                            |  |
| ------------------------------------------ | - |
| *选项：如果设置为共享则为真，否则为假。* |  |

#### virtual INDEX Table::size () const\[pure virtual\]

返回此常量的大小。

重载 **Constant** .

#### virtual bool Table::sizeable () const\[pure virtual\]

返回此常量是否可以调整大小。

重载 **Constant** .

#### virtual void Table::sortBy (Heap \* *heap*, const ObjectSP & *sortExpr*, const ConstantSP & *sortOrder*)\[inline\], \[virtual\]

对该表的某些列进行排序。

##### 参数

|                                                                 |                              |
| --------------------------------------------------------------- | ---------------------------- |
| *堆：*                                                        |                              |
| *sortExpr*                                                    | 一个表示列需要排序的表达式。 |
| *sortOrder：一个布尔标量，表示按升序（真）或降序（假）排序。* |                              |

#### virtual void Table::update (Heap \* *heap*, const SQLContextSP & *context*, const ConstantSP & *updateColNames*, const ObjectSP & *updateExpr*, const ConstantSP & *filterExprs*)\[inline\], \[virtual\]

根据 updateExpr 和 filterExprs 更新这个表的一些列。

##### 参数

|                                                       |                                      |
| ----------------------------------------------------- | ------------------------------------ |
| *堆：*                                              |                                      |
| *上下文*                                            |                                      |
| *updateColNames*                                    | 一个字符串向量，表示需要更新的列名。 |
| *updateExpr：指示如何更新列的表达式。*              |                                      |
| *filterExprs：一个元代码向量，指示哪些行需要更新。* |                                      |

#### virtual bool Table::update (vector\< ConstantSP \> & *values*, const ConstantSP & *indexSP*, vector\< string \> & *colNames*, string & *errMsg*)\[pure virtual\]

根据索引用新值更新此表的某些列。

##### 参数

|                              |                                              |
| ---------------------------- | -------------------------------------------- |
| *values*                   | 新值列，假设values的向量大小与colNames相同。 |
| *indexSP：行索引。*        |                                              |
| *colNames：列名需要更新。* |                                              |
| *errMsg*                   | 如果追加失败，错误信息存储在errMsg中。       |

##### 返回

> 如果更新成功则为真，否则为假。

#### virtual bool Table::upsert (vector\< ConstantSP \> & *values*, bool *ignoreNull*, INDEX & *insertedRows*, string & *errMsg*)\[inline\], \[virtual\]

将行插入到键控表或索引表中，如果 主键尚不存在，如果存在则更新它们。

##### 参数

|                          |                                                          |
| ------------------------ | -------------------------------------------------------- |
| *values：要更新的值。* |                                                          |
| *ignoreNull*           | 如果设置为true，对于values中的NULL值，不更新对应的元素。 |
| *insertedRows*         | out参数，表示实际upsert了多少行。                        |
| *errMsg*               | 如果追加失败，错误信息存储在errMsg中。                   |

##### 返回

> 如果 romove 成功则为真，否则为假。

#### virtual ConstantSP Table::values () const\[pure virtual\]

从字典中获取所有值。

##### 返回

> 所有值。

重载 **Constant** .

## 类：Vector

继承自 **Constant** .

\#include <CoreConcept.h>

### Public 成员函数

- **Vector** (DATA_TYPE dt, DATA_CATEGORY dc)
- virtual ConstantSP **getIterator** (const ConstantSP &self) const

*获取这个常量本身的迭代器。*

- virtual ConstantSP **getColumnLabel** () const
- const string & **getName** () const

*返回这个向量的名称。*

- void **setName** (const string &name)

*设置这个向量的名称。*

- virtual bool **isLargeConstant** () const

*返回这个向量是否是一个大常数。 请注意，大于 1024 的矩阵或常数是大常数。*

- virtual bool **isView** () const

*返回此向量是 SicedVector 还是 SubVector。*

- virtual bool **isRepeatingVector** () const

*返回这个向量是否是一个重复向量。*

- virtual VECTOR_TYPE **getVectorType** () const

*获取此向量的向量类型。*

- virtual void **initialize** ()

*将vector中大小为length的数据初始化为0。*

- virtual INDEX **getCapacity** () const =0

*获取这个向量的容量。*

- virtual INDEX **reserve** (INDEX capacity)

*设置这个向量的容量。*

- virtual void **resize** (INDEX **size**)

*设置这个向量的大小。*

- virtual short **getUnitLength** () const =0

*获取此向量数据类型的大小。*

- virtual void **clear** ()=0

*将此向量的大小设置为0，内存尚未释放。*

- virtual bool **isTableColumn** () const

*返回这个向量是否是一个柱状元组。*

- virtual bool **remove** (INDEX **count**)

*从此向量中删除最后计数的元素。*

- virtual bool **remove** (const ConstantSP &index)

*根据索引从此向量中删除元素。*

- virtual bool **append** (const ConstantSP &value)

*将给定向量的值附加到该向量的末尾。*

- virtual bool **append** (const ConstantSP &value, INDEX **count**)

*将给定向量的指定元素数附加到此向量的末尾。*

- virtual bool **append** (const ConstantSP &value, INDEX start, INDEX **count**)

*将给定向量在指定范围内的值附加到当前向量的末尾。*

- virtual bool **append** (const ConstantSP &value, const ConstantSP &index)

*将索引指定的值附加到向量的末尾。*

- virtual bool **appendBool** (const char \*buf, int len)

*将数据附加到此向量。*

- virtual bool **appendChar** (const char \*buf, int len)

*将数据附加到此向量。*

- virtual bool **appendShort** (const short \*buf, int len)

*将数据附加到此向量。*

- virtual bool **appendInt** (const int \*buf, int len)

*将数据附加到此向量。*

- virtual bool **appendLong** (const long long \*buf, int len)

*将数据附加到此向量。*

- virtual bool **appendIndex** (const INDEX \*buf, int len)

*将数据附加到此向量。*

- virtual bool **appendFloat** (const float \*buf, int len)

*将数据附加到此向量。*

- virtual bool **appendDouble** (const double \*buf, int len)

*将数据附加到此向量。*

- virtual bool **appendString** (const DolphinString \*\*buf, int len)

*将数据附加到此向量。*

- virtual bool **appendString** (const string \*buf, int len)

*将数据附加到此向量。*

- virtual bool **appendString** (const char \*\*buf, int len)

*将数据附加到此向量。*

- virtual bool **appendGuid** (const Guid \*buf, int len)

*将数据附加到此向量。*

- virtual bool **appendBinary** (const unsigned char \*buf, int len, int unitLength)

*将二进制数据附加到此向量。*

- virtual string **getString** () const

*获取此常量的字符串版本。 这个函数对调试很有帮助。*

- virtual string **getString** (Heap \*heap) const

*根据堆中的会话类型获取此常量的字符串版本。 这个函数对调试很有帮助。*

- virtual string **getScript** () const

*返回这个常量的描述。*

- virtual string **getString** (INDEX index) const =0

*获取此常量中第 index 个元素的字符串值。 注意索引要有效，否则会出现越界访问。*

- virtual string **getString** (Heap \*heap, INDEX index) const
- virtual ConstantSP **getInstance** () const

*获取此常量的副本，实际数据除外。*

- virtual ConstantSP **getInstance** (INDEX **size**) const =0

*获取此向量的副本，其中包含空数据和指定大小。*

- virtual ConstantSP **getValue** (INDEX capacity) const

*复制这个向量。*

- virtual ConstantSP **get** (INDEX column, INDEX rowStart, INDEX rowEnd) const

*获取此vertor中指定列的subVector。 请注意，子向量是该向量的副本。*

- virtual ConstantSP **get** (INDEX index) const =0

*获取该常量中第 index 个元素的数据。 注意索引要有效，否则会出现越界访问。*

- virtual ConstantSP **getWindow** (INDEX colStart, int colLength, INDEX rowStart, int rowLength) const

*从这个常量中获取一个子表。 注意，子表是原表的副本。*

- virtual ConstantSP **getSubVector** (INDEX start, INDEX length) const

*获取这个向量的子向量。 请注意，子向量是该向量的副本。*

- virtual ConstantSP **getSubVector** (INDEX start, INDEX length, INDEX capacity) const

*获取此向量的指定容量的子向量。 请注意，子向量是该向量的副本。*

- virtual void **fill** (INDEX start, INDEX length, const ConstantSP &value, INDEX valueOffset=0)=0

*用给定向量的值填充指定范围内向量的值。*

- virtual void **fill** (INDEX start, INDEX length, const ConstantSP &value, const ConstantSP &index)

*用指定索引处给定向量的值填充指定范围内向量的值。*

- virtual void **next** (INDEX steps)=0

*将此向量的元素向左移动一些位置。*

- virtual void **prev** (INDEX steps)=0

*将此向量的元素向右移动一些位置。*

- virtual void **reverse** ()=0

*反转这个向量。*

- virtual void **reverse** (INDEX start, INDEX length)=0

*反转这个向量的指定范围。*

- virtual void **shuffle** ()

*对该向量的数据进行洗牌。*

- virtual void **replace** (const ConstantSP &oldVal, const ConstantSP &newVal)

*将此向量中所有值为 oldVal 的元素替换为 newVal。*

- virtual bool **validIndex** (INDEX uplimit)

*返回这个向量是否是一个有效的索引向量。 一个有效的索引向量不应包含空值，最大值不应超过上限。*

- virtual bool **validIndex** (INDEX start, INDEX length, INDEX uplimit)

*返回这个向量的子向量是否是一个有效的索引向量。 一个有效的索引向量不应包含空值，最大值不应超过上限。*

- virtual void **addIndex** (INDEX start, INDEX length, INDEX offset)

*将值偏移量添加到此向量中指定的值范围。*

- virtual void **neg** ()=0

*将此向量设置为相反的值。*

- virtual void **find** (INDEX start, INDEX length, const ConstantSP &target, const ConstantSP &resultSP)

*在这个向量中找到指定范围内目标第一次出现的索引。 如果目标没有出现在这个向量中，resultSP 将被设置为 -1。*

- virtual void **find** (const ConstantSP &target, const ConstantSP &resultSP)

*找到目标在这个向量中第一次出现的索引。 如果目标没有出现在这个向量中，resultSP 将被设置为 -1。*

- virtual void **binarySearch** (INDEX start, INDEX length, const ConstantSP &target, const ConstantSP &resultSP)

*使用二分查找在这个vector中找到指定范围内目标出现的索引。 如果目标没有出现在这个向量中，resultSP 将被设置为 -1。*

- virtual void **binarySearch** (const ConstantSP &target, const ConstantSP &resultSP)

*使用二分查找找到目标出现在这个向量中的索引。 如果目标没有出现在这个向量中，resultSP 将被设置为 -1。*

- virtual void **asof** (INDEX start, INDEX length, const ConstantSP &target, const ConstantSP &resultSP)

*在此向量中查找指定范围的最后一个不大于目标的元素的索引。 如果没有找到，返回-1。*

- virtual void **asof** (const ConstantSP &target, const ConstantSP &resultSP)

*查找此向量中不大于目标的最后一个元素的索引。 如果没有找到，返回-1。*

- virtual void **upper** ()

*将此向量中的所有字符转换为大写。*

- virtual void **lower** ()

*将此向量中的所有字符转换为小写。*

- virtual void **trim** ()

*修剪这个向量中的所有空白。*

- virtual void **strip** ()

*删除此向量中元素头部和尾部的所有空格、制表符、换行符和回车符。*

- virtual long long **count** () const =0

*返回此向量中非空元素的数量。*

- virtual long long **count** (INDEX start, INDEX length) const =0

*返回此向量中指定范围内的非空元素的数量。*

- virtual ConstantSP **minmax** () const

*返回这个向量的最小值和最大值。*

- virtual ConstantSP **minmax** (INDEX start, INDEX length) const

*返回此向量中指定范围的最小值和最大值。*

- virtual ConstantSP **max** () const =0

*返回这个向量的最大值。*

- virtual ConstantSP **max** (INDEX start, INDEX length) const =0

*返回此向量中指定范围的最大值。*

- virtual void **max** (INDEX start, INDEX length, const ConstantSP &out, INDEX outputStart=0) const =0

*在这个vector中找到指定范围内的最大值，根据outputStart把结果设置到out中。*

- virtual INDEX **imax** () const =0

*返回此向量中最大值的索引。*

- virtual INDEX **imax** (INDEX start, INDEX length) const =0

*返回此向量指定范围内最大值的索引。*

- virtual ConstantSP **min** () const =0

*返回这个向量的最小值。*

- virtual ConstantSP **min** (INDEX start, INDEX length) const =0

*返回此向量指定范围内的最小值。*

- virtual void **min** (INDEX start, INDEX length, const ConstantSP &out, INDEX outputStart=0) const =0

*找到这个vector指定范围内的最小值，根据outputStart把结果设置到out中。*

- virtual INDEX **imin** () const =0

*返回此向量中最小值的索引。*

- virtual INDEX **imin** (INDEX start, INDEX length) const =0

*返回此向量指定范围内最小值的索引。*

- virtual ConstantSP **avg** () const =0

*返回这个向量的平均值。*

- virtual ConstantSP **avg** (INDEX start, INDEX length) const =0

*返回此向量指定范围内的平均值。*

- virtual void **avg** (INDEX start, INDEX length, const ConstantSP &out, INDEX outputStart=0) const =0

*计算这个vector在指定范围内的平均值，根据outputStart把结果设置到out中。*

- virtual ConstantSP **sum** () const =0

*返回此向量中所有元素的总和。*

- virtual ConstantSP **sum** (INDEX start, INDEX length) const =0

*返回此向量指定范围内所有元素的总和。*

- virtual void **sum** (INDEX start, INDEX length, const ConstantSP &out, INDEX outputStart=0) const =0

*计算这个vector指定范围内所有元素的和，根据outputStart把结果设置到out中。*

- virtual ConstantSP **sum2** () const =0

*返回此向量中所有元素的平方和。*

- virtual ConstantSP **sum2** (INDEX start, INDEX length) const =0

*返回此向量指定范围内所有元素的平方和。*

- virtual void **sum2** (INDEX start, INDEX length, const ConstantSP &out, INDEX outputStart=0) const =0

*计算这个vector指定范围内所有元素的平方和，根据outputStart把结果设置到out中。*

- virtual ConstantSP **prd** () const =0

*返回此向量中所有元素的乘积。*

- virtual ConstantSP **prd** (INDEX start, INDEX length) const =0

*返回此向量指定范围内所有元素的乘积。*

- virtual void **prd** (INDEX start, INDEX length, const ConstantSP &out, INDEX outputStart=0) const =0

*计算这个vector指定范围内所有元素的乘积，根据outputStart把结果设置到out中。*

- virtual ConstantSP **var** () const =0

*返回这个向量的方差。*

- virtual ConstantSP **var** (INDEX start, INDEX length) const =0

*返回此向量中指定范围的方差。*

- virtual void **var** (INDEX start, INDEX length, const ConstantSP &out, INDEX outputStart=0) const =0

*计算这个vector中指定范围的方差，根据outputStart把结果设置到out中。*

- virtual ConstantSP **std** () const =0

*返回这个向量的标准偏差。*

- virtual ConstantSP **std** (INDEX start, INDEX length) const =0

*返回此向量中指定范围的标准偏差。*

- virtual void **std** (INDEX start, INDEX length, const ConstantSP &out, INDEX outputStart=0) const =0

*计算这个vector中指定范围的标准差，根据outputStart把结果设置到out中。*

- virtual ConstantSP **median** () const =0

*返回这个向量的中位数。*

- virtual ConstantSP **median** (INDEX start, INDEX length) const =0

*返回此向量中指定范围的中值。*

- virtual void **median** (INDEX start, INDEX length, const ConstantSP &out, INDEX outputStart=0) const =0

*计算这个vector中指定范围的中位数，根据outputStart把结果设置到out中。*

- virtual ConstantSP **searchK** (INDEX k) const =0

*返回此向量的第 k 个最小元素。 而 NULL 值将被忽略。*

- virtual ConstantSP **searchK** (INDEX start, INDEX length, INDEX k) const =0

*返回此向量中指定范围的第 k 个最小元素。 而 NULL 值将被忽略。*

- virtual void **searchK** (INDEX start, INDEX length, INDEX k, const ConstantSP &out, INDEX outputStart=0) const =0
- virtual ConstantSP **mode** () const =0

*返回此向量中出现频率最高的值。*

- virtual ConstantSP **mode** (INDEX start, INDEX length) const =0

*返回此向量中指定范围内最常出现的值。*

- virtual void **mode** (INDEX start, INDEX length, const ConstantSP &out, INDEX outputStart=0) const =0

*在这个vector中找到指定范围内最常出现的值，根据outputStart将结果设置到out。*

- virtual ConstantSP **stat** () const

*返回一个关于这个向量的描述统计的字典，包括 avg、max、min、count、median 和 std。*

- virtual ConstantSP **stat** (INDEX start, INDEX length) const

*返回关于此向量中指定范围的描述性统计数据的字典，包括 avg、max、min、count、median 和 std。*

- virtual ConstantSP **firstNot** (const ConstantSP &exclude) const =0

*返回此向量的第一个既不排除也不为 NULL 的元素。*

- virtual ConstantSP **firstNot** (INDEX start, INDEX length, const ConstantSP &exclude) const =0

*返回此向量中指定范围的第一个既不排除也不为 NULL 的元素。*

- virtual void **firstNot** (INDEX start, INDEX length, const ConstantSP &exclude, const ConstantSP &out, INDEX outputStart=0) const =0

*在这个vector中找到指定范围的第一个既不是exclude也不是NULL的元素，根据outputStart将结果设置为out。*

- virtual ConstantSP **lastNot** (const ConstantSP &exclude) const =0

*返回此向量的最后一个既不排除也不为 NULL 的元素。*

- virtual ConstantSP **lastNot** (INDEX start, INDEX length, const ConstantSP &exclude) const =0

*返回此向量中指定范围内既不排除也不为 NULL 的最后一个元素。*

- virtual void **lastNot** (INDEX start, INDEX length, const ConstantSP &exclude, const ConstantSP &out, INDEX outputStart=0) const =0

*在这个vector中找到指定范围的最后一个既不是exclude也不是NULL的元素，根据outputStart将结果设置为out。*

- virtual bool **isSorted** (bool asc, bool strict=false) const

*返回此向量是否已排序。*

- virtual bool **isSorted** (INDEX start, INDEX length, bool asc, bool strict=false) const =0

*返回此向量的指定范围是否已排序。*

- virtual INDEX **lowerBound** (INDEX start, const ConstantSP &target)=0

*在排序后的向量中找到第一个不小于目标值的元素。如果所有元素都 小于目标值，返回向量的大小。*

- virtual void **equalRange** (const ConstantSP &target, pair\< INDEX, INDEX \> &range) const

*在已排序的向量中查找指定值的范围。*

- virtual bool **equalToPrior** (INDEX start, INDEX length, bool \*result)

*获取这个vector指定范围内的某个元素的值是否与前一个元素相同。*

- virtual bool **equalToPrior** (INDEX prior, const INDEX \*indices, INDEX length, bool \*result)

*获取此向量中第 indices\[i\] 个元素的值是否与第 indices\[i-1\] 个元素相同。*

- virtual ConstantSP **topK** (INDEX start, INDEX length, INDEX top, bool asc, bool extendEqualValue)

*返回此向量指定范围内前 k 个元素的索引。*

- virtual ConstantSP **topK** (INDEX top, bool asc, bool extendEqualValue)

*返回此向量中前 k 个元素的索引。*

- virtual bool **sort** (bool asc)=0

*按给定顺序对整个向量进行排序。*

- virtual bool **sort** (bool asc, **Vector** \*indices)=0

*按给定的顺序对向量和相应的索引进行排序。*

- virtual INDEX **sortTop** (bool asc, **Vector** \*indices, INDEX top)

*按给定顺序对该向量的第一个元素和相应的索引进行排序。*

- virtual bool **sortSelectedIndices** (**Vector** \*indices, INDEX start, INDEX length, bool asc)=0

*根据给定顺序的相应数据对所选索引进行排序。*

- virtual bool **findDuplicatedElements** (**Vector** \*indices, INDEX start, INDEX length, vector\< pair\< INDEX, INDEX \> \> &duplicates)=0
- virtual bool **findDuplicatedElements** (INDEX start, INDEX length, vector\< pair\< INDEX, INDEX \> \> &duplicates)=0

*在升序数组的段中查找重复的元素。*

- virtual bool **findUniqueElements** (INDEX start, INDEX length, vector\< pair\< INDEX, INDEX \> \> &uniques)=0

*在升序数组的段中查找唯一元素。*

- virtual bool **findRange** (INDEX \*ascIndices, const ConstantSP &target, INDEX \*targetIndices, vector\< pair\< INDEX, INDEX \> \> &ranges)=0

*在向量中查找目标的第一个和最后一个索引。*

- virtual bool **findRange** (const ConstantSP &target, INDEX \*targetIndices, vector\< pair\< INDEX, INDEX \> \> &ranges)=0

*在排序向量中查找目标的第一个和最后一个索引。*

- virtual long long **getAllocatedMemory** (INDEX **size**) const
- virtual int **serialize** (char \*buf, int bufSize, INDEX indexStart, int offset, int &numElement, int &partial) const

*将常量序列化到缓冲区。 请注意，文字常量可能会被部分序列化。*

- virtual int **serialize** (char \*buf, int bufSize, INDEX indexStart, int offset, int targetNumElement, int &numElement, int &partial) const

*将常量序列化到缓冲区。 请注意，文字常量可能会被部分序列化。*

- virtual bool **isNullSafe** (INDEX offset, INDEX \*indices, int len, char \*buf) const

*根据索引判断数据是否为空。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）*

- virtual bool **isValidSafe** (INDEX offset, INDEX \*indices, int len, char \*buf) const

*根据索引判断数据是否有效。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）*

- virtual bool **getBoolSafe** (INDEX offset, INDEX \*indices, int len, char \*buf) const

*根据索引获取布尔数据。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）*

- virtual bool **getCharSafe** (INDEX offset, INDEX \*indices, int len, char \*buf) const

*根据索引获取字符数据。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）*

- virtual bool **getShortSafe** (INDEX offset, INDEX \*indices, int len, short \*buf) const

*根据索引获取数据的短值。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）*

- virtual bool **getIntSafe** (INDEX offset, INDEX \*indices, int len, int \*buf) const

*根据索引获取数据的int值。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）*

- virtual bool **getLongSafe** (INDEX offset, INDEX \*indices, int len, long long \*buf) const

*根据索引获取数据的 long long 值。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）*

- virtual bool **getIndexSafe** (INDEX offset, INDEX \*indices, int len, INDEX \*buf) const

*根据索引获取数据的索引值。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）*

- virtual bool **getFloatSafe** (INDEX offset, INDEX \*indices, int len, float \*buf) const

*根据索引获取数据的浮点值。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）*

- virtual bool **getDoubleSafe** (INDEX offset, INDEX \*indices, int len, double \*buf) const

*根据索引获取数据的双精度值。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）*

- virtual bool **getSymbolSafe** (INDEX offset, INDEX \*indices, int len, int \*buf, SymbolBase \*symBase, bool insertIfNotThere) const

*根据索引获取符号数据。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）*

- virtual bool **getStringSafe** (INDEX offset, INDEX \*indices, int len, DolphinString \*\*buf) const

*根据索引获取 DolphinString 数据。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）*

- virtual bool **getStringSafe** (INDEX offset, INDEX \*indices, int len, char \*\*buf) const

*根据索引获取字符串数据。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）*

- virtual bool **getBinarySafe** (INDEX offset, INDEX \*indices, int len, int unitLength, unsigned char \*buf) const

*根据索引获取二进制数据。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）*

- virtual ConstantSP **flatten** (INDEX rowStart, INDEX **count**) const

*将指定的行转换为向量。*

- virtual ConstantSP **rowFirst** (INDEX rowStart, INDEX **count**) const

*返回此向量指定行的第一个元素。*

- virtual ConstantSP **rowLast** (INDEX rowStart, INDEX **count**) const

*返回此向量指定行的最后一个元素。*

- virtual ConstantSP **rowFirstNot** (INDEX rowStart, INDEX **count**, const ConstantSP &exclude) const

*返回此向量指定行的第一个元素，该元素既不排除也不为 NULL。*

- virtual ConstantSP **rowLastNot** (INDEX rowStart, INDEX **count**, const ConstantSP &exclude) const

*返回此向量指定行的最后一个元素，该元素既不排除也不为 NULL。*

- virtual ConstantSP **rowSum** (INDEX rowStart, INDEX **count**) const

*计算此向量指定行的总和。*

- virtual ConstantSP **rowSum2** (INDEX rowStart, INDEX **count**) const

*计算此向量指定行的平方和。*

- virtual ConstantSP **rowCount** (INDEX rowStart, INDEX **count**) const

*返回此向量指定行的非空元素数。*

- virtual ConstantSP **rowSize** (INDEX rowStart, INDEX **count**) const

*返回此向量指定行的大小。*

- virtual ConstantSP **rowAvg** (INDEX rowStart, INDEX **count**) const

*计算此向量指定行的平均值。*

- virtual ConstantSP **rowStd** (INDEX rowStart, INDEX **count**) const

*计算该向量指定行的标准偏差。*

- virtual ConstantSP **rowStdp** (INDEX rowStart, INDEX **count**) const

*计算该向量指定行的总体标准差。*

- virtual ConstantSP **rowVar** (INDEX rowStart, INDEX **count**) const

*计算该向量指定行的方差。*

- virtual ConstantSP **rowVarp** (INDEX rowStart, INDEX **count**) const

*计算该向量指定行的总体方差。*

- virtual ConstantSP **rowMin** (INDEX rowStart, INDEX **count**) const

*返回此向量指定行的最小元素。*

- virtual ConstantSP **rowMax** (INDEX rowStart, INDEX **count**) const

*返回此向量指定行的最大元素。*

- virtual ConstantSP **rowProd** (INDEX rowStart, INDEX **count**) const

*计算此向量指定行的乘积。*

- virtual ConstantSP **rowAnd** (INDEX rowStart, INDEX **count**) const

*计算此向量指定行的所有元素的和结果。*

- virtual ConstantSP **rowOr** (INDEX rowStart, INDEX **count**) const

*计算此向量指定行的所有元素的或结果。*

- virtual ConstantSP **rowXor** (INDEX rowStart, INDEX **count**) const

*计算此向量指定行的所有元素的异或结果。*

- virtual ConstantSP **rowMed** (INDEX rowStart, INDEX **count**) const

*计算此向量指定行的中位数。*

- virtual ConstantSP **rowKurtosis** (INDEX rowStart, INDEX **count**, bool biased) const

*计算该向量指定行的峰度。*

- virtual ConstantSP **rowSkew** (INDEX rowStart, INDEX **count**, bool biased) const

*计算该向量指定行的偏度。*

- virtual ConstantSP **rowPercentile** (INDEX rowStart, INDEX **count**, double percentile) const

*计算此向量指定行的百分位数。 请注意，此功能尚未实现。*

- virtual ConstantSP **rowRank** (INDEX rowStart, INDEX **count**, bool ascending, int groupNum, bool ignoreNA, int tiesMethod, bool percent) const

*计算此向量指定行的排名。 请注意，此功能尚未实现。*

- virtual ConstantSP **rowDenseRank** (INDEX rowStart, INDEX **count**, bool ascending, bool ignoreNA, bool percent) const

*计算此向量指定行的denseRank。 请注意，此功能尚未实现。*

- virtual void **setExternalBuffer** (bool option)

*将此向量设置为外部缓冲区。 请注意向量的内存，因为外部缓冲区在析构时不会被释放。*

- virtual bool **isExternalBuffer** () const
- virtual ConstantSP **get** (INDEX index) const
- virtual ConstantSP **get** (INDEX column, INDEX row) const
- virtual ConstantSP **get** (const ConstantSP &index) const
- virtual ConstantSP **get** (INDEX offset, const ConstantSP &index) const

#### Public 成员函数 继承自 Constant

- **Constant** (unsigned short flag)
- **Constant** (DATA_FORM df, DATA_TYPE dt, DATA_CATEGORY dc)
- bool **isTemporary** () const
- void **setTemporary** (bool val)

*设置常量是否为临时的。*

- bool **isIndependent** () const

*返回这个常量是否独立。 非独立常量表示包含在其他常量中，例如表包含的向量。 常量默认设置为独立。*

- void **setIndependent** (bool val)

*设置常量是否独立。*

- bool **isReadOnly** () const

*返回此常量是否为只读。 只读常量的值不会改变。 常量默认不是只读的。*

- void **setReadOnly** (bool val)

*设置常量是否只读。*

- bool **isReadOnlyArgument** () const

*返回此常量是否为只读参数。 只读参数不能应用于可变函数。 常量默认不设置为只读参数*

- void **setReadOnlyArgument** (bool val)

*设置常量是否为只读参数。*

- bool **isNothing** () const

*返回这个常量是否为空。 在大多数情况下，nothing 常量用作函数的默认参数。 默认情况下，只有 void 标量会被设置为 nothing。*

- void **setNothing** (bool val)

*设置常量是否为空。*

- bool **isStatic** () const

*返回这个常量是否是静态的。 static是指在sql中用常量初始化，所以它的值是不能改变的。 常量默认不是静态的*

- void **setStatic** (bool val)

*设置静态标志。*

- bool **transferAsString** () const

*返回 transferAsString 标志。 TransferAsString 标志用于在将数据附加到远程块时提高序列化的性能。*

- void **transferAsString** (bool val)

*设置 transferAsString 标志。*

- bool **isSynchronized** () const

*返回这个常量是否可以被多个线程访问。 必须在持有锁的同时访问同步常量。 常量默认不同步。*

- void **setSynchronized** (bool val)

*设置同步标志。*

- bool **isOOInstance** () const

*返回这个常量是否是一个面向对象的实例。 默认情况下，常量不是面向对象的实例。*

- void **setOOInstance** (bool val)

*设置面向对象的实例标志。*

- bool **isIndexed** () const

*返回这个常量是否有索引。 常量默认没有索引。*

- void **setIndexed** (bool val)

*设置索引标志。*

- bool **isSeries** () const

*返回这个常量是否有索引。 Series 表示索引单列矩阵。 常量默认不是系列。*

- void **setSeries** (bool val)

*设置系列标志。*

- bool **isTransient** () const

*返回这个常量是否是瞬态的。 必须通过 **getValue()** 获取副本才能使用瞬态常量。 默认情况下常量不是瞬态的。*

- void **setTransient** (bool val)

*等临时标志。*

- DATA_FORM **getForm** () const

*返回此常量的 DATA_FORM。*

- void **setForm** (DATA_FORM df)

*设置此常量的 DATA_FORM。*

- DATA_TYPE **getType** () const

*返回此常量的 DATA_TYPE。*

- DATA_CATEGORY **getCategory** () const

*返回此常量的 DATA_CATEGORY。*

- bool **isScalar** () const

*返回这个常量是否是标量。*

- bool **isArray** () const

*返回这个常量是否是数组。*

- bool **isPair** () const

*返回这个常量是否是对。*

- bool **isMatrix** () const

*返回这个常量是否是矩阵。*

- bool **isVector** () const

*返回这个常量是否是向量。 向量可以是数组、对或矩阵。*

- bool **isTable** () const

*返回这个常量是否是表。*

- bool **isSet** () const

*返回是否设置了这个常量。*

- bool **isDictionary** () const

*返回这个常量是否是字典。*

- bool **isChunk** () const

*返回这个常量是否是块。*

- bool **isSysObj** () const

*返回这个常量是否是系统对象。*

- bool **isTuple** () const

*返回这个常量是否是元组。 元组必须是任意向量。*

- bool **isNumber** () const

*返回这个常量是否是数字。 数字可以是整数、浮点数或十进制。*

- virtual ConstantSP **getIterator** (const ConstantSP &self) const

*获取这个常量本身的迭代器。*

- virtual ConstantSP **next** ()

*返回下一个项目。如果迭代器已经到达末尾，它返回一个空指针，即 item.isNull() 为真。*

- virtual uint64_t **hash** () const
- virtual bool **equal** (const ConstantSP &other) const
- virtual bool **isDatabase** () const

*返回此常量是否为数据库句柄。*

- virtual char **getBool** () const

*返回这个常量的布尔值。*

- virtual char **getChar** () const

*返回这个常量的字符值。*

- virtual short **getShort** () const

*返回这个常量的短值。*

- virtual int **getInt** () const

*返回此常量的 int 值。*

- virtual long long **getLong** () const

*返回这个常量的长值。*

- virtual INDEX **getIndex** () const

*返回这个常量的索引值。*

- virtual float **getFloat** () const

*返回这个常量的浮点值。*

- virtual double **getDouble** () const

*返回这个常量的双精度值。*

- virtual string **getString** () const

*获取此常量的字符串版本。 这个函数对调试很有帮助。*

- virtual string **getString** (Heap \*heap) const

*根据堆中的会话类型获取此常量的字符串版本。 这个函数对调试很有帮助。*

- virtual string **getScript** () const

*返回这个常量的描述。*

- virtual string **getScript** (Heap \*heap) const

*根据堆中的会话类型返回这个常量的描述。*

- virtual const DolphinString & **getStringRef** () const

*返回此常量的 dolphinString 引用*

- virtual const Guid **getInt128** () const

*返回此常量的 guid 值。*

- virtual const unsigned char \* **getBinary** () const

*获取 int128 标量的二进制数据。*

- virtual bool **isNull** () const

*返回这个常量是否为空。*

- virtual void **setBool** (char val)

*将 bool 值设置为此常量。*

- virtual void **setChar** (char val)

*将 char 值设置为此常量。*

- virtual void **setShort** (short val)

*将短值设置为此常量。*

- virtual void **setInt** (int val)

*将 int 值设置为此常量。*

- virtual void **setLong** (long long val)

*将 long 值设置为此常量。*

- virtual void **setIndex** (INDEX val)

*将索引值设置为此常量。*

- virtual void **setFloat** (float val)

*将浮点值设置为此常量。*

- virtual void **setDouble** (double val)

*将双精度值设置为此常量。*

- virtual void **setString** (const DolphinString &val)

*将 DolphinString 值设置为此常量。*

- virtual void **setBinary** (const unsigned char \*val, int unitLength)

*将二进制数据设置为常量。 请注意，此接口仅在 int128 标量中实现。*

- virtual void **setNull** ()

*将空值设置为此常量。*

- virtual char **getBool** (INDEX index) const

*获取此常量中第 index 个元素的 bool 值。 注意索引要有效，否则会出现越界访问。*

- virtual char **getChar** (INDEX index) const

*获取此常量中第 index 个元素的 char 值。 注意索引要有效，否则会出现越界访问。*

- virtual short **getShort** (INDEX index) const

*获取此常量中第 index 个元素的短值。 注意索引要有效，否则会出现越界访问。*

- virtual int **getInt** (INDEX index) const

*获取此常量中第 index 个元素的 int 值。 注意索引要有效，否则会出现越界访问。*

- virtual long long **getLong** (INDEX index) const

*获取此常量中第 index 个元素的 long 值。 注意索引要有效，否则会出现越界访问。*

- virtual INDEX **getIndex** (INDEX index) const

*获取该常量中第 index 个元素的索引值。 注意索引要有效，否则会出现越界访问。*

- virtual float **getFloat** (INDEX index) const

*获取此常量中第 index 个元素的浮点值。 注意索引要有效，否则会出现越界访问。*

- virtual double **getDouble** (INDEX index) const

*获取此常量中第 index 个元素的双精度值。 注意索引要有效，否则会出现越界访问。*

- virtual string **getString** (INDEX index) const

*获取此常量中第 index 个元素的字符串值。 注意索引要有效，否则会出现越界访问。*

- virtual const DolphinString & **getStringRef** (INDEX index) const

*获取此常量中第 index 个元素的 dolphinString 引用。 注意索引要有效，否则会出现越界访问。*

- virtual const Guid **getInt128** (INDEX index) const

*获取此常量中第 index 个元素的 guid 值。 注意索引要有效，否则会出现越界访问。*

- virtual const unsigned char \* **getBinary** (INDEX index) const

*获取此常量中第 index 个元素的二进制数据。 注意索引要有效，否则会出现越界访问。*

- virtual bool **isNull** (INDEX index) const

*返回第 index 个元素是否为 null。*

- virtual int **getDecimal32** (INDEX index, int scale) const

*获取此常量中第 index 个元素的特定比例的 decimal32 数据。 注意索引要有效，否则会出现越界访问。*

- virtual long long **getDecimal64** (INDEX index, int scale) const

*获取此常量中第 index 个元素的特定比例的 decimal64 数据。 注意索引要有效，否则会出现越界访问。*

- virtual ConstantSP **get** (INDEX index) const

*获取该常量中第 index 个元素的数据。 注意索引要有效，否则会出现越界访问。*

- virtual ConstantSP **get** (INDEX column, INDEX row) const

*根据表或矩阵中的行和列索引获取数据。 注意列索引和行索引都要有效，否则会出现越界访问。*

- virtual ConstantSP **get** (const ConstantSP &index) const

*根据索引获取数据。*

- virtual ConstantSP **get** (INDEX offset, const ConstantSP &index) const

*根据索引获取数据。*

- virtual ConstantSP **getColumn** (INDEX index) const

*根据索引获取表或矩阵中指定列的数据。 注意索引要有效，否则会出现越界访问。*

- virtual ConstantSP **getRow** (INDEX index) const

*根据索引获取矩阵中指定行的数据。 注意索引要有效，否则会出现越界访问。*

- virtual ConstantSP **getItem** (INDEX index) const

*根据索引获取指定项的数据。 请注意，第 index 列将在表或矩阵中返回，并且 index 应该有效。*

- virtual ConstantSP **getItems** (const ConstantSP &index) const

*根据索引获取指定item的数据。 如果索引超出范围，则对应返回空值。 请注意，索引列将在表或矩阵中返回。*

- virtual ConstantSP **getWindow** (INDEX colStart, int colLength, INDEX rowStart, int rowLength) const

*从这个常量中获取一个子表。 注意，子表是原表的副本。*

- virtual ConstantSP **getSlice** (const ConstantSP &rowIndex, const ConstantSP &colIndex) const

*根据rowIndex 和colIndex 获取切片数据。 如果索引超出范围，则对应返回空值。*

- virtual ConstantSP **getRowLabel** () const
- virtual ConstantSP **getColumnLabel** () const
- virtual bool **isNull** (INDEX start, int len, char \*buf) const

*判断从start到(start + len -1)的数据是否为null*

- virtual bool **isValid** (INDEX start, int len, char \*buf) const

*判断从start到(start + len -1)的数据是否有效。 所有的param和return都和isNull一样。*

- virtual bool **getBool** (INDEX start, int len, char \*buf) const

*获取从 start 到 (start + len -1) 的布尔数据。*

- virtual bool **getChar** (INDEX start, int len, char \*buf) const

*获取从start到(start + len -1)的字符数据。*

- virtual bool **getShort** (INDEX start, int len, short \*buf) const

*获取从start到(start + len -1)的数据的短值。*

- virtual bool **getInt** (INDEX start, int len, int \*buf) const

*获取从start到(start + len -1)的数据的int值。*

- virtual bool **getLong** (INDEX start, int len, long long \*buf) const

*获取数据从start到(start + len -1)的long long值。*

- virtual bool **getIndex** (INDEX start, int len, INDEX \*buf) const

*获取数据从start到(start + len -1)的索引值。*

- virtual bool **getFloat** (INDEX start, int len, float \*buf) const

*获取数据从start到(start + len -1)的浮点值。*

- virtual bool **getDouble** (INDEX start, int len, double \*buf) const

*获取从 start 到 (start + len -1) 的数据的双精度值。*

- virtual bool **getSymbol** (INDEX start, int len, int \*buf, SymbolBase \*symBase, bool insertIfNotThere) const

*获取从 start 到 (start + len -1) 的符号数据。*

- virtual bool **getString** (INDEX start, int len, DolphinString \*\*buf) const

*获取从 start 到 (start + len -1) 的 DolphinString 数据。*

- virtual bool **getString** (INDEX start, int len, char \*\*buf) const

*获取从start到(start + len -1)的字符串数据。*

- virtual bool **getBinary** (INDEX start, int len, int unitLength, unsigned char \*buf) const

*获取从 start 到 (start + len -1) 的二进制数据。*

- virtual bool **getDecimal32** (INDEX start, int len, int scale, int \*buf) const

*获取从 start 到 (start + len -1) 的特定比例的 decimal32 数据。*

- virtual bool **getDecimal64** (INDEX start, int len, int scale, long long \*buf) const

*获取从 start 到 (start + len -1) 的特定比例的 decimal64 数据。*

- virtual bool **isNull** (INDEX \*indices, int len, char \*buf) const

*根据索引判断数据是否为空。*

- virtual bool **isValid** (INDEX \*indices, int len, char \*buf) const

*根据索引判断数据是否有效。*

- virtual bool **getBool** (INDEX \*indices, int len, char \*buf) const

*根据索引获取布尔数据。*

- virtual bool **getChar** (INDEX \*indices, int len, char \*buf) const

*根据索引获取字符数据。*

- virtual bool **getShort** (INDEX \*indices, int len, short \*buf) const

*根据索引获取数据的短值。*

- virtual bool **getInt** (INDEX \*indices, int len, int \*buf) const

*根据索引获取数据的int值。*

- virtual bool **getLong** (INDEX \*indices, int len, long long \*buf) const

*根据索引获取数据的 long long 值。*

- virtual bool **getIndex** (INDEX \*indices, int len, INDEX \*buf) const

*根据索引获取数据的索引值。*

- virtual bool **getFloat** (INDEX \*indices, int len, float \*buf) const

*根据索引获取数据的浮点值。*

- virtual bool **getDouble** (INDEX \*indices, int len, double \*buf) const

*根据索引获取数据的双精度值。*

- virtual bool **getSymbol** (INDEX \*indices, int len, int \*buf, SymbolBase \*symBase, bool insertIfNotThere) const

*根据索引获取符号数据。*

- virtual bool **getString** (INDEX \*indices, int len, DolphinString \*\*buf) const

*根据索引获取 DolphinString 数据。*

- virtual bool **getString** (INDEX \*indices, int len, char \*\*buf) const

*根据索引获取字符串数据。*

- virtual bool **getBinary** (INDEX \*indices, int len, int unitLength, unsigned char \*buf) const

*根据索引获取二进制数据。*

- virtual bool **getDecimal32** (INDEX \*indices, int len, int scale, int \*buf) const

*根据索引获取特定scale的decimal32数据。*

- virtual bool **getDecimal64** (INDEX \*indices, int len, int scale, long long \*buf) const

*根据索引获取特定比例的decimal64数据。*

- virtual const char \* **getBoolConst** (INDEX start, int len, char \*buf) const

*获取从 start 到 (start + len -1) 的布尔数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const char \* **getCharConst** (INDEX start, int len, char \*buf) const

*获取从 start 到 (start + len -1) 的字符值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const short \* **getShortConst** (INDEX start, int len, short \*buf) const

*获取从 start 到 (start + len -1) 的短值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const int \* **getIntConst** (INDEX start, int len, int \*buf) const

*获取从 start 到 (start + len -1) 的 int 值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const long long \* **getLongConst** (INDEX start, int len, long long \*buf) const

*获取从 start 到 (start + len -1) 的 long long 值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const INDEX \* **getIndexConst** (INDEX start, int len, INDEX \*buf) const

*获取从start到(start + len -1)的索引值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const float \* **getFloatConst** (INDEX start, int len, float \*buf) const

*获取浮点值开始到 (start + len -1)。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const double \* **getDoubleConst** (INDEX start, int len, double \*buf) const

*获取从 start 到 (start + len -1) 的双精度值。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const int \* **getSymbolConst** (INDEX start, int len, int \*buf, SymbolBase \*symBase, bool insertIfNotThere) const

*获取从 start 到 (start + len -1) 的符号数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual DolphinString \*\* **getStringConst** (INDEX start, int len, DolphinString \*\*buf) const

*获取从 start 到 (start + len -1) 的 DolphinString 数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual char \*\* **getStringConst** (INDEX start, int len, char \*\*buf) const

*获取从start到(start + len -1)的字符串数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const unsigned char \* **getBinaryConst** (INDEX start, int len, int unitLength, unsigned char \*buf) const

*获取从 start 到 (start + len -1) 的二进制数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const int \* **getDecimal32Const** (INDEX start, int len, int scale, int \*buf) const

*获取从 start 到 (start + len -1) 的特定刻度的 decimal32 数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual const long long \* **getDecimal64Const** (INDEX start, int len, int scale, long long \*buf) const

*获取从 start 到 (start + len -1) 的特定比例的 decimal64 数据。 这是在 **Constant** 中查看/迭代数据的推荐方法。 注意，如果需要的底层数据是连续的，那么就有 该函数没有发生拷贝，直接返回底层缓冲区； 否则，将数据复制到buf中，并返回buf。*

- virtual char \* **getBoolBuffer** (INDEX start, int len, char \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setBool(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual char \* **getCharBuffer** (INDEX start, int len, char \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setChar(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual short \* **getShortBuffer** (INDEX start, int len, short \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setShort(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual int \* **getIntBuffer** (INDEX start, int len, int \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setInt(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual long long \* **getLongBuffer** (INDEX start, int len, long long \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setLong(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual INDEX \* **getIndexBuffer** (INDEX start, int len, INDEX \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setIndex(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual float \* **getFloatBuffer** (INDEX start, int len, float \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setFloat(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual double \* **getDoubleBuffer** (INDEX start, int len, double \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setDouble(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual unsigned char \* **getBinaryBuffer** (INDEX start, int len, int unitLength, unsigned char \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setBinary(start, len, unitLength, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual void \* **getDataBuffer** (INDEX start, int len, void \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setData(start, len, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual int \* **getDecimal32Buffer** (INDEX start, int len, int scale, int \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setDecimal32(start, len, scale, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual long long \* **getDecimal64Buffer** (INDEX start, int len, int scale, long long \*buf) const

*获取一个缓冲区，用于从 start 到 (start + len -1) 写入数据。 这是在 **Constant** 中写入数据的推荐方法。 此函数始终与 setDecimal64(start, len, scale, buf) 一起使用。 注意如果需要的底层数据是连续的， 则直接返回底层缓冲区； 否则，返回 buf。*

- virtual IO_ERR **serialize** (Heap \*pHeap, const ByteArrayCodeBufferSP &buffer) const

*使用类型代码或数据源序列化常量以进行缓冲。*

- virtual IO_ERR **serialize** (const ByteArrayCodeBufferSP &buffer) const

*将常量序列化到缓冲区。*

- virtual int **serialize** (char \*buf, int bufSize, INDEX indexStart, int offset, int &numElement, int &partial) const

*将常量序列化到缓冲区。 请注意，文字常量可能会被部分序列化。*

- virtual int **serialize** (char \*buf, int bufSize, INDEX indexStart, int offset, int targetNumElement, int &numElement, int &partial) const

*将常量序列化到缓冲区。 请注意，文字常量可能会被部分序列化。*

- virtual IO_ERR **deserialize** (DataInputStream \*in, INDEX indexStart, int offset, INDEX targetNumElement, INDEX &numElement, int &partial)

*从 DataInputStream 中读取数据并将其反序列化为常量。 请注意，文字常量可能会被部分反序列化。*

- virtual void **nullFill** (const ConstantSP &val)

*用val填充空值。*

- virtual void **setBool** (INDEX index, char val)

*将第 index 个元素设置为 val。*

- virtual void **setChar** (INDEX index, char val)

*将第 index 个元素设置为 val。*

- virtual void **setShort** (INDEX index, short val)

*将第 index 个元素设置为 val。*

- virtual void **setInt** (INDEX index, int val)

*将第 index 个元素设置为 val。*

- virtual void **setLong** (INDEX index, long long val)

*将第 index 个元素设置为 val。*

- virtual void **setIndex** (INDEX index, INDEX val)

*将第 index 个元素设置为 val。*

- virtual void **setFloat** (INDEX index, float val)

*将第 index 个元素设置为 val。*

- virtual void **setDouble** (INDEX index, double val)

*将第 index 个元素设置为 val。*

- virtual void **setString** (INDEX index, const DolphinString &val)

*将第 index 个元素设置为 val。*

- virtual void **setBinary** (INDEX index, int unitLength, const unsigned char \*val)

*将第 index 个元素设置为 val。*

- virtual void **setNull** (INDEX index)

*将第 index 个元素设置为 null。*

- virtual void **setDecimal32** (INDEX index, int scale, int val)

*将第 index 个元素设置为特定比例的 val。*

- virtual void **setDecimal64** (INDEX index, int scale, long long val)

*将第 index 个元素设置为特定比例的 val。*

- virtual bool **set** (INDEX index, const ConstantSP &value, INDEX valueIndex)

*将索引指定的单元格值替换为 valueIndex 指定的新值。*

- virtual bool **set** (INDEX index, const ConstantSP &value)

*用新值替换索引指定的单元格值。*

- virtual bool **set** (INDEX column, INDEX row, const ConstantSP &value)

*用新值替换由列和行索引指定的单元格值。*

- virtual bool **set** (const ConstantSP &index, const ConstantSP &value)

*用值替换索引指定的单元格值。*

- virtual bool **set** (const ConstantSP &index, const ConstantSP &value, const ConstantSP &valueIndex)

*将索引指定的单元格值替换为 valueIndex 指定的新值。*

- virtual bool **setNonNull** (const ConstantSP &index, const ConstantSP &value)

*用值替换索引指定的单元格值。 请注意，setNonNull 在替换数据时将忽略空值。*

- virtual bool **setItem** (INDEX index, const ConstantSP &value)

*用新值替换索引项。 请注意，第 index 列将替换为矩阵中的新值。*

- virtual bool **setColumn** (INDEX index, const ConstantSP &value)

*用新值替换索引列。*

- virtual void **setRowLabel** (const ConstantSP &label)
- virtual void **setColumnLabel** (const ConstantSP &label)
- virtual bool **reshape** (INDEX cols, INDEX **rows**)

*用新的列和行重塑矩阵。 请注意，如果列或行小于当前大小，则矩阵将被截断， 否则附加 void。*

- virtual bool **assign** (const ConstantSP &value)

*将基础数据替换为值。*

- virtual bool **setBool** (INDEX start, int len, const char \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getBoolBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setChar** (INDEX start, int len, const char \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getCharBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setShort** (INDEX start, int len, const short \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getShortBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setInt** (INDEX start, int len, const int \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getIntBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setLong** (INDEX start, int len, const long long \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getLongBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setIndex** (INDEX start, int len, const INDEX \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getIndexBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setFloat** (INDEX start, int len, const float \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getFloatBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setDouble** (INDEX start, int len, const double \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getDoubleBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setString** (INDEX start, int len, const string \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getStringBuffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setString** (INDEX start, int len, char \*\*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setString** (INDEX start, int len, const DolphinString \*\*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setBinary** (INDEX start, int len, int unitLength, const unsigned char \*buf)

*用buf设置从start到(start + len -1)的二进制数据。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setData** (INDEX start, int len, void \*buf)

*用buf设置从start到(start + len -1)的数据。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setDecimal32** (INDEX start, int len, int scale, const int \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getDecimal32Buffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **setDecimal64** (INDEX start, int len, int scale, const long long \*buf)

*用buf设置从start到(start + len -1)的数据。 这是在 **Constant** 中写入数据的推荐方法。 该函数常与getDecimal64Buffer(start, len, buf)一起使用。 注意如果buf已经在底层数据中，那么实际上 这个函数没有任何反应；否则，buf 的数据将是 复制。*

- virtual bool **add** (INDEX start, INDEX length, long long inc)

*将 inc 添加到从 start 到 (start + length -1) 的基础数据中。*

- virtual bool **add** (INDEX start, INDEX length, double inc)

*将 inc 添加到从 start 到 (start + length -1) 的基础数据中。*

- virtual void **validate** ()

*检查类型时间、分钟、秒、纳米时间的验证， 并且无效的元素将被替换为 null。*

- virtual int **compare** (INDEX index, const ConstantSP &target) const

*将第 index 个单元格与常量目标进行比较*

- virtual bool **getNullFlag** () const

*获取空标志。*

- virtual void **setNullFlag** (bool containNull)

*设置空标志。*

- virtual bool **hasNull** ()

*返回这个常量是否有空值。*

- virtual bool **hasNull** (INDEX start, INDEX length)

*返回这个常量从start到(start + length -1)是否有空值。*

- virtual bool **sizeable** () const

*返回此常量是否可以调整大小。*

- virtual bool **copyable** () const

*返回这个常量是否可以被复制。*

- virtual INDEX **size** () const

*返回此常量的大小。*

- virtual INDEX **itemCount** () const

*返回此常量的项目数。 请注意，矩阵将返回列数。*

- virtual INDEX **rows** () const

*返回行数。*

- virtual INDEX **uncompressedRows** () const

*返回未压缩的行数。*

- virtual INDEX **columns** () const

*返回列数。*

- virtual ConstantSP **getMember** (const ConstantSP &key) const

*根据键从字典中获取数据，空值将用于填充任何缺失的数据。*

- virtual ConstantSP **keys** () const

*从字典中获取所有键。*

- virtual ConstantSP **values** () const

*从字典中获取所有值。*

- virtual bool **hasMethod** (const string &name) const

*返回指定的方法是否在当前对象所属的类中。*

- virtual bool **hasOperator** (const string &name) const

*返回指定的操作符是否在当前对象所属的类中。 运算符可以是对象的方法或属性。*

- virtual FunctionDefSP **getMethod** (const string &name) const

*返回当前对象所属类的指定方法。*

- virtual FunctionDefSP **getOperator** (const string &name) const

*返回当前对象所属类的指定运算符。 运算符可以是对象的方法或属性。*

- virtual ConstantSP **getMember** (const string &key) const

*根据key从字典中获取数据。*

- virtual long long **releaseMemory** (long long target, bool &satisfied)

*如果使用的内存不小于目标，则释放此常量的内存。*

- virtual long long **getAllocatedMemory** () const

*获取此常量的分配内存（以字节为单位）。*

- virtual DATA_TYPE **getRawType** () const =0

*获取此常量的原始数据类型。*

- virtual int **getExtraParamForType** () const

*返回这个常量的额外属性。*

- virtual ConstantSP **getInstance** () const =0

*获取此常量的副本，实际数据除外。*

- virtual ConstantSP **getValue** () const =0

*获取此常量的副本。*

- virtual ConstantSP **getValue** (Heap \*pHeap)

*从指定的堆中获取此常量的副本。*

- virtual ConstantSP **getReference** (Heap \*pHeap)

*如果此常量是临时的，则返回其本身，否则返回此常量的副本。*

- virtual OBJECT_TYPE **getObjectType** () const

*返回此常量的对象类型。*

- virtual SymbolBaseSP **getSymbolBase** () const

*从具有符号类型的常量中获取 SymbolBase。*

- virtual void **contain** (const ConstantSP &target, const ConstantSP &resultSP) const

*返回目标常量是否存在于当前集合或字典键中。 并将返回的布尔值设置为resultSP。*

- virtual bool **isFastMode** () const

*返回此向量是否为规则向量，即所有数据是否连续。*

- virtual void \* **getDataArray** () const

*获取这个常量的底层数据数组。如果数据不连续，则返回 0。*

- virtual void \*\* **getDataSegment** () const

*获取该常量的底层数据数组段。如果数据是连续的，则返回 0。*

- virtual bool **isIndexArray** () const

*返回这个常量是否是索引数组。*

- virtual INDEX \* **getIndexArray** () const

*获取该常量的底层索引数据数组。如果数据不连续，则返回 0。*

- virtual bool **isHugeIndexArray** () const

*返回这个向量是否是一个巨大的索引向量，即所有数据都不连续。*

- virtual INDEX \*\* **getHugeIndexArray** () const

*获取该常量的底层索引数据数组段。如果数据是连续的，则返回 0。*

- virtual int **getSegmentSize** () const

*获取此常量的基础数据数组段的大小。*

- virtual int **getSegmentSizeInBit** () const

*获取数据段的大小（以位为单位）。*

- virtual bool **containNotMarshallableObject** () const

*返回此常量是否包含不可编组的对象。*

### 额外继承的成员函数

#### 静态 Public 属性 继承自 Constant

- static DolphinString **DEMPTY**

*一个空的 DolphinString。*

- static string **EMPTY**

*一个空字符串。*

- static string **NULL_STR**

*此字符串的值为“NULL”。*

#### Protected 成员函数 继承自 Constant

- void **setType** (DATA_TYPE dt)
- void **setCategory** (DATA_CATEGORY dc)
- void **setTypeAndCategory** (DATA_TYPE dt, DATA_CATEGORY dc)

### 成员函数说明

#### virtual void Vector::addIndex (INDEX *start*, INDEX *length*, INDEX *offset*)\[inline\], \[virtual\]

将值偏移量添加到此向量中指定的值范围。

##### 参数

|            |                      |
| ---------- | -------------------- |
| *start*  | 指定范围的起始位置。 |
| *length* | 指定范围的长度。     |
| *offset* | 将添加的值。         |

#### virtual bool Vector::append (const ConstantSP & *value*)\[inline\], \[virtual\]

将给定向量的值附加到该向量的末尾。

##### 参数

|           |                |
| --------- | -------------- |
| *value* | 要追加的向量。 |

##### 返回

> 成功则为真，否则为假。

#### virtual bool Vector::append (const ConstantSP & *value*, const ConstantSP & *index*)\[inline\], \[virtual\]

将索引指定的值附加到向量的末尾。

##### 参数

|                |                                                                  |
| -------------- | ---------------------------------------------------------------- |
| *值：向量。* |                                                                  |
| *index*      | 索引向量。确保向量中的所有索引均有效，即不小于零且小于值的大小。 |

#### virtual bool Vector::append (const ConstantSP & *value*, INDEX *count*)\[inline\], \[virtual\]

将给定向量的指定元素数附加到此向量的末尾。

##### 参数

|                       |                |
| --------------------- | -------------- |
| *value*             | 要追加的向量。 |
| *count：附加大小。* |                |

##### 返回

> 成功则为真，否则为假。

#### virtual bool Vector::append (const ConstantSP & *value*, INDEX *start*, INDEX *count*)\[inline\], \[virtual\]

将给定向量在指定范围内的值附加到当前向量的末尾。

##### 参数

|           |                            |
| --------- | -------------------------- |
| *value* | 要追加的向量。             |
| *start* | 然后指定值范围的起始索引。 |
| *count* | 范围大小。                 |

##### 返回

> 成功则为真，否则为假。

#### virtual bool Vector::appendBinary (const unsigned char \* *buf*, int *len*, int *unitLength*)\[inline\], \[virtual\]

将二进制数据附加到此向量。

##### 参数

|                                            |                  |
| ------------------------------------------ | ---------------- |
| *buf*                                    | 一个数据缓冲区。 |
| *len*                                    | 缓冲区长度。     |
| *unitLength：这个向量的数据类型的大小。* |                  |

##### 返回

> 成功则为真，否则为假。

#### virtual bool Vector::appendBool (const char \* *buf*, int *len*)\[inline\], \[virtual\]

将数据附加到此向量。

##### 参数

|         |                  |
| ------- | ---------------- |
| *buf* | 一个数据缓冲区。 |
| *len* | 缓冲区长度。     |

##### 返回

> 成功则为真，否则为假。

#### virtual bool Vector::appendChar (const char \* *buf*, int *len*)\[inline\], \[virtual\]

将数据附加到此向量。

##### 参数

|         |                  |
| ------- | ---------------- |
| *buf* | 一个数据缓冲区。 |
| *len* | 缓冲区长度。     |

##### 返回

> 成功则为真，否则为假。

#### virtual bool Vector::appendDouble (const double \* *buf*, int *len*)\[inline\], \[virtual\]

将数据附加到此向量。

##### 参数

|         |                  |
| ------- | ---------------- |
| *buf* | 一个数据缓冲区。 |
| *len* | 缓冲区长度。     |

##### 返回

> 成功则为真，否则为假。

#### virtual bool Vector::appendFloat (const float \* *buf*, int *len*)\[inline\], \[virtual\]

将数据附加到此向量。

##### 参数

|         |                  |
| ------- | ---------------- |
| *buf* | 一个数据缓冲区。 |
| *len* | 缓冲区长度。     |

##### 返回

> 成功则为真，否则为假。

#### virtual bool Vector::appendGuid (const Guid \* *buf*, int *len*)\[inline\], \[virtual\]

将数据附加到此向量。

##### 参数

|         |                  |
| ------- | ---------------- |
| *buf* | 一个数据缓冲区。 |
| *len* | 缓冲区长度。     |

##### 返回

> 成功则为真，否则为假。

#### virtual bool Vector::appendIndex (const INDEX \* *buf*, int *len*)\[inline\], \[virtual\]

将数据附加到此向量。

##### 参数

|         |                  |
| ------- | ---------------- |
| *buf* | 一个数据缓冲区。 |
| *len* | 缓冲区长度。     |

##### 返回

> 成功则为真，否则为假。

#### virtual bool Vector::appendInt (const int \* *buf*, int *len*)\[inline\], \[virtual\]

将数据附加到此向量。

##### 参数

|         |                  |
| ------- | ---------------- |
| *buf* | 一个数据缓冲区。 |
| *len* | 缓冲区长度。     |

##### 返回

> 成功则为真，否则为假。

#### virtual bool Vector::appendLong (const long long \* *buf*, int *len*)\[inline\], \[virtual\]

将数据附加到此向量。

##### 参数

|         |                  |
| ------- | ---------------- |
| *buf* | 一个数据缓冲区。 |
| *len* | 缓冲区长度。     |

##### 返回

> 成功则为真，否则为假。

#### virtual bool Vector::appendShort (const short \* *buf*, int *len*)\[inline\], \[virtual\]

将数据附加到此向量。

##### 参数

|         |                  |
| ------- | ---------------- |
| *buf* | 一个数据缓冲区。 |
| *len* | 缓冲区长度。     |

##### 返回

> 成功则为真，否则为假。

#### virtual bool Vector::appendString (const char \*\* *buf*, int *len*)\[inline\], \[virtual\]

将数据附加到此向量。

##### 参数

|         |                  |
| ------- | ---------------- |
| *buf* | 一个数据缓冲区。 |
| *len* | 缓冲区长度。     |

##### 返回

> 成功则为真，否则为假。

#### virtual bool Vector::appendString (const DolphinString \*\* *buf*, int *len*)\[inline\], \[virtual\]

将数据附加到此向量。

##### 参数

|         |                  |
| ------- | ---------------- |
| *buf* | 一个数据缓冲区。 |
| *len* | 缓冲区长度。     |

##### 返回

> 成功则为真，否则为假。

#### virtual bool Vector::appendString (const string \* *buf*, int *len*)\[inline\], \[virtual\]

将数据附加到此向量。

##### 参数

|         |                  |
| ------- | ---------------- |
| *buf* | 一个数据缓冲区。 |
| *len* | 缓冲区长度。     |

##### 返回

> 成功则为真，否则为假。

#### virtual void Vector::asof (const ConstantSP & *target*, const ConstantSP & *resultSP*)\[inline\], \[virtual\]

查找此向量中不大于目标的最后一个元素的索引。 如果没有找到，返回-1。

##### 参数

|              |                                |
| ------------ | ------------------------------ |
| *target*   | 一个标量。                     |
| *resultSP* | 一个标量，将被设置为结果索引。 |

#### virtual void Vector::asof (INDEX *start*, INDEX *length*, const ConstantSP & *target*, const ConstantSP & *resultSP*)\[inline\], \[virtual\]

在此向量中查找指定范围的最后一个不大于目标的元素的索引。 如果没有找到，返回-1。

##### 参数

|              |                                |
| ------------ | ------------------------------ |
| *start*    | 指定范围的起始位置。           |
| *length*   | 指定范围的长度。               |
| *target*   | 一个标量。                     |
| *resultSP* | 一个标量，将被设置为结果索引。 |

#### virtual ConstantSP Vector::avg () const\[pure virtual\]

返回这个向量的平均值。

##### 返回

> ConstantSP：最小值。

#### virtual ConstantSP Vector::avg (INDEX *start*, INDEX *length*) const\[pure virtual\]

返回此向量指定范围内的平均值。

##### 参数

|            |                      |
| ---------- | -------------------- |
| *start*  | 指定范围的起始位置。 |
| *length* | 指定范围的长度。     |

##### 返回

> ConstantSP：平均值。

#### virtual void Vector::avg (INDEX *start*, INDEX *length*, const ConstantSP & *out*, INDEX *outputStart* = 0) const\[pure virtual\]

计算这个vector在指定范围内的平均值，根据outputStart把结果设置到out中。

##### 参数

|                                                          |                      |
| -------------------------------------------------------- | -------------------- |
| *start*                                                | 指定范围的起始位置。 |
| *length*                                               | 指定范围的长度。     |
| *out*                                                  | 将被设置为结果值。   |
| *outputStart：索引表示out的哪个元素将被设置为结果值。* |                      |

#### virtual void Vector::binarySearch (const ConstantSP & *target*, const ConstantSP & *resultSP*)\[inline\], \[virtual\]

使用二分查找找到目标出现在这个向量中的索引。 如果目标没有出现在这个向量中，resultSP 将被设置为 -1。

##### 参数

|              |                                |
| ------------ | ------------------------------ |
| *target*   | 一个标量。                     |
| *resultSP* | 一个标量，将被设置为结果索引。 |

#### virtual void Vector::binarySearch (INDEX *start*, INDEX *length*, const ConstantSP & *target*, const ConstantSP & *resultSP*)\[inline\], \[virtual\]

使用二分查找在这个vector中找到指定范围内目标出现的索引。 如果目标没有出现在这个向量中，resultSP 将被设置为 -1。

##### 参数

|              |                                |
| ------------ | ------------------------------ |
| *start*    | 指定范围的起始位置。           |
| *length*   | 指定范围的长度。               |
| *target*   | 标量                           |
| *resultSP* | 一个标量，将被设置为结果索引。 |

#### virtual long long Vector::count (INDEX *start*, INDEX *length*) const\[pure virtual\]

返回此向量中指定范围内的非空元素的数量。

##### 参数

|            |                      |
| ---------- | -------------------- |
| *start*  | 指定范围的起始位置。 |
| *length* | 指定范围的长度。     |

#### virtual void Vector::equalRange (const ConstantSP & *target*, pair\< INDEX, INDEX \> & *range*) const\[inline\], \[virtual\]

在已排序的向量中查找指定值的范围。

##### 参数

|            |                                                                                                                                                          |
| ---------- | -------------------------------------------------------------------------------------------------------------------------------------------------------- |
| *target* | 目标值。它必须是一个标量。                                                                                                                               |
| *range*  | in/out参数，格式为pair\<offset, length\>。作为输入参数时， 它指定要搜索的范围。搜索结果也存储在该参数中。 如果没有元素等于目标值，则输出的长度设置为 0。 |

#### virtual bool Vector::equalToPrior (INDEX *prior*, const INDEX \* *indices*, INDEX *length*, bool \* *result*)\[inline\], \[virtual\]

获取此向量中第 indices\[i\] 个元素的值是否与第 indices\[i-1\] 个元素相同。

##### 参数

|                                                            |                               |
| ---------------------------------------------------------- | ----------------------------- |
| *prior：此向量中的第一个元素将与第*                      | indices\[0\] 个元素进行比较。 |
| *indices：索引向量。*                                    |                               |
| *length*                                                 | 指定范围的长度。              |
| *result：将设置结果布尔值的缓冲区，应至少与长度一样长。* |                               |

##### 返回

> 成功则为真，否则为假。

#### virtual bool Vector::equalToPrior (INDEX *start*, INDEX *length*, bool \* *result*)\[inline\], \[virtual\]

获取这个vector指定范围内的某个元素的值是否与前一个元素相同。

##### 参数

|                                                            |                                                      |
| ---------------------------------------------------------- | ---------------------------------------------------- |
| *start*                                                  | 指定范围的起始位置。请注意，start-1 必须是有效索引。 |
| *length*                                                 | 指定范围的长度。                                     |
| *result：将设置结果布尔值的缓冲区，应至少与长度一样长。* |                                                      |

##### 返回

> 成功则为真，否则为假。

#### virtual void Vector::fill (INDEX *start*, INDEX *length*, const ConstantSP & *value*, const ConstantSP & *index*)\[inline\], \[virtual\]

用指定索引处给定向量的值填充指定范围内向量的值。

##### 参数

|            |                                                                  |
| ---------- | ---------------------------------------------------------------- |
| *start*  | 当前要填充的向量的起始位置。                                     |
| *length* | 当前要填充的向量的单元格数。                                     |
| *value*  | 向量                                                             |
| *index*  | 索引向量。确保向量中的所有索引均有效，即不小于零且小于值的大小。 |

#### virtual void Vector::fill (INDEX *start*, INDEX *length*, const ConstantSP & *value*, INDEX *valueOffset* = 0)\[pure virtual\]

用给定向量的值填充指定范围内向量的值。

##### 参数

|                 |                              |
| --------------- | ---------------------------- |
| *start*       | 当前要填充的向量的起始位置。 |
| *length*      | 当前要填充的向量的单元格数。 |
| *value*       | 向量或标量                   |
| *valueOffset* | 一个有效的索引               |

#### virtual void Vector::find (const ConstantSP & *target*, const ConstantSP & *resultSP*)\[inline\], \[virtual\]

找到目标在这个向量中第一次出现的索引。 如果目标没有出现在这个向量中，resultSP 将被设置为 -1。

##### 参数

|              |                                |
| ------------ | ------------------------------ |
| *target*   | 标量                           |
| *resultSP* | 一个标量，将被设置为结果索引。 |

#### virtual void Vector::find (INDEX *start*, INDEX *length*, const ConstantSP & *target*, const ConstantSP & *resultSP*)\[inline\], \[virtual\]

在这个向量中找到指定范围内目标第一次出现的索引。 如果目标没有出现在这个向量中，resultSP 将被设置为 -1。

##### 参数

|              |                                |
| ------------ | ------------------------------ |
| *start*    | 指定范围的起始位置。           |
| *length*   | 指定范围的长度。               |
| *target*   | 标量                           |
| *resultSP* | 一个标量，将被设置为结果索引。 |

#### virtual bool Vector::findDuplicatedElements (INDEX *start*, INDEX *length*, vector\< pair\< INDEX, INDEX \> \> & *duplicates*)\[pure virtual\]

在升序数组的段中查找重复的元素。

##### 参数

|                                      |                                                                                |
| ------------------------------------ | ------------------------------------------------------------------------------ |
| *start*                            | 段的起始位置。                                                                 |
| *length：段的长度。*               |                                                                                |
| *duplicates：重复元素的输出向量。* | pair.first 存储起始位置 数组中重复元素的数量，pair.second 具有重复元素的数量。 |

##### 返回

> 如果找到成功则为真，否则为假。

#### virtual bool Vector::findDuplicatedElements (Vector \* *indices*, INDEX *start*, INDEX *length*, vector\< pair\< INDEX, INDEX \> \> & *duplicates*)\[pure virtual\]

@brief 在升序排列的数组中查找重复的元素。

@param indices：要搜索的数据索引。基础数据未排序。然而指数

是按升序排列的。

##### 参数

|                                      |                                                                                    |
| ------------------------------------ | ---------------------------------------------------------------------------------- |
| *start：索引向量中的起始位置。*    |                                                                                    |
| *length：要处理的索引数。*         |                                                                                    |
| *duplicates：重复元素的输出向量。* | pair.first 存储起始位置 索引数组中重复元素的数量，pair.second 具有重复元素的数量。 |

##### 返回

> 如果找到成功则为真，否则为假。

#### virtual bool Vector::findRange (const ConstantSP & *target*, INDEX \* *targetIndices*, vector\< pair\< INDEX, INDEX \> \> & *ranges*)\[pure virtual\]

在排序向量中查找目标的第一个和最后一个索引。

##### 参数

|                        |                                    |
| ---------------------- | ---------------------------------- |
| *target：目标元素。* |                                    |
| *targetIndices*      | target的索引，假设长度与范围相同。 |
| *ranges*             | 将查找的结果设置为范围。           |

##### 返回

> 如果找到成功则为真，否则为假。

#### virtual bool Vector::findRange (INDEX \* *ascIndices*, const ConstantSP & *target*, INDEX \* *targetIndices*, vector\< pair\< INDEX, INDEX \> \> & *ranges*)\[pure virtual\]

在向量中查找目标的第一个和最后一个索引。

##### 参数

|                          |                                    |
| ------------------------ | ---------------------------------- |
| *ascIndices：此向量按* | ascIndices 升序排列。              |
| *target：目标元素。*   |                                    |
| *targetIndices*        | target的索引，假设长度与范围相同。 |
| *ranges*               | 将查找的结果设置为范围。           |

##### 返回

> 如果找到成功则为真，否则为假。

#### virtual bool Vector::findUniqueElements (INDEX *start*, INDEX *length*, vector\< pair\< INDEX, INDEX \> \> & *uniques*)\[pure virtual\]

在升序数组的段中查找唯一元素。

##### 参数

|                                      |                                                                                |
| ------------------------------------ | ------------------------------------------------------------------------------ |
| *start*                            | 段的起始位置。                                                                 |
| *length：段的长度。*               |                                                                                |
| *duplicates：唯一元素的输出向量。* | pair.first 存储起始位置 数组中唯一元素的数量，pair.second 具有唯一元素的数量。 |

##### 返回

> 如果找到成功则为真，否则为假。

#### virtual ConstantSP Vector::firstNot (const ConstantSP & *exclude*) const\[pure virtual\]

返回此向量的第一个既不排除也不为 NULL 的元素。

##### 参数

|                      |  |
| -------------------- | - |
| *排除：一个标量。* |  |

##### 返回

> ConstantSP：第一个元素。

#### virtual ConstantSP Vector::firstNot (INDEX *start*, INDEX *length*, const ConstantSP & *exclude*) const\[pure virtual\]

返回此向量中指定范围的第一个既不排除也不为 NULL 的元素。

##### 参数

|                      |                      |
| -------------------- | -------------------- |
| *start*            | 指定范围的起始位置。 |
| *length*           | 指定范围的长度。     |
| *排除：一个标量。* |                      |

##### 返回

> ConstantSP：第一个元素。

#### virtual void Vector::firstNot (INDEX *start*, INDEX *length*, const ConstantSP & *exclude*, const ConstantSP & *out*, INDEX *outputStart* = 0) const\[pure virtual\]

在这个vector中找到指定范围的第一个既不是exclude也不是NULL的元素，根据outputStart将结果设置为out。

##### 参数

|                                                          |                      |
| -------------------------------------------------------- | -------------------- |
| *start*                                                | 指定范围的起始位置。 |
| *length*                                               | 指定范围的长度。     |
| *exclude*                                              | 一个标量             |
| *out*                                                  | 将被设置为结果值。   |
| *outputStart：索引表示out的哪个元素将被设置为结果值。* |                      |

#### virtual ConstantSP Vector::flatten (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

将指定的行转换为向量。

> 数组向量必须实现以下方法。

##### 参数

|                                  |        |
| -------------------------------- | ------ |
| *rowStart：开始转换的行索引。* |        |
| *count*                        | 行数。 |

##### 返回

> ConstantSP：一个向量。

#### virtual ConstantSP Constant::get (const ConstantSP & *index*) const\[inline\], \[virtual\]

> 一旦你从基类重载了一个函数（虚函数或普通函数） 在派生类中，基类中具有相同名称的所有函数都隐藏在 派生类。 参考：https://stackoverflow.com/questions/8816794/overloading-a-virtual-function-in-a-child-class

重载 **Constant** .

#### virtual ConstantSP Constant::get (INDEX *column*, INDEX *row*) const\[inline\], \[virtual\]

> 一旦你从基类重载了一个函数（虚函数或普通函数） 在派生类中，基类中具有相同名称的所有函数都隐藏在 派生类。 参考：https://stackoverflow.com/questions/8816794/overloading-a-virtual-function-in-a-child-class

重载 **Constant** .

#### virtual ConstantSP Vector::get (INDEX *column*, INDEX *rowStart*, INDEX *rowEnd*) const\[inline\], \[virtual\]

获取此vertor中指定列的subVector。 请注意，子向量是该向量的副本。

##### 参数

|            |          |
| ---------- | -------- |
| *column* | 列索引。 |
| *行开始* |          |
| *rowEnd* |          |

##### 返回

> ConstantSP：子向量。

#### virtual ConstantSP Constant::get (INDEX *index*) const\[inline\], \[virtual\]

> 一旦你从基类重载了一个函数（虚函数或普通函数） 在派生类中，基类中具有相同名称的所有函数都隐藏在 派生类。 参考：https://stackoverflow.com/questions/8816794/overloading-a-virtual-function-in-a-child-class

重载 **Constant** .

#### virtual ConstantSP Vector::get (INDEX *index*) const\[pure virtual\]

获取该常量中第 index 个元素的数据。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |

##### 返回

> ConstantSP：数据。

重载 **Constant** .

#### virtual ConstantSP Constant::get (INDEX *offset*, const ConstantSP & *index*) const\[inline\], \[virtual\]

> 一旦你从基类重载了一个函数（虚函数或普通函数） 在派生类中，基类中具有相同名称的所有函数都隐藏在 派生类。 参考：https://stackoverflow.com/questions/8816794/overloading-a-virtual-function-in-a-child-class

重载 **Constant** .

#### virtual bool Vector::getBinarySafe (INDEX *offset*, INDEX \* *indices*, int *len*, int *unitLength*, unsigned char \* *buf*) const\[inline\], \[virtual\]

根据索引获取二进制数据。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）

##### 参数

|                                        |                           |
| -------------------------------------- | ------------------------- |
| *offset*                             | indices的值被offset抵消。 |
| *len*                                | 要检索的数据的长度。      |
| *unitLength：该常量数据类型的长度。* |                           |
| *buf*                                | 结果存储在buf中。         |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual bool Vector::getBoolSafe (INDEX *offset*, INDEX \* *indices*, int *len*, char \* *buf*) const\[inline\], \[virtual\]

根据索引获取布尔数据。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）

##### 参数

|            |                           |
| ---------- | ------------------------- |
| *offset* | indices的值被offset抵消。 |
| *len*    | 要检索的数据的长度。      |
| *buf*    | 结果存储在buf中。         |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual bool Vector::getCharSafe (INDEX *offset*, INDEX \* *indices*, int *len*, char \* *buf*) const\[inline\], \[virtual\]

根据索引获取字符数据。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）

##### 参数

|            |                           |
| ---------- | ------------------------- |
| *offset* | indices的值被offset抵消。 |
| *len*    | 要检索的数据的长度。      |
| *buf*    | 结果存储在buf中。         |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual ConstantSP Vector::getColumnLabel () const\[virtual\]

重载 **Constant** .

#### virtual bool Vector::getDoubleSafe (INDEX *offset*, INDEX \* *indices*, int *len*, double \* *buf*) const\[inline\], \[virtual\]

根据索引获取数据的双精度值。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）

##### 参数

|            |                           |
| ---------- | ------------------------- |
| *offset* | indices的值被offset抵消。 |
| *len*    | 要检索的数据的长度。      |
| *buf*    | 结果存储在buf中。         |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual bool Vector::getFloatSafe (INDEX *offset*, INDEX \* *indices*, int *len*, float \* *buf*) const\[inline\], \[virtual\]

根据索引获取数据的浮点值。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）

##### 参数

|            |                           |
| ---------- | ------------------------- |
| *offset* | indices的值被offset抵消。 |
| *len*    | 要检索的数据的长度。      |
| *buf*    | 结果存储在buf中。         |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual bool Vector::getIndexSafe (INDEX *offset*, INDEX \* *indices*, int *len*, INDEX \* *buf*) const\[inline\], \[virtual\]

根据索引获取数据的索引值。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）

##### 参数

|            |                           |
| ---------- | ------------------------- |
| *offset* | indices的值被offset抵消。 |
| *len*    | 要检索的数据的长度。      |
| *buf*    | 结果存储在buf中。         |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual ConstantSP Vector::getInstance () const\[inline\], \[virtual\]

获取此常量的副本，实际数据除外。

实现了 **Constant**   .

#### virtual bool Vector::getIntSafe (INDEX *offset*, INDEX \* *indices*, int *len*, int \* *buf*) const\[inline\], \[virtual\]

根据索引获取数据的int值。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）

##### 参数

|            |                           |
| ---------- | ------------------------- |
| *offset* | indices的值被offset抵消。 |
| *len*    | 要检索的数据的长度。      |
| *buf*    | 结果存储在buf中。         |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual ConstantSP Vector::getIterator (const ConstantSP & *self*) const\[virtual\]

获取这个常量本身的迭代器。

重载 **Constant** .

#### virtual bool Vector::getLongSafe (INDEX *offset*, INDEX \* *indices*, int *len*, long long \* *buf*) const\[inline\], \[virtual\]

根据索引获取数据的 long long 值。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）

##### 参数

|            |                           |
| ---------- | ------------------------- |
| *offset* | indices的值被offset抵消。 |
| *len*    | 要检索的数据的长度。      |
| *buf*    | 结果存储在buf中。         |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### const string & Vector::getName () const\[inline\]

返回这个向量的名称。

##### 返回

> 向量名称。

#### virtual string Vector::getScript () const\[virtual\]

返回这个常量的描述。

重载 **Constant** .

#### virtual bool Vector::getShortSafe (INDEX *offset*, INDEX \* *indices*, int *len*, short \* *buf*) const\[inline\], \[virtual\]

根据索引获取数据的短值。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）

##### 参数

|            |                           |
| ---------- | ------------------------- |
| *offset* | indices的值被offset抵消。 |
| *len*    | 要检索的数据的长度。      |
| *buf*    | 结果存储在buf中。         |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual string Vector::getString () const\[virtual\]

获取此常量的字符串版本。 这个函数对调试很有帮助。

重载 **Constant** .

#### virtual string Vector::getString (Heap \* *heap*) const\[virtual\]

根据堆中的会话类型获取此常量的字符串版本。 这个函数对调试很有帮助。

##### 参数

|          |                        |
| -------- | ---------------------- |
| *heap* | 堆表示不同形式的结果。 |

重载 **Constant** .

#### virtual string Vector::getString (INDEX *index*) const\[pure virtual\]

获取此常量中第 index 个元素的字符串值。 注意索引要有效，否则会出现越界访问。

##### 参数

|           |                           |
| --------- | ------------------------- |
| *index* | 要获取的第 index 个元素。 |

重载 **Constant** .

#### virtual bool Vector::getStringSafe (INDEX *offset*, INDEX \* *indices*, int *len*, char \*\* *buf*) const\[inline\], \[virtual\]

根据索引获取字符串数据。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）

##### 参数

|            |                           |
| ---------- | ------------------------- |
| *offset* | indices的值被offset抵消。 |
| *len*    | 要检索的数据的长度。      |
| *buf*    | 结果存储在buf中。         |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual bool Vector::getStringSafe (INDEX *offset*, INDEX \* *indices*, int *len*, DolphinString \*\* *buf*) const\[inline\], \[virtual\]

根据索引获取 DolphinString 数据。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）

##### 参数

|            |                           |
| ---------- | ------------------------- |
| *offset* | indices的值被offset抵消。 |
| *len*    | 要检索的数据的长度。      |
| *buf*    | 结果存储在buf中。         |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual ConstantSP Vector::getSubVector (INDEX *start*, INDEX *length*) const\[inline\], \[virtual\]

获取这个向量的子向量。 请注意，子向量是该向量的副本。

##### 参数

|          |           |
| -------- | --------- |
| *开始* | @参数长度 |

##### 返回

> ConstantSP：子向量。

#### virtual ConstantSP Vector::getSubVector (INDEX *start*, INDEX *length*, INDEX *capacity*) const\[inline\], \[virtual\]

获取此向量的指定容量的子向量。 请注意，子向量是该向量的副本。

##### 参数

|          |           |
| -------- | --------- |
| *开始* | @参数长度 |

##### 返回

> ConstantSP：子向量。

#### virtual bool Vector::getSymbolSafe (INDEX *offset*, INDEX \* *indices*, int *len*, int \* *buf*, SymbolBase \* *symBase*, bool *insertIfNotThere*) const\[inline\], \[virtual\]

根据索引获取符号数据。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）

##### 参数

|                      |                                                          |
| -------------------- | -------------------------------------------------------- |
| *start*            | 起始索引。                                               |
| *len*              | 要检索的数据的长度。                                     |
| *buf*              | 结果存储在buf中。                                        |
| *symBase*          | SymbolBase 是一个从字符串到整数的映射。                  |
| *insertIfNotThere* | 是否应该将在 Symbase 中找不到的字符串插入到 Symbase 中。 |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual ConstantSP Vector::getValue (INDEX *capacity*) const\[inline\], \[virtual\]

复制这个向量。

##### 参数

|                              |  |
| ---------------------------- | - |
| *capacity：新向量的容量。* |  |

##### 返回

> ConstantSP：新向量。

#### virtual ConstantSP Vector::getWindow (INDEX *colStart*, int *colLength*, INDEX *rowStart*, int *rowLength*) const\[inline\], \[virtual\]

从这个常量中获取一个子表。 注意，子表是原表的副本。

##### 参数

|                                            |                                    |
| ------------------------------------------ | ---------------------------------- |
| *colStart：开始列的索引，应该是有效的。* |                                    |
| *colLength*                              | 列长度。可以为负数，表示反向长度。 |
| *rowStart：开始行的索引，应该是有效的。* |                                    |
| *rowLength*                              | 行长度。可以为负数，表示反向长度。 |

##### 返回

> ConstantSP: 子表。

重载 **Constant** .

#### virtual INDEX Vector::imax () const\[pure virtual\]

返回此向量中最大值的索引。

##### 返回

> 最大值的索引。

#### virtual INDEX Vector::imax (INDEX *start*, INDEX *length*) const\[pure virtual\]

返回此向量指定范围内最大值的索引。

##### 参数

|            |                      |
| ---------- | -------------------- |
| *start*  | 指定范围的起始位置。 |
| *length* | 指定范围的长度。     |

##### 返回

> 最大值的索引。

#### virtual INDEX Vector::imin () const\[pure virtual\]

返回此向量中最小值的索引。

##### 返回

> 最小值索引。

#### virtual INDEX Vector::imin (INDEX *start*, INDEX *length*) const\[pure virtual\]

返回此向量指定范围内最小值的索引。

##### 参数

|            |                      |
| ---------- | -------------------- |
| *start*  | 指定范围的起始位置。 |
| *length* | 指定范围的长度。     |

##### 返回

> 最小值索引。

#### virtual bool Vector::isNullSafe (INDEX *offset*, INDEX \* *indices*, int *len*, char \* *buf*) const\[inline\], \[virtual\]

根据索引判断数据是否为空。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）

##### 参数

|             |                           |
| ----------- | ------------------------- |
| *offset*  | indices的值被offset抵消。 |
| *indices* | 一个排序的索引向量。      |
| *len*     | 需要判断的数据长度。      |
| *buf*     | 结果存储在buf中。         |

##### 返回

> 如果函数调用成功则为真，否则为假。

#### virtual bool Vector::isSorted (bool *asc*, bool *strict* = false) const\[inline\], \[virtual\]

返回此向量是否已排序。

##### 参数

|            |                                                 |
| ---------- | ----------------------------------------------- |
| *asc*    | 表示这个向量是按升序(true)还是降序(false)排序。 |
| *strict* | 表示这个向量是否严格递增（递减）。              |

##### 返回

> 如果排序则为真，否则为假。

#### virtual bool Vector::isSorted (INDEX *start*, INDEX *length*, bool *asc*, bool *strict* = false) const\[pure virtual\]

返回此向量的指定范围是否已排序。

##### 参数

|            |                                                 |
| ---------- | ----------------------------------------------- |
| *start*  | 指定范围的起始位置。                            |
| *length* | 指定范围的长度。                                |
| *asc*    | 表示这个向量是按升序(true)还是降序(false)排序。 |
| *strict* | 表示这个向量是否严格递增（递减）。              |

##### 返回

> 如果排序则为真，否则为假。

#### virtual bool Vector::isValidSafe (INDEX *offset*, INDEX \* *indices*, int *len*, char \* *buf*) const\[inline\], \[virtual\]

根据索引判断数据是否有效。 安全运营商假设： (1) indices 是升序排列的 (2) offset + indices 保证有效（介于 0 和 size -1 之间）

##### 参数

|             |                           |
| ----------- | ------------------------- |
| *offset*  | indices的值被offset抵消。 |
| *indices* | 一个排序的索引向量。      |
| *len*     | 需要判断的数据长度。      |
| *buf*     | 结果存储在buf中。         |

##### 返回

> true: 函数调用成功。
>
> 如果函数调用成功则为真，否则为假。

#### virtual ConstantSP Vector::lastNot (const ConstantSP & *exclude*) const\[pure virtual\]

返回此向量的最后一个既不排除也不为 NULL 的元素。

##### 参数

|             |          |
| ----------- | -------- |
| *exclude* | 一个标量 |

##### 返回

> ConstantSP：第一个元素。

#### virtual ConstantSP Vector::lastNot (INDEX *start*, INDEX *length*, const ConstantSP & *exclude*) const\[pure virtual\]

返回此向量中指定范围内既不排除也不为 NULL 的最后一个元素。

##### 参数

|             |                      |
| ----------- | -------------------- |
| *start*   | 指定范围的起始位置。 |
| *length*  | 指定范围的长度。     |
| *exclude* | 一个标量             |

##### 返回

> ConstantSP：最后一个元素。

#### virtual void Vector::lastNot (INDEX *start*, INDEX *length*, const ConstantSP & *exclude*, const ConstantSP & *out*, INDEX *outputStart* = 0) const\[pure virtual\]

在这个vector中找到指定范围的最后一个既不是exclude也不是NULL的元素，根据outputStart将结果设置为out。

##### 参数

|                                                          |                      |
| -------------------------------------------------------- | -------------------- |
| *start*                                                | 指定范围的起始位置。 |
| *length*                                               | 指定范围的长度。     |
| *exclude*                                              | 一个标量             |
| *out*                                                  | 将被设置为结果值。   |
| *outputStart：索引表示out的哪个元素将被设置为结果值。* |                      |

#### virtual INDEX Vector::lowerBound (INDEX *start*, const ConstantSP & *target*)\[pure virtual\]

在排序后的向量中找到第一个不小于目标值的元素。如果所有元素都 小于目标值，返回向量的大小。

##### 参数

|           |              |
| --------- | ------------ |
| *start* | 搜索的起点。 |

##### 返回

> 找到的元素的索引。

#### virtual ConstantSP Vector::max () const\[pure virtual\]

返回这个向量的最大值。

##### 返回

> ConstantSP：最大值。

#### virtual ConstantSP Vector::max (INDEX *start*, INDEX *length*) const\[pure virtual\]

返回此向量中指定范围的最大值。

##### 参数

|            |                      |
| ---------- | -------------------- |
| *start*  | 指定范围的起始位置。 |
| *length* | 指定范围的长度。     |

##### 返回

> ConstantSP：最大值。

#### virtual void Vector::max (INDEX *start*, INDEX *length*, const ConstantSP & *out*, INDEX *outputStart* = 0) const\[pure virtual\]

在这个vector中找到指定范围内的最大值，根据outputStart把结果设置到out中。

##### 参数

|                                                          |                      |
| -------------------------------------------------------- | -------------------- |
| *start*                                                | 指定范围的起始位置。 |
| *length*                                               | 指定范围的长度。     |
| *out*                                                  | 将被设置为结果值。   |
| *outputStart：索引表示out的哪个元素将被设置为结果值。* |                      |

#### virtual ConstantSP Vector::median () const\[pure virtual\]

返回这个向量的中位数。

##### 返回

> ConstantSP：中位数。

#### virtual ConstantSP Vector::median (INDEX *start*, INDEX *length*) const\[pure virtual\]

返回此向量中指定范围的中值。

##### 参数

|            |                      |
| ---------- | -------------------- |
| *start*  | 指定范围的起始位置。 |
| *length* | 指定范围的长度。     |

##### 返回

> ConstantSP：中位数。

#### virtual void Vector::median (INDEX *start*, INDEX *length*, const ConstantSP & *out*, INDEX *outputStart* = 0) const\[pure virtual\]

计算这个vector中指定范围的中位数，根据outputStart把结果设置到out中。

##### 参数

|                                                          |                      |
| -------------------------------------------------------- | -------------------- |
| *start*                                                | 指定范围的起始位置。 |
| *length*                                               | 指定范围的长度。     |
| *out*                                                  | 将被设置为结果值。   |
| *outputStart：索引表示out的哪个元素将被设置为结果值。* |                      |

#### virtual ConstantSP Vector::min () const\[pure virtual\]

返回这个向量的最小值。

##### 返回

> ConstantSP：最小值。

#### virtual ConstantSP Vector::min (INDEX *start*, INDEX *length*) const\[pure virtual\]

返回此向量指定范围内的最小值。

##### 参数

|            |                      |
| ---------- | -------------------- |
| *start*  | 指定范围的起始位置。 |
| *length* | 指定范围的长度。     |

##### 返回

> ConstantSP：最小值。

#### virtual void Vector::min (INDEX *start*, INDEX *length*, const ConstantSP & *out*, INDEX *outputStart* = 0) const\[pure virtual\]

找到这个vector指定范围内的最小值，根据outputStart把结果设置到out中。

##### 参数

|                                                          |                      |
| -------------------------------------------------------- | -------------------- |
| *start*                                                | 指定范围的起始位置。 |
| *length*                                               | 指定范围的长度。     |
| *out*                                                  | 将被设置为结果值。   |
| *outputStart：索引表示out的哪个元素将被设置为结果值。* |                      |

#### virtual ConstantSP Vector::minmax () const\[virtual\]

返回这个向量的最小值和最大值。

##### 返回

> ConstantSP: 一对，第一个元素为最小值，第二个元素为最大值。

#### virtual ConstantSP Vector::minmax (INDEX *start*, INDEX *length*) const\[virtual\]

返回此向量中指定范围的最小值和最大值。

##### 参数

|            |                      |
| ---------- | -------------------- |
| *start*  | 指定范围的起始位置。 |
| *length* | 指定范围的长度。     |

##### 返回

> ConstantSP: 一对，第一个元素为最小值，第二个元素为最大值。

#### virtual ConstantSP Vector::mode () const\[pure virtual\]

返回此向量中出现频率最高的值。

##### 返回

> ConstantSP：值。

#### virtual ConstantSP Vector::mode (INDEX *start*, INDEX *length*) const\[pure virtual\]

返回此向量中指定范围内最常出现的值。

##### 参数

|            |                      |
| ---------- | -------------------- |
| *start*  | 指定范围的起始位置。 |
| *length* | 指定范围的长度。     |

##### 返回

> ConstantSP：值。

#### virtual void Vector::mode (INDEX *start*, INDEX *length*, const ConstantSP & *out*, INDEX *outputStart* = 0) const\[pure virtual\]

在这个vector中找到指定范围内最常出现的值，根据outputStart将结果设置到out。

##### 参数

|                                                          |                      |
| -------------------------------------------------------- | -------------------- |
| *start*                                                | 指定范围的起始位置。 |
| *length*                                               | 指定范围的长度。     |
| *out*                                                  | 将被设置为结果值。   |
| *outputStart：索引表示out的哪个元素将被设置为结果值。* |                      |

#### virtual void Vector::next (INDEX *steps*)\[pure virtual\]

将此向量的元素向左移动一些位置。

##### 参数

|           |                    |
| --------- | ------------------ |
| *steps* | 表示要移动的长度。 |

#### virtual ConstantSP Vector::prd () const\[pure virtual\]

返回此向量中所有元素的乘积。

##### 返回

> ConstantSP：产品。

#### virtual ConstantSP Vector::prd (INDEX *start*, INDEX *length*) const\[pure virtual\]

返回此向量指定范围内所有元素的乘积。

##### 参数

|            |                      |
| ---------- | -------------------- |
| *start*  | 指定范围的起始位置。 |
| *length* | 指定范围的长度。     |

##### 返回

> ConstantSP：产品。

#### virtual void Vector::prd (INDEX *start*, INDEX *length*, const ConstantSP & *out*, INDEX *outputStart* = 0) const\[pure virtual\]

计算这个vector指定范围内所有元素的乘积，根据outputStart把结果设置到out中。

##### 参数

|                                                          |                      |
| -------------------------------------------------------- | -------------------- |
| *start*                                                | 指定范围的起始位置。 |
| *length*                                               | 指定范围的长度。     |
| *out*                                                  | 将被设置为结果值。   |
| *outputStart：索引表示out的哪个元素将被设置为结果值。* |                      |

#### virtual void Vector::prev (INDEX *steps*)\[pure virtual\]

将此向量的元素向右移动一些位置。

##### 参数

|           |                        |
| --------- | ---------------------- |
| *steps* | 表示要移动多少个位置。 |

#### virtual bool Vector::remove (const ConstantSP & *index*)\[inline\], \[virtual\]

根据索引从此向量中删除元素。

##### 参数

|                                                     |  |
| --------------------------------------------------- | - |
| *index：确保索引有效，即不小于零且小于列的大小。* |  |

#### virtual void Vector::replace (const ConstantSP & *oldVal*, const ConstantSP & *newVal*)\[inline\], \[virtual\]

将此向量中所有值为 oldVal 的元素替换为 newVal。

##### 参数

|            |              |
| ---------- | ------------ |
| *oldVal* | 要替换的值。 |
| *newVal* | 新值。       |

#### virtual INDEX Vector::reserve (INDEX *capacity*)\[inline\], \[virtual\]

设置这个向量的容量。

##### 返回

> 保留后的实际容量。

#### virtual void Vector::resize (INDEX *size*)\[inline\], \[virtual\]

设置这个向量的大小。

##### 返回

> 调整大小后的实际大小。

#### virtual void Vector::reverse (INDEX *start*, INDEX *length*)\[pure virtual\]

反转这个向量的指定范围。

##### 参数

|                              |                    |
| ---------------------------- | ------------------ |
| *start*                    | 要反转的起始位置。 |
| *length：要反转的元素数。* |                    |

#### virtual ConstantSP Vector::rowAnd (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

计算此向量指定行的所有元素的和结果。

##### 参数

|                                  |        |
| -------------------------------- | ------ |
| *rowStart：开始计算的行索引。* |        |
| *count*                        | 行数。 |

##### 返回

> ConstantSP ：一个向量，包含指定行的计算结果。

#### virtual ConstantSP Vector::rowAvg (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

计算此向量指定行的平均值。

##### 参数

|                                  |        |
| -------------------------------- | ------ |
| *rowStart：开始计算的行索引。* |        |
| *count*                        | 行数。 |

##### 返回

> ConstantSP：一个向量，包含指定行的计算结果。

#### virtual ConstantSP Vector::rowCount (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

返回此向量指定行的非空元素数。

##### 参数

|                                  |        |
| -------------------------------- | ------ |
| *rowStart：开始计数的行索引。* |        |
| *count*                        | 行数。 |

##### 返回

> ConstantSP：一个向量。

#### virtual ConstantSP Vector::rowFirst (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

返回此向量指定行的第一个元素。

##### 参数

|              |          |
| ------------ | -------- |
| *rowStart* | 行索引。 |
| *count*    | 行数。   |

##### 返回

> ConstantSP：一个向量。

#### virtual ConstantSP Vector::rowFirstNot (INDEX *rowStart*, INDEX *count*, const ConstantSP & *exclude*) const\[inline\], \[virtual\]

返回此向量指定行的第一个元素，该元素既不排除也不为 NULL。

##### 参数

|                      |          |
| -------------------- | -------- |
| *rowStart*         | 行索引。 |
| *count*            | 行数。   |
| *排除：一个标量。* |          |

##### 返回

> ConstantSP：一个向量。

#### virtual ConstantSP Vector::rowKurtosis (INDEX *rowStart*, INDEX *count*, bool *biased*) const\[inline\], \[virtual\]

计算该向量指定行的峰度。

##### 参数

|                                  |        |
| -------------------------------- | ------ |
| *rowStart：开始计算的行索引。* |        |
| *count*                        | 行数。 |

##### 返回

> ConstantSP ：一个向量，包含指定行的计算结果。

#### virtual ConstantSP Vector::rowLast (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

返回此向量指定行的最后一个元素。

##### 参数

|              |          |
| ------------ | -------- |
| *rowStart* | 行索引。 |
| *count*    | 行数。   |

##### 返回

> ConstantSP：一个向量。

#### virtual ConstantSP Vector::rowLastNot (INDEX *rowStart*, INDEX *count*, const ConstantSP & *exclude*) const\[inline\], \[virtual\]

返回此向量指定行的最后一个元素，该元素既不排除也不为 NULL。

##### 参数

|                      |          |
| -------------------- | -------- |
| *rowStart*         | 行索引。 |
| *count*            | 行数。   |
| *排除：一个标量。* |          |

##### 返回

> ConstantSP：一个向量。

#### virtual ConstantSP Vector::rowMax (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

返回此向量指定行的最大元素。

##### 参数

|              |          |
| ------------ | -------- |
| *rowStart* | 行索引。 |
| *count*    | 行数。   |

##### 返回

> ConstantSP ：一个向量。

#### virtual ConstantSP Vector::rowMed (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

计算此向量指定行的中位数。

##### 参数

|                                  |        |
| -------------------------------- | ------ |
| *rowStart：开始计算的行索引。* |        |
| *count*                        | 行数。 |

##### 返回

> ConstantSP ：一个向量，包含指定行的计算结果。

#### virtual ConstantSP Vector::rowMin (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

返回此向量指定行的最小元素。

##### 参数

|              |          |
| ------------ | -------- |
| *rowStart* | 行索引。 |
| *count*    | 行数。   |

##### 返回

> ConstantSP：一个向量。

#### virtual ConstantSP Vector::rowOr (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

计算此向量指定行的所有元素的或结果。

##### 参数

|                                  |        |
| -------------------------------- | ------ |
| *rowStart：开始计算的行索引。* |        |
| *count*                        | 行数。 |

##### 返回

> ConstantSP ：一个向量，包含指定行的计算结果。

#### virtual ConstantSP Vector::rowProd (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

计算此向量指定行的乘积。

##### 参数

|                                  |        |
| -------------------------------- | ------ |
| *rowStart：开始计算的行索引。* |        |
| *count*                        | 行数。 |

##### 返回

> ConstantSP ：一个向量，包含指定行的计算结果。

#### virtual ConstantSP Vector::rowSize (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

返回此向量指定行的大小。

##### 参数

|              |          |
| ------------ | -------- |
| *rowStart* | 行索引。 |
| *count*    | 行数。   |

##### 返回

> ConstantSP：一个向量。

#### virtual ConstantSP Vector::rowSkew (INDEX *rowStart*, INDEX *count*, bool *biased*) const\[inline\], \[virtual\]

计算该向量指定行的偏度。

##### 参数

|                                  |                      |
| -------------------------------- | -------------------- |
| *rowStart：开始计算的行索引。* |                      |
| *count*                        | 行数。               |
| *biased*                       | 表示结果是否有偏差。 |

##### 返回

> ConstantSP ：一个向量，包含指定行的计算结果。

#### virtual ConstantSP Vector::rowStd (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

计算该向量指定行的标准偏差。

##### 参数

|                                  |        |
| -------------------------------- | ------ |
| *rowStart：开始计算的行索引。* |        |
| *count*                        | 行数。 |

##### 返回

> ConstantSP：一个向量，包含指定行的计算结果。

#### virtual ConstantSP Vector::rowStdp (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

计算该向量指定行的总体标准差。

##### 参数

|                                  |        |
| -------------------------------- | ------ |
| *rowStart：开始计算的行索引。* |        |
| *count*                        | 行数。 |

##### 返回

> ConstantSP：一个向量，包含指定行的计算结果。

#### virtual ConstantSP Vector::rowSum (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

计算此向量指定行的总和。

##### 参数

|                                  |        |
| -------------------------------- | ------ |
| *rowStart：开始计算的行索引。* |        |
| *count*                        | 行数。 |

##### 返回

> ConstantSP：一个向量，包含指定行的计算结果。

#### virtual ConstantSP Vector::rowSum2 (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

计算此向量指定行的平方和。

##### 参数

|                                  |        |
| -------------------------------- | ------ |
| *rowStart：开始计算的行索引。* |        |
| *count*                        | 行数。 |

##### 返回

> ConstantSP：一个向量，包含指定行的计算结果。

#### virtual ConstantSP Vector::rowVar (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

计算该向量指定行的方差。

##### 参数

|                                  |        |
| -------------------------------- | ------ |
| *rowStart：开始计算的行索引。* |        |
| *count*                        | 行数。 |

##### 返回

> ConstantSP：一个向量，包含指定行的计算结果。

#### virtual ConstantSP Vector::rowVarp (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

计算该向量指定行的总体方差。

##### 参数

|                                  |        |
| -------------------------------- | ------ |
| *rowStart：开始计算的行索引。* |        |
| *count*                        | 行数。 |

##### 返回

> ConstantSP：一个向量，包含指定行的计算结果。

#### virtual ConstantSP Vector::rowXor (INDEX *rowStart*, INDEX *count*) const\[inline\], \[virtual\]

计算此向量指定行的所有元素的异或结果。

##### 参数

|                                  |        |
| -------------------------------- | ------ |
| *rowStart：开始计算的行索引。* |        |
| *count*                        | 行数。 |

##### 返回

> ConstantSP ：一个向量，包含指定行的计算结果。

#### virtual ConstantSP Vector::searchK (INDEX *k*) const\[pure virtual\]

返回此向量的第 k 个最小元素。 而 NULL 值将被忽略。

##### 参数

|       |  |
| ----- | - |
| *k* |  |

##### 返回

> ConstantSP：第k小的元素。

#### virtual ConstantSP Vector::searchK (INDEX *start*, INDEX *length*, INDEX *k*) const\[pure virtual\]

返回此向量中指定范围的第 k 个最小元素。 而 NULL 值将被忽略。

##### 参数

|            |                      |
| ---------- | -------------------- |
| *start*  | 指定范围的起始位置。 |
| *length* | 指定范围的长度。     |
| *k*      |                      |

##### 返回

> ConstantSP：第k小的元素。

#### virtual int Vector::serialize (char \* *buf*, int *bufSize*, INDEX *indexStart*, int *offset*, int & *numElement*, int & *partial*) const\[inline\], \[virtual\]

将常量序列化到缓冲区。 请注意，文字常量可能会被部分序列化。

##### 参数

|                |                                            |
| -------------- | ------------------------------------------ |
| *buf*        | 序列化后的数据存放在buf中。                |
| *bufSize*    | 缓冲区的长度。                             |
| *indexStart* | 开始序列化的元素的索引                     |
| *offset*     | 第indexStart元素中已经部分序列化的字节数。 |
| *numElement* | 返回已经完全序列化的元素个数。             |
| *partial*    | 返回已部分序列化的元素的字节数。           |

##### 返回

> 实际序列化到buf中的字节数。

重载 **Constant** .

#### virtual int Vector::serialize (char \* *buf*, int *bufSize*, INDEX *indexStart*, int *offset*, int *targetNumElement*, int & *numElement*, int & *partial*) const\[virtual\]

将常量序列化到缓冲区。 请注意，文字常量可能会被部分序列化。

##### 参数

|                                                |                                            |
| ---------------------------------------------- | ------------------------------------------ |
| *buf*                                        | 序列化后的数据存放在buf中。                |
| *bufSize*                                    | 缓冲区的长度。                             |
| *indexStart：开始序列化的元素的索引。*       |                                            |
| *offset*                                     | 第indexStart元素中已经部分序列化的字节数。 |
| *targetNumElement：表示要序列化的元素个数。* |                                            |
| *numElement*                                 | 返回已经完全序列化的元素个数。             |
| *partial*                                    | 返回已部分序列化的元素的字节数。           |

##### 返回

> 实际序列化到buf中的字节数。

重载 **Constant** .

#### virtual void Vector::setExternalBuffer (bool *option*)\[inline\], \[virtual\]

将此向量设置为外部缓冲区。 请注意向量的内存，因为外部缓冲区在析构时不会被释放。

##### 参数

|            |                                |
| ---------- | ------------------------------ |
| *option* | 表明这个向量是否是外部缓冲区。 |

#### void Vector::setName (const string & *name*)\[inline\]

设置这个向量的名称。

##### 参数

|          |            |
| -------- | ---------- |
| *name* | 向量名称。 |

#### virtual bool Vector::sort (bool *asc*)\[pure virtual\]

按给定顺序对整个向量进行排序。

##### 参数

|         |                  |
| ------- | ---------------- |
| *asc* | 表示是否为升序。 |

##### 返回

> 如果排序成功则为真，否则为假。

#### virtual bool Vector::sort (bool *asc*, Vector \* *indices*)\[pure virtual\]

按给定的顺序对向量和相应的索引进行排序。

##### 参数

|             |                                                                                   |
| ----------- | --------------------------------------------------------------------------------- |
| *asc*     | 表示是否为升序。                                                                  |
| *indices* | 待排序数据对应的索引，数据长度应等于 索引的长度。索引将在排序期间相应地重新排列。 |

##### 返回

> 如果排序成功则为真，否则为假。

#### virtual bool Vector::sortSelectedIndices (Vector \* *indices*, INDEX *start*, INDEX *length*, bool *asc*)\[pure virtual\]

根据给定顺序的相应数据对所选索引进行排序。

##### 参数

|                                   |                  |
| --------------------------------- | ---------------- |
| *indices：要排序的选定索引。*   |                  |
| *start：索引向量中的起始位置。* |                  |
| *length：要排序的索引数。*      |                  |
| *asc*                           | 表示是否为升序。 |

##### 返回

> 如果排序成功则为真，否则为假。

#### virtual INDEX Vector::sortTop (bool *asc*, Vector \* *indices*, INDEX *top*)\[inline\], \[virtual\]

按给定顺序对该向量的第一个元素和相应的索引进行排序。

##### 参数

|             |                        |
| ----------- | ---------------------- |
| *asc*     | 表示是否为升序。       |
| *indices* | 待排序数据对应的索引。 |

##### 返回

> 如果排序成功则为真，否则为假。

#### virtual ConstantSP Vector::stat () const\[virtual\]

返回一个关于这个向量的描述统计的字典，包括 avg、max、min、count、median 和 std。

##### 返回

> ConstantSP：字典。

#### virtual ConstantSP Vector::stat (INDEX *start*, INDEX *length*) const\[virtual\]

返回关于此向量中指定范围的描述性统计数据的字典，包括 avg、max、min、count、median 和 std。

##### 返回

> ConstantSP：字典。

#### virtual ConstantSP Vector::std () const\[pure virtual\]

返回这个向量的标准偏差。

##### 返回

> ConstantSP：总和。

#### virtual ConstantSP Vector::std (INDEX *start*, INDEX *length*) const\[pure virtual\]

返回此向量中指定范围的标准偏差。

##### 参数

|            |                      |
| ---------- | -------------------- |
| *start*  | 指定范围的起始位置。 |
| *length* | 指定范围的长度。     |

##### 返回

> ConstantSP：标准偏差。

#### virtual void Vector::std (INDEX *start*, INDEX *length*, const ConstantSP & *out*, INDEX *outputStart* = 0) const\[pure virtual\]

计算这个vector中指定范围的标准差，根据outputStart把结果设置到out中。

##### 参数

|                                                          |                      |
| -------------------------------------------------------- | -------------------- |
| *start*                                                | 指定范围的起始位置。 |
| *length*                                               | 指定范围的长度。     |
| *out*                                                  | 将被设置为结果值。   |
| *outputStart：索引表示out的哪个元素将被设置为结果值。* |                      |

#### virtual ConstantSP Vector::sum () const\[pure virtual\]

返回此向量中所有元素的总和。

##### 返回

> ConstantSP：总和。

#### virtual ConstantSP Vector::sum (INDEX *start*, INDEX *length*) const\[pure virtual\]

返回此向量指定范围内所有元素的总和。

##### 参数

|            |                      |
| ---------- | -------------------- |
| *start*  | 指定范围的起始位置。 |
| *length* | 指定范围的长度。     |

##### 返回

> ConstantSP：总和。

#### virtual void Vector::sum (INDEX *start*, INDEX *length*, const ConstantSP & *out*, INDEX *outputStart* = 0) const\[pure virtual\]

计算这个vector指定范围内所有元素的和，根据outputStart把结果设置到out中。

##### 参数

|                                                          |                      |
| -------------------------------------------------------- | -------------------- |
| *start*                                                | 指定范围的起始位置。 |
| *length*                                               | 指定范围的长度。     |
| *out*                                                  | 将被设置为结果值。   |
| *outputStart：索引表示out的哪个元素将被设置为结果值。* |                      |

#### virtual ConstantSP Vector::sum2 () const\[pure virtual\]

返回此向量中所有元素的平方和。

##### 返回

> ConstantSP：平方和。

#### virtual ConstantSP Vector::sum2 (INDEX *start*, INDEX *length*) const\[pure virtual\]

返回此向量指定范围内所有元素的平方和。

##### 参数

|            |                      |
| ---------- | -------------------- |
| *start*  | 指定范围的起始位置。 |
| *length* | 指定范围的长度。     |

##### 返回

> ConstantSP：平方和。

#### virtual void Vector::sum2 (INDEX *start*, INDEX *length*, const ConstantSP & *out*, INDEX *outputStart* = 0) const\[pure virtual\]

计算这个vector指定范围内所有元素的平方和，根据outputStart把结果设置到out中。

##### 参数

|                                                          |                      |
| -------------------------------------------------------- | -------------------- |
| *start*                                                | 指定范围的起始位置。 |
| *length*                                               | 指定范围的长度。     |
| *out*                                                  | 将被设置为结果值。   |
| *outputStart：索引表示out的哪个元素将被设置为结果值。* |                      |

#### virtual ConstantSP Vector::topK (INDEX *start*, INDEX *length*, INDEX *top*, bool *asc*, bool *extendEqualValue*)\[inline\], \[virtual\]

返回此向量指定范围内前 k 个元素的索引。

##### 参数

|                      |                                                                                                          |
| -------------------- | -------------------------------------------------------------------------------------------------------- |
| *start*            | 指定范围的起始位置。请注意，start-1 必须是有效索引。                                                     |
| *length*           | 指定范围的长度。                                                                                         |
| *top*              | 顶部元素的数量。                                                                                         |
| *asc*              | 按升序(true) 或降序(false)。                                                                             |
| *extendEqualValue* | 如果为 false：返回 topk 元素的索引。 如果为真：按照asc查找第k个元素， 返回大于或等于它的所有元素的索引。 |

##### 返回

> : ConstantSP: 索引向量。

#### virtual ConstantSP Vector::topK (INDEX *top*, bool *asc*, bool *extendEqualValue*)\[inline\], \[virtual\]

返回此向量中前 k 个元素的索引。

##### 参数

|                      |                                                                                                          |
| -------------------- | -------------------------------------------------------------------------------------------------------- |
| *top*              | 顶部元素的数量。                                                                                         |
| *asc*              | 按升序(true) 或降序(false)。                                                                             |
| *extendEqualValue* | 如果为 false：返回 topk 元素的索引。 如果为真：按照asc查找第k个元素， 返回大于或等于它的所有元素的索引。 |

##### 返回

> : ConstantSP: 索引向量。

#### virtual bool Vector::validIndex (INDEX *start*, INDEX *length*, INDEX *uplimit*)\[inline\], \[virtual\]

返回这个向量的子向量是否是一个有效的索引向量。 一个有效的索引向量不应包含空值，最大值不应超过上限。

##### 参数

|                                                    |                    |
| -------------------------------------------------- | ------------------ |
| *start*                                          | 子向量的起始位置。 |
| *length*                                         | 子向量的长度。     |
| *uplimit：有效索引向量的最大值不应超过uplimit。* |                    |

##### 返回

> 如果有效则为真，否则为假

#### virtual bool Vector::validIndex (INDEX *uplimit*)\[inline\], \[virtual\]

返回这个向量是否是一个有效的索引向量。 一个有效的索引向量不应包含空值，最大值不应超过上限。

##### 参数

|                                                    |  |
| -------------------------------------------------- | - |
| *uplimit：有效索引向量的最大值不应超过uplimit。* |  |

##### 返回

> 如果有效则为真，否则为假

#### virtual ConstantSP Vector::var () const\[pure virtual\]

返回这个向量的方差。

##### 返回

> ConstantSP：方差。

#### virtual ConstantSP Vector::var (INDEX *start*, INDEX *length*) const\[pure virtual\]

返回此向量中指定范围的方差。

##### 参数

|            |                      |
| ---------- | -------------------- |
| *start*  | 指定范围的起始位置。 |
| *length* | 指定范围的长度。     |

##### 返回

> ConstantSP：方差。

#### virtual void Vector::var (INDEX *start*, INDEX *length*, const ConstantSP & *out*, INDEX *outputStart* = 0) const\[pure virtual\]

计算这个vector中指定范围的方差，根据outputStart把结果设置到out中。

##### 参数

|                                                          |                      |
| -------------------------------------------------------- | -------------------- |
| *start*                                                | 指定范围的起始位置。 |
| *length*                                               | 指定范围的长度。     |
| *out*                                                  | 将被设置为结果值。   |
| *outputStart：索引表示out的哪个元素将被设置为结果值。* |                      |

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

- 包含类 Constant, Vector, Table及其成员函数的头文件：[CoreConcept.h](include/CoreConcept.h)
- 包含上述除了类 constant, vector 和 table 的类、结构体和成员函数的头文件：[Swordfish.h](include/Swordfish.h)
- [DolphinDB 用户手册](https://www.dolphindb.cn/cn/help/index.html)
