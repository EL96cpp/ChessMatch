#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class ClientConnection;


struct Message {

    Message();

    Message(const Message& other);

    std::shared_ptr<ClientConnection> sender;
    uint32_t message_size = 0;
    std::vector<uint8_t> body;

};

