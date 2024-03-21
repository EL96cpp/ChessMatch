#pragma once

#include <memory>

#include "ThreadSafeQueue.h"

class GameMessage;
class ClientConnection;


class ThreadSafeGameMessagesQueue : public ThreadSafeQueue<GameMessage> {

public:
    void push_back(const std::shared_ptr<GameMessage>& item, const std::shared_ptr<ClientConnection>& sender); 


};

