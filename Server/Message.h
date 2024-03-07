#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


struct Message {

    Message() {}

    Message(const Message& other) : message_size(other.message_size), body(other.body) {}

    uint32_t message_size = 0;
    std::vector<uint8_t> body;

};

