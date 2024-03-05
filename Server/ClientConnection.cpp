#include "ClientConnection.h"

ClientConnection::ClientConnection(boost::asio::ip::tcp::socket&& socket, 
                                   boost::asio::io_context& io_context) : socket(std::move(socket)), io_context(io_context), logged_in(false), state(ClientState::DEFAULT) {}

bool ClientConnection::IsConnected() {

    return socket.is_open();

}

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

void ClientConnection::SetGame(std::shared_ptr<Game>& game) {

    this->game = game;

}


