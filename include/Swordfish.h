#ifndef EMBEDDED_OLTP_H_
#define EMBEDDED_OLTP_H_

#include "CoreConcept.h"
#include "Concepts.h"

#include <exception>
#include <memory>
#include <string>
#include <vector>
#include <type_traits>

/**
 * @defgroup Version Version Macros
 * @{
 */
/** Library major version. */
#define SWORDFISH_VERSION_MAJOR 3
/** Library minor version. */
#define SWORDFISH_VERSION_MINOR 0
/** Library patch version. */
#define SWORDFISH_VERSION_PATCH 1
/** @} */

//==============================================================================
// DolphinDB Runtime Utilities
//==============================================================================

class DolphinDBLib {
public:
    /**
     * @brief Initialize DolphinDB runtime.
     *
     * @warning This function (or the following one) MUST be called before
     * using any other API functions.
     */
    static void initializeRuntime();
    /**
     * @brief Initialize DolphinDB runtime with command line arguments.
     */
    static void initializeRuntime(int argc, char **argv);

    /**
     * @brief Finalize DolphinDB runtime.
     *
     * @warning If DolphinDB runtime is initialized, MUST call this function to
     * finalize it. And once this function is called, can NOT call other API
     * functions (include `initializeRuntime`).
     */
    static void finalizeRuntime();

    /** Get a C-style string representation of the library version. */
    static const char *getVersionString();

    /**
     * @brief Create a Session, then can use it to run DolphinDB builtin functions.
     */
    static SessionSP createSession();

    /**
     * @brief Run a piece of DolphinDB script.
     */
    static ConstantSP execute(const std::string &script);

    /**
     * @brief Run a piece of DolphinDB script in the given session.
     */
    static ConstantSP execute(SessionSP session, const std::string &script);
};


namespace oltp {

using std::string;
using std::vector;
using std::shared_ptr;
using std::pair;

class DBImpl;
class ConnectionContext;

class StringView;
struct RawTableBuffer;
struct TableDescription;
typedef SmartPointer<TableDescription> TableDescriptionSP;

//==============================================================================
// Database
//==============================================================================

struct DBOption {
    /**
     * If true, will open the database in read-only mode, means only allow to
     * query, not allow to write data (insert/delete/update) or create/drop
     * table.
     *
     * A database can open in read-only mode multiple times simultaneously
     * (in the same process or a different process). But once opened in write
     * mode, no one else can open the database at the same time.
     *
     * @attention If not necessary, do NOT open a database simultaneously
     * (in read-only mode) in the same process, because every time the database
     * is opened, all data is loaded into memory.
     */
    bool readOnly = false;

    /**
     * If false, all data is stored in memory and will be lost once the database
     * is closed.
     * If true, will turn on write-ahead logging for persistence.
     *
     * @attention If do not want to lose any data, MUST enable WAL.
     */
    bool enableWAL = true;
    /**
     * This option is meaningful only when `enableWAL` is true.
     *
     * If true, before a write-transaction commit, MUST wait until all
     * write-ahead logs of this transaction are persisted.
     *
     * If false, database can survive from process crash, but can not survive
     * from OS crash. In this mode, performance is better typically.
     *
     * @attention If do not want to lose any data even when OS crash, set this
     * option to true. If wants better performance and do not care OS crash, set
     * this option to false;
     */
    bool syncOnTxnCommit = false;

    /**
     * If true, will do checkpoint automatically according to
     * `checkpointThreshold` and `checkpointInterval`.
     */
    bool enableCheckpoint = true;
    /**
     * Force to do checkpoint when the size of write-ahead logs exceed # MiB.
     */
    size_t checkpointThreshold = 100;
    /**
     * Force to do checkpoint every # seconds.
     */
    size_t checkpointInterval = 60;
};

/**
 * The database object. Open (or create) a database, then connect to the
 * database to operate it.
 *
 * @attention Before closing a DB (call destructor), make sure all clients
 * (Connection) which connect to the DB are inactive.
 */
class DB {
public:
    /**
     * @brief Open a database with specific option.
     * @param path The directory of the database.
     */
    explicit DB(const string &path, const DBOption& option);

    ~DB();

    DB(const DB &) = delete;
    DB& operator=(const DB &) = delete;

private:
    friend class Connection;
    shared_ptr<DBImpl> impl_;
};

//==============================================================================
// Connection
//==============================================================================

/**
 * A connection is a client, connect to a database.
 *
 * @warning Do NOT use a same connection object in multiple threads.
 *
 * @attention Make sure the DB object is valid when the connection is active.
 */
class Connection {
public:
    explicit Connection(DB &db);

    ~Connection();

    Connection(const Connection &) = delete;
    Connection& operator=(const Connection &) = delete;

    /**
     * Execute a SQL or DolphinDB script, e.g.,
     * execute("select * from table order by a"),
     * execute("a = 1; b = 2; a + b").
     *
     * @warning Do NOT use this API to create/drop table, use DDL API instead.
     */
    ConstantSP execute(const string &script);

    //--------------------------- Transaction API ---------------------------//

    /**
     * Begin a new transaction, then can execute multiple DML operations
     * (query/insert/delete/update) atomically. All DML in a transaction context
     * will succeed (if commit), or fail as nothing happened (if rollback).
     *
     * @warning Can NOT execute any DDL operations (create/drop table) in a
     * transaction context.
     *
     * @warning If already in an active transaction context, can NOT begin a new
     * transaction, otherwise RuntimeException will be thrown.
     */
    void beginTransaction();
    void commit();
    void rollback();

    /**
     * A utility method used to execute mutiple DML operations in the same
     * transaction.
     *
     * Usage:
     * @code {.cpp}
     *   conn.transaction([&]() {
     *       conn.insert(...);
     *       conn.update(...);
     *       conn.update(...);
     *   });
     * @endcode
     *
     * @tparam Code A callable type, e.g., lambda.
     * @param code DML operations want to execute.
     *
     * @note No need to catch `NeedRetryException` inside `code`.
     */
    template <typename Code>
    void transaction(Code code);

    /** Check whether is in an active transaction context. */
    bool isInActiveTransactionContext();

    //------------------------------- DDL API -------------------------------//

    /**
     * @brief Create a table in current database.
     *
     * @param tableName Table name.
     * @param colDesc Columns description.
     * @param primaryKey Columns use as primary index key.
     * @param secondaryKeys Secondary index keys.
     *
     * @note Primary index is required, secondary index is optional and can have
     * multiple secondary indexes.
     *
     * @note Description of secondary index key `pair<bool, vector<string>>`:
     * first => whether it is a unique index. second => columns use as index
     * key.
     */
    void createTable(const string &tableName, const vector<ColumnDesc> &colDesc,
                     const vector<string> &primaryKey,
                     const vector<pair<bool, vector<string>>> &secondaryKeys);

    /**
     * @brief Drop a table of current database.
     *
     * @param tableName Which table.
     */
    void dropTable(const string &tableName);

    //------------------------- High Level DML API -------------------------//

    /**
     * @brief Query data from the table.
     *
     * @param tableName Which table.
     * @param columnNames Columns (all columns if it's empty) wants to select.
     * @param filters Where clause in SQL, select all if empty. Use
     * `makeFilters` to generate filters, e.g., `makeFilters("a = 1")`,
     * `makeFilters("a = 1, b > 2")`.
     * @return A `Table` object represent the query result.
     *
     * @throw NeedRetryException
     * @throw NotRetryException
     *
     * @note This API equivalent to a simple SQL like
     * "select xxx form table where a = 2". If wants to execute complex SQL,
     * e.g., has "order by", "group by" and so on, use `execute` API.
     */
    TableSP query(const string &tableName, const vector<string> &columnNames,
                  const vector<ObjectSP> &filters);

    /**
     * @brief Insert data into the table.
     *
     * @param tableName Which table.
     * @param data A `Table` object represent data wants to insert.
     * @return How many rows were inserted.
     *
     * @throw NeedRetryException
     * @throw NotRetryException
     */
    size_t insert(const string &tableName, const TableSP &data);

    /**
     * @brief Delete data from the table.
     *
     * @param tableName Which table.
     * @param filters Where clause in SQL, select all if empty. Use
     * `makeFilters` to generate filters.
     * @return How many rows were deleted.
     *
     * @throw NeedRetryException
     * @throw NotRetryException
     */
    size_t remove(const string &tableName, const vector<ObjectSP> &filters);

    /**
     * @brief Update data in the table.
     *
     * @param tableName Which table.
     * @param updateExpr How to update it. Use `makeColumnUpdateDefs` to
     * generate update expression, e.g., `makeColumnUpdateDefs("a = a +1")`.
     * @param filters Where clause in SQL, select all if empty. Use
     * `makeFilters` to generate filters.
     * @return How many rows were updated.
     *
     * @throw NeedRetryException
     * @throw NotRetryException
     */
    size_t update(const string &tableName,
                  const vector<ColumnDefSP> &updateExpr,
                  const vector<ObjectSP> &filters);

    //------------------------- Low Level DML API -------------------------//

    /**
     * This is low level query API. Caller allocates memory
     * (see `RawTableBuffer`) in advance, and will write result to this memory
     * region. `RawTableBuffer::rows` indicates maximum number of rows to
     * return. Caller MUST make sure a tuple in `RawTableBuffer` is big enough
     * to store a row of the query result.
     *
     * @note This API has ~20% performance improvement compared to high level
     * API. If not necessary, this API is NOT recommended.
     *
     * @param columnNames Columns (all columns if it's empty) wants to select.
     * @param result The buffer use to store query result.
     * @return How many rows were wrote to the buffer.
     *
     * @throw NeedRetryException
     * @throw NotRetryException
     *
     * @note This API equivalent to a simple SQL like
     * "select xxx form table where a = 2". If wants to execute complex SQL,
     * e.g., has "order by", "group by" and so on, use `execute` API.
     */
    size_t query(const string &tableName, const vector<string> &columnNames,
                 const vector<ObjectSP> &filters, const RawTableBuffer &result);

    /**
     * This is low level insert API. Caller MUST make sure the schema of data
     * passed in matches the schema of the table.
     *
     * @warning This API has NO performance improvement compared to high level
     * API. If not necessary, this API is NOT recommended.
     *
     * @throw NeedRetryException
     * @throw NotRetryException
     */
    size_t insert(const string &tableName, const RawTableBuffer &data);

    //-------------------------------- Misc --------------------------------//

    /**
     * @brief Request to do checkpoint.
     *
     * @param force Force checkpoint, even if no changes have been made.
     * @param wait Whether wait checkpoint finish.
     */
    void requestCheckpoint(bool force, bool wait);

    SessionSP getCurrentSession() const;

    /**
     * Generate filters used in SQL `where` clause, e.g., "a = 1",
     * "a > 3, b = 2".
     *
     * @param text String text. Select all if empty.
     *
     * @note Do NOT use "a > 3 and b = 2", instead, use "a > 3, b = 2".
     */
    vector<ObjectSP> makeFilters(const string &text);

    /**
     * Generate update expression used in SQL update, e.g., "a = 1",
     * "a = a + 1, b = b - 1".
     */
    vector<ColumnDefSP> makeColumnUpdateDefs(const string &text);

    /** Get name of all tables in current database. */
    vector<string> listAllTable();

    /**
     * Get the table info of a specific table, or NULL-SP if it cannot be found.
     */
    TableDescriptionSP getTableInfo(const string &tableName);

private:
    shared_ptr<ConnectionContext> context_;
};

//==============================================================================
// Exception
//==============================================================================

/**
 * DML operations (query/insert/delete/update) may throw NeedRetryException.
 * When catch this exception, should rollback current transaction and begin a
 * new transaction to retry.
 */
class NeedRetryException : public std::exception {
public:
    NeedRetryException(const string &err) : err_(err) {
    }

    virtual const char* what() const noexcept override {
        return err_.c_str();
    }

private:
    string err_;
};

/**
 * DML operations (query/insert/delete/update) may throw NotRetryException
 * When catch this exception, just rollback current transaction and do NOT
 * retry.
 */
class NotRetryException : public std::exception {
public:
    NotRetryException(const string &err) : err_(err) {
    }

    virtual const char* what() const noexcept override {
        return err_.c_str();
    }

private:
    string err_;
};

template <typename Code>
void Connection::transaction(Code code) {
    for (;;) {
        try {
            this->beginTransaction();
            code();
            this->commit();
            break;
        } catch (const NeedRetryException &ex) {
            // sleep a while to avoid conflict again
            Util::sleep(/*milliSeconds*/2);
            continue;
        } catch (...) {
            this->rollback();
            throw;
        }
    }
}

//==============================================================================
// RawTupleBuffer / RawTableBuffer
//==============================================================================

/**
 * @attention Use `RawTupleBufferGenerator` to generator a `RawTupleBuffer`.
 */
struct RawTupleBuffer {
    char *buffer;
};

/**
 * @attention Use `RawTableBufferGenerator` to generator a `RawTableBuffer`.
 */
struct RawTableBuffer {
    /** Maximum tuples this table can store. */
    int rows;
    RawTupleBuffer *tuples;
};

class RawTupleBufferReader {
public:
    RawTupleBufferReader();
    RawTupleBufferReader(const RawTupleBuffer &tuple);
    ~RawTupleBufferReader();

    void reset(const RawTupleBuffer &tuple);

    char readBool();
    char readChar();
    short readShort();
    int readInt();
    long long readLong();
    float readFloat();
    double readDouble();
    StringView readString();

private:
    template <typename T>
    T read();

    const char *buffer_ = nullptr;
    int cursor_ = 0;
};

/// @cond
/**
 * @attention Do NOT use this class, use `RawTableBufferWriter` instead.
 */
class RawTupleBufferWriter {
    template <typename T>
    struct remove_cv_ref {
        typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type type;
    };
    template <typename T>
    using remove_cv_ref_t = typename remove_cv_ref<T>::type;
public:
    RawTupleBufferWriter();
    RawTupleBufferWriter(const RawTupleBuffer &tuple);
    ~RawTupleBufferWriter();

    void reset(const RawTupleBuffer &tuple);

    RawTupleBufferWriter& writeBool(char value);
    RawTupleBufferWriter& writeChar(char value);
    RawTupleBufferWriter& writeShort(short value);
    RawTupleBufferWriter& writeInt(int value);
    RawTupleBufferWriter& writeLong(long long value);
    RawTupleBufferWriter& writeFloat(float value);
    RawTupleBufferWriter& writeDouble(double value);

    RawTupleBufferWriter& writeString(const std::string &value);
    RawTupleBufferWriter& writeString(const char *str);
    RawTupleBufferWriter& writeString(const char *str, int len);
    RawTupleBufferWriter& writeString(const StringView &value);
    template <typename T,
              typename U = typename std::enable_if<std::is_arithmetic<remove_cv_ref_t<T>>::value>::type>
    RawTupleBufferWriter& writeString(T value) {
        throw RuntimeException("Can not convert arithmetic type to string");
    }

    RawTupleBufferWriter& writeDate(int value);
    RawTupleBufferWriter& writeMonth(int value);
    RawTupleBufferWriter& writeTime(int value);
    RawTupleBufferWriter& writeMinute(int value);
    RawTupleBufferWriter& writeSecond(int value);
    RawTupleBufferWriter& writeDatetime(int value);
    RawTupleBufferWriter& writeTimestamp(long long value);
    RawTupleBufferWriter& writeNanoTime(long long value);
    RawTupleBufferWriter& writeNanoTimestamp(long long value);
    RawTupleBufferWriter& writeDateHour(int value);

    RawTupleBufferWriter& writeBool(ConstantSP value);
    RawTupleBufferWriter& writeChar(ConstantSP value);
    RawTupleBufferWriter& writeShort(ConstantSP value);
    RawTupleBufferWriter& writeInt(ConstantSP value);
    RawTupleBufferWriter& writeLong(ConstantSP value);
    RawTupleBufferWriter& writeFloat(ConstantSP value);
    RawTupleBufferWriter& writeDouble(ConstantSP value);
    RawTupleBufferWriter& writeString(ConstantSP value);

    RawTupleBufferWriter& writeDate(ConstantSP value);
    RawTupleBufferWriter& writeMonth(ConstantSP value);
    RawTupleBufferWriter& writeTime(ConstantSP value);
    RawTupleBufferWriter& writeMinute(ConstantSP value);
    RawTupleBufferWriter& writeSecond(ConstantSP value);
    RawTupleBufferWriter& writeDatetime(ConstantSP value);
    RawTupleBufferWriter& writeTimestamp(ConstantSP value);
    RawTupleBufferWriter& writeNanoTime(ConstantSP value);
    RawTupleBufferWriter& writeNanoTimestamp(ConstantSP value);
    RawTupleBufferWriter& writeDateHour(ConstantSP value);

private:
    template <typename T>
    void write(T value);

    char *buffer_ = nullptr;
    int cursor_ = 0;
};
/// @endcond

class RawTableBufferWriter {
public:
    RawTableBufferWriter(const RawTableBuffer &table,
                         const vector<ColumnDesc> &schema);

    ~RawTableBufferWriter();

    void reset(const RawTableBuffer &table);
    void reset(const RawTableBuffer &table, const vector<ColumnDesc> &schema);

    /**
     * Usage:
     * @code {.cpp}
     *   RawTableBufferWriter writer(table, schema);
     *
     *   writer.append(1, false, 1.0, "a")
     *         .append(2,  true, 2.0, "b")
     *         .append(3, false, 3.0, "c");
     * @endcode
     */
    template<typename ...Args>
    RawTableBufferWriter& append(Args... args) {
        if (row_ >= table_.rows) {
            throw RuntimeException("Table buffer is full");
        }

        if (schema_.size() != sizeof...(args)) {
            throw RuntimeException("Schema not match: expect " +
                    std::to_string(schema_.size()) + " columns, but given " +
                    std::to_string(sizeof...(args)) + " columns");
        }

        {
            RawTupleBufferWriter writer(table_.tuples[row_]);
            int i = 0;
            std::initializer_list<int> {
                (write(writer, schema_[i].getType(), args), ++i)...
            };
        }

        row_ += 1;
        return *this;
    }

private:
    template <typename T>
    void write(RawTupleBufferWriter &writer, DATA_TYPE type, const T &data);

    void write(RawTupleBufferWriter &writer, DATA_TYPE type,
               const string &data);

    void write(RawTupleBufferWriter &writer, DATA_TYPE type, const char *data);

    RawTableBuffer table_;
    vector<ColumnDesc> schema_;
    int row_ = 0;
};

template <typename T>
void RawTableBufferWriter::write(RawTupleBufferWriter &writer, DATA_TYPE type,
                                 const T &data) {
    switch (type) {
        case DT_BOOL: writer.writeBool(data); break;
        case DT_CHAR: writer.writeChar(data); break;
        case DT_SHORT: writer.writeShort(data); break;
        case DT_INT: writer.writeInt(data); break;
        case DT_LONG: writer.writeLong(data); break;
        case DT_FLOAT: writer.writeFloat(data); break;
        case DT_DOUBLE: writer.writeDouble(data); break;
        case DT_DATE: writer.writeDate(data); break;
        case DT_MONTH: writer.writeMonth(data); break;
        case DT_TIME: writer.writeTime(data); break;
        case DT_MINUTE: writer.writeMinute(data); break;
        case DT_SECOND: writer.writeSecond(data); break;
        case DT_DATETIME: writer.writeDatetime(data); break;
        case DT_TIMESTAMP: writer.writeTimestamp(data); break;
        case DT_NANOTIME: writer.writeNanoTime(data); break;
        case DT_NANOTIMESTAMP: writer.writeNanoTimestamp(data); break;
        case DT_DATEHOUR: writer.writeDateHour(data); break;
        case DT_STRING: writer.writeString(data); break;
        default:
            throw RuntimeException("Not support data type: " +
                    Util::getDataTypeString(type));
    }
}

/// @cond
class RawTupleBufferGenerator {
public:
    RawTupleBufferGenerator(size_t bytes_per_row);

    ~RawTupleBufferGenerator();

    RawTupleBufferGenerator(const RawTupleBufferGenerator&) = delete;
    RawTupleBufferGenerator& operator=(const RawTupleBufferGenerator&) = delete;

    const RawTupleBuffer& get() const;

private:
    RawTupleBuffer tuple_;
    char *buffer_ = nullptr;
};
/// @endcond

class RawTableBufferGenerator {
public:
    RawTableBufferGenerator(int rows, size_t bytes_per_row);

    ~RawTableBufferGenerator();

    RawTableBufferGenerator(const RawTableBufferGenerator&) = delete;
    RawTableBufferGenerator& operator=(const RawTableBufferGenerator&) = delete;

    const RawTableBuffer& get() const;

private:
    RawTableBuffer table_;
    RawTupleBuffer *tuples_ = nullptr;
};

//==============================================================================
// RawTupleBuffer / RawTableBuffer
//==============================================================================

struct IndexDescription {
    /** Index name. */
    string name;
    /** Whether is primary index. */
    bool primary;
    /** Whether is unique index. */
    bool unique;
    /** Columns use as index key. */
    vector<ColumnDesc> key;
};

struct TableDescription {
    /** Table name. */
    string name;
    /** Columns description. */
    vector<ColumnDesc> columns;
    /** Indexes of this table. */
    vector<IndexDescription> indexes;
};

//==============================================================================
// Util
//==============================================================================

class StringView {
public:
    StringView() = default;

    StringView(const char *str, int size) : str_(str), size_(size) {
    }

    const char *str() const { return str_; }

    int size() const { return size_; }

    std::string toString() const {
        return std::string(str_, size_);
    }

    bool empty() const { return size_ == 0; }

private:
    const char *str_ = "";
    int size_ = 0;
};

/**
 * @brief Compute how many bytes is enough to store the data type.
 * @param max_bytes_for_varlen Max length of varlen type.
 * @return Bytes.
 */
size_t computeBytes(DATA_TYPE type, size_t max_bytes_for_varlen);
size_t computeBytes(const std::vector<DATA_TYPE> &types,
                    size_t max_bytes_for_varlen);
size_t computeBytes(const std::vector<ColumnDesc> &descs,
                    size_t max_bytes_for_varlen);

}  // namespace oltp

#endif  // EMBEDDED_OLTP_H_
