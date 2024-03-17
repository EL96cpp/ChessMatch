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

    
private:
    ThreadSafeQueue<std::shared_ptr<Game>> games;
    ThreadSafeQueue<std::shared_ptr<GameMessage>> game_messages;
    ThreadSafeQueue<std::shared_ptr<ClientConnection>> waiting_players;


};
