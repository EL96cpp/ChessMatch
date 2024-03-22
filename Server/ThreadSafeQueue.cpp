#include "ThreadSafeQueue.h"
#include "Game.h"
#include "Message.h"
#include "GameMessage.h"

template class ThreadSafeQueue<Message>;
template class ThreadSafeQueue<GameMessage>;
template class ThreadSafeQueue<Game>;



template<typename T>
std::shared_ptr<T>& ThreadSafeQueue<T>::front() {

    std::scoped_lock lock(blocking_mutex);
    return deque.front();

}

template<typename T>
std::shared_ptr<T>& ThreadSafeQueue<T>::back() {

    std::scoped_lock lock(blocking_mutex);
    return deque.back();

}

template<typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::pop_front() {

    std::scoped_lock lock(blocking_mutex);
    auto message = std::move(deque.front());
    deque.pop_front();
    return message;

}

template<typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::pop_back() {

    std::scoped_lock lock(blocking_mutex);
    auto message = std::move(deque.back());
    deque.pop_back();
    return message;

}

template<typename T>
void ThreadSafeQueue<T>::push_back(const std::shared_ptr<T>& item) {

    std::scoped_lock lock(blocking_mutex);
    deque.emplace_back(std::move(item));

    std::unique_lock<std::mutex> ul(cv_mutex);
    condition_variable.notify_one();

}

template<typename T>
void ThreadSafeQueue<T>::push_front(const std::shared_ptr<T>& item) {

    std::scoped_lock lock(blocking_mutex);
    deque.emplace_front(std::move(item));

    std::unique_lock<std::mutex> ul(cv_mutex);
    condition_variable.notify_one();

}


template<typename T>
bool ThreadSafeQueue<T>::empty() {

    std::scoped_lock lock(blocking_mutex);
    return deque.empty();

}


template<typename T>
void ThreadSafeQueue<T>::erase(const std::shared_ptr<T>& item) {

    std::scoped_lock lock(blocking_mutex);
    std::cout << "size before erase: " << deque.size() << "\n";
    auto it = std::remove(deque.begin(), deque.end(), item);
    deque.erase(it, deque.end());
    std::cout << "size after erase: " << deque.size() << "\n";

}

template<typename T>
size_t ThreadSafeQueue<T>::count() {

    std::scoped_lock lock(blocking_mutex);
    return deque.size();

}

template<typename T>
void ThreadSafeQueue<T>::clear() {

    std::scoped_lock lock(blocking_mutex);
    deque.clear();

}


template<typename T>
void ThreadSafeQueue<T>::wait() {

    while (empty()) {

        std::unique_lock<std::mutex> ul(cv_mutex);
        condition_variable.wait(ul);

    }

}


