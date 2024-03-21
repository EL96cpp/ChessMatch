#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <memory>
#include <chrono>

#include "ThreadSafeQueue.h"
#include "ThreadSafeGameMessagesQueue.h"
#include "ThreadSafeClientsQueue.h"


class GameMessage;
class ClientConnection;


class GamesManager {

public:
    GamesManager();

    void Start();

    void AddWaitingPlayer(std::shared_ptr<ClientConnection>& player); 
    ThreadSafeGameMessagesQueue& GetGameMessagesReference();
    
    void MatchOpponents();
    void UpdateGameMessages();


private:
    std::thread games_creation_thread;
    std::thread messages_update_thread;

    ThreadSafeQueue<Game> games;
    ThreadSafeGameMessagesQueue game_messages;
    ThreadSafeClientsQueue waiting_players;

};
