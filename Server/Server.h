#pragma once

#include <boost/asio.hpp>

#include "ClientConnection.h"
#include "SqlService.h"
#include "ThreadSafeQueue.h"
#include "Message.h"


class Server {

public:
    Server(const size_t& prot_id);
    
    bool Start();
    
private:
    void WaitForClients();

private:
    ThreadSafeQueue<Message> incoming_messages;

    boost::asio::io_context io_context;
    std::thread context_thread;

    boost::asio::ip::tcp::acceptor acceptor;
    SqlService sql_service;

};
