/*
 * Concepts.h
 * Header for various objects and statements used for ZBase
 * Created on: Jul 21, 2012
 *      Author: dzhou
 */

#ifndef CONCEPTS_H_
#define CONCEPTS_H_

#include <cstdint>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>
#include <stack>
#include <set>
#include <sstream>
#include <stdlib.h>
#include <iterator>
#include <iostream>
#include <tr1/random>

#include "CoreConcept.h"
#include "ComputingModel.h"
#include "ConcurrentFlatHashmap.h"
#include "ScalarImp.h"
#include "Types.h"

#define uniform_real std::tr1::uniform_real
#define uniform_int std::tr1::uniform_int
#undef min
#undef max

using std::unordered_set;
using std::unordered_multimap;
using std::set;
using std::cout;
using std::endl;

class SharedHeap;
class Expression;
class Function;
class Variable;
class ObjectAttribute;
class Global;
class DolphinClass;
class DolphinInstance;
class ColumnDef;
class SortAttribute;
class SQLQuery;
class SQLUpdate;
class SQLDelete;
class TableJoiner;
class GlobalTable;
class GroupTask;
class ColumnSelector;
class MacroVariable;
class ObjectMethodCall;
class SQLStatement;
class AggregationMapReduce;
class RemoteCall;
class LocalCall;
class AsynchronousRemoteExecutor;
class MessageThrottle;
class AsynchronousPublisher;
class AsynchronousSubscriber;
class ConnectionGroup;
class BatchJobManager;
class WorkerManager;
class ComputingNodeScheduler;
class ComputingNodeSchedulerFactory;
class SnapshotEngineManager;
class StatementAttribute;
class SQLFactory;
class TableJoinerFactory;
class StorageEngine;
class IoTransaction;
class CacheEngine;
class InMemoryTableFactory;
class InMemoryOltpFactory;
class AsyncReplicationOp;
class LocalSubscriber;
class PublishConnection;
class CatalogMgmt;

typedef SmartPointer<SharedHeap> SharedHeapSP;
typedef SmartPointer<DolphinClass> DolphinClassSP;
typedef SmartPointer<DolphinInstance> DolphinInstanceSP;
typedef SmartPointer<ObjectAttribute> ObjectAttributeSP;
typedef SmartPointer<Global> GlobalSP;
typedef SmartPointer<Variable> VariableSP;
typedef SmartPointer<Expression> ExpressionSP;
typedef SmartPointer<Function> FunctionSP;
typedef SmartPointer<ColumnDef> ColumnDefSP;
typedef SmartPointer<SortAttribute> SortAttributeSP;
typedef SmartPointer<SQLQuery> SQLQuerySP;
typedef SmartPointer<SQLUpdate> SQLUpdateSP;
typedef SmartPointer<SQLDelete> SQLDeleteSP;
typedef SmartPointer<TableJoiner> TableJoinerSP;
typedef SmartPointer<GlobalTable> GlobalTableSP;
typedef SmartPointer<GroupTask> GroupTaskSP;
typedef SmartPointer<ColumnSelector> ColumnSelectorSP;
typedef SmartPointer<MacroVariable> MacroVarSP;
typedef SmartPointer<ObjectMethodCall> ObjectMethodCallSP;
typedef SmartPointer<SQLStatement> SQLStatementSP;
typedef SmartPointer<AggregationMapReduce> AggregationMapReduceSP;
typedef SmartPointer<RemoteCall> RemoteCallSP;
typedef SmartPointer<LocalCall> LocalCallSP;
typedef SmartPointer<AsynchronousRemoteExecutor> AsynchronousRemoteExecutorSP;
typedef SmartPointer<MessageThrottle> MessageThrottleSP;
typedef SmartPointer<AsynchronousPublisher> AsynchronousPublisherSP;
typedef SmartPointer<AsynchronousSubscriber> AsynchronousSubscriberSP;
typedef SmartPointer<ConnectionGroup> ConnectionGroupSP;
typedef SmartPointer<BatchJobManager> BatchJobManagerSP;
typedef SmartPointer<WorkerManager> WorkerManagerSP;
typedef SmartPointer<ComputingNodeScheduler> ComputingNodeSchedulerSP;
typedef SmartPointer<ComputingNodeSchedulerFactory> ComputingNodeSchedulerFactorySP;
typedef SmartPointer<CacheEngine> ChunkCacheEngineSP;
typedef SmartPointer<SnapshotEngineManager> SnapshotEngineManagerSP;
typedef SmartPointer<StatementAttribute> StatementAttributeSP;
typedef SmartPointer<SQLFactory> SQLFactorySP;
typedef SmartPointer<TableJoinerFactory> TableJoinerFactorySP;
typedef SmartPointer<StorageEngine> StorageEngineSP;
typedef SmartPointer<InMemoryTableFactory> InMemoryTableFactorySP;
typedef SmartPointer<InMemoryOltpFactory> InMemoryOltpFactorySP;
typedef SmartPointer<AsyncReplicationOp> AsyncReplicationOpSP;
typedef SmartPointer<LocalSubscriber> LocalSubscriberSP;
typedef SmartPointer<PublishConnection> PublishConnectionSP;
typedef SmartPointer<CatalogMgmt> CatalogMgmtSP;

#define JIT_UNKNOWN_INFERRED_TYPE InferredType()
#define JIT_IGNORE_INFERRED_TYPE InferredType((DATA_FORM)-1, DT_VOID, (DATA_CATEGORY)-1)
#define LLVM_FUNC_RETURN_VAR_NAME "__returnVar__"

string getDataFormString(DATA_FORM form);
void addEdgeBetweenBlocks(const StatementSP& fromBlock, const StatementSP& nextBlock);
ConstantSP newConstantFromInferredType(const InferredType &t);
bool updateObject(Heap* heap, const ConstantSP& var, const ConstantSP& index, const ConstantSP& val, bool validateIndex);

#ifdef INDEX64
namespace std{
	int min(int a, INDEX b);
	int min(INDEX a, int b);
}
#endif

class SharedHeap{
public:
	SharedHeap();
	ConstantSP getReference(int index) const;
	ConstantSP getReference(const string& name) const;
	void getReference(int index, ConstantSP& obj, string& name, string& owner) const;
	int size() const {return values_.size();}
	int getIndex (const string& name) const;
	string getName(int index) const;
	string getOwner(int index) const;
	bool contains(const string& name) const;
	int addItem(const string& owner, const string& name, const ConstantSP& value);
	bool removeItem(const string& name);
	bool set(unsigned int index,const ConstantSP& value);
	bool set(unsigned int index,const ConstantSP& value, const string& owner);
	inline bool isSameObject(int index, Constant* obj) const { return values_[index].get() == obj;}

private:
	mutable ConcurrentFlatHashmap<string,int> nameHash_;
	Array<pair<string, string>> names_;
	Array<ConstantSP> values_;
	mutable Mutex mutex_;
};

class DolphinClass : public OOClass {
public:
	DolphinClass(const string& qualifier, const string& name, DolphinClassSP baseCls = nullptr);
	DolphinClass(const string& qualifier, const string& name, bool builtin);
	virtual ~DolphinClass(){}
	int getAttributeIndex(const string& name) const;
	int getPublicAttributeIndex(const string& name) const;
	const string& getAttribute(int index) const { return attributes_[index].first;}
	int getAttributeCount() const { return attributes_.size();}
	bool addAttributeWithType(const string& string, OO_ACCESS access, const ConstantSP &typeObj);
	bool addAttribute(const string& string, OO_ACCESS access);
	bool addAttributes(const vector<string>&, OO_ACCESS access);
	bool addMethod(const FunctionDefSP& method, OO_ACCESS access);
	bool addMethods(const vector<FunctionDefSP>& methods, OO_ACCESS access);
	bool overrideMethod(const std::string &methodName, const FunctionDefSP& method);
	int getMethodIndex(const string& name) const;
	virtual void collectInternalUserDefinedFunctionsAndClasses(Heap* pHeap, unordered_map<string,FunctionDef*>& functionDefs, unordered_map<string,OOClass*>& classes) const;
	virtual ConstantSP getValue() const;
	virtual bool hasMethod(const string& name) const;
	virtual bool hasOperator(const string& name) const;
	virtual FunctionDefSP getMethod(const string& name) const;
	virtual FunctionDefSP getMethod(INDEX index) const;
	virtual FunctionDefSP getOperator(const string& name) const;
	virtual void getMethods(vector<FunctionDefSP>& methods) const;
	virtual ConstantSP getMember(const string& key) const;
	virtual IO_ERR serializeClass(const ByteArrayCodeBufferSP& buffer) const;
	virtual IO_ERR deserializeClass(Session* session, const DataInputStreamSP& in);

	/** Call class constructor, return an instance of this class. */
	virtual ConstantSP call(Heap *heap, const ConstantSP &self, vector<ObjectSP> &arguments) override;
	virtual ConstantSP call(Heap *heap, const ConstantSP &self, vector<ConstantSP> &arguments) override;

	static OOClassSP createDolphinClass(const string& qualifier, const string& name){
		return new DolphinClass(qualifier, name);
	}
	bool isBodyDefined() { return bodyDefined_; }
	void setBodyDefined(bool b) { bodyDefined_ = b; }

	ConstantSP &getAttributeType(INDEX index);
	ConstantSP &getAttributeType(const std::string &name);
	void setConstructor(const FunctionDefSP & constructor) { constructor_ = constructor; }
	FunctionDefSP getConstructor() { return constructor_; }
	DolphinClassSP getBaseClass() { return baseCls_; }
	void setBaseClass(DolphinClassSP &baseCls) { baseCls_ = baseCls; }

protected:
	DolphinClass(const string& qualifier, const string& name, int flag, const vector<pair<string, char> >& attributes, const vector<FunctionDefSP>& methods);

private:
	vector<pair<string, char> > attributes_;
	vector<FunctionDefSP> methods_;
	unordered_map<string, int> dict_;
	vector<pair<string, ConstantSP>> types_;
	bool bodyDefined_ = false;
	FunctionDefSP constructor_;
	DolphinClassSP baseCls_;
};

class DolphinInstance : public OOInstance{
public:
	DolphinInstance(const DolphinClassSP& ddbClass);
	DolphinInstance(const DolphinClassSP& ddbClass, const vector<ConstantSP>& data);
	DolphinInstance(Session* session, const DataInputStreamSP& in);
	virtual ~DolphinInstance(){}
	inline ConstantSP getAttribute(int index) const { return data_[index];}
	ConstantSP getAttribute(const string& name) const;
	void setAttribute(int index, const ConstantSP& attr);
	void setAttribute(const string& name, const ConstantSP& attr);
	virtual ConstantSP getValue() const;
	virtual ConstantSP getMember(const string& key) const;
	virtual IO_ERR serialize(const ByteArrayCodeBufferSP& buffer) const;
	virtual string getString() const;

	static ConstantSP createDolphinInstance(Session* session, const DataInputStreamSP& in){
		return new DolphinInstance(session, in);
	}

protected:
	vector<ConstantSP> data_;

private:
	ConstantSP typeCheck(const std::string &name, const ConstantSP &attr, const ConstantSP &attrType);
};

class Tuple : public Object {
public:
	Tuple(const vector<ObjectSP>& arguments, bool isFunctionArgument = false, bool isDynamicVector = false, bool readonly=false): arguments_(arguments),
		isFunctionArgument_(isFunctionArgument), isDynamicVector_(isDynamicVector), readonly_(readonly){}
	Tuple(Session* session, const DataInputStreamSP& in);
	inline bool isFunctionArgument() const { return isFunctionArgument_;}
	inline bool isDynamicVector() const { return isDynamicVector_;}
	inline bool readonly() const { return readonly_;}
	ObjectSP getElement(int index) const {return arguments_[index];}
	int getElementCount() const { return arguments_.size();}
	virtual ConstantSP getValue(Heap* pHeap) { return getReference(pHeap);}
	virtual ConstantSP getReference(Heap* pHeap);
	virtual OBJECT_TYPE getObjectType() const {return TUPLE;}
	virtual ConstantSP getComponent() const;
	virtual string getScript() const;
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;
	virtual void collectUserDefinedFunctions(unordered_map<string,FunctionDef*>& functionDefs) const;
	virtual void collectUserDefinedFunctionsAndClasses(Heap* pHeap, unordered_map<string,FunctionDef*>& functionDefs, unordered_map<string,OOClass*>& classes) const;
	virtual void collectVariables(vector<int>& vars, int minIndex, int maxIndex) const;
	virtual void retrieveColumns(const TableSP& table, vector<pair<string,string>>& columns) const;
	virtual int checkSpecicalFunction(bool aggrOnly) const;
	virtual ObjectSP copy(Heap* pHeap, const SQLContextSP& context, bool localize) const;
	virtual ObjectSP copyAndMaterialize(Heap* pHeap, const SQLContextSP& context, const TableSP& table) const;
	virtual bool mayContainColumnRefOrVariable() const { return true;}

private:
	vector<ObjectSP> arguments_;
	bool isFunctionArgument_;
	bool isDynamicVector_;
	bool readonly_;
};

class ColumnDef: public Object {
public:
	ColumnDef(const ObjectSP& colDef, const string& colName);
	ColumnDef(const SQLContextSP& context, Session* session, const DataInputStreamSP& buffer);
	const string& getName() const {return colName_;}
	ObjectSP getColumnDef() const {return colDef_;}
	bool isWildColumn() const { return colName_ == "*";}
	bool isMacroColumnName() const { return colName_.size()>2 && colName_[0] == '_' && colName_[1] == '$';}
	bool isCompositeColumn() const;
	void splitCompositeColumn(vector<ColumnDefSP>& cols) const;
	void splitCompositeColumn(vector<string>& cols) const;
	virtual ConstantSP getValue(Heap* pHeap);
	virtual ConstantSP getReference(Heap* pHeap);
	virtual OBJECT_TYPE getObjectType() const {return COLUMNDEF;}
	virtual ConstantSP getComponent() const;
	virtual string getScript() const;
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;
	virtual void collectUserDefinedFunctions(unordered_map<string,FunctionDef*>& functionDefs) const { colDef_->collectUserDefinedFunctions(functionDefs);}
	virtual void collectUserDefinedFunctionsAndClasses(Heap* pHeap, unordered_map<string,FunctionDef*>& functionDefs, unordered_map<string,OOClass*>& classes) const {
		colDef_->collectUserDefinedFunctionsAndClasses(pHeap, functionDefs, classes);
	}
	virtual void collectVariables(vector<int>& vars, int minIndex, int maxIndex) const { colDef_->collectVariables(vars, minIndex, maxIndex);}
	virtual void retrieveColumns(const TableSP& table, vector<pair<string,string>>& columns) const;
	virtual int checkSpecicalFunction(bool aggrOnly) const;
	virtual ObjectSP copy(Heap* pHeap, const SQLContextSP& context, bool localize) const;
	virtual ObjectSP copyAndMaterialize(Heap* pHeap, const SQLContextSP& context, const TableSP& table) const;
	virtual bool mayContainColumnRefOrVariable() const { return true;}
	static bool retrieveColumnName(Heap* heap, const ObjectSP& obj, string& name);
	static void analyzeUsedColumns(vector<ColumnDefSP>& objs, Table* table, unordered_map<string,int>& dict);
	static void analyzeUsedColumns(vector<ObjectSP>& objs, Table* table, unordered_map<string,int>& dict);
	static void analyzeUsedColumns(Object* obj, Table* table, unordered_map<string,int>& dict);
private:
    ObjectSP colDef_;
    string colName_;
};

class ObjectContainer {
public:
	ObjectContainer(const ObjectSP& obj) : obj_(obj){}
	void execute(Heap* heap);
	ConstantSP get(const string& name, int index) const;
	const ObjectSP& getObject() { return obj_; }
	bool mayContainColumnRefOrVariable() const { return obj_->mayContainColumnRefOrVariable(); }

private:
	ObjectSP obj_;
	ConstantSP result_;
};

class MappedColumn : public Object{
public:
	MappedColumn(SmartPointer<ObjectContainer>& objContainer, const string& name, int index, bool initObject) : objContainer_(objContainer), name_(name), index_(index), initObject_(initObject){};
	virtual ~MappedColumn(){}
	virtual ConstantSP getValue(Heap* pHeap) {return getReference(pHeap);}
	virtual ConstantSP getReference(Heap* pHeap);
	virtual OBJECT_TYPE getObjectType() const { return MAPPEDCOL; }
	virtual string getScript() const;
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const {throw RuntimeException("MappedColumn::serialize not supported.");}
	virtual const SmartPointer<ObjectContainer>& getObjContainer() { return objContainer_; }
	ObjectSP copyAndMaterialize(Heap* pHeap, const SQLContextSP& context, const TableSP& table) const override;
	bool mayContainColumnRefOrVariable() const override { return objContainer_->mayContainColumnRefOrVariable(); }

private:
	SmartPointer<ObjectContainer> objContainer_;
	string name_;
	int index_;
	bool initObject_;
};

class TableJoiner: public Object{
public:
	TableJoiner(const string& name, SysFunc joiner, const vector<ObjectSP>& params):name_(name), joiner_(joiner), params_(params){}
	TableJoiner(Session* session, const DataInputStreamSP& in);
	virtual ~TableJoiner(){}
	void setFilters(vector<ObjectSP>& filters);
	void getUnusedFilters(vector<ObjectSP>& filters);
	void setSQLContext(const SQLContextSP& context){contextSP_=context;}
	void setSegment(const DomainPartitionSP& leftSegment, const DomainPartitionSP& rightSegment) {
		segments_[0] = leftSegment;
		segments_[1] = rightSegment;
	}
	void collectColumnNames(Heap* pHeap, unordered_set<string>& list, bool collectRightTableForWindowJoin);
	TableSP collectColumnNames(Heap* pHeap, bool collectRightTableForWindowJoin) const;
	/**
	 * reshuffle: an in-out parameter. If reshuffle is true, check if the partitioned tables in a join need reshuffle.
	 * The checking result is stored in the same parameter. If reshuffle is false and the partitioned tables in a join
	 * can't perform naive join, an exception will be raised.
	 */
	virtual void collectPartitionedTables(Heap* pHeap, bool distributedTable, bool validate, bool& reshuffle, vector<TableSP>& tables, vector<bool>& isPartitionedTable) const = 0;
	virtual ObjectSP copy() const = 0;
	/**
	 * This method is used before the system sends a distributed query to a remote site. If the join has one
	 * local table and one remote table, the system would send the local table to the remote site for joining.
	 * Therefore, the system has to materialize the local table. In order to transfer as few data as possible,
	 * the system will use the where condition to narrow down the local table. The unused where conditions will
	 * be placed in the in/out argument.
	 */
	virtual void materializeLocalTable(Heap* heap, vector<ObjectSP>& filters) = 0;
	/**
	 * This method is used immediately before the system sends a distributed query to a remote site.
	 * Convert a dfs segmented table to a GlobalTable object and convert  a dfs snapshot dimensional table to
	 * a DimTable object.
	 */
	virtual void materializeDFSTable(Heap* heap) = 0;
	virtual void materializeLocalTable(Heap* heap) = 0;
	virtual bool optimizeJoinForValuePartition(const vector<ColumnRef>& partitionCols) = 0;
	/**
	 * A synchronized table requires locking even for read only. Therefore, collect all synchronized in-memory tables.
	 */
	virtual void collectSynchronizedTables(Heap* heap, vector<TableSP>& tables) = 0;
	/**
	 * Set segment and collect synchronized in-memory partitions
	 */
	virtual void collectSynchronizedTables(Heap* heap, const DomainPartitionSP& segment, const DomainPartitionSP& rightSegment, vector<TableSP>& tables) = 0;
	const string& getName() const { return name_;}
	SysFunc getJoiner() const { return joiner_;}
	inline int getParamCount() const { return params_.size();}
	inline const ObjectSP getParam(int index) const { return params_[index];}
	/**
	 * -1 : the column in neither table
	 * 0: the column belongs to left table
	 * 1: the column belongs to right table
	 */
	int getTableIndex(ColumnRef* col);
	virtual bool canOptimizeTableJoin(int tableIndex) = 0;
	virtual ConstantSP getValue(Heap* pHeap){return getReference(pHeap);}
	virtual ConstantSP getReference(Heap* pHeap) = 0;
	virtual OBJECT_TYPE getObjectType() const {return TABLEJOINER;}
	virtual ConstantSP getComponent() const;
	virtual string getScript() const;
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;
	virtual void collectUserDefinedFunctions(unordered_map<string,FunctionDef*>& functionDefs) const;
	virtual void collectUserDefinedFunctionsAndClasses(Heap* pHeap, unordered_map<string,FunctionDef*>& functionDefs, unordered_map<string,OOClass*>& classes) const;
	virtual void collectVariables(vector<int>& vars, int minIndex, int maxIndex) const;
	virtual void retrieveColumns(const TableSP& table, vector<pair<string,string>>& columns) const;
	virtual int checkSpecicalFunction(bool aggrOnly) const;
    void setTaskFirstLevel(bool flag) {isTaskFirstLevel_ = flag;}
    bool isTaskFirstLevel() const {return isTaskFirstLevel_;}
	void setIsStandardCJ(bool flag) { isStandardCJ_ = flag; }
	bool isStandardCJ() const { return isStandardCJ_; }
protected:
	bool getColumnRefs(const ObjectSP& expr, vector<ColumnRefSP>& cols) const;

	string name_;
	SysFunc joiner_;
	vector<ObjectSP> params_;
	vector<ObjectSP> filters_;
	SQLContextSP contextSP_;
	DomainPartitionSP segments_[2];
    bool isTaskFirstLevel_ = true;
	bool isStandardCJ_ = false;
};

class SortAttribute{
public:
	SortAttribute(const ObjectSP& colDef, bool asc, char nullsOrder = 0):colDef_(colDef), asc_(asc), nullsOrder_(nullsOrder){}
	SortAttribute(const SQLContextSP& context, Session* session, const DataInputStreamSP& in);
	ObjectSP getSortColumn() const {return colDef_;}
	bool isAscOrder() const {return asc_;}
	char getNullsOrder() const {return nullsOrder_;}
	string getScript() const;
	IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;
	SortAttributeSP copy(Heap* pHeap, const SQLContextSP& context, bool localize) const;
	SortAttributeSP copyAndMaterialize(Heap* pHeap, const SQLContextSP& context, const TableSP& table) const;
	bool mayContainColumnRefOrVariable() const { return true;}
	void retrieveColumns(const TableSP& table, vector<pair<string,string>>& columns) const;
	int checkSpecicalFunction(bool aggrOnly) const;
	void collectUserDefinedFunctions(unordered_map<string,FunctionDef*>& functionDefs) const {colDef_->collectUserDefinedFunctions(functionDefs);}
	void collectUserDefinedFunctionsAndClasses(Heap* pHeap, unordered_map<string,FunctionDef*>& functionDefs, unordered_map<string,OOClass*>& classes) const {
		colDef_->collectUserDefinedFunctionsAndClasses(pHeap, functionDefs, classes);
	}

private:
	ObjectSP colDef_;
	bool asc_;
	char nullsOrder_; //0: none, 1: nulls first, 2: nulls last
};

class CaseWhen : public Object {
public:
	CaseWhen(const vector<ObjectSP>& when, const vector<ObjectSP>& value, const ObjectSP otherValue) : when_(when), value_(value), otherValue_(otherValue) {

	}
	CaseWhen(const ObjectSP& caseExpr, const vector<ObjectSP>& when, const vector<ObjectSP>& value, const ObjectSP otherValue);
	CaseWhen(const SQLContextSP& context, Session* session, const DataInputStreamSP& in);
	virtual ConstantSP getReference(Heap* pHeap);
	virtual ConstantSP getValue(Heap* pHeap) { return getReference(pHeap); }
	virtual OBJECT_TYPE getObjectType() const { return CASEWHEN; }
	virtual ConstantSP getComponent() const;
	virtual string getScript() const;
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;
	virtual void collectUserDefinedFunctions(unordered_map<string,FunctionDef*>& functionDefs) const;
	virtual void collectUserDefinedFunctionsAndClasses(Heap* pHeap, unordered_map<string,FunctionDef*>& functionDefs, unordered_map<string,OOClass*>& classes) const;
	virtual void collectVariables(vector<int>& vars, int minIndex, int maxIndex) const;
	virtual void retrieveColumns(const TableSP& table, vector<pair<string,string>>& columns) const;
	virtual int checkSpecicalFunction(bool aggrOnly) const;
	virtual ObjectSP copy(Heap* pHeap, const SQLContextSP& context, bool localize) const;
	virtual ObjectSP copyAndMaterialize(Heap* pHeap, const SQLContextSP& context, const TableSP& table) const;
	virtual bool mayContainColumnRefOrVariable() const { return true;}
	inline int getBranchCount() const { return when_.size();}
	inline const ObjectSP& getWhenObject(int index) const { return when_[index];}
	inline const ObjectSP& getThenObject(int index) const { return value_[index];}
	inline const ObjectSP& getElseObject() const { return otherValue_;}
	inline const ObjectSP& getCaseObject() const { return caseExpr_;}

private:
	ConstantSP evaluateWithCaseExpr(Heap* heap);
	ConstantSP evaluateWithoutCaseExpr(Heap* heap);

private:
	ObjectSP caseExpr_;
	vector<ObjectSP> when_;
	vector<ObjectSP> value_;
	ObjectSP otherValue_;
};

// modern window frame definition.
enum class FrameType {
	kRows = 0,
	kRanges,
	kGroups,
};

enum class BoundType {
	kFollowing = 0,
	kPreceding,
	kCurrentRow,
};

struct FrameBound {
	BoundType type;
	bool unbounded;
	int num;

	bool duration;
	DURATION unit;

	IO_ERR serialize(const ByteArrayCodeBufferSP& buffer) const;
	string deserialize(const DataInputStreamSP& reader, char version);
};

struct WindowFrame {
	FrameType type;
	FrameBound start;
	FrameBound end;

	IO_ERR serialize(const ByteArrayCodeBufferSP& buffer) const;
	string deserialize(const DataInputStreamSP& reader, char version);
};

// window frame helper function.
inline bool isCurrentRow(const FrameBound& f) { return f.type == BoundType::kCurrentRow; }

inline bool isUnbounded(const FrameBound& f) { return f.unbounded; }

inline bool isLimited(const FrameBound& f) { return f.type != BoundType::kCurrentRow && !f.unbounded; }

inline bool isPreceding(const FrameBound& f) { return f.type == BoundType::kPreceding; }

inline bool isFollowing(const FrameBound& f) { return f.type == BoundType::kFollowing; }

inline bool isDuration(const FrameBound& f) { return f.duration; }

class AnalyticFunction final : public Object {
   public:
	AnalyticFunction(const FunctionSP& func, const FunctionSP& windowFunc, const FunctionSP& contextbyFunc,
					 const vector<ObjectSP>& partitions, const vector<SortAttributeSP>& orders, WindowFrame frame);
	AnalyticFunction(const FunctionSP& func, const vector<ObjectSP>& partitions, const vector<SortAttributeSP>& orders,
					 WindowFrame frame);
	AnalyticFunction(const SQLContextSP& context, Session* session, const DataInputStreamSP& in);
	~AnalyticFunction() final = default;

	/**
	 * Convert the analytic function to a built-in window function call. Usually the retrieved window function will
	 * work together with CONTEXTBY clause.
	 *
	 * Example: avg(price) over (partition by secId order by timestamp) between 1 preceding and current row
	 * return mavg(price, 2)
	 *
	 * partition clause and order clause will appear in contextby clause.
	 */
	const FunctionSP& getWindowFunction() const { return windowFunc_; }

	/**
	 *Convert the analytic function to an equivalent function of contextby.
	 */
	const FunctionSP& getContextByFunction() const { return contextbyFunc_; }
	const FunctionSP& getFunction() const { return func_; }
	const vector<ObjectSP>& getPartitionClause() const { return partitions_; }
	const vector<SortAttributeSP>& getOrderClause() const { return orders_; }
	FrameBound getWindowStart() const { return frame_.start; }
	FrameBound getWindowEnd() const { return frame_.end; }
	FrameType windowFrameType() const { return frame_.type; }
	bool isDuration() const { return ::isDuration(frame_.start) || ::isDuration(frame_.end); }

	// trivial private member interface.
	SQLContextSP& context() { return context_; }
	bool& group() { return group_; }
	bool& sqlStandardCompatible() { return sqlStandardCompatible_; }

	/**
	 * Convert to a call of function contextby and then evaluate. The returned vector will be in the same order with
	 *  the original input.
	 */
	virtual ConstantSP getReference(Heap* pHeap);
	virtual ConstantSP getValue(Heap* pHeap) { return getReference(pHeap); }
	virtual OBJECT_TYPE getObjectType() const { return ANALYTICALFUNC; }
	virtual ConstantSP getComponent() const;
	virtual string getScript() const;
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;
	virtual void collectUserDefinedFunctions(unordered_map<string, FunctionDef*>& functionDefs) const;
	virtual void collectVariables(vector<int>& vars, int minIndex, int maxIndex) const;
	virtual void retrieveColumns(const TableSP& table, vector<pair<string, string>>& columns) const;
	virtual int checkSpecicalFunction(bool aggrOnly) const;
	virtual ObjectSP copy(Heap* pHeap, const SQLContextSP& context, bool localize) const;
	virtual ObjectSP copyAndMaterialize(Heap* pHeap, const SQLContextSP& context, const TableSP& table) const;
	virtual bool mayContainColumnRefOrVariable() const { return true; }
	virtual bool containAnalyticFunction() const { return true; }

   private:
	void convertToBuiltinFunction(Session* session);
	string getWindowSpecificationScript() const;
	void checkGroupedReference() const;

	FunctionSP func_{};
	pair<vector<bool>, FunctionSP> converted_{};
	FunctionSP windowFunc_{};
	FunctionSP contextbyFunc_{};
	vector<ObjectSP> partitions_;
	vector<SortAttributeSP> orders_;
	WindowFrame frame_{};
	// context for ranking functions, default partition and Eq implementation for multi-column order
	SQLContextSP context_{};
	bool group_{};					// set if used with group by clause
	bool sqlStandardCompatible_{};	// sql standard only allow window function appears in select and order
	char versionNum_ = 1;
};

class ExistsObj : public Object {
public:
	ExistsObj(const ObjectSP& query) : query_(query) {}
    ExistsObj(const SQLContextSP& context, const ObjectSP& query, const vector<ObjectSP> &innerWhere, const vector<ObjectSP> &outerWhere)
        : context_(context), query_(query), innerWhere_(innerWhere), outerWhere_(outerWhere) {}
    ExistsObj(Session* session, const DataInputStreamSP& in);
	virtual ConstantSP getReference(Heap* pHeap);
	virtual ConstantSP getValue(Heap* pHeap) { return getReference(pHeap); }
	virtual OBJECT_TYPE getObjectType() const { return SQLEXISTS; }
	virtual ConstantSP getComponent() const;
	virtual string getScript() const;
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;

private:
	SQLContextSP context_;
	ObjectSP query_;
	vector<ObjectSP> innerWhere_;
	vector<ObjectSP> outerWhere_;
};

class OuterColumnRef : public Object {
public:
	OuterColumnRef(const ColumnRefSP& columnRef) : columnRef_(columnRef), level_(-1), rowNum_(-1) {}
	OuterColumnRef(Session* session, const DataInputStreamSP& in);
	virtual ConstantSP getReference(Heap* pHeap);
	virtual ConstantSP getValue(Heap* pHeap) { return getReference(pHeap); }
	virtual OBJECT_TYPE getObjectType() const { return OUTERCOLUMN; }
	virtual string getScript() const;
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;
	void initOuterColumnRef(Heap* pHeap); // init it before getReference
	int getRowNum() const { return rowNum_; }
	void setLevel(int level) { level_ = level; }

private:
	ColumnRefSP columnRef_;
	int level_;
	int rowNum_;
	VectorSP column_;
};

class SQLWithQuery : public Object {
public:
	SQLWithQuery(const vector<VariableSP>& tmpNames, const vector<vector<string>>& asColumnAlias,
			const vector<SQLQuerySP>&asQueries, const SQLQuerySP& lastQuery) : tmpNames_(tmpNames), asColumnAlias_(asColumnAlias),
			asQueries_(asQueries), lastQuery_(lastQuery) {}
	SQLWithQuery(Session* session, const DataInputStreamSP& in);
	~SQLWithQuery();

	virtual ConstantSP getReference(Heap* pHeap);
	virtual ConstantSP getValue(Heap* pHeap) { return getReference(pHeap); }
	virtual OBJECT_TYPE getObjectType() const { return SQLWITHQUERY; }
	virtual ConstantSP getComponent() const;
	virtual string getScript() const;
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;

private:
	vector<VariableSP> tmpNames_;
	vector<vector<string>> asColumnAlias_;
	vector<SQLQuerySP> asQueries_;
	SQLQuerySP lastQuery_;
};

class SQLQuery: public Object{
public:
	SQLQuery(const SQLContextSP& contextSP,int start, int end, char exec, vector<ColumnDefSP>& select, ObjectSP& from, vector<ObjectSP>& where,
			vector<SortAttributeSP>& sorting, vector<SortAttributeSP>& sortBy, vector<ColumnDefSP>& groupBy, ObjectSP& having, char groupFlag,
			char cgroups, int hint);
	SQLQuery(const SQLContextSP& contextSP, ObjectSP& rowOffset, ObjectSP& rowCount, char exec, vector<ColumnDefSP>& select, ObjectSP& from, vector<ObjectSP>& where,
			vector<SortAttributeSP>& sorting, vector<SortAttributeSP>& sortBy, vector<ColumnDefSP>& groupBy, ObjectSP& having, char groupFlag,
			char cgroups, int hint);
	SQLQuery(Session* session, const DataInputStreamSP& in) : start_(0), end_(-1), exec_(0), groupFlag_(-1), cgroups_(0), hint_(0) {}
	virtual ~SQLQuery(){}
	void collectColumnNames(Heap* pHeap,unordered_set<string>& list) const;
	int getHint() const {return hint_;}
	bool isHintLocalCall() const { return hint_ & 1;}
	bool isHintFurtherPartition() const {return hint_ & 2;}
	bool isHintLocalVariableMatrialized() const { return hint_ & 4;}
	bool isHintSQLContextReplaced() const {return hint_ & 8;}
	bool isHintValuePartitionOptimized() const {return hint_ & 16;}
	bool isHintHashGrouping() const {return hint_ & 32;}
	bool isHintUseSnapshotTable() const {return hint_ & 64;}
	bool isHintKeepOrder() const {return hint_ & 128;}
	bool isHintUseDBPartition() const {return hint_ & 256;}
	bool isHintSequentialExecution() const {return hint_ & 512;}
	void setHintPartitionedOutput(bool option) {if(option) hint_ |= 1024; else hint_ &= ~1024;}
	bool isHintPartitionedOutput() const { return hint_ & 1024;}
	void setHintFillNeeded(bool option) {if(option) hint_ |= 2048; else hint_ &= ~2048;}
	bool isHintFillNeeded() const { return hint_ & 2048;}
	bool isHintPreload() const { return hint_ & 4096;}
	void setHintNonColumnExprEvaluated(bool option) { if(option) hint_ |= 8192; else hint_ &= ~8192;}
	bool isHintNonColumnExprEvaluated() const { return hint_ & 8192;}
	void setHintReturnNothingForEmptyTableToAggregate(bool option) {if(option) hint_ |= 16384; else hint_ &= ~16384;}
	bool isHintReturnNothingForEmptyTableToAggregate() const { return hint_ & 16384;}
	void setHintExplainQuery(bool option) {if(option) hint_ |= 32768; else hint_ &= ~32768;}
	bool isHintExplainQuery() const { return hint_ & 32768;}
	void setHintReturnResultsAndExplainQuery(bool option) {if(option) hint_ |= 65536; else hint_ &= ~65536;}
	bool isHintReturnResultsAndExplainQuery() const { return hint_ & 65536;}
    void setHintForbidProbingGroupSize(bool option){ if(option) hint_ |= 131072; else hint_ &= ~131072;}
    bool isHintForbidProbingGroupSize() const { return hint_ & 131072;}
	void setHintForReshuffleContextBy(bool option){ if(option) hint_ |= 262144; else hint_ &= ~262144;}
    bool isHintForReshuffleContextBy() const { return hint_ & 262144;}
	void setHintForSortGroup(bool option){ if(option) hint_ |= 524288; else hint_ &= ~524288;}
    bool isHintForSortGroup() const { return hint_ & 524288;}
	void setHintForValuePartitionOptimizedForTemporal(bool option){ if(option) hint_ |= 1048576; else hint_ &= ~1048576;}
    bool isHintForValuePartitionOptimizedForTemporal() const { return hint_ & 1048576;}
	void setHintForVectorizedGroup(bool option){ if(option) hint_ |= 4194304; else hint_ &= ~4194304;}
    bool isHintForVectorizedGroup() const { return hint_ & 4194304;}
	void setHintForPlanning(bool option){ if(option) hint_ |= 8388608; else hint_ &= ~8388608;}
	bool isHintForPlanning() const { return hint_ & 8388608;}
    void setHinNotAddGroupingColumn(bool option){ if(option) hint_ |= 16777216; else hint_ &= ~16777216;}
    bool isHintNotAddGroupingColumn() const { return hint_ & 16777216;}
	void setHintForPytorchTensor(bool option) { if (option) hint_ |= 33554432; else hint_ &= ~33554432; }
	bool isHintForPytorchTensor() const { return hint_ & 33554432; }
	void setHintForAnalyticFunctionInSelectList(bool option) { if(option) hint_ |= 67108864; else hint_ &= ~67108864; }
	bool isHintForAnalyticFunctionInSelectList() const { return hint_ & 67108864;}
	void setHintForLimitParallelCall(bool option) { if(option) hint_ |= 134217728; else hint_ &= ~134217728; }
	bool isHintForLimitParallelCall() const { return hint_ & 134217728;}
    int getStart() const {return start_;}
	int getEnd() const {return end_;}
	bool isExecMode() const {return exec_ & 1;}
	bool isSubqueryMode() const {return exec_ & 2;}
	bool isDistinctMode() const {return exec_ & 4;}
	void setExecMode(bool option) { if(option) exec_ |= 1; else exec_ &= ~1;}
	void setSubqueryMode(bool option) { if(option) exec_ |= 2; else exec_ &= ~2;}
	void setDistinctMode(bool option) { if(option) exec_ |= 4; else exec_ &= ~4;}
	char getExec() const {return exec_;}
	DomainPartitionSP getSegment() const { return segment_;}
	const vector<ColumnDefSP>& getSelectList() const { return select_;}
	const vector<ObjectSP>& getWhereClause() const { return where_;}
	const vector<SortAttributeSP>& getSortClause() const {return sortBy_;}
	const vector<SortAttributeSP>& getContextSortClause() const {return sorting_;}
	const vector<ColumnDefSP>& getGroupingClause() const {return groupBy_;}
	int getGroupFlag() const {return groupFlag_;}
	int getRunningGroupCount() const { return cgroups_;}
	ObjectSP getHavingClause() const {return having_;}
	ObjectSP getFromObject() const {return from_;}
	ObjectSP getRowCount() const {return rowCount_;}
	ObjectSP getRowOffset() const {return rowOffset_;}
	virtual ConstantSP getValue(Heap* pHeap){ return getReference(pHeap);}
	virtual ConstantSP getReference(Heap* pHeap) = 0;

	/**
	 * Split the query to multiple data sources stored in a tuple.
	 */
	virtual ConstantSP getDataSource(Heap* pHeap, bool forcePartition = false) = 0;

	/**
	 * Return all domain partitions the query involves.
	 */
	virtual vector<DomainPartitionSP> getPartitions(Heap* pHeap) = 0;

	/**
	 * Analyze FROM clause and obtain a set of column names. If a ColumnRef object isn't in the column list
	 * and its index isn't negative, replace it with the referred object.
	 */
	virtual void materializeColumns(Heap* heap) = 0;

	/**
	 * Reset the index attribute of a ColumnRef object to -1 and replace a VAR object with the referred object.
	 */
	virtual SQLQuery* materializeLocalVariables(Heap* pHeap) = 0;

	/**
	 * This method will be called before executing the query if the query is a sub query.
	 */
	virtual void materializeLocalTable(Heap* heap) = 0;

	/**
	 * copy the query with specified SQLContext.
	 */
	virtual SQLQuery* copy(Heap* heap, const SQLContextSP& context) = 0;

	virtual OBJECT_TYPE getObjectType() const {return SQLQUERY;}
	virtual ConstantSP getComponent() const;
	virtual string getScript() const;
	virtual void collectUserDefinedFunctions(unordered_map<string,FunctionDef*>& functionDefs) const;
	virtual void collectUserDefinedFunctionsAndClasses(Heap* pHeap, unordered_map<string,FunctionDef*>& functionDefs, unordered_map<string,OOClass*>& classes) const;
	virtual void collectVariables(vector<int>& vars, int minIndex, int maxIndex) const;
	virtual bool mayContainColumnRefOrVariable() const { return true;}
	/**
	 * reshuffle: an in-out parameter. If reshuffle is true, check if the partitioned tables in a join need reshuffle.
	 * The checking result is stored in the same parameter. If reshuffle is false and the partitioned tables in a join
	 * can't perform naive join, an exception will be raised.
	 */
	virtual void collectPartitionedTables(Heap* pHeap, bool isDistributed, bool& reshuffle, vector<TableSP>& tables, vector<bool>& isPartitionedTable) const = 0;
	virtual void getFinalSelectList(TableSP table, vector<ObjectSP>& selects, vector<string>& colNames) const  = 0;
	static ObjectSP copyWithNewSQLContext(const SQLContextSP& context, const ObjectSP& originalObj);
	static ObjectSP materializeWithNewSQLContext(Heap* heap, const SQLContextSP& context, const ObjectSP& originalObj);
	static bool generateColumnDefListFromMetaCode(const SQLContextSP& context, const ConstantSP& columnDefs, vector<ColumnDefSP>& output);
	static bool generateObjectListFromMetaCode(const SQLContextSP& context, const ConstantSP& columnDefs, vector<ObjectSP>& output);

protected:
	SQLContextSP context_;
	int start_;
	/**
	 * The semantics of end_.
	 * end_ >= 0: rowCount is specified
	 * end_ == -1: rowCount isn't specified
	 * end_ == -2: rowCount is zero
	 * end_ == -3: rowCount is a variable and rowOffset isn't specified
	 * end_ == -4: rowCount and rowOffset are specified and at least one of them is a variable.
	 */
	int end_;
	//bit0: 0: select mode, 1: execution mode
	//bit1: 0: query mode, 1: sub-query mode
	//bit2: 0: regular mode, 1: distinct mode
	char exec_;
	vector<ColumnDefSP> select_;
	vector<int> afList_;
	ObjectSP from_;
	vector<ObjectSP> where_;
	vector<SortAttributeSP> sorting_;
	vector<SortAttributeSP> sortBy_;
	vector<ColumnDefSP> groupBy_;
	ObjectSP having_;
	ObjectSP rowCount_;
	ObjectSP rowOffset_;
	char groupFlag_; //0:contextBy, 1: aggregation, 2: pivot
	//when groupFlag_ == 1, one can set the last n grouping columns as running group
	char cgroups_;
	//bit0: 0 may not be local call, 1 local call
	//bit1: 0 may need further partition, 1 no further partition
	//bit2: 0 may contain local variables, 1 all local variables are materialized and all ColumnRef object removes local variable index.
	//bit3: 0 need to replace SQLContext to avoid multiple threading issue at runtime, 1 don't have to
	//bit4: 0 not grouping optimized for value partition, 1: grouping optimized for value partition
	//bit5: 0 automatically select algorithm for GROUP BY clause, 1: use hashing algorithm for GROUP BY clause if possible
	//bit6: 0 use specified table, 1: use corresponding snapshot table
	//bit7: 0 not keeping order with context-by clause, 1: keeping original row order even with context-by clause
	//bit8: 0 no repartition, 1: use database partition
	//bit9: 0 parallel execution, 1: sequential execution
	//bit10: 0 output in-memory table, 1: output partitioned in-memory table
	//bit11: 0 no interval fill needed, 1: interval fill needed
	//bit12: 0 do nothing 1: preload chunk data block by block for TSDB engine
	//bit13: 0 non-column expression not evaluated yet 1: non-column expression evaluated
	//bit14: 0 return one row with null values if the table to aggregate is empty, 1: return nothing if the table to aggregate is empty
	//bit15: 0 means no need to explain query, 1: explain query
	//bit16: 0 means no need to return results and explain query, 1: return results and explain query
	//bit17: 0 means nothing, 1: bid probing group size
	//bit18: 0 means nothing, 1: reshuffle for context by
	//bit19: 0 may use hash group, 1: use sort group
	//bit20: 0 may means nothing, 1: optimize group by for temporal column with value partition
	//bit21: 0 may means nothing, 1: use compression for remote call
	//bit22: 0 may means nothing, 1: use vectorized group if possible
	//bit23: 0 may means nothing, 1: SQL planning
	//bit24: 0 automatically add grouping columns to select list, 1: don't add
	//bit25: 0 means nothing, 1: use Pytorch Tensor
	//bit26: 0 select list doesn't contain analytic functions, 1: contain analytic functions
	//bit27: 0 means nothing, 1: use parallel call for limit
	int hint_;
	DomainPartitionSP segment_;
	DomainPartitionSP rightSegment_;
};

class SQLUpdate : public Object {
public:
	SQLUpdate(const SQLContextSP& contextSP, const ObjectSP& tableSP, vector<ColumnDefSP>& updates,
			const ObjectSP& from,vector<ObjectSP>& where, vector<ColumnDefSP>& contextBy, 
			const vector<SortAttributeSP>& csort, const ObjectSP& having);
	SQLUpdate(Session* session, const DataInputStreamSP& in);
	void setSegment(const DomainPartitionSP& segment) { segment_ = segment;}
	DomainPartitionSP getSegment() const { return segment_;}
	void setHint(int hint) { hint_ = hint;}
	int getHint() const { return hint_;}
	void setTransactionId(long long transId) { transactionId_ = transId;}
	long long getTransactionId() const { return transactionId_;}
	virtual ~SQLUpdate(){}
	virtual OBJECT_TYPE getObjectType() const {return SQLUPDATE;}
	virtual ConstantSP getComponent() const;
	virtual string getScript() const;
	virtual void collectUserDefinedFunctions(unordered_map<string,FunctionDef*>& functionDefs) const;
	virtual void collectUserDefinedFunctionsAndClasses(Heap* pHeap, unordered_map<string,FunctionDef*>& functionDefs, unordered_map<string,OOClass*>& classes) const;
	virtual void collectVariables(vector<int>& vars, int minIndex, int maxIndex) const;
	virtual ConstantSP getValue(Heap* pHeap) { return getReference(pHeap);}
	virtual ConstantSP getReference(Heap* pHeap) = 0;
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;

	/**
	 * Analyze FROM clause and obtain a set of column names. If a ColumnRef object isn't in the column list
	 * and its index isn't negative, replace it with the referred object.
	 */
	virtual void materializeColumns(Heap* heap) = 0;
	virtual SQLUpdate* copy() const = 0;

protected:
	SQLContextSP context_;
	ObjectSP table_;
	DomainPartitionSP segment_;
	vector<ColumnDefSP> update_;
	ObjectSP from_;
	vector<ObjectSP> where_;
	vector<ColumnDefSP> contextBy_;
	long long transactionId_;
	/*
	 * bit0: sync chunk to disk
	 * bit1: used for updating distributed in-memory table.
	 *       if set, execute this SQLUpdate at local, otherwise
	 * 	     dispatch this SQLUpdate to related nodes.
	 */
	int hint_;

	char version_ = 1;
	vector<SortAttributeSP> csort_;
	ObjectSP having_{};
};

class SQLDelete : public Object {
public:
	SQLDelete(const SQLContextSP& contextSP, const ObjectSP& tableSP,  vector<ObjectSP>& where) : context_(contextSP), table_(tableSP), where_(where), transactionId_(-1), hint_(0){}
	SQLDelete(Session* session, const DataInputStreamSP& in);
	virtual ~SQLDelete(){}
	void setSegment(const DomainPartitionSP& segment) { segment_ = segment;}
	DomainPartitionSP getSegment() const { return segment_;}
	void setHint(int hint) { hint_ = hint;}
	int getHint() const { return hint_;}
	void setTransactionId(long long transId) { transactionId_ = transId;}
	long long getTransactionId() const { return transactionId_;}
	virtual OBJECT_TYPE getObjectType() const {return SQLDELETE;}
	virtual ConstantSP getComponent() const;
	virtual string getScript() const;
	virtual void collectUserDefinedFunctions(unordered_map<string,FunctionDef*>& functionDefs) const;
	virtual void collectUserDefinedFunctionsAndClasses(Heap* pHeap, unordered_map<string,FunctionDef*>& functionDefs, unordered_map<string,OOClass*>& classes) const;
	virtual void collectVariables(vector<int>& vars, int minIndex, int maxIndex) const;
	virtual ConstantSP getValue(Heap* pHeap) { return getReference(pHeap);}
	virtual ConstantSP getReference(Heap* pHeap) = 0;
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;

	/**
	 * Analyze FROM clause and obtain a set of column names. If a ColumnRef object isn't in the column list
	 * and its index isn't negative, replace it with the referred object.
	 */
	virtual void materializeColumns(Heap* heap) = 0;
	virtual SQLDelete* copy() const = 0;

protected:
	SQLContextSP context_;
	ObjectSP table_;
	vector<ObjectSP> where_;
	DomainPartitionSP segment_;
	long long transactionId_;
	/*
	 * bit0: sync chunk to disk
	 * bit1: used for updating distributed in-memory table.
	 *       if set, execute this SQLUpdate at local, otherwise
	 * 	     dispatch this SQLUpdate to related nodes.
	 */
	int hint_;
};

class Variable:public Object{
public:
	Variable(int index, const string& name):index_(index),name_(name){}
	Variable(const DataInputStreamSP& in);
	virtual ~Variable(){}
	virtual OBJECT_TYPE getObjectType() const {return VAR;}
	virtual ConstantSP getComponent() const;
	virtual ConstantSP getValue(Heap* pHeap);
	virtual ConstantSP getReference(Heap* pHeap);
	bool setValue(Heap* pHeap,const ConstantSP& val, bool constant);
	bool setValue(Heap* pHeap,const ConstantSP& index, const ConstantSP& val, bool validateIndex=true);
	bool setValue(Heap* pHeap,const ConstantSP& index, bool multidimensionalIndex, const ConstantSP& val, bool validateIndex=true);
	bool isConstantVariable(Heap* pHeap) const {return pHeap->isConstant(index_);}
	ConstantSP getValue(Heap* pHeap, const ConstantSP& indexSP);
	int getIndex() const{return index_;}
	const string& getName() const {return name_;}
	virtual string getScript() const {return name_;}
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;
	virtual void collectVariables(vector<int>& vars, int minIndex, int maxIndex) const {if(index_<=maxIndex && index_>=minIndex) vars.push_back(index_);}
	virtual ObjectSP copy(Heap* pHeap, const SQLContextSP& context, bool localize) const;
	virtual ObjectSP copyAndMaterialize(Heap* pHeap, const SQLContextSP& context, const TableSP& table) const;
	virtual bool mayContainColumnRefOrVariable() const { return true;}

private:
	int index_;
	string name_;
};

class Global:public Object{
public:
	Global(const string& name):name_(name){}
	Global(const DataInputStreamSP& in);
	virtual ~Global(){}
	virtual OBJECT_TYPE getObjectType() const {return GLOBAL;}
	virtual ConstantSP getValue(Heap* pHeap);
	virtual ConstantSP getReference(Heap* pHeap);
	bool setValue(Heap* pHeap,const ConstantSP& val, bool copyIfDifferent = false);
	bool setValue(Heap* pHeap,const ConstantSP& index, const ConstantSP& val, bool validateIndex=true);
	ConstantSP getValue(Heap* pHeap, const ConstantSP& indexSP);
	const string& getName() const {return name_;}
	virtual string getScript() const { return "@" + name_;}
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;
private:
	string name_;
};

class CatalogTable : public Object{
public:
	CatalogTable(const string& dbUrl, const string& table, const string& schema, const string& catalog);
	CatalogTable(const DataInputStreamSP& in);
	virtual ~CatalogTable(){}
	virtual OBJECT_TYPE getObjectType() const {return CATALOGTABLE;}
	virtual ConstantSP getComponent() const;
	virtual ConstantSP getValue(Heap* pHeap) { return getReference(pHeap);}
	virtual ConstantSP getReference(Heap* pHeap);
	virtual string getScript() const;
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;
	const string& getTableName() const {return table_;}
	const string& getSchemaName() const {return schema_;}
	const string& getCatalogName() const {return catalog_;}

private:
	string dbUrl_;
	string table_;
	string schema_;
	string catalog_;
};

class ObjectAttribute : public Object{
public:
	ObjectAttribute(int attrIndex, const string& attrName) : attrName_(attrName), attrIndex_(attrIndex) {}
	ObjectAttribute(const DataInputStreamSP& in);
	virtual ~ObjectAttribute(){}
	virtual OBJECT_TYPE getObjectType() const {return ATTR;}
	virtual ConstantSP getValue(Heap* pHeap);
	virtual ConstantSP getReference(Heap* pHeap);
	bool setValue(Heap* pHeap,const ConstantSP& val, bool constant);
	bool setValue(Heap* pHeap,const ConstantSP& index, const ConstantSP& val, bool validateIndex=true);
	bool setValue(Heap* pHeap,const ConstantSP& index, bool multidimensionalIndex, const ConstantSP& val, bool validateIndex=true);
	ConstantSP getValue(Heap* pHeap, const ConstantSP& indexSP);
	int getAttrIndex() const{return attrIndex_;}
	const string& getAttrName() const { return attrName_; }
	virtual string getScript() const { return attrName_; }
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;
	virtual void collectVariables(vector<int>& vars, int minIndex, int maxIndex) const;

private:
	string attrName_;
	int attrIndex_;
};

class SelfObject : public Object {
public:
	SelfObject() {}
	SelfObject(const DataInputStreamSP &in) {
		int version = -1;
		in->readInt(version);
		if (version != 0) throw RuntimeException("Failed to deserialize self object.");
	}
	virtual ~SelfObject() {}
	virtual OBJECT_TYPE getObjectType() const {return SELF;}
	virtual ConstantSP getValue(Heap *heap);
	virtual ConstantSP getReference(Heap *heap);
	virtual string getScript() const { return "self"; }
	virtual IO_ERR serialize(Heap *heap, const ByteArrayCodeBufferSP &buffer) const {
		const int currentVersion = 0;
		return buffer->write(currentVersion);
	}
	virtual void collectVariables(vector<int>& vars, int minIndex, int maxIndex) const {}
};

class ObjectMethodCall : public Object{
public:
	ObjectMethodCall(const ObjectSP& obj, const string& methodName, vector<ObjectSP>& args, bool partialCall=false);
	ObjectMethodCall(const ObjectSP& obj, const string& methodName, INDEX methodIndex, vector<ObjectSP>& args, bool partialCall=false);
	ObjectMethodCall(Session* session, const DataInputStreamSP& in);
	virtual ~ObjectMethodCall(){}
	virtual OBJECT_TYPE getObjectType() const {return METHODCALL;}
	virtual ConstantSP getValue(Heap* pHeap);
	virtual ConstantSP getReference(Heap* pHeap);
	virtual string getScript() const;
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;
	virtual void collectUserDefinedFunctions(unordered_map<string,FunctionDef*>& functionDefs) const;
	virtual void collectUserDefinedFunctionsAndClasses(Heap* pHeap, unordered_map<string,FunctionDef*>& functionDefs, unordered_map<string,OOClass*>& classes) const;
	virtual void collectVariables(vector<int>& vars, int minIndex, int maxIndex) const;
	virtual ObjectSP copy(Heap* pHeap, const SQLContextSP& context, bool localize) const;
	virtual ObjectSP copyAndMaterialize(Heap* pHeap, const SQLContextSP& context, const TableSP& table) const;
	virtual bool mayContainColumnRefOrVariable() const { return true;}
	virtual void retrieveColumns(const TableSP& table, vector<pair<string,string>>& columns) const;
	virtual int checkSpecicalFunction(bool aggrOnly) const;
	inline const string& getMethodName() const { return methodName_;}
	inline int getArgumentCount() const { return args_.size();}
	inline const ObjectSP& getArgument(int index) const { return args_[index];}
	inline bool isPartialCall() const { return partialCall_;}
	inline ObjectSP getObject() const { return obj_; }

private:
	ObjectSP obj_;
	string methodName_;
	INDEX methodIndex_ = -1;
	vector<ObjectSP> args_;
	bool partialCall_;
};

class Dimension: public Object{
public:
	Dimension(const ObjectSP& dim):dims_(1){ dims_[0]=dim;}
	Dimension(vector<ObjectSP>& dims):dims_(dims){}
	Dimension(Session* session, const DataInputStreamSP& in);
	virtual ~Dimension(){}
	virtual OBJECT_TYPE getObjectType() const {return DIM;}
	virtual ConstantSP getComponent() const;
	virtual ConstantSP getValue(Heap* pHeap);
	virtual ConstantSP getReference(Heap* pHeap);
	int getDimensionCount() const {return dims_.size();}
	ObjectSP getDim(int index) const {return dims_[index];}
	virtual string getScript() const;
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;
	virtual void collectUserDefinedFunctions(unordered_map<string,FunctionDef*>& functionDefs) const;
	virtual void collectUserDefinedFunctionsAndClasses(Heap* pHeap, unordered_map<string,FunctionDef*>& functionDefs, unordered_map<string,OOClass*>& classes) const;
	virtual void collectVariables(vector<int>& vars, int minIndex, int maxIndex) const;
	virtual void retrieveColumns(const TableSP& table, vector<pair<string,string>>& columns) const;
	virtual int checkSpecicalFunction(bool aggrOnly) const;
	virtual ObjectSP copy(Heap* pHeap, const SQLContextSP& context, bool localize) const;
	virtual ObjectSP copyAndMaterialize(Heap* pHeap, const SQLContextSP& context, const TableSP& table) const;
	virtual bool mayContainColumnRefOrVariable() const { return true;}

private:
	vector<ObjectSP> dims_;
};

class Expression: public Object{
public:
	Expression(const vector<ObjectSP> & objs, const vector<OperatorSP>&  optrs):
		objs_(objs),optrs_(optrs), annotation_(0){}
	Expression(const vector<ObjectSP> & objs, const vector<OperatorSP>&  optrs, int annotation):
		objs_(objs),optrs_(optrs), annotation_(annotation){}
	Expression(const SQLContextSP& context, Session* session, const DataInputStreamSP& in);
	virtual ~Expression(){}
	virtual OBJECT_TYPE getObjectType() const {return EXPRESSION;}
	virtual ConstantSP getComponent() const;
	virtual ConstantSP getReference(Heap* pHeap);
	virtual ConstantSP getValue(Heap* pHeap);
	int getObjectCount() const {return objs_.size();}
	const ObjectSP& getObject(int index) const {return objs_[index];}
	const vector<ObjectSP>& getObjects() const {return objs_;}
	int getOperatorCount() const {return optrs_.size();}
	const OperatorSP& getOperator(int index) const {return optrs_[index];}
	const vector<OperatorSP>& getOperators() const {return optrs_;}
	virtual string getScript() const;
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;
	virtual void collectUserDefinedFunctions(unordered_map<string,FunctionDef*>& functionDefs) const;
	virtual void collectUserDefinedFunctionsAndClasses(Heap* pHeap, unordered_map<string,FunctionDef*>& functionDefs, unordered_map<string,OOClass*>& classes) const;
	virtual void collectVariables(vector<int>& vars, int minIndex, int maxIndex) const;
	ObjectSP realizeNonColumnExpression(Heap* pHeap, unordered_set<string>& colNames);
	ObjectSP realizeNonColumnExpression(Heap* pHeap, const TableSP& table);
	//Assume all local variables in the expression has been materialized
	ObjectSP realizeNonColumnExpression(Heap* pHeap);
	inline int getAnnotation() const {return annotation_;}
	inline bool getAnnotation(int bitOffset) const {return annotation_ & (bitOffset << 1);}
	void setAnnotation(int bitOffset, bool bitOn);
	virtual ObjectSP copy(Heap* pHeap, const SQLContextSP& context, bool localize) const;
	virtual ObjectSP copyAndMaterialize(Heap* pHeap, const SQLContextSP& context, const TableSP& table) const;
	virtual bool mayContainColumnRefOrVariable() const { return true;}
	virtual void retrieveColumns(const TableSP& table, vector<pair<string,string>>& columns) const;
	virtual int checkSpecicalFunction(bool aggrOnly) const;
	virtual bool containAnalyticFunction() const;

	static ConstantSP void_;
	static ConstantSP null_;
	static ConstantSP default_;
	static ConstantSP true_;
	static ConstantSP false_;
	static ConstantSP one_;
	static ConstantSP zero_;
	static ConstantSP voidDouble2_;
	static OperatorSP logicAnd_;
	static SQLContextSP context_;

private:
	static ObjectSP realizeNonColumnObject(Heap* pHeap, unordered_set<string>& colNames, const ObjectSP& obj);
	static ObjectSP realizeNonColumnObject(Heap* pHeap, const TableSP&, const ObjectSP& obj);
	//Assume all local variables in the expression has been materialized
	static ObjectSP realizeNonColumnObject(Heap* pHeap, const ObjectSP& obj);

private:
	vector<ObjectSP> objs_;
    vector<OperatorSP> optrs_;
    /**
     * The field doesn't involve in the calculation of the expression. But it is used to annotate the
     * expression. For example, expressions in the SQL where clause may help decide which partition is
     * relevant to the query. In the case of range partition or value partition, this flag can further
     * tell if one can optimize the where clause, i.e. removal of the whole or part of the expression
     * from the where clause. In this use case:
     * 	bit0: 0 the part of the expression is used to decide the relevant partition.
     * 	      1 the whole expression is used to decide the relevant partition.
     * 	bit1: value partitioning column is used to decide the relevant partition.
     * 	bit2: range partitioning column is used to decide the relevant partition.
     * 	bit3: list partitioning column is used to decide the relevant partition.
     */
    int annotation_;
};

class Function: public Object{
public:
	Function(const FunctionDefSP& funcSP) : funcSP_(funcSP){}
	virtual ~Function(){}
	virtual OBJECT_TYPE getObjectType() const {return FUNCTION;}
	virtual ConstantSP getComponent() const;
	const FunctionDefSP& getFunctionDef() const {return funcSP_;}
	virtual ConstantSP getValue(Heap* pHeap) {return getReference(pHeap);}
	virtual ConstantSP getReference(Heap* pHeap) = 0;
	virtual int getArgumentCount() const = 0;
	virtual ObjectSP getArgument(int index) const = 0;
	virtual string getScript() const = 0;
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const = 0;
	virtual string getTemplateName() const  = 0;
	virtual FUNCTIONCALL_TYPE getFunctionCallType() const = 0;
	virtual bool isPartialCall() const  = 0;
	virtual Function* getInstance(const vector<ObjectSP>& arguments) const = 0;
	virtual Function* getInstance(const FunctionDefSP& func, const vector<ObjectSP>& arguments) const = 0;
	virtual void collectUserDefinedFunctions(unordered_map<string,FunctionDef*>& functionDefs) const;
	virtual void collectUserDefinedFunctionsAndClasses(Heap* pHeap, unordered_map<string,FunctionDef*>& functionDefs, unordered_map<string,OOClass*>& classes) const;
	virtual ObjectSP copy(Heap* pHeap, const SQLContextSP& context, bool localize) const;
	virtual ObjectSP copyAndMaterialize(Heap* pHeap, const SQLContextSP& context, const TableSP& table) const;
	virtual bool mayContainColumnRefOrVariable() const { return true;}
	virtual void retrieveColumns(const TableSP& table, vector<pair<string,string>>& columns) const;
	virtual int checkSpecicalFunction(bool aggrOnly) const;
	virtual bool containAnalyticFunction() const;

protected:
	FunctionDefSP funcSP_;
};

class GlobalTable: public Object {
public:
	GlobalTable(const string& dbUrl, const string& tableName, const string& tableAlias);
	GlobalTable(Session* session, const DataInputStreamSP& in);
	const string& getDatabaseUrl() const { return dbUrl_;}
	const string& getTableName() const { return tableName_;}
	const string& getTableAlias() const { return tableAlias_;}
	virtual ~GlobalTable(){}
	virtual OBJECT_TYPE getObjectType() const {return GLOBALTABLE;}
	virtual ConstantSP getValue(Heap* pHeap) {return getReference(pHeap);}
	virtual ConstantSP getReference(Heap* pHeap);
	virtual string getScript() const {return tableAlias_;}
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;

private:
	string dbUrl_;
	string tableName_;
	string tableAlias_;
};

class DimTable: public Object {
public:
	DimTable(const GlobalTable& globalTable, const DomainPartitionSP& segment) : globalTable_(globalTable), segment_(segment){}
	DimTable(Session* session, const DataInputStreamSP& in);
	virtual ~DimTable(){}
	virtual OBJECT_TYPE getObjectType() const {return DIMTABLE;}
	virtual ConstantSP getValue(Heap* pHeap) {return getReference(pHeap);}
	virtual ConstantSP getReference(Heap* pHeap);
	virtual string getScript() const {return globalTable_.getTableAlias();}
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;
	const GlobalTable &getGlobalTable() const { return globalTable_; }

private:
	GlobalTable globalTable_;
	DomainPartitionSP segment_;
};

class GroupTask: public Object {
public:
	GroupTask(const string& siteAlias, int capacity, bool parallel);
	GroupTask(const string& siteAlias, vector<DistributedCallSP>& tasks, bool parallel);
	GroupTask(Session* session, const DataInputStreamSP& in);
	virtual ~GroupTask(){}
	void add(const DistributedCallSP& task);
	inline int getTaskCount() const {return tasks_.size();}
	const DistributedCallSP& getTask(int index) const { return tasks_[index];}
	void done(const ConstantSP& result);
	void done(const string& errMsg, bool tryOtherSite);
	void markUsed(int siteIndex);
	void nextSite();
	virtual OBJECT_TYPE getObjectType() const {return GROUPTASK;}
	virtual ConstantSP getValue(Heap* pHeap) {return getReference(pHeap);}
	virtual ConstantSP getReference(Heap* pHeap);
	virtual string getScript() const {return "<GroupTask>";}
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;
	virtual void collectUserDefinedFunctions(unordered_map<string,FunctionDef*>& functionDefs) const;
	virtual void collectUserDefinedFunctionsAndClasses(Heap* pHeap, unordered_map<string,FunctionDef*>& functionDefs, unordered_map<string,OOClass*>& classes) const;
	virtual void collectVariables(vector<int>& vars, int minIndex, int maxIndex) const;

private:
	bool local_;
	bool parallel_;
	string siteAlias_;
	vector<DistributedCallSP> tasks_;
};

class ColumnSelector : public Object {
public:
	ColumnSelector(const ConstantSP& obj, const ColumnContextSP& context) : obj_(obj), colIndex_(context){}
	virtual ~ColumnSelector(){}
	virtual ConstantSP getValue(Heap* pHeap) {return getReference(pHeap);}
	virtual ConstantSP getReference(Heap* pHeap);
	virtual string getScript() const {return "<ColumnSelector>";}
	virtual OBJECT_TYPE getObjectType() const {return COLSELECTOR;}
	virtual IO_ERR serialize(Heap* pHeap, const ByteArrayCodeBufferSP& buffer) const;

private:
	ConstantSP obj_;
	ColumnContextSP colIndex_;
};

class ConfigVariable : public Object {
public:
    ConfigVariable(INDEX index, long long version, ConstantSP data, const std::string& key)
        : index_(index), version_(version), data_(data), key_(key) {}
    ConfigVariable(Session* session, const DataInputStreamSP& in);
    virtual ~ConfigVariable() {}
    virtual OBJECT_TYPE getObjectType() const {
        return CONFIGVAR;
    }
    virtual ConstantSP getValue(Heap *heap);
    virtual ConstantSP getReference(Heap *heap);
    virtual std::string getScript() const;
    virtual IO_ERR serialize(Heap*, const ByteArrayCodeBufferSP&) const;

    INDEX getVariableIndex() {return index_;}
    long long getVariableVersion() {return version_;}
    ConstantSP getVariableCache() {return data_;}
    const std::string &getVariableKey() {return key_;}

    void setVarialeIndex(INDEX index) {index_ = index;}
    void setVariableVersion(long long version) {version_ = version;}
    void setVariableCache(ConstantSP value) {data_ = value;}
    void setVariableKey(const std::string &key) {key_ = key;}

protected:
    INDEX index_;
    long long version_;
    ConstantSP data_;
    std::string key_;
};

class MacroVariable : public Object {
public:
	MacroVariable(const VariableSP& var, int subType, int subIndex = -1);
	MacroVariable(Session* session, const DataInputStreamSP& in);
    virtual ~MacroVariable() {}
    virtual OBJECT_TYPE getObjectType() const {return MACROVAR;}
    virtual ConstantSP getValue(Heap *heap) { return getReference(heap);}
    virtual ConstantSP getReference(Heap *heap);
    virtual std::string getScript() const;
    virtual IO_ERR serialize(Heap*, const ByteArrayCodeBufferSP&) const;

    inline const VariableSP& getVariable() const { return var_;}
    inline const string& getName() const { return var_->getName();}
    inline int getSubType() const { return subType_;}
    inline int getSubIndex() const { return subIndex_;}
    inline bool isFunctionMacro() const { return subType_ == MACRO_FUNCTION;}
    inline bool isColumnMacro() const { return subType_ == MACRO_COLUMN;}
    inline bool isColumnTupleMacro() const { return subType_ == MACRO_COLUMNS;}

private:
	VariableSP var_;
	int subType_;
	int subIndex_;
};

struct ConfigEngineEntries {
    std::vector<ConstantSP> configValues;
    std::unordered_map<std::string, int> configMap;
};

struct ConfigEngineData {
    SmartPointer<ConfigEngineEntries> data_;
    std::atomic<long long> version_;
    Mutex mutex_;

    ObjectSP createConfigVariableReference(const std::string &key) {
        LockGuard<Mutex> lk(&mutex_);
        long long version = version_.load();
        auto data = data_;
        lk.unlock();

        int index = -1;
        ConstantSP value = Expression::void_;
        if (data->configMap.find(key) != data->configMap.end()) {
            index = data->configMap[key];
            value = data->configValues[index];
        }
        return new ConfigVariable{index, version, value, key};
    }

    ConstantSP loadConfigValue(ConfigVariable *configVariable) {
        if (LIKELY(configVariable->getVariableVersion() == version_.load())) {
            if (configVariable->getVariableIndex() < 0) {
                throw RuntimeException("config variable '" + configVariable->getVariableKey() + "' doesn't exist.");
            }
            return configVariable->getVariableCache();
        }
        LockGuard<Mutex> lk(&mutex_);
        auto data = data_;
        long long version = version_.load();
        lk.unlock();

        if (configVariable->getVariableIndex() >= 0) {
            configVariable->setVariableCache(data->configValues[configVariable->getVariableIndex()]);
        } else {
            auto iter = data->configMap.find(configVariable->getVariableKey());
            if (iter != data->configMap.end()) {
                INDEX index = iter->second;
                configVariable->setVarialeIndex(index);
                configVariable->setVariableCache(data->configValues[index]);
            } else {
                throw RuntimeException("config variable '" + configVariable->getVariableKey() + "' doesn't exist.");
            }
        }
        configVariable->setVariableVersion(version);
        return configVariable->getVariableCache();
    }
};

class AggregationMapReduce{
public:
	AggregationMapReduce(const string& functionName, size_t paramCount, const vector<FunctionDefSP>& mappingFunctions,
			const vector<int> paramMapping, const FunctionDefSP& reduceFunction,
			const vector<FunctionDefSP>& runningMapFunctions, const FunctionDefSP& runningFunction);
	void map(const vector<ObjectSP>& arguments, vector<ObjectSP>& functions) const;
	ObjectSP runningMap(const vector<ObjectSP>& arguments) const;
	ObjectSP slidingMap(const vector<ObjectSP>& arguments, const ObjectSP& timeCol, const ObjectSP& window) const;
	const FunctionDefSP& getReduceFunction() const {return reduceFunction_;}

private:
	string name_;
	size_t paramCount_;
	vector<FunctionDefSP> mappingFunctions_;
	vector<int> paramMapping_;
	FunctionDefSP reduceFunction_;
	vector<FunctionDefSP> runningMapFunctions_;
	vector<FunctionDefSP> slidingMapFunctions_;
	FunctionDefSP runningFunction_;
	FunctionDefSP slidingFunction_;
};

class RemoteCall : public DistributedCall{
public:
	RemoteCall(const ObjectSP& obj, const DomainSitePoolSP& sites, std::function<void (bool, const ConstantSP&)> callback);
	RemoteCall(const ObjectSP& obj, const DomainSitePoolSP& sites, const ReducerContainerSP& reducer = ReducerContainerSP());
	virtual ~RemoteCall(){}
	void materializeObject(Heap* heap);
	inline bool hasNextSite() const { return sites_->hasNextSite();}
	inline int getNextSite() const { return sites_->getNextSite();}
	inline int getSiteCount() const { return sites_->getSiteCount();}
	inline void initiateSite(bool useLastSuccessfulSite = false) {sites_->initiateSite(useLastSuccessfulSite);}
	inline void resetInitialSite() { sites_->resetInitialSite();}
	inline void setIntialSite(int index){ sites_->setIntialSite(index);}
	inline bool containLocalSite() const { return sites_->containLocalSite();}
	int nextSite() const;
	void markUsed(int siteIndex);
	inline bool isUrgent() const { return urgent_;}
	inline void setUrgent(bool urgent) { urgent_ = urgent;}
	inline bool isSecondary() const {return secondary_;}
	inline void setSecondary(bool secondary) { secondary_ = secondary;}
	inline bool getCompressionOption() const { return compression_;}
	inline void setCompressionOption(bool option) { compression_ = option;}
	inline bool isInfra() const { return infra_;}
	inline void setInfra(bool option) { infra_ = option;}
	long long getFlag() const;
	void getAllSites(vector<int>& sites) const;
	void logStart() const;
	inline bool autoTransfer() const { return autoTransfer_;}
	inline void setAutoTransfer(bool option) { autoTransfer_ = option;}
	inline int getHAGroupId() const { return haGroupId_;}
	inline void setHAGroupId(int groupId) { haGroupId_ = groupId;}
	inline bool updateLeader() const { return updateLeader_;}
	inline int getHAAttemptCount() const { return haAttempts_;}
	void updateRemoteSite(const DomainSitePoolSP& sites, bool updateLeader = false);
	virtual void setLastSuccessfulSite() {sites_->setLastSuccessfulSite();}
	virtual int getLastSite(){return sites_->getLastSite();}

private:
	DomainSitePoolSP sites_;
	bool objLocalized_;
	bool urgent_;
	bool secondary_;
	bool compression_;
	/**
	 * autoTransfer_ and haGroupId are used in the scenario of high availability (HA). When autoTransfter_ is on,
	 * automatically transfer the remote call to new leader or new leader candidate.
	 */
	bool autoTransfer_;
	bool updateLeader_;
	/**
	 * The boolean flag indicate if the remote call is related to infrastructure such as raft and task management.
	 * When one sets this flag to true, the call is assumed not to generate any sub task. We have a separate set
	 * of workers to process calls relating to infrastructure.
	 */
	bool infra_;
	int haGroupId_;
	int haAttempts_;
};

class LocalCall : public DistributedCall{
public:
	LocalCall(const ObjectSP& obj) : DistributedCall(obj, true){}
	LocalCall(const ObjectSP& obj, const ReducerContainerSP& reducer) : DistributedCall(obj, reducer, true){}
	virtual ~LocalCall(){}
};
class LocalExecutor: public Runnable{
public:
    LocalExecutor(SmartPointer<TaskManager<DistributedCallSP> >& localCallQueue, int id, SmartPointer<std::atomic<int>>& runningTaskCounter, SmartPointer<std::atomic<long long>> accumulatedJobCount);
    int getId() const {return id_;}

protected:
    virtual void run();

protected:
    SmartPointer<TaskManager<DistributedCallSP> > localCallQueue_;
    int id_;
    SmartPointer<std::atomic<int>> runningTaskCounter_;
    SmartPointer<std::atomic<long long>> accumulatedJobCount_;
    HeapSP heap_;
};


class RemoteTaskDispatcher: public Runnable{
public:
	RemoteTaskDispatcher(SmartPointer<SynchronizedQueue<RemoteCallSP> >& remoteCallQueue);

protected:
	virtual void run();

private:
	SmartPointer<SynchronizedQueue<RemoteCallSP> > remoteCallQueue_;
};

class AsynchronousRemoteExecutor: public Runnable{
public:
	AsynchronousRemoteExecutor(){};
	virtual ~AsynchronousRemoteExecutor(){};
	virtual bool addTask(const RemoteCallSP& remoteCall, bool retry = false) = 0;
	virtual void cancelTask(const Guid& rootJobId) = 0;
	virtual TableSP getConnectionStatus() = 0;
	virtual int getQueuedTaskCount() = 0;
	virtual int getSiteCapacity() const = 0;
	virtual int getSiteCount() const = 0;
	virtual bool setMaxConnections(int newConnectionLimit) = 0;
};

class UDFFactory {
public:
	virtual ~UDFFactory(){}
	virtual FunctionDef* createUserDefinedFunction(const string& name, Session* session, const DataInputStreamSP& buffer) = 0;
	virtual FunctionDef* createUserDefinedFunction(const string& name, const vector<ParamSP>& params, bool hasReturnValue=true, bool aggregation=false) = 0;
	virtual FunctionDef* createUserDefinedFunction(const string& name, const HeapSP& heapSP, const vector<ParamSP>& params, const vector<StatementSP>& statements,
			bool hasReturnValue=true, bool aggregation=false) = 0;
	/**
	 * Create a reusable JIT caller which is not thread safe.
	 */
	virtual FunctionDef* createJITCaller(const FunctionDefSP& func, vector<ConstantSP>& args, int flag) = 0;
};

class ConnectionGroup : public Runnable {
public:
	virtual int getConnectionNum() const = 0;
	virtual void shutdownListener() = 0;
	virtual int getRunningWorkerCount() = 0;
	virtual int getRunningTaskCount() = 0;
	virtual int getRunningJobCount() = 0;
	virtual void closeConnection(SOCKET socketId, Console* pConsole) = 0;
	virtual void closeSession(const vector<long long>& sessionIds) = 0;
	virtual SessionSP getSession(long long sessionId) = 0;
	virtual ConstantSP getSessionMemoryStat() = 0;
	virtual void getSessions(vector<SessionSP>& sessions) = 0;
	virtual int getMaxConnections() const = 0;
	virtual void setMaxConnections(int maxConnections) = 0;
};

class MessageThrottle : public Runnable{
public:
	MessageThrottle(){}
	virtual ~MessageThrottle(){}
	virtual void addTopic(const TopicSubscribeSP& topic) = 0;
	virtual void removeTopic(const string& topic) = 0;
};

class AsynchronousPublisher : public Runnable {
public:
	AsynchronousPublisher(){}
	virtual ~AsynchronousPublisher(){}
	virtual PublishConnectionSP addConnection(const TableUpdateQueueSP& messageQueue, const DomainSite& site) = 0;
	virtual bool removeConnection(const DomainSite& site) = 0;
	virtual ConstantSP getStat() const = 0;
	virtual void addRequest(ConsoleSP requestConsole) = 0;
	/**
	 * @brief Get the memory size consumed by items in the publish queue.
	 *
	 * @return memory size in bytes.
	 */
	virtual DomainSite getSubscribe(const string & action) = 0;
	virtual bool addSubscribe(const string & action, DomainSite& site) = 0;
	virtual bool removeSubscribe(const string & action) = 0;
	virtual void subscribeDisconnect(const string subscribeSite) = 0;
	virtual long long getConsumedMemSize() const = 0;
    virtual void addToEpoll(PublishConnection* conn) = 0;
};

class AsynchronousSubscriber: public Runnable{
public:
	AsynchronousSubscriber(){}
	virtual ~AsynchronousSubscriber(){}
	virtual int getRunningWorkers() = 0;
	virtual ConstantSP getStat() const = 0;
	virtual void getLatencyMetrics(long long& lastMsgLatency, long long& cumMsgLatency) const = 0;
	virtual void removeTopic(const string& topic, int executorHash) = 0;
	virtual ConstantSP requestRemotePublishTable(Heap* heap,vector<ConstantSP>& arguments) = 0;
	virtual bool addTopicInReconnProcessing(const string & topic) = 0;
	virtual bool removeTopicInReconnProcessing(const string & topic) = 0;
	/**
	 * @brief Get the memory size consumed by items in the subscribe queue.
	 *
	 * @return memory size in bytes.
	 */
	virtual long long getConsumedMemSize() const = 0;
};

class BatchJobManager {
public:
	virtual ~BatchJobManager(){}
	virtual ConstantSP submitJob(const AuthenticatedUserSP& user, const string& jobIdPrefix, const string& jobDesc,
			int priority, int parallelism, const string &clientIp, int clientPort, const FunctionDefSP& func,
			vector<ConstantSP>& args, bool setViewMode, const FunctionDefSP& onComplete, bool isPythonSession) = 0;
	virtual TableSP getRecentJobs(int count) = 0;
	virtual TableSP getJobStatus(const string& jobId) = 0;
	virtual TableSP getJobStatusById(const vector<string>& jobId) = 0;
	virtual ConstantSP getJobReturn(Heap* heap, const string& jobId, bool blocking) = 0;
	virtual ConstantSP getJobMessage(const string& jobId) = 0;
	virtual int getRunningJobCount() = 0;
	virtual int getQueuedJobCount() = 0;
	virtual bool checkJobStartedAfter(const string& jobIdPrefix, const string& jobDesc, long long timestamp) = 0;
	// throw exception if job didn't submmitted by accessUser when accessUser isn't empty.
	virtual bool cancelJob(const string& jobId, bool& running, const string& accessUser = "") = 0;
	virtual Guid getJobUuid(const string& jobId) const = 0;
	virtual int getWorkerCount() const = 0;
    virtual void getTasks(vector<TaskDesc>& tasks) = 0;
};

class WorkerManager : public Runnable {
public:
	virtual ~WorkerManager(){}
	virtual int getWorkerCount() const = 0;

protected:
	virtual void run() = 0;
};

class ComputingNodeScheduler {
public:
	virtual ~ComputingNodeScheduler(){}
	virtual void schedule(const vector<DistributedCallSP>& tasks) = 0;
};

class ComputingNodeSchedulerFactory {
public:
	virtual ~ComputingNodeSchedulerFactory(){}
	virtual ComputingNodeSchedulerSP createScheduler(const ClusterNodesSP& clusterNodes) = 0;
};

class SQLFactory {
public:
	virtual ~SQLFactory(){}
	virtual SQLQuerySP createSQLQuery(const SQLContextSP& contextSP,int start, int end, char exec, vector<ColumnDefSP>& select, ObjectSP from, vector<ObjectSP>& where,
			vector<SortAttributeSP>& sorting, vector<SortAttributeSP>& sortBy, vector<ColumnDefSP>& groupBy, ObjectSP& having, char groupFlag,
			char cgroups, int hint) = 0;
	virtual SQLQuerySP createSQLQuery(const SQLContextSP& contextSP, ObjectSP& rowOffset, ObjectSP& rowCount, char exec, vector<ColumnDefSP>& select, ObjectSP from, vector<ObjectSP>& where,
			vector<SortAttributeSP>& sorting, vector<SortAttributeSP>& sortBy, vector<ColumnDefSP>& groupBy, ObjectSP& having, char groupFlag,
			char cgroups, int hint) = 0;
	virtual SQLQuerySP createSQLQuery(Guid& rootId, char level, const SQLContextSP& contextSP,int start, int end, char exec, vector<ColumnDefSP>& select, ObjectSP from,
			vector<ObjectSP>& where, vector<SortAttributeSP>& sorting, vector<SortAttributeSP>& sortBy, vector<ColumnDefSP>& groupBy, ObjectSP& having,
			char groupFlag, char cgroups, int hint) = 0;
	virtual SQLQuerySP createSQLQuery(Session* session, const DataInputStreamSP& in) = 0;
	virtual SQLUpdateSP createSQLUpdate(const SQLContextSP& contextSP, const ObjectSP& tableSP,  vector<ColumnDefSP>& updates,	const ObjectSP& from,
			vector<ObjectSP>& where, vector<ColumnDefSP>& contextBy, const vector<SortAttributeSP>& csort, const ObjectSP& having) = 0;
	virtual SQLUpdateSP createSQLUpdate(Session* session, const DataInputStreamSP& in) = 0;
	virtual SQLDeleteSP createSQLDelete(const SQLContextSP& contextSP, const ObjectSP& tableSP,  vector<ObjectSP>& where) = 0;
	virtual SQLDeleteSP createSQLDelete(Session* session, const DataInputStreamSP& in) = 0;
	virtual vector<DomainPartitionSP> getPartitions(Heap* heap, Table* table, vector<ObjectSP>& where) = 0;
	virtual void initializeFilters(Heap* heap, const SQLContextSP& context, const TableSP& table, const vector<ObjectSP>& filters, vector<ObjectSP>& finalFilters) = 0;
	virtual ObjectSP copyAndMaterializeWithNewSQLContext(Heap* heap, const SQLContextSP& context, const ObjectSP& obj, const TableSP& table) = 0;
	virtual ConstantSP calcAggregation(Heap* heap, const FunctionDefSP& func, vector<ConstantSP>& args, const vector<INDEX>& groups) = 0;
};

class TableJoinerFactory {
public:
	virtual ~TableJoinerFactory(){}
	virtual TableJoinerSP createTableJoiner(const string& name, SysFunc joiner, const vector<ObjectSP>& params) = 0;
	virtual TableJoinerSP createTableJoiner(Session* session, const DataInputStreamSP& in) = 0;
};

class SnapshotEngineManager {
public:
	virtual ~SnapshotEngineManager(){}
	virtual void registerSnapshotEngine(const string& dbUrl, const string& tableName, const TableSP& model, const string& keyColumnName) = 0;
	virtual void unregisterSnapshotEngine(const string& dbUrl, const string& tableName) = 0;
	virtual string getKeyColumnName(const string& dbUrl, const string& tableName) = 0;
	/**
	 * Return a copy of the specified snapshot engine.
	 *
	 * dbUrl: the url of the database. it begins with 'dfs://'
	 * tableName: the table name.
	 * keySet: If keySet is a null pointer, return all keys. Otherwise, return specified keys.
	 * columnNames: If columnNames is empty, return all columns. Otherwise return the specified columns.
	 *
	 * Return: if the specified snapshot engine doesn't exist, return a null pointer. Otherwise return a copy.
	 */
	virtual TableSP getSnapshotEngine(const string& dbUrl, const string& tableName, const ConstantSP& keySet, const vector<string>& columnNames) = 0;
	virtual TableSP getSnapshotEngine(const string& dbUrl, const string& tableName, SmartPointer<LockGuard<Mutex>>& lockGuard, string& keyColumn) = 0;
};

/**
 * The abstract interface for text search engine.
 *
 * Descriptions for common parameters:
 * dbUrl: the url for distributed databases, e.g. dfs://demodb
 *
 * columnUrl: the column url including the partition path, table name and column name, e.g. /20190101/IBM/taq/protocol,
 * where '/20190101/IBM' is the partition path, 'taq' is the table name, and 'protocol' is the column name.
 *
 */
class TextSearchEngine {
public:
	virtual ~TextSearchEngine(){}

	/**
	 * Exact boolean search. The query is an expression using AND, OR, NOT operators.
	 *
	 * Example query: ('IBM' or 'MSFT') and 'Stock' and not 'Options'
	 *
	 * The return is a vector of relevant rows. If the original table has no key,
	 * it would be the row offset, otherwise the row key.
	 */
	virtual ConstantSP boolQuery(const string& dbUrl, const string& columnUrl, const ExpressionSP& query) = 0;

	/**
	 * Search the index and return top n rows with key and score. The query in in natural language and the search
	 * engine must convert it into one or multiple query terms before searching.
	 *
	 * The return is a tuple of two vectors. The first vector is the key of the rows (documents). If the original table
	 * has no key, it would be the row offset, otherwise the row key. The second vector is the score of the rows (documents)
	 * representing how similar is a row to the input query.
	 */
	virtual ConstantSP search(const string& dbUrl, const string& columnUrl, const string& query, int top) = 0;

	/**
	 * Add indexes for rows of one column in a particular tablet of the given database. The table has no keys so that
	 * rowOffset is used as document key.
	 */
	virtual void addIndex(const string& dbUrl, const string& columnUrl, INDEX rowOffset, const ConstantSP& vec) = 0;

	/**
	 * Add indexes for rows of one column in a particular tablet of the given database. The table must have key columns
	 * and the row key is used as document key.
	 */
	virtual void addIndex(const string& dbUrl, const string& columnUrl, const ConstantSP& key, const ConstantSP& vec) = 0;

	/**
	 * Delete the entire indexes of one column in a particular tablet of the given database. If the columnUrl is empty,
	 * delete the indexes of the whole database.
	 */
	virtual void deleteIndex(const string& dbUrl, const string& columnUrl) = 0;

	/**
	 * Delete the indexes of some rows of one column in a particular tablet of the given database. The rows are specified
	 * by the key object. This method is used for tables with key columns.
	 */
	virtual void deleteIndex(const string& dbUrl, const string& columnUrl, const ConstantSP& key) = 0;

	/**
	 * Update the indexes of some rows of one column in a particular tablet of the given database. The rows are specified
	 * by the key object. This method is used for tables with key columns.
	 */
	virtual void updateIndex(const string& dbUrl, const string& columnUrl, const ConstantSP& key, const ConstantSP& vec) = 0;
};

class StatementAttribute {
public:
	virtual ~StatementAttribute(){}
	virtual void getAssignmentAttribute(const StatementSP& st, VariableSP& left, ObjectSP& index, OperatorSP& optr, ObjectSP& right) = 0;
	virtual void getMultiAssignmentAttribute(const StatementSP& st, vector<VariableSP>& left, ObjectSP& right) = 0;
	virtual void getReturnAttribute(const StatementSP& st, ObjectSP& ret) = 0;
};


class RecoveryManager {
public:
    virtual ~RecoveryManager(){}
    virtual void replicateChunkToRemoteNode(DFSChunkMetaSP& chunkData, const Guid &recoveryId, const string& remoteSiteAlias, bool isAsyncRecovery,
                                            bool isIncrementalRecovery, long long syncRecoveryChunkRowSize,
                                            bool isTieredStore, const VectorSP& dstVolumes) = 0;
    virtual void promoteChunkVersion(const DFSChunkMetaSP& chunkMeta, int version) = 0;
};

class CacheEngine {
public:
    virtual ~CacheEngine() {}
    /**
     * Read give number of rows in cache engine and append to the input vector.
     * cachedRowOffset = (cachedColumns<<32) + firstRowOffset
     * if the cache doesn't exist, return -2.
     * If the starting row number in the cache is larger than the size of the input vector, set cachedRowOffset and return -1.
     * If the available rows is less than the desired rows, throw an exception.
     * If the starting row number in the cache equals to the size of the input vector, set cachedRowOffset,
     * return the file offset of the starting row, otherwise return 0.
     */
    virtual long long loadColumn(const Guid& chunkId, const string& tableName, int columnIndex, INDEX rows, const VectorSP& vec, long long& cachedRowOffset) = 0;
    virtual bool loadColumn(const Guid& chunkId, const string& logicalTableName, int columnIndex, INDEX startRows, INDEX length, const VectorSP& vec) = 0;
	virtual long long getCachedTabletOffset(const Guid& chunk, const string& tableName) = 0;
    virtual void flushChunkTablet(const Guid& chunk, const string& tableName) = 0;
    virtual void flushChunk(const Guid& chunkId) = 0;
    virtual bool isTransactionFlushed(long long tid) = 0;
    virtual void garbageCollect(bool force = false) = 0;
    virtual SymbolBaseSP getSymbolBase(const Guid& guid) = 0;
    virtual SymbolBaseSP getCachedSymbolBase(const Guid& guid) = 0;
    virtual void rollback(const vector<Guid>& chunkIds, long long tid) = 0;
    virtual void complete(Heap* heap, const vector<Guid>& chunkIds, long long tid, long long cid) = 0;
    virtual bool hasUnFinishedJobs() = 0;
    virtual void dfsTableCacheRemoved(const string& dbId, const vector<Guid> & chunkIds, const string & logicalTableName){}
    virtual long long getCapacity() = 0;
};

class TabletWriter {
public:
	virtual ~TabletWriter(){}

	/*
	 * Append data to chunks in batch
	 */
	virtual void append(Heap* heap, const string& dbUrl, const Guid& chunkId, const string& chunkPath, const string& tableName, const string& physicalTableName, const string& fullPhysicalPath,  const SymbolBaseSP& symbase,
                           long long tid, INDEX existingTableSize, const ConstantSP& table, const vector<ColumnDesc>& columns, IoTransaction* tran, int compressionMode, bool saveSymbolBase, long long lsn, bool appendForDelete) = 0;

	/*
	 * Replace the whole chunk with the given data. The compression of the new data follows the schema of the table.
	 */
	virtual void replace(const string& dbUrl, const string& tableName, const DFSChunkMetaSP& chunkMeta, const TableSP& data, long long transactionId) = 0;

    virtual void overwriteTablet(Heap* heap, const string& dbUrl, const Guid& chunkId, const string& tableName, const string& physicalTableName, const string& oldFullPath, const string& fullPhysicalPath, const SymbolBaseSP& symbase,
                         const ConstantSP& table, const vector<ColumnDesc>& columns, IoTransaction* tran, int compressionMode) = 0;

    virtual void overwriteTabletCols(Heap* heap, const string& dbUrl, const Guid& chunkId, const string& tableName, const string& physicalTableName,
                             const string& oldTableFullPath, const string& newTableFullPath,const vector<string>& colNames, const VectorSP& colDatas,
                             const SymbolBaseSP& symbase, const vector<ColumnDesc>& columns, IoTransaction* tran) = 0;
    virtual void overwriteComplete(long long tid, long long cid)  = 0;
};

class TabletReader {
public:
	TabletReader(bool sortKey, bool blockFilter, bool cacheTablet, bool columnOptimize) : sortKey_(sortKey), blockFilter_(blockFilter),
		cacheTablet_(cacheTablet), columnOptimize_(columnOptimize){}
	virtual ~TabletReader(){}

	/*
	 * Load the specified chunk with filters.
	 *
	 * filters is an in/out parameter. If all loaded rows satisfy one filter, the filter will be removed from filters.
	 *
	 * colNames: if colNames is empty, all columns of the tablet might be used. Otherwise, the specified columns would be used.
	 * colNames might include columns that don't exist in the table. Simply ignore the column if it doesn't exist. Column names
	 * are always in lower case.
	 *
	 * valuePartitionCols: value-partitioned columns and their corresponding values on this particular chunk.
	 * 
	 * limit: the number of rows to read, positive means from the start, negative means from the end, zero means all
	 * byKey: whether the limit is by key
	 */
	virtual TableSP load(Heap* heap, const string& dbUrl, const string& tableName, const DFSChunkMetaSP& chunk, vector<ObjectSP>& filters,
			const vector<string>& colNames, const vector<pair<string, ConstantSP>>& valuePartitionCols,
			INDEX limit=0, bool byKey=false) const = 0;

	inline bool cacheTablet() const { return cacheTablet_;}
	inline bool supportSortKey() const  {return sortKey_;}
	inline bool supportBlockFilter() const {return blockFilter_;}
	/*
	 * Column optimization refers to that the performance of load method will be improved if the names of required columns are specified.
	 * When this feature is on, the caller of load method should prepare names of used columns. When this feature is off, simply leave
	 * the parameter [colNames] of load method empty.
	 */
	inline bool supportColumnOptimize() const {return columnOptimize_;}

private:
	bool sortKey_;
	bool blockFilter_;
	bool cacheTablet_;
	bool columnOptimize_;
};

class RedoLogManager {
public:
	RedoLogManager(DBENGINE_TYPE type) : engineType_(type){}
	virtual ~RedoLogManager(){}
	virtual void init() = 0;
	virtual void addSymbolBaseRecord(long long tid, long long lsn, const Guid& chunkId, const string& path, int symbaseSize, const ConstantSP& newSymbols) = 0;
	virtual void commitTransaction(long long tid) = 0;
	virtual void rollbackTransaction(long long tid) = 0;
	virtual void completeTransaction(long long tid) =0;
	virtual void garbageCollect() = 0;
	virtual void syncAll() = 0;
	virtual long long getDiskUsage() = 0;
	virtual long long getLogSeqNumber() = 0;
	virtual ConstantSP getHeaderQueueDepth() = 0;
	virtual  ConstantSP getDataQueueDepth() = 0;
	virtual void applyTidRedoLog(long long tid, bool fromResolution = false) = 0;
	virtual  set<long long> getAllResolvedTids() = 0;
	virtual bool isGcFinished() = 0;
	inline RWLock* getLock() { return &lock_;}
	inline DBENGINE_TYPE getEngineType(){return engineType_;}

protected:
	DBENGINE_TYPE engineType_;
	RWLock lock_;
};

class StorageEngine {
public:
	StorageEngine(RedoLogManager* redoLogManager, RecoveryManager* recoveryManager, CacheEngine* cacheEngine, TabletReader* reader,
			TabletWriter* writer) : redoLogManager_(redoLogManager), recoveryManager_(recoveryManager), cacheEngine_(cacheEngine),
			tabletReader_(reader), tabletWriter_(writer){
	}

	virtual ~StorageEngine(){
		/* Do not delete all pointers to avoid crash when quit application, because there are other smart pointer to refer these object,
		 * like OlapRecoveryMg,OlapRedoLogMg, IotRedoLogMg ... */
	}

	RedoLogManager* getRedoLogManager() {return redoLogManager_;}
	RecoveryManager* getRecoveryManager() {return recoveryManager_;}
	CacheEngine* getCacheEngine() const {return cacheEngine_;}
	TabletReader* getTabletReader() const {return tabletReader_;}
	TabletWriter* getTabletWriter() const {return tabletWriter_;}

protected:
    RedoLogManager* redoLogManager_;
	RecoveryManager* recoveryManager_;
	CacheEngine* cacheEngine_;
	TabletReader* tabletReader_;
	TabletWriter* tabletWriter_;

public:
	static StorageEngine* get(DBENGINE_TYPE type) { return engines[(int)type].get();}
	static void registerEngine(DBENGINE_TYPE type, const StorageEngineSP& engine) {
		if(type >= DBENGINE_TYPE::MAX_DBENGINE_TYPES)
			throw RuntimeException("Invalid storage engine type.");
		engines[(int)type] = engine;
		if(type == DBENGINE_TYPE::IOT)
			IOT_ENGINE = engine;
		else if(type == DBENGINE_TYPE::OLAP)
			OLAP_ENGINE = engine;
		else if(type == DBENGINE_TYPE::OLTP)
			OLTP_ENGINE = engine;
		else if(type == DBENGINE_TYPE::IMOLTP)
			IMOLTP_ENGINE = engine;
	}

	static string getEngineName(DBENGINE_TYPE type) {
		switch(type){
		case DBENGINE_TYPE::IOT :
			return "TSDB";
		case DBENGINE_TYPE::OLAP :
			return "OLAP";
		case DBENGINE_TYPE::OLTP :
			return "OLTP";
		case DBENGINE_TYPE::IMOLTP:
			return "IMOLTP";
		default:
			return "";
		}
	}

	static void initialize() {
		for(int i=0; i<(int)DBENGINE_TYPE::MAX_DBENGINE_TYPES; ++i)
			engines[i] =  nullptr;
	}

	static StorageEngineSP IOT_ENGINE;
	static StorageEngineSP OLAP_ENGINE;
	static StorageEngineSP OLTP_ENGINE;
	static StorageEngineSP IMOLTP_ENGINE;

protected:
	static StorageEngineSP engines[(int)DBENGINE_TYPE::MAX_DBENGINE_TYPES];
};

class InMemoryTableFactory {
public:
	virtual ~InMemoryTableFactory(){}
	virtual Table* createTable(TABLE_TYPE type, const vector<ConstantSP>& cols, const vector<string>& colNames, const vector<int>& keys) = 0;
	virtual void addColumn(const TableSP& table, const vector<string>& colNames, const vector<DATA_TYPE>& colTypes,
						   const vector<int> &colExtras) = 0;
	virtual bool clearData(const TableSP& table) = 0;
	static InMemoryTableFactorySP inst_;
};

class InMemoryOltpFactory {
public:
	virtual ~InMemoryOltpFactory() {}

	/**
	 * open an IMOLTP database, if the database does not exist, create it.
	 */
	virtual ConstantSP openDatabase(Heap *heap, const string &dbName, const DomainSP &domain) = 0;

	virtual void dropDatabase(Heap *heap, const string &dbName) = 0;

	virtual void createTable(Heap *heap, const string &dbName, const string &tableName,
							 const vector<ColumnDesc> &colDesc, const vector<string> &pkDesc,
							 const vector<pair<bool, vector<string>>> &secondaryDesc) = 0;

	virtual void dropTable(Heap *heap, const string &dbName, const string &tableName) = 0;

	virtual TableSP loadTable(Heap *heap, const string &dbName, const string &tableName) = 0;

	virtual TableSP loadTable(Heap *heap, const string &tableName) = 0;

	virtual bool existsDatabase(Heap *heap, const string &dbName) = 0;

	virtual bool existsTable(Heap *heap, const string &dbName, const string &tableName) = 0;

	/**
	 * @brief Insert data to a OLTP table.
	 *
	 * @param table OLTP table handler.
	 * @param data Data wants to insert, typically it's a Table.
	 *
	 * @return long long How many rows were inserted.
	 */
	virtual long long insert(Heap *heap, const TableSP &table, const ConstantSP &data) = 0;

	static InMemoryOltpFactorySP inst_;
};

enum class AsyncReplicationTaskOp {
	APPEND,
	SQL_UPDATE,
	SQL_DELETE,
	UPSERT,
	DROP_TABLE,
	DROP_PARTITION,
	DROP_DB,
	ADD_RANGE_PARTITION,
	ADD_VALUE_PARTITION,
	CREATE_TABLE,
	CREATE_PARTITIONED_TABLE,
	CREATE_DB,
	ADD_COLUMN,
	DROP_COLUMN,
	REPLACE_COLUMN,
	RENAME_TABLE,
	TRUNCATE_TABLE,
	SET_COLUMN_COMMENT,
	RENAME_COLUMN,
	CREATE_DOMAIN,
	APPEND_CHUNK_GRANULARITY,
};

enum class ReplicationLevel {
    GLOBAL,
    DB,
};

struct ReplicationTaskData {
	string funcName;
	vector<ConstantSP> args;
};

using ReplicationTaskDataSP = SmartPointer<ReplicationTaskData>;

class AsyncReplicationOp {
public:

	class Operation {
	public:
		Operation(std::function<void()> succ = [](){}, std::function<void()> fail = [](){}): succ_(std::move(succ)), fail_(std::move(fail)) {}
		~Operation() { cancel(); }
		void done() { if (!done_) {done_ = true; succ_();} }
		void cancel() { if (!done_) {done_ = true; fail_();} }
	private:
		std::function<void()> succ_, fail_;
		bool done_ = false;
	};
	using OperationSP = SmartPointer<Operation>;
	using TaskInfo = std::pair<DictionarySP, OperationSP>;

	virtual ~AsyncReplicationOp() {}
	/**
	 * @brief generate replication task meta
	 * @return DictionarySP dict contains replication metaInfo, ready to send it to controller.
	 */
	virtual DictionarySP replicationInfo(Session* session, AsyncReplicationTaskOp op, const string& srcNodeAlias, const string& dbName,
		DomainSP domain = DomainSP{}, long long tid = -1, const string& tableName = "", size_t dataRows = 0) = 0;
	virtual ReplicationTaskDataSP replicateAppend(DictionarySP taskInfo, const ConstantSP& object) = 0;
	virtual ReplicationTaskDataSP replicateDropDatabase(DictionarySP taskInfo) = 0;
	virtual ReplicationTaskDataSP replicateUpsert(DictionarySP taskInfo, const ConstantSP& keyColNames, const ConstantSP& table, bool ignoreNull, const ConstantSP& sortColumns) = 0;
	virtual ReplicationTaskDataSP replicateDropPartition(DictionarySP taskInfo, const string& dbUrl, const ConstantSP& partitionPaths, bool forceDelete, bool deleteSchema) = 0;
	virtual ReplicationTaskDataSP replicateDropTable(DictionarySP taskInfo, const string& dbUrl) = 0;
	virtual ReplicationTaskDataSP replicateAddRangePartitions(DictionarySP taskInfo, const vector<ConstantSP>& arguments) = 0;
	virtual ReplicationTaskDataSP replicateAddValuePartitions(DictionarySP taskInfo, const vector<ConstantSP>& arguments) = 0;
	virtual ReplicationTaskDataSP replicateSQLDelete(DictionarySP taskInfo, SQLDelete* sqlDelete, Heap* heap) = 0;
	virtual ReplicationTaskDataSP replicateSQLUpdate(DictionarySP taskInfo, SQLUpdate* sqlUpdate, Heap* heap) = 0;
	virtual ReplicationTaskDataSP replicateCreateTable(DictionarySP taskInfo, const vector<ConstantSP>& args) = 0;
	virtual ReplicationTaskDataSP replicateCreatePartitionedTable(DictionarySP taskInfo, const vector<ConstantSP>& args, Heap* heap) = 0;
	virtual ReplicationTaskDataSP replicateCreateDB(DictionarySP taskInfo, const vector<ConstantSP>& args) = 0;
	virtual ReplicationTaskDataSP replicateAddColumn(DictionarySP taskInfo, const vector<ConstantSP>& args) = 0;
	virtual ReplicationTaskDataSP replicateDropColumn(DictionarySP taskInfo, ConstantSP colNames) = 0;
	virtual ReplicationTaskDataSP replicateReplaceColumn(DictionarySP taskInfo, ConstantSP colName, ConstantSP newCol) = 0;
	virtual ReplicationTaskDataSP replicateRenameTable(DictionarySP taskInfo, const string& newName)= 0;
	virtual ReplicationTaskDataSP replicateTruncateTable(DictionarySP taskInfo) = 0;
	virtual ReplicationTaskDataSP replicateSetColumnComment(DictionarySP taskInfo, const vector<string>& colNames, const vector<string>& comments) = 0;
	virtual ReplicationTaskDataSP replicateRenameColumn(DictionarySP taskInfo, ConstantSP oldColName, ConstantSP newColName) = 0;
	virtual ReplicationTaskDataSP replicateCreateDomain(DictionarySP taskInfo, DomainSP domain) = 0;
	virtual ReplicationTaskDataSP replicateAppendChunk(DictionarySP taskInfo, VectorSP datas) = 0;
	// If txn commited, call Operation::done(). If txn rollback, txn->commit() will throw exception and
	// Operation::~Operation will clean TaskData of txn.
	virtual OperationSP submitTask(DictionarySP taskInfo, ReplicationTaskDataSP taskData) = 0;
	virtual ReplicationLevel level() = 0;
	
	static AsyncReplicationOpSP inst_;
	using Callback = std::function<TaskInfo(long long, DomainSP)>;
	virtual DictionarySP setSendReplicationDB(const DictionarySP& taskInfo) = 0;
	virtual bool sendReplicationDB(const DictionarySP& taskInfo) = 0;
	virtual DictionarySP setPriority(const string& db, int priority, const DictionarySP& taskInfo) = 0;
	virtual std::pair<string, short> getPriority(const DictionarySP& taskInfo) = 0;
};

class CatalogMgmt {
public:
	struct NameId {
		NameId(const string& pattern): name(pattern), id(Util::lower(pattern)) {}
		NameId(const char* pattern): NameId(string(pattern)) {}
		const string name;
		const string id;
	};

	struct Catalog {
		struct Schema {
			string name;
			string dbUrl;
		};

		string getDBUrl(const NameId& schema) {
			if (schemas.count(schema.id) == 0)
				return "";
			return schemas[schema.id].dbUrl;
		}

		string name;
		// map: <schemaName, schema>
		std::unordered_map<string, Schema> schemas;
	};

	CatalogMgmt() = default;
	virtual ~CatalogMgmt() = default;

	bool existCatalog(const NameId& cat) {
		LockGuard<Mutex> guard{&m_};
		return cMap_.count(cat.id) > 0;
	}

	Catalog getCatalog(const NameId& cat) {
		LockGuard<Mutex> g{&m_};
		if (cMap_.count(cat.id) == 0) {
			return Catalog{};
		}
		return cMap_[cat.id];
	}

	std::unordered_map<string, Catalog> getAllCatalog() {
		LockGuard<Mutex> g{&m_};
		return cMap_;
	}

	string getDBUrl(const NameId& catalog, const NameId& schema) {
		LockGuard<Mutex> g{&m_};
		if (cMap_.count(catalog.id) == 0) {
			return "";
		}

		return cMap_[catalog.id].getDBUrl(schema);
	}

	std::pair<string, string> getSchemaByDBUrl(const string& dbUrl) {
		LockGuard<Mutex> g{&m_};
		return getSchemaByDBUrl(dbUrl, true);
	}

	// singleton
	static CatalogMgmtSP inst;

protected:
	std::pair<string, string> getSchemaByDBUrl(const string& dbUrl, bool cacheFirst) {
		if (cacheFirst && !dbUrl2SchemaCache_.empty()) {
			if (dbUrl2SchemaCache_.count(dbUrl) > 0) {
				return dbUrl2SchemaCache_[dbUrl];
			} else {
				return {"", ""};
			}
		}

		std::pair<string, string> result("", "");
		for (const auto& cat: cMap_) {
			for (const auto& schema: cat.second.schemas) {
				dbUrl2SchemaCache_[schema.second.dbUrl] = std::make_pair(cat.second.name, schema.second.name);
				if (schema.second.dbUrl == dbUrl) {
					result = dbUrl2SchemaCache_[dbUrl];
				}
			}
		}

		return result;
	}

	void invalidateDBUrlCache() {
		dbUrl2SchemaCache_.clear();
	}

	std::unordered_map<string, Catalog> cMap_;
	std::unordered_map<string, std::pair<string, string>> dbUrl2SchemaCache_;
	Mutex m_;
};

#endif /* CONCEPTS_H_ */
