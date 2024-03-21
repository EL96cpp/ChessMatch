#pragma once

#include <mutex>
#include <deque>
#include <vector>
#include <algorithm>
#include <iostream>
#include <condition_variable>
#include <memory>
#include <cmath>
#include <type_traits>

template<typename T>
class ThreadSafeQueue {

public:
    ThreadSafeQueue() {}

    ThreadSafeQueue(const ThreadSafeQueue<T>&) = delete;
    

    std::shared_ptr<T>& front(); 
    
    std::shared_ptr<T>& back(); 
    
    
    std::shared_ptr<T> pop_front(); 
   
    std::shared_ptr<T> pop_back(); 
    
    void push_back(const std::shared_ptr<T>& item); 
 
    void push_front(const std::shared_ptr<T>& item); 
   
    bool empty(); 

    void erase(const std::shared_ptr<T>& item); 
    
    size_t count(); 

    void clear();  
       
    void wait(); 


protected:
    std::deque<std::shared_ptr<T>> deque;
    std::mutex blocking_mutex;
    std::mutex cv_mutex;
    std::condition_variable condition_variable;


};
