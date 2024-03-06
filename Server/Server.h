#pragma once

#include <boost/asio.hpp>
#include <memory>


#include "ThreadSafeQueue.h"
#include "ClientConnection.h"
#include "SqlService.h"
#include "Message.h"

template<typename T>
class ThreadSafeQueue;


class Server {

public:
    Server(const size_t& prot_id);
    
    bool Start();
    
private:
    void WaitForClients();
    void Update();
    void OnMessage(std::shared_ptr<Message>& message);

private:
    ThreadSafeQueue<std::shared_ptr<Message>> incoming_messages;
    ThreadSafeQueue<std::shared_ptr<ClientConnection>> client_connections;

    boost::asio::io_context io_context;
    std::thread context_thread;

    boost::asio::ip::tcp::acceptor acceptor;
    SqlService sql_service;

};


