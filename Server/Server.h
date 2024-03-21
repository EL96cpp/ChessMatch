#pragma once

#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <memory>
#include <sstream>
#include <vector>

#include "ThreadSafeMessagesQueue.h"
#include "ThreadSafeClientsQueue.h"
#include "GamesManager.h"
#include "SqlService.h"

class Server {

public:
    Server(const size_t& prot_id);
    
    bool Start();
    void UpdateIncomingMessages();

private:
    void WaitForClients();
    void OnMessage(std::shared_ptr<Message>& message);

    void OnLogin(const std::string& nickname, const std::string& password, std::shared_ptr<ClientConnection>& client_connection);
    void OnLogout(const std::string& nickname, std::shared_ptr<ClientConnection>& client_connection);
    void OnDisconnect(const std::shared_ptr<ClientConnection>& client_connection);
    void OnRegister(const std::string& nickname, const std::string& password, std::shared_ptr<ClientConnection>& client_connection);
    void OnStopWaiting(std::shared_ptr<ClientConnection>& client_connection);
    void OnResign(const std::string& sender_nickname);
    void OnOfferDraw(const std::string& sender_nickname);
    void OnCancelDraw(const std::string& sender_nickname);   


private:
    ThreadSafeMessagesQueue incoming_messages;
    ThreadSafeClientsQueue client_connections;

    GamesManager games_manager;

    boost::asio::io_context io_context;
    std::thread context_thread;

    boost::asio::ip::tcp::acceptor acceptor;
    SqlService sql_service;

};

