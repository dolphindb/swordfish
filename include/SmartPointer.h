/*
 * SmartPointer.h
 *
 *  Created on: Jun 24, 2012
 *      Author: dzhou
 */

#ifndef SMARTPOINTER_H_
#define SMARTPOINTER_H_

#include <atomic>
#include <type_traits>

class Counter {
public:
	Counter(void* p): p_(p), obj_(nullptr), count_(0){}
	int addRef(){ return atomic_fetch_add(&count_,1)+1;} //atomic operation
	int release(){return atomic_fetch_sub(&count_,1)-1;} //atomic operation
	int getCount() const {return count_.load();}
	void* p_;
	void* obj_;

private:
	std::atomic<int> count_;
};

class RefCountHelper {
public:
	virtual ~RefCountHelper() {}
	virtual void incRef(void* counter, void* pyObj) = 0;
	virtual void decRef(void* counter, void* pyObj) = 0;
	virtual void zeroHandler(void* counter) = 0;
public:
	static RefCountHelper* inst_;
};


template <class T>
class SmartPointer {
public:
	SmartPointer(T* p=0): counterP_(new Counter(p)){
		counterP_->addRef();
	}

	SmartPointer(T* p, Counter* counter): counterP_(counter){
		if (counterP_ == nullptr) {
			counterP_ = new Counter(p);
		}
		counterP_->addRef();
	}

	Counter* getCounter() {
		return counterP_;
	}

        Counter* getCounter() const {
                return counterP_;
	}

	SmartPointer(const SmartPointer<T>& sp){
		counterP_=sp.counterP_;
		counterP_->addRef();
	}

	template <class U>
	SmartPointer(const SmartPointer<U>& sp){
		static_assert(std::is_convertible<U*, T*>::value || std::is_base_of<U, T>::value, "U must be implicitly convertible to T or T must be a subclass of U");
		counterP_=sp.counterP_;
		counterP_->addRef();
	}

	T& operator *() const{
		return *((T*)counterP_->p_);
	}

	T* operator ->() const{
		return (T*)counterP_->p_;
	}

	T& operator =(const SmartPointer<T>& sp){
		if(this==&sp)
			return *((T*)counterP_->p_);

		Counter* tmp = sp.counterP_;
		if(counterP_ == tmp)
			return *((T*)tmp->p_);
		tmp->addRef();

		Counter* oldCounter = counterP_;
		counterP_= tmp;

		if(oldCounter->release()==0){
			if (oldCounter->obj_) {
				RefCountHelper::inst_->zeroHandler((void*)oldCounter);
			}
			delete (T*)oldCounter->p_;
			delete oldCounter;
		}
		return *((T*)tmp->p_);
	}

	bool operator ==(const SmartPointer<T>& sp) const{
		return counterP_ == sp.counterP_;
	}

	bool operator !=(const SmartPointer<T>& sp) const{
		return counterP_ != sp.counterP_;
	}

	void clear(){
		Counter* tmp = new Counter(0);
		tmp->addRef();

		Counter* oldCounter = counterP_;
		counterP_= tmp;

		if(oldCounter->release()==0){
            if (oldCounter->obj_) {
                RefCountHelper::inst_->zeroHandler((void*)oldCounter);
            }
			delete (T*)oldCounter->p_;
			delete oldCounter;
		}
	}

	bool isNull() const{
		return counterP_->p_ == 0;
	}

	int count() const{
		return counterP_->getCount();
	}

	T* get() const{
		return (T*)counterP_->p_;
	}

	~SmartPointer(){
		if(counterP_->release()==0){
            if (counterP_->obj_) {
                RefCountHelper::inst_->zeroHandler((void*)counterP_);
            }
			delete static_cast<T*>(counterP_->p_);
			delete counterP_;
			counterP_=0;
		}
	}

    T& operator =(T* p){
        if(p == counterP_->p_)
            return *p;

        if(counterP_->release()==0) {
            delete static_cast<T *>(counterP_->p_);
            counterP_->p_ = p;
            counterP_->addRef();
        }
        else{
            counterP_ = new Counter(p);
            counterP_->addRef();
        }
        return *p;
    }

private:
	template<class U> friend class SmartPointer;
	Counter* counterP_;
};

#endif /* SMARTPOINTER_H_ */
