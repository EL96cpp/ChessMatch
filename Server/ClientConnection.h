#pragma once 

#include <string>
#include <iostream>
#include <boost/asio.hpp>

enum class ClientState {

    PLAYING,
    WAITING_FOR_AN_OPPONENT,
    DEFAULT

};


class ClientConnection {

public:
    ClientConnection(boost::asio::ip::tcp::socket&& socket );

    void SetLogin(const std::string& login);
    void SetRating(const size_t& rating);
    void SetLoggedIn(const bool& logged_in);
    void SetClientState(const ClientState& state);

    std::string GetLogin();
    size_t GetRating();
    bool LoggedIn();
    ClientState GetClientState();


private:
    boost::asio::ip::tcp::socket socket;

    std::string login;
    size_t rating;
    bool logged_in;
    ClientState state;

};
