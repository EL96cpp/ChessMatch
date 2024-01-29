#pragma once

#include <boost/asio.hpp>

#include "ClientConnection.h"

class Server {

public:
    Server(const size_t& prot_id);
    
    
private:
    void start_accept();

private:
    boost::asio::io_context io_context;
    std::thread context_thread;

    boost::asio::ip::tcp::acceptor acceptor;


};
