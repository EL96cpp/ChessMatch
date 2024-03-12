#pragma once

#include <mutex>
#include <deque>
#include <iostream>
#include <condition_variable>
#include <memory>

//#include "Message.h"

class ClientConnection;

        
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
    
    
    void push_back(const T& item) {
    
        std::scoped_lock lock(blocking_mutex);
        deque.emplace_back(std::move(item));
    
        std::unique_lock<std::mutex> ul(cv_mutex);
        condition_variable.notify_one();
   
    }

    void push_back(const T& item, const std::shared_ptr<ClientConnection>& sender) {
    
        std::scoped_lock lock(blocking_mutex);

        item->sender = sender;
        deque.emplace_back(std::move(item));
    
        std::unique_lock<std::mutex> ul(cv_mutex);
        condition_variable.notify_one();
   
    }



    void push_front(const T& item) {
    
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
    
        std::cout << "Start waiting...\n";

        while (empty()) {
    
            std::unique_lock<std::mutex> ul(cv_mutex);
            condition_variable.wait(ul);
    
        }

        std::cout << "Return from wait cycle\n";
    
    }   




private:
    std::deque<T> deque;
    std::mutex blocking_mutex;
    std::mutex cv_mutex;
    std::condition_variable condition_variable;

};
