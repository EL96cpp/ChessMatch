#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "ClientConnection.h"
#include "Game.h"


struct GameMessage {

    GameMessage() {}

    GameMessage(const GameMessage& other) : game(other.game), sender(other.sender), 
                                            message_size(other.message_size), body(other.body) {}


    std::shared_ptr<Game> game;
    std::shared_ptr<ClientConnection> sender;

    uint32_t message_size = 0;    
    std::vector<uint8_t> body;


};
