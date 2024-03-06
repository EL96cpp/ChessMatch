#include "ClientConnection.h"

ClientConnection::ClientConnection(boost::asio::ip::tcp::socket&& socket, 
                                   boost::asio::io_context& io_context) : socket(std::move(socket)), io_context(io_context), logged_in(false), 
                                                                          state(ClientState::DEFAULT) { 

                                                                                //StartReadingMessage();
                                                                                std::cout << temporary_message.message_size << " size\n"; 

                                                                              }

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


void ClientConnection::StartReadingMessage() {

    //socket.wait(boost::asio::ip::tcp::socket::wait_read);

    std::cout << temporary_message.message_size << " size of message\n";

    boost::asio::async_read(socket, boost::asio::buffer(&temporary_message.message_size, sizeof(uint32_t)), [this](std::error_code ec, size_t length) {		


        std::cout << temporary_message.message_size << " size of message inside lambda\n";

		if (!ec) {

            temporary_message.body.resize(temporary_message.message_size);
            std::cout << "No error code!\n";
            
            if (temporary_message.message_size > 0) {

                ReadMessageBody();

            }
                            
        } else {

            std::cout << ec.message() << " error!\n";

        }
    
    });

}


void ClientConnection::ReadMessageBody() {

    boost::asio::async_read(socket, boost::asio::buffer(temporary_message.body.data(), temporary_message.message_size), [this](std::error_code ec, size_t length) {

        std::cout << "Read " << temporary_message.body.size() << " \n";

        if (!ec) {

            for (auto& letter : temporary_message.body) {

                std::cout << letter;

            }

            std::cout << "\n";
            
            temporary_message.body.clear();
            temporary_message.message_size = 0;

            StartReadingMessage();

        }


    });


}
