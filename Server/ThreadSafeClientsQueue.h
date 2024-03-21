#pragma once

#include <string>
#include <memory>

#include "ClientConnection.h"


class Game;
class ClientConnection;

class ThreadSafeClientsQueue : public ThreadSafeQueue<ClientConnection> {

public:
    
    bool contains_nickname(const std::string& nickname); 

    void wait_for_players(); 

    std::vector<std::shared_ptr<Game>> create_games(); 



};
