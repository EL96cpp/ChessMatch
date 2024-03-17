#pragma once

#include <iostream>
#include <string>
#include <memory>

#include "Game.h"
#include "GameMessage.h"
#include "ClientConnection.h"
#include "ThreadSafeQueue.h"


class GamesManager {

public:
    GamesManager();

    void AddWaitingPlayer(std::shared_ptr<ClientConnection>& player); 
    ThreadSafeQueue<std::shared_ptr<GameMessage>>& GetGameMessagesReference();

    
private:
    std::thread games_creation_thread;
    std::thread messages_update_thread;

    ThreadSafeQueue<std::shared_ptr<Game>> games;
    ThreadSafeQueue<std::shared_ptr<GameMessage>> game_messages;
    ThreadSafeQueue<std::shared_ptr<ClientConnection>> waiting_players;

};
