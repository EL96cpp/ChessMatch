#include "ClientConnection.h"

ClientConnection::ClientConnection(boost::asio::ip::tcp::socket&& socket, 
                                   boost::asio::io_context& io_context,
                                   ThreadSafeQueue<std::shared_ptr<Message>>& incoming_messages) : socket(std::move(socket)), io_context(io_context), logged_in(false), 
                                                                                                   incoming_messages(incoming_messages), state(ClientState::DEFAULT) { 

                                                                                //StartReadingMessage();
                                                                                std::cout << temporary_message.message_size << " size\n"; 

                                                                              }

bool ClientConnection::IsConnected() {

    return socket.is_open();

}

void ClientConnection::OnLoggedIn(const std::string& nickname, const size_t& rating) {

    this->nickname = nickname;
    this->rating = rating;
    logged_in = true;

}
    
void ClientConnection::SetClientState(const ClientState& state) {

    this->state = state;

}

std::string ClientConnection::GetNickname() {

    return nickname;

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

        temporary_message.sender = shared_from_this();

        if (!ec) {

            incoming_messages.push_back(std::make_shared<Message>(temporary_message));

            temporary_message.body.clear();
            temporary_message.message_size = 0;

            StartReadingMessage();

        }


    });


}
