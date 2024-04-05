#include "GameMessage.h"
#include "ClientConnection.h"


GameMessage::GameMessage() {}

GameMessage::GameMessage(const GameMessage& other) : game(other.game), sender(other.sender), 
                                                     message_size(other.message_size), body(other.body) {}

GameMessage::GameMessage(std::shared_ptr<Message>& message, std::shared_ptr<Game>& game) : game(game) {

    this->game = game;
    this->sender = message->sender;
    this->body = message->body;
    this->message_size = body.size();

}



