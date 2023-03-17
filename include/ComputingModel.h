/*
 * ComputingModel.h
 *
 *  Created on: Oct 2, 2017
 *      Author: dzhou
 */

#ifndef COMPUTINGMODEL_H_
#define COMPUTINGMODEL_H_

#include <chrono>

#include "CoreConcept.h"
#include "Util.h"
#include "Logger.h"

#define MAX_JOB_PRIORITY 9
#define DEF_JOB_PRIORITY 4
#define MIN_JOB_PRIORITY 0
#define MIN_PARALLELISM  1
#define DEF_PARALLELISM  2
#define MAX_PARALLELISM  64
#define MAX_JOB_DEPTH 4

void checkTaskErrorMsg(const vector<DistributedCallSP>& calls);
namespace OperatorImp{

ConstantSP localReducer(Heap* heap,vector<ConstantSP>& arguments);

}

struct TaskDesc {
	TaskDesc(const Guid& rootJobId, const string& userId, const string& type, const string& desc, int priority, int parallelism,
			std::chrono::system_clock::time_point& startTime, long long sessionId, const string& remoteIP, int remotePort,
			int totalTaskCount, int finishedTaskCount, int runningTaskCount, long long firstTaskStartTime, long long latestTaskStartTime) :
		rootJobId(rootJobId), userId(userId), type(type), desc(desc), priority(priority), parallelism(parallelism), startTime(startTime),
		sessionId(sessionId), remoteIP(remoteIP), remotePort(remotePort),
 		totalTaskCount(totalTaskCount), finishedTaskCount(finishedTaskCount), runningTaskCount(runningTaskCount), 
		firstTaskStartTime(firstTaskStartTime),latestTaskStartTime(latestTaskStartTime)	{}
	Guid rootJobId;
	string userId;
	string type;
	string desc;
	int priority;
	int parallelism;
	std::chrono::system_clock::time_point startTime;
	long long sessionId;
	string remoteIP;
	int remotePort;
	int totalTaskCount; 
	int finishedTaskCount; 
	int runningTaskCount; 
	long long firstTaskStartTime;
	long long latestTaskStartTime;	
};

template<class T>
class TaskManager {
private:
    struct JobContainer;
    typedef SmartPointer<JobContainer> JobContainerSP;

public:
    TaskManager() : taskQueues_(MAX_JOB_PRIORITY + 1), nelts_(0) {}
    TaskManager(const TaskManager & rhs)=delete;
    TaskManager(const TaskManager && rhs)=delete;

    void push(const T & item, bool fixedPriority = false) {
        int priority = item->getPriority();
        int parallelism = item->getParallelism();
        const Guid& rootJobId = item->getRootJobId();

        LockGuard<Mutex> g(&mtx_);
        if (priority > MAX_JOB_PRIORITY || priority < MIN_JOB_PRIORITY) {
            LOG_WARN("Task priority must be in the range of [" + std::to_string(MIN_JOB_PRIORITY) + "," + std::to_string(MAX_JOB_PRIORITY) + "]");
        }
        if (parallelism > MAX_PARALLELISM || parallelism < MIN_PARALLELISM) {
            LOG_WARN("Task parallelism must be in the range of [" + std::to_string(MIN_PARALLELISM) + "," + std::to_string(MAX_PARALLELISM) + "]");
        }

        auto it = jobCurrentPriorityMap_.find(rootJobId);
        if(it != jobCurrentPriorityMap_.end()){
        	auto & queue = taskQueues_[it->second];
			auto job = findJobById(queue, rootJobId);
			if(!job.isNull()){
				if (!job->cancelled || !item->isCancellable()) {
					job->tasks.push_back(item);
					job->totalTaskCount++;
					nelts_++;
				}
				else
					item->cancel(false);
				 cv_.notify();
				 return;
			}
			else{
				LOG_WARN("The job [" + rootJobId.getString() + "] exists in jobCurrentPriorityMap, but not in corresponding job queue [priority=" + std::to_string(it->second) + "]");
				jobCurrentPriorityMap_.erase(rootJobId);
			}
        }

		auto & queue = taskQueues_[priority];
		string userId, desc, type;
		long long sessionId = LLONG_MIN;
		string remoteIP;
		int remotePort = INT_MIN;
		item->getTaskDesc(type, desc);
		Session* session = item->getSession().get();
		if(session != NULL){
			userId = session->getUserAsIs()->getUserId();
			sessionId = session->getSessionID();
			remotePort = session->getRemotePort();
			remoteIP = session->getRemoteIP();
		}
		JobContainerSP newJob = new JobContainer(rootJobId, sessionId, remoteIP, remotePort, userId, type, desc, priority, parallelism, fixedPriority);
		newJob->tasks.push_back(item);
		jobCurrentPriorityMap_[rootJobId] = priority;
		nelts_++;
		newJob->totalTaskCount++;
		addJobToQueue(queue, newJob);
        cv_.notify();
    }

	void blockingPop(T & item) {
		LockGuard<Mutex> g(&mtx_);
		int workerId = Thread::getID();
		finishTask(workerId);

		if(!taskQueues_[0].empty() && taskQueues_[0].front()->priority > 0){
			moveJobBetweenQueues(taskQueues_[0].front(), 0);
		}

		JobContainerSP job = nullptr;
		while (!pickCandidateJob(job) && !pickCandidateJobRelaxed(job))
			cv_.wait(mtx_);
		item = job->tasks.front();
		job->tasks.pop_front();
		while(true){
			//Can't throw OOM. So we sleep for one second and then try again till complete.
			try{
				workerTaskAssignmentMap_[workerId] = pair<T, JobContainerSP>(item, job);
				break;
			}
			catch(...){
				Thread::sleep(1000);
			}
		}
		job->runningTaskCount++;
		--nelts_;
		job->quotaLeft--;
		if (job->firstTaskStartTime == LLONG_MIN) {
			job->firstTaskStartTime = Util::toLocalTimestamp(std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1));
		}
		job->latestTaskStartTime = Util::toLocalTimestamp(std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1));
    }

	bool blockingPop(T& item, int milliSeconds) {
        LockGuard<Mutex> g(&mtx_);
        int workerId = Thread::getID();
        finishTask(workerId);

		if(!taskQueues_[0].empty() && taskQueues_[0].front()->priority > 0){
			moveJobBetweenQueues(taskQueues_[0].front(), 0);
		}

        do {
            JobContainerSP job = nullptr;
            if (pickCandidateJob(job) || pickCandidateJobRelaxed(job)) {
                item = job->tasks.front();
                job->tasks.pop_front();
        		while(true){
        			//Can't throw OOM. So we sleep for one second and then try again till complete.
        			try{
        				 workerTaskAssignmentMap_[workerId] = pair<T, JobContainerSP>(item, job);
        				break;
        			}
        			catch(...){
        				Thread::sleep(1000);
        			}
        		}
                job->runningTaskCount++;
                --nelts_;
                job->quotaLeft--;
				if (job->firstTaskStartTime == LLONG_MIN) {
					job->firstTaskStartTime = Util::toLocalTimestamp(std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1));
				}
				job->latestTaskStartTime = Util::toLocalTimestamp(std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1));
                return true;
            }
            long long start = std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1);
            cv_.wait(mtx_, milliSeconds);
            long long end = std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1);
            milliSeconds -= end - start;
        } while (milliSeconds > 0);
        return false;
    }

	/**
	 * Cancel all tasks which has the given root job identification.
	 * Return the number of running tasks.
	 */
    int cancel(const Guid& rootJobId) {
        LockGuard<Mutex> g(&mtx_);

        LOG_INFO("Received cancel request: " + rootJobId.getString());
        auto it = jobCurrentPriorityMap_.find(rootJobId);
        if (it == jobCurrentPriorityMap_.end()) {
            return 0;
        }
        int priority = it->second;
        auto job = findJobById(taskQueues_[priority], rootJobId);
        if(job.isNull()){
    	   LOG_WARN("The job [" + rootJobId.getString() + "] exists in jobCurrentPriorityMap, but not in corresponding job queue [priority=" + std::to_string(priority) + "]");
    	   jobCurrentPriorityMap_.erase(rootJobId);
    	   return 0;
        }

        int cancelledTasks = job->cancel();
        nelts_ -= cancelledTasks;
        if(job->empty())
        	removeJobFromQueue(taskQueues_[priority], rootJobId);

        int runningTasks = 0;
        auto item = workerTaskAssignmentMap_.begin();
        while(item != workerTaskAssignmentMap_.end()){
        	if(item->second.second->rootJobId == rootJobId && item->second.first->isCancellable()){
        		item->second.first->cancel(true);
        		++runningTasks;
        	}
        	++item;
        }
        LOG_INFO("Cancelled " + std::to_string(cancelledTasks) + " task(s). Notified " + std::to_string(runningTasks) + " running task(s) to cancel.");
        return runningTasks;
    }

    void getTasks(vector<TaskDesc>& tasks){
    	LockGuard<Mutex> g(&mtx_);
    	for(auto& queue : taskQueues_){
    		for(JobContainerSP job : queue){
    			if(job->rootJobId.isZero())
    				continue;
    			tasks.emplace_back(job->rootJobId, job->userId, job->type, job->desc, job->priority,
								   job->quota, job->startTime, job->sessionId, job->remoteIP, job->remotePort,
								   job->totalTaskCount, job->finishedTaskCount, job->runningTaskCount,
								   job->firstTaskStartTime, job->latestTaskStartTime);
    		}
    	}
    }

    void finishWorker() {
        int workerId = Thread::getID();
        LockGuard<Mutex> g(&mtx_);

        finishTask(workerId);
    }

    // Returns number of tasks waiting to be scheduled
	inline int size() const { return nelts_; }
	inline bool empty() const { return nelts_ == 0;}

private:

    // Assume mtx_ is taken. Finish the task previously running by this worker if any.
    void finishTask(int workerId) {
        auto it = workerTaskAssignmentMap_.find(workerId);
        if (it == workerTaskAssignmentMap_.end()) {
            return;
        }
        JobContainerSP job = it->second.second;
        Guid rootJobId = job->rootJobId;
        int priority = jobCurrentPriorityMap_[rootJobId];
        job->runningTaskCount--;
		job->finishedTaskCount++;
        workerTaskAssignmentMap_.erase(workerId);
        if (job->empty()) {
            removeJobFromQueue(taskQueues_[priority], rootJobId);
            jobCurrentPriorityMap_.erase(rootJobId);
        }
        else if (job->quotaLeft <= 0) {
			// restore quota for next round of allocation
			job->quotaLeft = job->quota;
			moveJobBetweenQueues(job, priority);
        }
    }

    // Find a job with highest priority and quotaLeft > 0
    bool pickCandidateJob(JobContainerSP & resJob, int startPriority = MAX_JOB_PRIORITY) {
        for (int p = startPriority; p >= 0; --p) {
            auto & queue = taskQueues_[p];
            for (unsigned int j = 0; j < queue.size(); ++j) {
                auto job = queue[j];
                if (job->quotaLeft > 0 && job->tasks.empty() == false) {
                    resJob = job;
                    return true;
                }
            }
        }
        return false;
    }

    // Worker resources are abundant, pick a job using relaxed conditions: job with the highest priority and pending tasks.
    bool pickCandidateJobRelaxed(JobContainerSP & resJob, int startPriority = MAX_JOB_PRIORITY) {
        for (int p = startPriority; p >= 0; --p) {
            auto & queue = taskQueues_[p];
            for (unsigned int j = 0; j < queue.size(); ++j) {
                auto job = queue[j];
                if (!job->tasks.empty()) {
                    resJob = job;
                    return true;
                }
            }
        }
        return false;
    }

    // Return true if there exist a job that has tasks to be executed with priority less than `priority`
    bool hasLowerPriorityPendingTask(int priority) {
        auto & queue = taskQueues_[priority];
        unsigned int jobCount = queue.size();
		for (unsigned int j = 1; j < jobCount; ++j) {
			if (!queue[j]->tasks.empty())
				return true;
		}
        JobContainerSP dummy;
        return pickCandidateJobRelaxed(dummy, priority - 1);
    }

    int moveJobBetweenQueues(JobContainerSP job, int currentPriority) {
        if (job->fixed) return 0;
        auto & queue = taskQueues_[currentPriority];
        auto & rootJobId = job->rootJobId;
        if (currentPriority>0 && hasLowerPriorityPendingTask(currentPriority)) {
            removeJobFromQueue(queue, rootJobId);
            addJobToQueue(taskQueues_[currentPriority - 1], job);
            jobCurrentPriorityMap_[rootJobId] = currentPriority - 1;
            LOG("Lower the priority of job " + rootJobId.getString() + " to " + std::to_string(currentPriority - 1));
            return 1;
        } else if (currentPriority < job->priority){
            removeJobFromQueue(queue, rootJobId);
            int newPriority = currentPriority <= 0 ? job->priority : currentPriority + 1;
            addJobToQueue(taskQueues_[newPriority], job);
            jobCurrentPriorityMap_[rootJobId] = newPriority;
            LOG("Upgrade the priority of job " + rootJobId.getString() + " to " + std::to_string(newPriority));
            return 2;
        } else {
            return 0;
        }
    }

    // Assume mtx_ is taken
    JobContainerSP findJobById(std::deque<JobContainerSP> & queue, const Guid & rootJobId) {
        for (size_t i = 0; i < queue.size(); ++i) {
            if (queue[i]->rootJobId == rootJobId) {
                return queue[i];
            }
        }
        return nullptr;
    }

    // Assume mtx_ is taken
    void removeJobFromQueue(std::deque<JobContainerSP> & queue, const Guid & rootJobId) {
        for (size_t i = 0; i < queue.size(); ++i) {
            auto & job = queue[i];
            if (job->rootJobId == rootJobId) {
                queue.erase(queue.begin() + i);
                break;
            }
        }
    }

    // Assume mtx_ is taken
    void addJobToQueue(std::deque<JobContainerSP> & queue, const JobContainerSP& job) {
        queue.push_back(job);
    }

private:
    struct JobContainer {
        JobContainer(Guid rootJobId, long long sessionId, const string& remoteIP, int remotePort,
                     const string& userId, const string& type, const string& desc, int priority,
                     int quota, bool fixed = false)
            : rootJobId(rootJobId), sessionId(sessionId), remoteIP(remoteIP), remotePort(remotePort),
              userId(userId), type(type), desc(desc), priority(priority), quota(quota), quotaLeft(quota),
              fixed(fixed), cancelled(false), runningTaskCount(0), finishedTaskCount(0), totalTaskCount(0),
              firstTaskStartTime(LLONG_MIN), latestTaskStartTime(LLONG_MIN) {
            startTime = std::chrono::system_clock::now();
        }

        bool hasPendingTask() {
            return tasks.empty() == false;
        }

        bool empty() {
            return tasks.empty() && runningTaskCount<=0;
        }

        int cancel(){
        	cancelled = true;
        	if(tasks.empty())
        		return 0;

        	int cancelledTasks = 0;
        	std::deque<T> notCancelled;
        	for(auto& task : tasks){
        		if(task->isCancellable()){
        			task->cancel(false);
        			++cancelledTasks;
        		}
        		else
        			notCancelled.push_back(task);
        	}
        	tasks.swap(notCancelled);
        	return cancelledTasks;
        }

        const Guid rootJobId;
        const long long sessionId;
        const string remoteIP;
        const int remotePort;
        string userId;
        string type;
        string desc;
        const int priority;
        int quota;
        int quotaLeft;
        const bool fixed;
        bool cancelled;
        int runningTaskCount;
        std::deque<T> tasks;

		int finishedTaskCount; // contains finished and canceled task
		int totalTaskCount;
        // startTime: job was added queue. no task already run;
        std::chrono::system_clock::time_point startTime;
 		// firstTaskStartTime: first job task popped from queue, will start next.
		// latestTaskStartTime: latest job task popped from queue, will start next.
		long long firstTaskStartTime;
		long long latestTaskStartTime;
    };

    vector<std::deque<JobContainerSP>> taskQueues_;
    unordered_map<int, pair<T, JobContainerSP> > workerTaskAssignmentMap_; // maps workerId to task
    unordered_map<Guid, int, GuidHash> jobCurrentPriorityMap_; // maps rootJobId to current priority
    Mutex mtx_;
    ConditionalVariable cv_;
    int nelts_;
};

class QueryMonitor{
public:
    QueryMonitor(){enable_.store(true);}
    long long  addQuery(const string& script, const vector<DistributedCallSP>& tasks, long long sessionId, const string& usrId){
        if(tasks.size() > 0 && !tasks.back()->getRootJobId().isNull())
            return -1;
        if(!enable_.load())
            return index_;
        LockGuard<Mutex> _(&mutex_);
        queries_.insert(std::make_pair(index_++, Info(script, tasks, sessionId, usrId)));
        return index_ - 1;
    }

    void removeQuery(long long id){
        if(!enable_.load())
            return;
        LockGuard<Mutex> _(&mutex_);
        queries_.erase(id);
    }

    TableSP getQueryProcess() ;
    void enable() { enable_.store(true); }
    void disable() { enable_.store(false); }
    void clear() {
        LockGuard<Mutex> _(&mutex_);
        queries_.clear();
    }
private:
    struct Info {
        Info(const string& s, const vector<DistributedCallSP>& tasks, long long sessionId, const string& usrId) :
            script(s), startTime(Util::getEpochTime()), tasks(tasks), sessionId(sessionId), usrId(usrId){}
        string script;
        long long startTime;
        vector<DistributedCallSP> tasks;
        long long sessionId;
        string usrId;
    };

    unordered_map<long long, Info> queries_;
    long long index_ = 0;
    std::atomic_bool enable_;
    Mutex mutex_;
};

class StageExecutor {
public:
	virtual ~StageExecutor(){}
	virtual vector<DistributedCallSP> execute(Heap* heap, const vector<DistributedCallSP>& tasks) = 0;
	virtual vector<DistributedCallSP> execute(Heap* heap, const vector<DistributedCallSP>& tasks, const JobProperty& jobProp) = 0;
};

class StaticStageExecutor : public StageExecutor{
public:
	StaticStageExecutor(bool parallel, bool reExecuteOnOOM, bool trackJobs, bool resumeOnError = false, bool scheduleRemoteSite = true) :  parallel_(parallel),
		reExecuteOnOOM_(reExecuteOnOOM), trackJobs_(trackJobs),	resumeOnError_(resumeOnError), scheduleRemoteSite_(scheduleRemoteSite){}
	virtual ~StaticStageExecutor(){}
	virtual vector<DistributedCallSP> execute(Heap* heap, const vector<DistributedCallSP>& tasks);
	virtual vector<DistributedCallSP> execute(Heap* heap, const vector<DistributedCallSP>& tasks, const JobProperty& jobProp);
    void setForbidProbingGroupSize(bool flag){forbidProbingGroupSize_ = flag;}
    bool getForbidProbingGroupSize() const {return forbidProbingGroupSize_;}
    void setMonitorProcessAndMemory(bool flag, const string& script){monitorProcessAndMemory_ = flag; script_ = script;}
    bool getMonitorProcessAndMemory() const {return monitorProcessAndMemory_;}

private:
    void groupRemoteCalls(const vector<vector<DistributedCallSP>>& tasks, vector<DistributedCallSP>& groupedCalls, const ClusterNodesSP& clusterNodes, int groupSize);

    bool probingGroupSize(bool& groupCall, const vector<DistributedCallSP>& tasks, const ClusterNodesSP& clusterNodes, vector<vector<DistributedCallSP >>& siteCalls,
                            vector<std::pair<int,DistributedCallSP>>& needCheckTasks);
private:
    const int MIN_TASK_COUNT_FOR_PROBING_GROUP_SIZE = 4; // if all site's task count is less or equal than it, will group call directly and won't prob group size
    const int TASK_LIMIT_OF_A_GROUP = 1024; // the max task size of a group
	bool parallel_;
	bool reExecuteOnOOM_;
	bool trackJobs_;
	bool resumeOnError_;
	bool scheduleRemoteSite_;
    bool forbidProbingGroupSize_ = true;
    bool monitorProcessAndMemory_ = false;
    long long monitorId_ = 0;
    string script_;
};

class PipelineStageExecutor : public StageExecutor {
public:
	PipelineStageExecutor(vector<FunctionDefSP>& followingFunctors, bool trackJobs, int queueDepth = 2, int parallel = 1) : followingFunctors_(followingFunctors), trackJobs_(trackJobs),
		queueDepth_(queueDepth), parallel_(parallel){}
	virtual ~PipelineStageExecutor(){}
	virtual vector<DistributedCallSP> execute(Heap* heap, const vector<DistributedCallSP>& tasks);
	virtual vector<DistributedCallSP> execute(Heap* heap, const vector<DistributedCallSP>& tasks, const JobProperty& jobProp);

private:
	void parallelExecute(Heap* heap, vector<DistributedCallSP>& tasks);

private:
	vector<FunctionDefSP> followingFunctors_;
	bool trackJobs_;
	int queueDepth_;
	int parallel_;
};

#endif /* COMPUTINGMODEL_H_ */
