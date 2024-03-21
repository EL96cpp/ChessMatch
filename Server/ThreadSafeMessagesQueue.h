#pragma once 

#include <memory>

#include "ThreadSafeQueue.h"

class Message;
class ClientConnection;


class ThreadSafeMessagesQueue : public ThreadSafeQueue<Message> {

public:
    void push_back_with_sender(const std::shared_ptr<Message>& item, const std::shared_ptr<ClientConnection>& sender);


};
