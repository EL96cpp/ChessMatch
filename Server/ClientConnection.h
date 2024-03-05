#pragma once 

#include <string>
#include <iostream>
#include <memory>
#include <boost/asio.hpp>

#include "Game.h"

enum class ClientState {

    PLAYING,
    WAITING_FOR_AN_OPPONENT,
    DEFAULT

};


class ClientConnection {

public:
    ClientConnection(boost::asio::ip::tcp::socket&& socket, boost::asio::io_context& io_context);

    bool IsConnected();

    void SetLogin(const std::string& login);
    void SetRating(const size_t& rating);
    void SetLoggedIn(const bool& logged_in);
    void SetClientState(const ClientState& state);

    std::string GetLogin();
    size_t GetRating();
    bool LoggedIn();
    ClientState GetClientState();
    void SetGame(std::shared_ptr<Game>& game);


private:
    boost::asio::ip::tcp::socket socket;
    boost::asio::io_context& io_context;

    std::string login;
    size_t rating;
    bool logged_in;
    ClientState state;
    
    std::shared_ptr<Game> game; //Pointer to Game object provides the ability to send game messages

};
