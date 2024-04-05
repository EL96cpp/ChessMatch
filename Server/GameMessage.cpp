#include "GameMessage.h"
#include "Message.h"
#include "ClientConnection.h"


GameMessage::GameMessage() {}

GameMessage::GameMessage(const GameMessage& other) : game(other.game), sender(other.sender), 
                                                     message_size(other.message_size), body(other.body) {}

GameMessage::GameMessage(const Message& message, std::shared_ptr<Game>& game) : game(game) {

    this->game = game;
    this->sender = message.sender;
    this->body = message.body;
    this->message_size = body.size();

}



