#pragma once

#include <string>
#include <memory>

#include "ClientConnection.h"
#include "ThreadSafeQueue.h"


class Game;
class GameResult;
class ClientConnection;

class ThreadSafeClientsQueue : public ThreadSafeQueue<ClientConnection> {

public:
    ThreadSafeClientsQueue(ThreadSafeQueue<GameResult>& game_results_queue);

    bool contains_nickname(const std::string& nickname); 

    void wait_for_players(); 

    std::vector<std::shared_ptr<Game>> create_games(); 

private:
    ThreadSafeQueue<GameResult>& game_results_queue;


};
