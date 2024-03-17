#pragma once

#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <memory>
#include <sstream>
#include <vector>

#include "ThreadSafeQueue.h"
#include "ClientConnection.h"
#include "SqlService.h"
#include "Message.h"
#include "GamesManager.h"
#include "GameIDs.h"


template<typename T>
class ThreadSafeQueue;


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
    void OnStartWaiting(std::shared_ptr<ClientConnection>& client_connection);
    void OnStopWaiting(std::shared_ptr<ClientConnection>& client_connection);
    void OnResign(const std::string& sender_nickname);
    void OnOfferDraw(const std::string& sender_nickname);
    void OnCancelDraw(const std::string& sender_nickname);   


private:
    ThreadSafeQueue<std::shared_ptr<Message>> incoming_messages;
    ThreadSafeQueue<std::shared_ptr<ClientConnection>> client_connections;

    GamesManager games_manager;

    boost::asio::io_context io_context;
    std::thread context_thread;

    boost::asio::ip::tcp::acceptor acceptor;
    SqlService sql_service;

};


