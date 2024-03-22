#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <memory>
#include <chrono>
#include <map>

#include "ThreadSafeQueue.h"
#include "ThreadSafeGameMessagesQueue.h"
#include "ThreadSafeClientsQueue.h"
#include "GameResult.h"


class GameMessage;
class ClientConnection;


class GamesManager {

public:
    GamesManager(ThreadSafeQueue<GameResult>& game_results);

    void Start();

    void AddWaitingPlayer(std::shared_ptr<ClientConnection>& player); 
    ThreadSafeGameMessagesQueue& GetGameMessagesReference();
    
    void MatchOpponents();
    void ProcessGameMessages();


private:
    std::thread games_creation_thread;
    std::thread messages_processing_thread;
    
    std::map<std::string, int> board_navigation_map;

    ThreadSafeQueue<Game> games;
    ThreadSafeQueue<GameResult>& game_results;
    ThreadSafeGameMessagesQueue game_messages;
    ThreadSafeClientsQueue waiting_players;

};
