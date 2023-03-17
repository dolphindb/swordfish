#pragma once

/*
* A concurrent hashmap implementation written in modern cpp(C++11).
*/
#include <sys/types.h>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <climits>
#include <cstring>
#include <atomic>
#include <string>
#include <algorithm>
#include <functional>
#include <utility>
#include <unordered_set>
#include <vector>

#include "Concurrent.h"
#include "HashmapUtil.h"

#ifndef BIT32
    typedef uint64_t word_t;
#else
    typedef uint32_t word_t;
#endif

struct OptLock
{
	std::atomic<word_t> typeVersionLockObsolete{0b100};

	bool isLocked(word_t version)
	{
		return ((version & 0b10) == 0b10);
	}

	word_t readLockOrRestart(bool &needRestart)
	{
		word_t version;
		version = typeVersionLockObsolete.load();
		if (isLocked(version))
		{
			//_mm_pause();
			needRestart = true;
		}
		return version;
	}

	void writeLockOrRestart(bool &needRestart)
	{
		word_t version;
		version = readLockOrRestart(needRestart);
		if (needRestart)
			return;

		upgradeToWriteLockOrRestart(version, needRestart);
		if (needRestart)
			return;
	}

	void upgradeToWriteLockOrRestart(word_t &version, bool &needRestart)
	{
		if (typeVersionLockObsolete.compare_exchange_strong(version, version + 0b10))
		{
			version = version + 0b10;
		}
		else
		{
			//_mm_pause();
			needRestart = true;
		}
	}

	void writeUnlock()
	{
		typeVersionLockObsolete.fetch_add(0b10);
	}

	bool isObsolete(word_t version)
	{
		return (version & 1) == 1;
	}

	void checkOrRestart(word_t startRead, bool &needRestart) const
	{
		readUnlockOrRestart(startRead, needRestart);
	}

	void readUnlockOrRestart(word_t startRead, bool &needRestart) const
	{
		needRestart = (startRead != typeVersionLockObsolete.load());
	}

	void writeUnlockObsolete()
	{
		typeVersionLockObsolete.fetch_add(0b11);
	}
};

enum class status
{
	OK,
	KEY_EXIST,
	KEY_NOT_EXIST,
	BUCKET_FULL,
	KEY_FOUND,
};

template <typename Key,
		typename T,
		typename Hash = murmur_hasher<Key>,
		typename KeyEqual = std::equal_to<Key>>
class ConcurrentFlatHashmapImpl
{
public:
	using key_type = Key;
	using value_type = T;
	using hash_type = Hash;
	using key_equal_type = KeyEqual;
	using key_value_type = std::pair<Key, T>;

	struct bucket : public OptLock
	{
		uint64_t bitmap;
		bucket() : bitmap(0) {}
		inline bool key_present(unsigned idx)
		{
			return bitmap & (1ULL << idx);
		}

		inline void set_key_present(unsigned idx)
		{
			bitmap |= 1ULL << idx;
		}

		inline void set_key_unpresent(unsigned idx)
		{
			bitmap &= ~(1ULL << idx);
		}

		size_t size() {
			return __builtin_popcountll(bitmap);
		}

        inline key_value_type* data() {
            return reinterpret_cast<key_value_type*>((char*)this + sizeof(struct bucket));
        }
#ifdef ABI
        key_value_type data_[0];
#else
		key_value_type data_[];
#endif
	};
	hash_type hasher;
	uint64_t bucket_entries;
	uint64_t capacity;
	uint64_t num_buckets;
	uint64_t bucket_size;
	uint64_t bucket_idx_hash_bits;
	uint64_t bucket_idx_mask;
	uint64_t storage_size;
	uint64_t probe_limit;
	uint64_t k;
	char *bucket_storage;
	int rehash_parallelism;

	inline struct bucket *get_bucket(int idx) { return (bucket *) (bucket_storage + idx * bucket_size); }

	ConcurrentFlatHashmapImpl(unsigned k, int rehash_parallelism) : bucket_entries(k), k(k), rehash_parallelism(rehash_parallelism)
	{
		bucket_entries = 2 * k;
		bucket_size = sizeof(bucket) + sizeof(key_value_type) * bucket_entries;
		num_buckets = std::ceil((1 << k) / (bucket_entries + 0.0));
		capacity = num_buckets * bucket_entries;
		storage_size = num_buckets * bucket_size;
		bucket_storage = (char*)mySmallAlloc(storage_size);
		auto t = std::log2(bucket_entries);
		bucket_idx_hash_bits = std::ceil(t);
		//printf("bucket_entries %d, bucket_idx_hash_bits %f %d\n", (int)bucket_entries, t, (int)bucket_idx_hash_bits);
		bucket_idx_mask = (1 << bucket_idx_hash_bits) - 1;
		if (bucket_storage == nullptr)
		{
			throw std::bad_alloc();
		}
		// initalize the OptLocks
		for (unsigned i = 0; i < num_buckets; ++i)
		{
			new ((char *)get_bucket(i)) bucket();
		}
	}

	ConcurrentFlatHashmapImpl * duplicate() {
		auto dup = new ConcurrentFlatHashmapImpl();
		dup->bucket_entries = this->bucket_entries;
		dup->bucket_size = this->bucket_size;
		dup->num_buckets = this->num_buckets;
		dup->capacity = this->capacity;
		dup->storage_size = this->storage_size;
		dup->bucket_idx_hash_bits = this->bucket_idx_hash_bits;
		dup->bucket_idx_mask = this->bucket_idx_mask;
		dup->bucket_storage = (char*)mySmallAlloc(storage_size);
		if (dup->bucket_storage == nullptr)
		{
			throw std::bad_alloc();
		}

		// initalize the OptLocks
		for (unsigned i = 0; i < num_buckets; ++i)
		{
			auto dup_bkt = dup->get_bucket(i);
			new ((char *)dup_bkt) bucket();
			auto bkt = get_bucket(i);
			for (unsigned j = 0; j < bucket_entries; ++j)
			{
				if (bkt->key_present(j))
				{
					dup_bkt->data()[j] = bkt->data()[j];
					dup_bkt->set_key_unpresent(j);
				}
			}
		}

		return dup;
	}

	ConcurrentFlatHashmapImpl(const ConcurrentFlatHashmapImpl &)=delete;
	ConcurrentFlatHashmapImpl& operator=(const ConcurrentFlatHashmapImpl &)=delete;

	~ConcurrentFlatHashmapImpl()
	{
		for (unsigned i = 0; i < num_buckets; ++i)
		{
			auto bkt = get_bucket(i);
			for (unsigned j = 0; j < bucket_entries; ++j)
			{
				if (bkt->key_present(j))
				{
					bkt->data()[j].first.~key_type();
					bkt->data()[j].second.~value_type();
					bkt->set_key_unpresent(j);
				}
			}
		}
		mySmallFree(bucket_storage);
	}

	inline size_t index_for_hash(size_t hash) {
		const uint64_t shift_amount = 64 - k;
		hash ^= hash >> shift_amount;
		return (11400714819323198485llu * hash) >> shift_amount;
	}

	inline unsigned locate_bucket(const key_type &key, unsigned &idx_within_bucket)
	{
		auto h = hasher(key);
		auto slot_idx = index_for_hash(h);
		auto bidx = slot_idx / bucket_entries;
		idx_within_bucket = slot_idx % bucket_entries;
		return bidx;
	}

	status search_probe_bucket(bucket *b, const key_type &key, unsigned start_idx, unsigned &res_idx)
	{
		unsigned i = start_idx;
		do
		{
			if (b->key_present(i) && b->data()[i].first == key)
			{
				res_idx = i;
				return status::KEY_EXIST;
			}
			++i;
		} while (i < bucket_entries);

		for (i = 0; i < start_idx; ++i) {
			if (b->key_present(i) && b->data()[i].first == key)
			{
				res_idx = i;
				return status::KEY_EXIST;
			}
		}
		return status::KEY_NOT_EXIST;
	}

	status insert_probe_bucket(bucket *b, const key_type &key, unsigned start_idx, unsigned &res_idx)
	{
		unsigned i = start_idx;
		int first_empty = -1;
		do
		{
			if (b->key_present(i))
			{
				if (b->data()[i].first == key)
				{
					res_idx = i;
					return status::KEY_EXIST;
				}
			}
			else
			{
				if (first_empty == -1)
					first_empty = i;
			}
			++i;
		} while (i != bucket_entries);

		i = 0;
		do
		{
			if (b->key_present(i))
			{
				if (b->data()[i].first == key)
				{
					res_idx = i;
					return status::KEY_EXIST;
				}
			}
			else
			{
				if (first_empty == -1)
					first_empty = i;
			}
			++i;
		} while (i < start_idx);

		if (first_empty == -1)
		{
			return status::BUCKET_FULL;
		}
		else
		{
			res_idx = first_empty;
			return status::KEY_NOT_EXIST;
		}
	}

	status insert(const key_type &key, const value_type &value)
	{
		unsigned idx_within_bucket;
		auto bidx = locate_bucket(key, idx_within_bucket);
		auto bkt = get_bucket(bidx);
		restart:
		bool need_restart = false;
		auto version = bkt->readLockOrRestart(need_restart);
		if (need_restart)
			goto restart;
		unsigned put_idx = 0;
		auto res = insert_probe_bucket(bkt, key, idx_within_bucket, put_idx);
		if (res == status::KEY_NOT_EXIST)
		{
			bkt->upgradeToWriteLockOrRestart(version, need_restart);
			if (need_restart)
				goto restart;
            new ((char*)&bkt->data()[put_idx]) key_value_type(key, value);
			bkt->set_key_present(put_idx);
			bkt->writeUnlock();
			return status::OK;
		}

		bkt->readUnlockOrRestart(version, need_restart);
		if (need_restart)
			goto restart;
		return res;
	}


	status unsafe_insert(const key_type &key, const value_type &value)
	{
		unsigned idx_within_bucket;
		auto bidx = locate_bucket(key, idx_within_bucket);
		auto bkt = get_bucket(bidx);
		unsigned put_idx = 0;
		auto res = insert_probe_bucket(bkt, key, idx_within_bucket, put_idx);
		if (res == status::KEY_NOT_EXIST)
		{
            new ((char*)&bkt->data()[put_idx]) key_value_type(key, value);
			bkt->set_key_present(put_idx);
			return status::OK;
		}
		return res;
	}

	status upsert(const key_type &key, const value_type &value)
	{
		unsigned idx_within_bucket;
		auto bidx = locate_bucket(key, idx_within_bucket);
		auto bkt = get_bucket(bidx);
		restart:
		bool need_restart = false;
		auto version = bkt->readLockOrRestart(need_restart);
		if (need_restart)
			goto restart;
		unsigned put_idx = 0;
		auto res = insert_probe_bucket(bkt, key, idx_within_bucket, put_idx);
		if (res == status::KEY_NOT_EXIST)
		{
			bkt->upgradeToWriteLockOrRestart(version, need_restart);
			if (need_restart)
				goto restart;
            new ((char*)&bkt->data()[put_idx]) key_value_type(key, value);
			bkt->set_key_present(put_idx);
			bkt->writeUnlock();
			return status::OK;
		} else if (res == status::KEY_EXIST) {
			bkt->upgradeToWriteLockOrRestart(version, need_restart);
			if (need_restart)
				goto restart;
			bkt->data()[put_idx].second = value;
			bkt->writeUnlock();
			return status::OK;
		}
		bkt->readUnlockOrRestart(version, need_restart);
		if (need_restart)
			goto restart;
		return res;
	}

	status upsert(const key_type & key, std::function<void(value_type &)> updater, const value_type & default_value) {
		unsigned idx_within_bucket;
		auto bidx = locate_bucket(key, idx_within_bucket);
		auto bkt = get_bucket(bidx);
		restart:
		bool need_restart = false;
		auto version = bkt->readLockOrRestart(need_restart);
		if (need_restart)
			goto restart;
		unsigned put_idx = 0;
		auto res = insert_probe_bucket(bkt, key, idx_within_bucket, put_idx);
		if (res == status::KEY_NOT_EXIST)
		{
			bkt->upgradeToWriteLockOrRestart(version, need_restart);
			if (need_restart)
				goto restart;
            new ((char*)&bkt->data()[put_idx]) key_value_type(key, default_value);
			bkt->set_key_present(put_idx);
			bkt->writeUnlock();
			return status::OK;
		} else if (res == status::KEY_EXIST) {
			bkt->upgradeToWriteLockOrRestart(version, need_restart);
			if (need_restart)
				goto restart;
			updater(bkt->data()[put_idx].second);
			bkt->writeUnlock();
			return status::OK;
		}
		bkt->readUnlockOrRestart(version, need_restart);
		if (need_restart)
			goto restart;
		return res;
	}

	status erase(const key_type &key)
	{
		unsigned idx_within_bucket;
		auto bidx = locate_bucket(key, idx_within_bucket);
		auto bkt = get_bucket(bidx);
		restart:
		bool need_restart = false;
		auto version = bkt->readLockOrRestart(need_restart);
		if (need_restart)
			goto restart;
		unsigned delete_idx = 0;
		auto res = search_probe_bucket(bkt, key, idx_within_bucket, delete_idx);
		if (res == status::KEY_EXIST)
		{
			bkt->upgradeToWriteLockOrRestart(version, need_restart);
			if (need_restart)
				goto restart;
            (bkt->data() + delete_idx)->~key_value_type();
			bkt->set_key_unpresent(delete_idx);
			bkt->writeUnlock();
			return status::OK;
		}

		bkt->readUnlockOrRestart(version, need_restart);
		if (need_restart)
			goto restart;
		return res;
	}

	status find(const key_type &key, value_type &value)
	{
		unsigned idx_within_bucket;
		auto bidx = locate_bucket(key, idx_within_bucket);
		auto bkt = get_bucket(bidx);
		restart:
		bool need_restart = false;
		auto version = bkt->readLockOrRestart(need_restart);
		if (need_restart)
			goto restart;
		unsigned get_idx = 0;
		auto res = search_probe_bucket(bkt, key, idx_within_bucket, get_idx);
		if (res == status::KEY_EXIST)
		{
			value = bkt->data()[get_idx].second;
		}
		bkt->readUnlockOrRestart(version, need_restart);
		if (need_restart)
			goto restart;
		return res;
	}

	void for_each(std::function<bool(const std::pair<Key, T> &)> processor) {
		for (unsigned i = 0; i < num_buckets; ++i)
		{
			auto bkt = get_bucket(i);

			restart:
			std::vector<std::pair<Key, T>> entries;
			bool need_restart = false;
			auto version = bkt->readLockOrRestart(need_restart);
			if (need_restart)
				goto restart;
			for (unsigned j = 0; j < bucket_entries; ++j)
			{
				if (bkt->key_present(j))
				{
					entries.push_back(bkt->data()[j]);
				}
			}
			bkt->readUnlockOrRestart(version, need_restart);
			if (need_restart)
				goto restart;
			bool early_return = false;
			for (unsigned j = 0; j < entries.size() && early_return == false; ++j) {
				early_return = processor(entries[j]);
			}
			if (early_return)
				return;
		}
	}


	template <typename F>
	void unsafe_iterate_entries(F processor)
	{
		bool should_exit = false;
		for (unsigned i = 0; i < num_buckets && should_exit == false; ++i)
		{
			auto bkt = get_bucket(i);
			for (unsigned j = 0; j < bucket_entries && should_exit == false; ++j)
			{
				if (bkt->key_present(j))
				{
					should_exit = processor(bkt->data()[j]);
				}
			}
		}
	}

    class IteratorRunnable : public Runnable {
    public:
        IteratorRunnable() : start_bucket_idx(0), n(0){}
        IteratorRunnable(std::function<void(int,int)> func, int start_bucket_idx, int n) : func(func), start_bucket_idx(start_bucket_idx), n(n) {}

        void run() override {
            func(start_bucket_idx, n);
        }
    private:
        std::function<void(int,int)> func;
        int start_bucket_idx;
        int n;
    };
    typedef SmartPointer<IteratorRunnable> IteratorRunnableSP;

	template <typename F>
	void unsafe_iterate_entries_in_parallel(F processor)
	{
		int worker_num = rehash_parallelism;
		auto worker_func = [this, processor](int start_bucket_idx, int n) {
			bool should_exit = false;
			for (unsigned i = 0; i < (unsigned int)n && should_exit == false; ++i)
			{
				auto bkt = get_bucket(start_bucket_idx + i);
				for (unsigned j = 0; j < bucket_entries && should_exit == false; ++j)
				{
					if (bkt->key_present(j))
					{
						should_exit = processor(bkt->data()[j]);
					}
				}
			}
		};
		std::vector<ThreadSP> workers;
		int work_size = num_buckets / worker_num;
		for (int i = 0; i < worker_num; ++i) {
			int read_work_size = std::min(work_size, (int)num_buckets - i * work_size);
            ThreadSP worker = new Thread(new IteratorRunnable(worker_func, i * work_size, read_work_size));
            workers.emplace_back(worker);
            worker->start();
		}

		for (int i = 0; i < worker_num; ++i) {
			workers[i]->join();
		}
	}


	size_t size() {
		size_t s = 0;
		for (unsigned i = 0; i < num_buckets; ++i) {
			auto bkt = get_bucket(i);
			restart:
			bool need_restart = false;
			size_t bsize = 0;
			auto version = bkt->readLockOrRestart(need_restart);
			//printf("size version %d %d\n", i, (int)version);
			if (need_restart)
				goto restart;
			bsize = bkt->size();
			bkt->readUnlockOrRestart(version, need_restart);
			if (need_restart)
				goto restart;
			s += bsize;
		}
		return s;
	}

	status find(const key_type &key, value_type **value_ptr) {
		unsigned idx_within_bucket;
		auto bidx = locate_bucket(key, idx_within_bucket);
		auto bkt = get_bucket(bidx);
		restart:
		bool need_restart = false;
		auto version = bkt->readLockOrRestart(need_restart);
		if (need_restart)
			goto restart;
		unsigned get_idx = 0;
		auto res = search_probe_bucket(bkt, key, idx_within_bucket, get_idx);
		if (res == status::KEY_EXIST) {
			*value_ptr = &(bkt->data()[get_idx].second);
		}
		bkt->readUnlockOrRestart(version, need_restart);
		if (need_restart)
			goto restart;
		return res;
	}

private:
	ConcurrentFlatHashmapImpl() : bucket_entries(0), capacity(0), num_buckets(0), bucket_size(0), bucket_idx_hash_bits(0), bucket_idx_mask(0),
		storage_size(0), probe_limit(0), k(0), bucket_storage(0), rehash_parallelism(0){}
};

template <typename Key,
		typename T,
		typename Hash,
		typename KeyEqual,
		bool ReaderOrWriter>
class thread_epoch;
template <typename Key,
		typename T,
		typename Hash,
		typename KeyEqual,
		bool ReaderOrWriter>
class epoch_manager;
template <typename Key,
		typename T,
		typename Hash,
		typename KeyEqual,
		bool ReaderOrWriter>
class synchronization_manager;

template <typename Key,
		typename T,
		typename Hash,
		typename KeyEqual,
		bool ReaderOrWriter>
class epoch_manager
{
public:
	using thread_epoch_type = thread_epoch<Key, T, Hash, KeyEqual, ReaderOrWriter>;
	using sync_manager_type = synchronization_manager<Key, T, Hash, KeyEqual, ReaderOrWriter>;
	epoch_manager()
	{
		sync_manager_type::add_all_epoch_to_one_epoch_manager(this);
	}
	~epoch_manager()
	{
		sync_manager_type::delete_manager(this);
	}
	void add_epoch(thread_epoch_type *te)
	{
		LockGuard<Mutex> g(&mtx);
		thread_epoches.push_back(te);
	}

	void delete_epoch(thread_epoch_type *te)
	{
		LockGuard<Mutex> g(&mtx);
		restart:
		for (unsigned i = 0; i < thread_epoches.size(); ++i)
		{
			if (thread_epoches[i] == te)
			{
				thread_epoches.erase(thread_epoches.begin() + i);
				goto restart;
			}
		}
	}

	void iterate_thread_epoches(std::function<void(thread_epoch_type *)> processor)
	{
		LockGuard<Mutex> g(&mtx);
		for (unsigned i = 0; i < thread_epoches.size(); ++i)
		{
			processor(thread_epoches[i]);
		}
	}

	// Assuming V won't be obtained by any new threads after calling this function,
	// busy-wait until no other threads have the same value stored in thread_epoch object as V.
	void spin_until_no_ref(uintptr_t V)
	{
		while (true)
		{
			bool might_have_refs = false;
			iterate_thread_epoches(
					[&might_have_refs,
							V](thread_epoch_type *te) {
						auto tev = te->get_value();
						if (te->active() && (tev == 0 || te->get_value() == V))
						{
							might_have_refs = true;
						}
					});
			if (might_have_refs == false)
				break;
			Thread::yield();
		}
	}

private:
	std::vector<thread_epoch_type *> thread_epoches;
	Mutex mtx;
};

template <typename Key,
		typename T,
		typename Hash,
		typename KeyEqual,
		bool ReaderOrWriter>
class synchronization_manager
{
public:
	using thread_epoch_type = thread_epoch<Key, T, Hash, KeyEqual, ReaderOrWriter>;
	using epoch_manager_type = epoch_manager<Key, T, Hash, KeyEqual, ReaderOrWriter>;

private:
	static std::unordered_set<epoch_manager_type *> & get_epoch_mgrs() {
		static std::unordered_set<epoch_manager_type *> epoch_mgrs;
		return epoch_mgrs;
	}

	static std::unordered_set<thread_epoch_type *> & get_epochs() {
		static std::unordered_set<thread_epoch_type *> epochs;
		return epochs;
	}
public:

	static Mutex* get_mutex() {
		static Mutex mtx;
		return &mtx;
	}

	static void add_epoch(thread_epoch_type *epoch)
	{
		LockGuard<Mutex> g(get_mutex());
		auto & epochs = get_epochs();
		if (epochs.find(epoch) == epochs.end())
		{
			epochs.insert(epoch);
		}
	}

	static void delete_epoch(thread_epoch_type *e)
	{
		LockGuard<Mutex> g(get_mutex());
		auto & epochs = get_epochs();
		epochs.erase(e);
		auto & epoch_mgrs = get_epoch_mgrs();
		for (auto epoch_mgr : epoch_mgrs)
		{
			epoch_mgr->delete_epoch(e);
		}
	}

	static void add_manager(epoch_manager_type *epoch_mgr)
	{
		LockGuard<Mutex> g(get_mutex());
		auto & epoch_mgrs = get_epoch_mgrs();
		if (epoch_mgrs.find(epoch_mgr) == epoch_mgrs.end())
		{
			epoch_mgrs.insert(epoch_mgr);
		}
	}

	static void delete_manager(epoch_manager_type *epoch_mgr)
	{
		LockGuard<Mutex> g(get_mutex());
		auto & epoch_mgrs = get_epoch_mgrs();
		epoch_mgrs.erase(epoch_mgr);
	}

	static bool epoch_manager_exists(epoch_manager_type *epoch_mgr)
	{
		LockGuard<Mutex> g(get_mutex());
		auto & epoch_mgrs = get_epoch_mgrs();
		return epoch_mgrs.find(epoch_mgr) != epoch_mgrs.end();
	}

	static void add_all_epoch_to_one_epoch_manager(epoch_manager_type *epoch_mgr)
	{
		LockGuard<Mutex> g(get_mutex());
		auto & epoch_mgrs = get_epoch_mgrs();
		auto & epochs = get_epochs();
		if (epoch_mgrs.find(epoch_mgr) == epoch_mgrs.end())
		{
			epoch_mgrs.insert(epoch_mgr);
		}
		for (auto e : epochs)
		{
			epoch_mgr->add_epoch(e);
		}
	}

	static void add_one_epoch_to_all_epoch_manager(thread_epoch_type *e)
	{
		LockGuard<Mutex> g(get_mutex());
		auto & epoch_mgrs = get_epoch_mgrs();
		auto & epochs = get_epochs();
		if (epochs.find(e) == epochs.end())
		{
			epochs.insert(e);
		}
		for (auto epoch_mgr : epoch_mgrs)
		{
			epoch_mgr->add_epoch(e);
		}
	}
};


#if defined(__amd64__) || defined(__amd64__) || defined(__x86_64__) || defined(__x86_64) || defined(_M_AMD64) || defined(i386) || defined(__i386) || defined(__i386__) || defined(_X86_)
#define MEMORY_MODEL_TSO
#endif

#ifdef MEMORY_MODEL_TSO

template <typename Key,
		typename T,
		typename Hash,
		typename KeyEqual,
		bool ReaderOrWriter>
class thread_epoch
{
public:
	using sync_manager_type = synchronization_manager<Key, T, Hash, KeyEqual, ReaderOrWriter>;
	thread_epoch() : v(0)
	{
		sync_manager_type::add_one_epoch_to_all_epoch_manager(this);
	}

	uintptr_t set_value(uintptr_t new_v)
	{
		v = new_v | active();
		return new_v;
	}

	uintptr_t get_value() { return v & (~1); }

	~thread_epoch()
	{
		sync_manager_type::delete_epoch(this);
	}

	void enter() { v |= 1; }

	void leave() { v = 0; }

	bool active() { return v & 1; }

private:
	uintptr_t v;
};

#else

template <typename Key,
	  typename T,
	  typename Hash,
	  typename KeyEqual,
	  bool ReaderOrWriter>
class thread_epoch
{
  public:
    using sync_manager_type = synchronization_manager<Key, T, Hash, KeyEqual, ReaderOrWriter>;
    thread_epoch() : v(0)
    {
        sync_manager_type::add_one_epoch_to_all_epoch_manager(this);
    }

    uintptr_t set_value(uintptr_t new_v)
    {
        v.store(new_v | active());// new_v | active();
        return new_v;
    }

    uintptr_t get_value() { return v & (~1); }

    ~thread_epoch()
    {
        sync_manager_type::delete_epoch(this);
    }

    void enter() { v.fetch_or(1); }

    void leave() { v.store(0); }

    bool active() { return v.load() & 1; }

  private:
    std::atomic<uintptr_t> v;
};
#endif

// RAII-style enter and leave
template<typename epoch_type>
class epoch_guard
{
public:
	epoch_guard(epoch_type &epoch) : epoch(epoch) {
		epoch.enter();
#ifdef MEMORY_MODEL_TSO
		// Typical epoch_guard usage pattern:
		//  1. epoch_guard g(epoch) which translates to epoch.enter();
		//  2. v = load from global variable
		//  3. epoch.set_value(v)

		// It 's possible the load of v is reordered before epoch.enter() which might cause following race condition:
		//  One thread X loaded v from global variable into register. The load is ordered before epoch.enter() which sets the active bit.
		//  GC threads skip the epoch of X since its active bit is off.
		//  GC threads physically delete the data structure pointed by v which is now only in the register of thread X.
		//  X then sets the active bit of its epoch and stores v into epoch.
		//  X goes on and accesses the deleted data structure which causes SEGFAULT!!!

		// This important fence prevents such reordering (i.e. Store/Load reordering on TSO memory model(e.g. x86)).
		std::atomic_thread_fence(std::memory_order_seq_cst);
#endif
	}
	~epoch_guard() { epoch.leave(); }

private:
	epoch_type &epoch;
};

enum class rehash_status
{
	NO_REHASH,
	PRE_REHASH,
	REHASHING
};

template <typename Key,
		typename T,
		typename Hash = murmur_hasher<Key>,
		typename KeyEqual = std::equal_to<Key>>
class ConcurrentFlatHashmap
{
public:
	using key_type = Key;
	using mapped_type = T;
	using hash_type = Hash;
	using key_equal_type = KeyEqual;
	using key_value_type = std::pair<Key, T>;
	using hash_impl_type = ConcurrentFlatHashmapImpl<Key, T, Hash, KeyEqual>;
	static constexpr int initial_k = 7;
	int rehash_parallelism;
	ConcurrentFlatHashmap(int rehash_parallelism=1) : rehash_parallelism(rehash_parallelism), k(initial_k), hash_impl(new hash_impl_type(initial_k, rehash_parallelism)) {}

	~ConcurrentFlatHashmap() {
		if (hash_impl.load()) {
			delete hash_impl.load();
			hash_impl.store(nullptr);
		}
	}

	ConcurrentFlatHashmap & operator=(const ConcurrentFlatHashmap & rhs) {
		if (this != &rhs) {
			assert(is_no_rehash());
			auto rhs_hash_impl_dup = rhs.hash_impl.load()->duplicate();
			auto tmp_hash_impl = hash_impl.load();
			hash_impl.store(rhs_hash_impl_dup);
			delete tmp_hash_impl;
		}
		return *this;
	}

	bool find(const key_type &key, mapped_type &recv)
	{
		reader_epoch_guard_type rg(reader_epoch);
		hash_impl_type *himpl = (hash_impl_type *)reader_epoch.set_value((uintptr_t)hash_impl.load());
		auto res = himpl->find(key, recv);
		if (res == status::KEY_EXIST)
		{
			return true;
		}
		return false;
	}

    bool find(const key_type &key, mapped_type** recv)
    {
        reader_epoch_guard_type rg(reader_epoch);
        hash_impl_type *himpl = (hash_impl_type *)reader_epoch.set_value((uintptr_t)hash_impl.load());
        auto res = himpl->find(key, recv);
        if (res == status::KEY_EXIST)
        {
            return true;
        }
        return false;
    }

	void for_each(std::function<bool (const std::pair<Key, T> &)> processor) {
		reader_epoch_guard_type rg(reader_epoch);
		hash_impl_type *himpl = (hash_impl_type *)reader_epoch.set_value((uintptr_t)hash_impl.load());
		himpl->for_each(processor);
	}

	// Returns a renference to the value corresponds to the given key.
	// If the key is not in the table, a key-value pair with the default
	// value of the mapped_type is inserted into the table.
	// !!!This is unsafe to concurrnet read & write operations
	// as it returns value reference to the caller!!!!.
	// Use this at your discretion.
	mapped_type &operator[](const key_type &key)
	{
		{
			reader_epoch_guard_type rg(reader_epoch);
			hash_impl_type *himpl = (hash_impl_type *)writer_epoch.set_value((uintptr_t)hash_impl.load());
			mapped_type *value_ptr;
			auto res = himpl->find(key, &value_ptr);
			if (res == status::KEY_EXIST)
			{
				return *value_ptr;
			}
		}
		insert(key, mapped_type());
		return operator[](key);
	}

	// insert key-value pair {key, val} into the table.
	// return true if the insertion is successful.
	// return false if key-value pair is already in the table.
	bool insert(const key_type &key, const mapped_type &val)
	{
		wait_until_no_rehash();
		writer_epoch_guard_type wg(writer_epoch);
		hash_impl_type *himpl = (hash_impl_type *)writer_epoch.set_value((uintptr_t)hash_impl.load());
		status res = himpl->insert(key, val);
		if (res == status::OK) {
			return true;
		} else if (res == status::KEY_EXIST) {
			return false;
		} else { // res == status::BUCKET_FULL
			writer_epoch.leave();
			rehash([this](hash_impl_type* old) { return this->do_rehash(old);});
			return insert(key, val);
		}
	}

	void upsert(const Key & key, std::function<void(T&)> updater, const mapped_type & default_value) {
		wait_until_no_rehash();
		writer_epoch_guard_type wg(writer_epoch);
		hash_impl_type *himpl = (hash_impl_type *)writer_epoch.set_value((uintptr_t)hash_impl.load());
		status res = himpl->upsert(key, updater, default_value);
		if (res == status::OK) {
			return;
		} else { // res == status::BUCKET_FULL
			assert(res == status::BUCKET_FULL);
			writer_epoch.leave();
			rehash([this](hash_impl_type* old) { return this->do_rehash(old);});
			upsert(key, updater, default_value);
		}
	}

	// upsert key-value pair {key, val} into the table.
	// return true if the insertion is successful.
	bool upsert(const key_type &key, const mapped_type &val)
	{
		wait_until_no_rehash();
		writer_epoch_guard_type wg(writer_epoch);
		hash_impl_type *himpl = (hash_impl_type *)writer_epoch.set_value((uintptr_t)hash_impl.load());
		status res = himpl->upsert(key, val);
		if (res == status::OK) {
			return true;
		} else { // res == status::BUCKET_FULL
			assert(res == status::BUCKET_FULL);
			writer_epoch.leave();
			rehash([this](hash_impl_type* old) { return this->do_rehash(old);});
			return upsert(key, val);
		}
	}

	//Return the approximate size of table.
	size_t size() const noexcept {
		// O(N)
		reader_epoch_guard_type rg(reader_epoch);
		hash_impl_type *himpl = (hash_impl_type *)reader_epoch.set_value((uintptr_t)hash_impl.load());
		return himpl->size();
	}

	// Erase the entry with key from the hashtable
	// return true if the deletion is successful,
	// false otherwise.
	bool erase(const key_type &key)
	{
		wait_until_no_rehash();
		writer_epoch_guard_type wg(writer_epoch);
		hash_impl_type *himpl = (hash_impl_type *)writer_epoch.set_value((uintptr_t)hash_impl.load());
		auto s = himpl->erase(key);
		return s == status::OK;
	}

	void clear()
	{
		auto do_clear = [this](hash_impl_type *old_hash_impl) -> hash_impl_type* {
			auto new_k = initial_k;
			auto new_hash_impl = new hash_impl_type(new_k, rehash_parallelism);
			return new_hash_impl;
		};

		while (!rehash(do_clear));
	}

	ConditionalVariable cv;
	Mutex cvm;
	int wait_until_no_rehash()
	{
		auto rs = rehash_state.load();
		while (!is_no_rehash(rs))
		{
			LockGuard<Mutex> lk(&cvm);
			cv.wait(cvm, 10);
			rs = rehash_state.load();
		}
		return rs;
	}

	hash_impl_type *do_rehash(hash_impl_type *old_hash_impl)
	{
		int new_k = k;
		std::unique_ptr<hash_impl_type> ret(nullptr);

		while (true)
		{
			++new_k;
			//printf("old_k %d new_k %d\n", k, new_k);
			auto new_hash_impl = new hash_impl_type(new_k, rehash_parallelism);
			ret.reset(new_hash_impl);

			std::atomic<bool> succeeded{true};
			if (rehash_parallelism != 1) {
				old_hash_impl->unsafe_iterate_entries_in_parallel([&new_hash_impl, &succeeded](const typename hash_impl_type::key_value_type &kv) -> bool {
					status res = new_hash_impl->insert(kv.first, kv.second);
					if (res != status::OK)
					{
						assert(res == status::BUCKET_FULL);
						succeeded = false;
						return true; // early exit
					}
					return false;
				});
			} else {
				old_hash_impl->unsafe_iterate_entries([&new_hash_impl, &succeeded](const typename hash_impl_type::key_value_type &kv) -> bool {
					status res = new_hash_impl->unsafe_insert(kv.first, kv.second);
					if (res != status::OK)
					{
						assert(res == status::BUCKET_FULL);
						succeeded = false;
						return true; // early exit
					}
					return false;
				});
			}

			if (succeeded == true)
			{
				break;
			}
		}
		k = new_k;
		return ret.release();
	}


	bool rehash(std::function<hash_impl_type*(hash_impl_type* old)> rehash_func)
	{
		int rs = wait_until_no_rehash();
		if (rehash_state.compare_exchange_strong(rs, become_pre_rehash(rs)))
		{
			// only one thread reaches here and becomes the rehasher
			auto old_hash_impl = hash_impl.load();

			// wait for existing writers on old_hash_impl to exit
			writer_manager.spin_until_no_ref((uintptr_t)old_hash_impl);

			rehash_state.store(become_rehashing(rs));

			auto new_hash_impl = rehash_func(old_hash_impl);

			// redirect new readers to new_hash_impl
			hash_impl.store(new_hash_impl);

			std::atomic_thread_fence(std::memory_order_seq_cst);

			// wait for old readers on old_hash_impl to exit
			reader_manager.spin_until_no_ref((uintptr_t)old_hash_impl);

			cvm.lock();
			// release the merge state to resume writability
			rehash_state.store(become_next_no_rehash(rs));
			cvm.unlock();
			cv.notifyAll();

			// delete old hash impl
			delete old_hash_impl;

			return true;
		}
		else
		{
			wait_until_no_rehash();
			return false;
		}
	}

	bool is_pre_rehash(int rs)
	{
		int state = rs % 3;
		return state == (int)rehash_status::PRE_REHASH;
	}

	bool is_rehashing()
	{
		return is_rehashing(rehash_state.load());
	}

	bool is_rehashing(int rs)
	{
		int state = rs % 3;
		return state == (int)rehash_status::REHASHING;
	}

	bool is_no_rehash(int rs)
	{
		int state = rs % 3;
		return state == (int)rehash_status::NO_REHASH;
	}

	bool is_no_rehash()
	{
		return is_no_rehash(rehash_state.load());
	}

	int become_pre_rehash(int rs)
	{
		assert(rs % 3 == 0);
		return rs + 1;
	}

	int become_rehashing(int rs)
	{
		assert(rs % 3 == 0);
		return rs + 2;
	}

	int become_next_no_rehash(int rs)
	{
		assert(rs % 3 == 0);
		return rs + 3;
	}

	/* modified only by rehash leader */
	int k;
	std::atomic<int> rehash_state{0};
	std::atomic<hash_impl_type *> hash_impl{nullptr};
	using reader_epoch_manager_type = epoch_manager<Key, T, Hash, KeyEqual, false>;
	using writer_epoch_manager_type = epoch_manager<Key, T, Hash, KeyEqual, true>;
	using reader_thread_epoch_type = thread_epoch<Key, T, Hash, KeyEqual, false>;
	using writer_thread_epoch_type = thread_epoch<Key, T, Hash, KeyEqual, true>;
	using reader_epoch_guard_type = epoch_guard<reader_thread_epoch_type>;
	using writer_epoch_guard_type = epoch_guard<writer_thread_epoch_type>;
	reader_epoch_manager_type reader_manager;
	writer_epoch_manager_type writer_manager;
	static thread_local reader_thread_epoch_type reader_epoch;
	static thread_local writer_thread_epoch_type writer_epoch;
};

template <typename Key,
		typename T,
		typename Hash,
		typename KeyEqual>
thread_local typename ConcurrentFlatHashmap<Key, T, Hash, KeyEqual>::reader_thread_epoch_type ConcurrentFlatHashmap<Key, T, Hash, KeyEqual>::reader_epoch;

template <typename Key,
		typename T,
		typename Hash,
		typename KeyEqual>
thread_local typename ConcurrentFlatHashmap<Key, T, Hash, KeyEqual>::writer_thread_epoch_type ConcurrentFlatHashmap<Key, T, Hash, KeyEqual>::writer_epoch;

// TODO: to further increase concurrency, use this hash-partitioned version
template <typename Key,
		typename T,
		typename Hash = murmur_hasher<Key>,
		typename KeyEqual = std::equal_to<Key>,
		unsigned PartitionNum = 16>
class ConcurrentPartitionedFlatHashmap {
public:
	using key_type = Key;
	using mapped_type = T;
	using hash_type = Hash;
	using key_equal_type = KeyEqual;
	using key_value_type = std::pair<Key, T>;
	using hash_impl_type = ConcurrentFlatHashmap<Key, T, Hash, KeyEqual>;
	hash_impl_type partitions[PartitionNum];

	hash_impl_type& get_partition(const key_type & key) {
		hash_type hasher;
		auto h = hasher(key);
		return partitions[h % PartitionNum];
	}

	bool find(const key_type &key, mapped_type &recv)
	{
		return get_partition(key).find(key, recv);
	}

	// Returns a renference to the value corresponds to the given key.
	// If the key is not in the table, a key-value pair with the default
	// value of the mapped_type is inserted into the table.
	// !!!This is unsafe to concurrnet read & write operations
	// as it returns value reference to the caller!!!!.
	// Use this at your discretion.
	mapped_type &operator[](const key_type &key)
	{
		return get_partition(key).operator[](key);
	}

	// insert key-value pair {key, val} into the table.
	// return true if the insertion is successful.
	// return false if key-value pair is already in the table.
	bool insert(const key_type &key, const mapped_type &val)
	{
		return get_partition(key).insert(key, val);
	}


	// upsert key-value pair {key, val} into the table.
	// return true if the insertion is successful.
	bool upsert(const key_type &key, const mapped_type &val)
	{
		return get_partition(key).upsert(key, val);
	}

	//Return the approximate size of table.
	size_t size() const noexcept {
		// O(N)
		size_t s = 0;
		for (unsigned i = 0; i < PartitionNum; ++i) {
			s += partitions[i].size();
		}
		return s;
	}

	// Erase the entry with key from the hashtable
	// return true if the deletion is successful,
	// false otherwise.
	bool erase(const key_type &key)
	{
		return get_partition(key).erase(key);
	}

	void clear()
	{
		for (unsigned i = 0; i < PartitionNum; ++i) {
			partitions[i].clear();
		}
	}
};

