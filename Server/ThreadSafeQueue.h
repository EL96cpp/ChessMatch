#pragma once

#include <mutex>
#include <deque>
#include <condition_variable>
#include <memory>

#include "Message.h"
#include "ClientConnection.h"

        
template<typename T>
class ThreadSafeQueue {

public:
    ThreadSafeQueue() {}

    ThreadSafeQueue(const ThreadSafeQueue<T>&) = delete;
    

    T& front() {
    
        std::scoped_lock lock(blocking_mutex);    
        return deque.front();
    
    }
    
    
    T& back() {
    
        std::scoped_lock lock(blocking_mutex);
        return deque.back();
    
    }
    
    
    T pop_front() {
    
        std::scoped_lock lock(blocking_mutex);
        auto message = std::move(deque.front());
        deque.pop_front();
        return message;
    
    }
    
    
    T pop_back() {
    
        std::scoped_lock lock(blocking_mutex);
        auto message = std::move(deque.back());
        deque.pop_back();
        return message;
    
    }
    
    
    void push_back(T& item) {
    
        std::scoped_lock lock(blocking_mutex);
        deque.emplace_back(std::move(item));
    
        std::unique_lock<std::mutex> ul(cv_mutex);
        condition_variable.notify_one();
    
    }

    void push_front(T& item) {
    
        std::scoped_lock lock(blocking_mutex);
        deque.emplace_front(std::move(item));
    
        std::unique_lock<std::mutex> ul(cv_mutex);
        condition_variable.notify_one();
    
    }
    
    
    
    bool empty() {
    
        std::scoped_lock lock(blocking_mutex);
        return deque.empty();
    
    }


    size_t count() {
    
        std::scoped_lock lock(blocking_mutex);
        return deque.size();
    
    }


    void clear() {
    
        std::scoped_lock lock(blocking_mutex);
        deque.clear();
    
    }
        
    void wait() {
    
        while (empty()) {
    
            std::unique_lock<std::mutex> ul(cv_mutex);
            condition_variable.wait(ul);
    
        }
    
    }   




private:
    std::deque<T> deque;
    std::mutex blocking_mutex;
    std::mutex cv_mutex;
    std::condition_variable condition_variable;

};
