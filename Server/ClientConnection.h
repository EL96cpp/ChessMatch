#pragma once 

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <boost/asio.hpp>

#include "Message.h"
#include "Game.h"
#include "ThreadSafeQueue.h"


enum class ClientState {

    PLAYING,
    WAITING_FOR_AN_OPPONENT,
    DEFAULT

};


class ClientConnection {

public:
    ClientConnection(boost::asio::ip::tcp::socket&& socket, boost::asio::io_context& io_context, ThreadSafeQueue<std::shared_ptr<Message>>& incoming_messages);

    bool IsConnected();

    void SetLogin(const std::string& login);
    void SetRating(const size_t& rating);
    void SetLoggedIn(const bool& logged_in);
    void SetClientState(const ClientState& state);

    void StartReadingMessage();
    void ReadMessageBody();

    std::string GetLogin();
    size_t GetRating();
    bool LoggedIn();
    ClientState GetClientState();
    void SetGame(std::shared_ptr<Game>& game);


private:
    boost::asio::ip::tcp::socket socket;
    boost::asio::io_context& io_context;

    Message temporary_message;
    ThreadSafeQueue<std::shared_ptr<Message>>& incoming_messages;
    std::string login;
    size_t rating;
    bool logged_in;
    ClientState state;
    std::vector<uint8_t> message_size;

    std::shared_ptr<Game> game; //Pointer to Game object provides the ability to send game messages

};
