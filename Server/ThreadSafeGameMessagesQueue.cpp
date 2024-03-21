#include "ThreadSafeGameMessagesQueue.h"
#include "GameMessage.h"
#include "ClientConnection.h"


void ThreadSafeGameMessagesQueue::push_back(const std::shared_ptr<GameMessage>& item, const std::shared_ptr<ClientConnection>& sender) {

    std::scoped_lock lock(blocking_mutex);

    item->sender = sender;
    deque.emplace_back(std::move(item));

    std::unique_lock<std::mutex> ul(cv_mutex);
    condition_variable.notify_one();

} 


