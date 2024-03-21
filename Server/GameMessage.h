#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Game;
class ClientConnection;


struct GameMessage {

    GameMessage();

    GameMessage(const GameMessage& other); 


    std::shared_ptr<Game> game;
    std::shared_ptr<ClientConnection> sender;

    uint32_t message_size = 0;    
    std::vector<uint8_t> body;


};
