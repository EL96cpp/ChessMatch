#include "ClientConnection.h"
#include "ThreadSafeMessagesQueue.h"
#include "ThreadSafeGameMessagesQueue.h"

class Message;
class Game;


ClientConnection::ClientConnection(boost::asio::ip::tcp::socket&& socket, 
                                   boost::asio::io_context& io_context,
                                   ThreadSafeMessagesQueue& incoming_messages,
                                   ThreadSafeGameMessagesQueue& incoming_game_messages) : socket(std::move(socket)), 
                                                                                          io_context(io_context), 
                                                                                          logged_in(false), 
                                                                                          is_waiting(false),
                                                                                          game(nullptr),
                                                                                          incoming_messages(incoming_messages), 
                                                                                          incoming_game_messages(incoming_game_messages) {}


bool ClientConnection::IsConnected() {

    return socket.is_open();

}

void ClientConnection::OnLoggedIn(const std::string& nickname, const size_t& rating) {

    this->nickname = nickname;
    this->rating = rating;
    logged_in = true;

}

void ClientConnection::Logout() {

    nickname.clear();
    rating = 0;
    logged_in = false;

}

void ClientConnection::SetIsWaiting(const bool& is_waiting) {

    this->is_waiting = is_waiting;

}

Color ClientConnection::GetPlayerColor() {

    return player_color;

}

void ClientConnection::SetPlayerColor(const Color& player_color) {

    this->player_color = player_color;

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

bool ClientConnection::IsWaiting() {

    return is_waiting;

}

void ClientConnection::SetGame(std::shared_ptr<Game>& game) {

    this->game = game;

}

void ClientConnection::WriteMessageHeader() {

    boost::asio::async_write(socket, boost::asio::buffer(&outcoming_messages.front()->message_size, sizeof(uint32_t)), 
        [this](std::error_code ec, size_t length) {

            std::cout << "Message header writing handler\n";

            if (!ec) {
            
                if (outcoming_messages.front()->body.size() > 0) {
                
                    WriteMessageBody();

                } else {

                    outcoming_messages.pop_front();

                    if (!outcoming_messages.empty()) {
                    
                        WriteMessageHeader();

                    }


                }

            } else {

                std::cout << "Error while writing message header " << ec.message() << "\n";            

            }


        });

}


void ClientConnection::WriteMessageBody() {

    boost::asio::async_write(socket, boost::asio::buffer(outcoming_messages.front()->body.data(), outcoming_messages.front()->body.size()),
        [this](std::error_code ec, size_t length) {

            if (!ec) {

                std::cout << "Sent message body!\n";
                outcoming_messages.pop_front();

                if (!outcoming_messages.empty()) {

                    WriteMessageHeader();

                }


            } else {

               std::cout << "Error while writing message body " << ec.message() << "\n"; 

            }


        });

}


void ClientConnection::SendMessage(std::shared_ptr<Message>& message) {

    std::cout << "Called SendMessage function\n";
    
    bool writing_message = !outcoming_messages.empty();
    outcoming_messages.push_back(message);
    
    if (!writing_message) {

        std::cout << "Start writing message header\n";
        WriteMessageHeader();

    }


}



void ClientConnection::StartReadingMessage() {

    
    std::cout << temporary_message.message_size << " size of message\n";

    if (!is_waiting && game == nullptr) {


        boost::asio::async_read(socket, boost::asio::buffer(&temporary_message.message_size, sizeof(uint32_t)), [this](std::error_code ec, size_t length) {		    
        
            std::cout << temporary_message.message_size << " size of message inside lambda\n";
        
		    if (!ec) {
    
                temporary_message.body.resize(temporary_message.message_size);
                std::cout << "no error code!\n";
                
                if (temporary_message.message_size > 0) {
    
                    ReadMessageBody();
    
                }
                                
            } else {

                std::cout << ec.message() << " error!\n";

            }
        
        });


    } else {


        boost::asio::async_read(socket, boost::asio::buffer(&temporary_game_message.message_size, sizeof(uint32_t)), [this](std::error_code ec, size_t length) {		

        std::cout << temporary_game_message.message_size << " size of message inside lambda\n";

		    if (!ec) {

                temporary_game_message.body.resize(temporary_game_message.message_size);
                std::cout << "no error code!\n";
            
                if (temporary_game_message.message_size > 0) {

                    ReadMessageBody();

                }
                            
            } else {

                std::cout << ec.message() << " error!\n";

            }
    
        });

    }

}


void ClientConnection::ReadMessageBody() {

    if (!is_waiting && game == nullptr) {


        boost::asio::async_read(socket, boost::asio::buffer(temporary_message.body.data(), temporary_message.message_size), [this](std::error_code ec, size_t length) {

            std::cout << "Read " << temporary_message.body.size() << " \n";

            if (!ec) {

                incoming_messages.push_back_with_sender(std::make_shared<Message>(temporary_message), this->shared_from_this());

                temporary_message.body.clear();
                temporary_message.message_size = 0;

                StartReadingMessage();

            }


        });


    } else {


         boost::asio::async_read(socket, boost::asio::buffer(temporary_game_message.body.data(), temporary_game_message.message_size), [this](std::error_code ec, size_t length) {

            std::cout << "Read " << temporary_game_message.body.size() << " \n";

            if (!ec) {

                temporary_game_message.game = game;   

                incoming_game_messages.push_back(std::make_shared<GameMessage>(temporary_game_message), this->shared_from_this());

                temporary_game_message.body.clear();
                temporary_game_message.message_size = 0;

                StartReadingMessage();

            }

        });

   
    }

}
