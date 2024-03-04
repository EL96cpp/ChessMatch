#pragma once

#include <mutex>
#include <deque>
#include <condition_variable>
		

template<typename T>
class ThreadSafeQueue {

public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue<T>&) = delete;
        
    T& front();
    T& back();
    T pop_front();
    T pop_back();
    void push_back(const T& item);
    void push_front(const T& item);
    bool empty();
    size_t count();
    void clear();
    void wait();


private:
    std::deque<T> deque;
    std::mutex blocking_mutex;
    std::mutex cv_mutex;
    std::condition_variable condition_variable;

};
