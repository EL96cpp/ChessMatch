#pragma once 

#include <list>
#include <mutex>


template<typename T>
class ThreadSafeList {

public:
    ThreadSafeList() = default;
    ThreadSafeList(const ThreadSafeList<T>&) = delete;
    ~ThreadSafeList();
 
    
private:
    std::list<T> list;
    std::mutex mutex;

};
