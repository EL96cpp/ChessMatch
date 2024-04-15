#include "ThreadSafeMessagesQueue.h"
#include "Message.h"
#include "ClientConnection.h"


void ThreadSafeMessagesQueue::push_back_with_sender(const std::shared_ptr<Message>& item, const std::shared_ptr<ClientConnection>& sender) {

    std::scoped_lock lock(blocking_mutex);

    std::cout << "Push back message with sender from " << sender->GetNickname() << "!\n";

    item->sender = sender;
    deque.emplace_back(std::move(item));

    std::unique_lock<std::mutex> ul(cv_mutex);
    condition_variable.notify_all();

}


