#include "ClientConnection.h"

ClientConnection::ClientConnection(boost::asio::ip::tcp::socket&& socket) : socket(std::move(socket)), logged_in(false), state(ClientState::DEFAULT) {}

void ClientConnection::SetLogin(const std::string& login) {

    this->login = login;

}
    
void ClientConnection::SetRating(const size_t& rating) {

    this->rating = rating;

}

void ClientConnection::SetLoggedIn(const bool& logged_in) {

    this->logged_in = logged_in;

}

void ClientConnection::SetClientState(const ClientState& state) {

    this->state = state;

}

std::string ClientConnection::GetLogin() {

    return login;

}

size_t ClientConnection::GetRating() {

    return rating;

}

bool ClientConnection::LoggedIn() {

    return logged_in;

}

ClientState ClientConnection::GetClientState() {

    return state;

}


