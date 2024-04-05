#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Game;
class ClientConnection;
class Message;


struct GameMessage {

    GameMessage();
    GameMessage(const GameMessage& other); 
    GameMessage(const Message& message, std::shared_ptr<Game>& game);


    std::shared_ptr<Game> game;
    std::shared_ptr<ClientConnection> sender;

    uint32_t message_size = 0;    
    std::vector<uint8_t> body;


};
