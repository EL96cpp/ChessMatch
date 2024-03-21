#include "GameMessage.h"
#include "ClientConnection.h"


GameMessage::GameMessage() {}

GameMessage::GameMessage(const GameMessage& other) : game(other.game), sender(other.sender), 
                                                     message_size(other.message_size), body(other.body) {}

