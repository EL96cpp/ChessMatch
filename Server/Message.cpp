#include "Message.h"

Message::Message(std::shared_ptr<ClientConnection>& client, std::vector<uint8_t>& body) : client(client), body(body) {}

