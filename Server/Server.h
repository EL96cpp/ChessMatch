#pragma once

#include <boost/asio.hpp>

#include "ClientConnection.h"

class Server {

public:
    Server(const size_t& prot_id);
    
    bool Start();
    
private:
    void WaitForClients();

private:
    boost::asio::io_context io_context;
    std::thread context_thread;

    boost::asio::ip::tcp::acceptor acceptor;


};
