#pragma once

#include <mutex>
#include <deque>


template<typename T>
class ThreadSafeQueue {

public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue<T>&) = delete;
    ~ThreadSafeQueue();
        
    T& front();
    T& back();
    T pop_front();
    T pop_back();
    void push_back(const T& item);
    void push_front(const T& item);
    bool empty();
    size_t count();
    void clear();
    

private:
    std::deque<T> queue;
    std::mutex mutex;

};
