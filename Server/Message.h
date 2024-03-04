#include <string>
#include <iostream>
#include <vector>
#include <memory>

#include "ClientConnection.h"


class Message {

public:
    Message(std::shared_ptr<ClientConnection>& client, std::vector<uint8_t>& body);


private:
    std::shared_ptr<ClientConnection> client;
    std::vector<uint8_t> body;

};

