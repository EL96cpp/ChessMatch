#pragma once 

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <boost/asio.hpp>

#include "ThreadSafeMessagesQueue.h"
#include "ThreadSafeGameMessagesQueue.h"
#include "GameMessage.h"
#include "Message.h"
#include "Game.h"

enum class PlayerColor {
    
    WHITE,
    BLACK,
    NONE

};


class ClientConnection : public std::enable_shared_from_this<ClientConnection> {

public:
    ClientConnection(boost::asio::ip::tcp::socket&& socket, boost::asio::io_context& io_context, ThreadSafeMessagesQueue& incoming_messages, 
                     ThreadSafeGameMessagesQueue& incoming_game_messages);

    bool IsConnected();

    void OnLoggedIn(const std::string& nickname, const size_t& rating);
    void Logout();
    void SetIsWaiting(const bool& is_waiting);

    void SendMessage(std::shared_ptr<Message>& message);

    void WriteMessageHeader();
    void WriteMessageBody();

    void StartReadingMessage();
    void ReadMessageBody();

    PlayerColor GetPlayerColor();
    std::string GetNickname();
    size_t GetRating();
    bool LoggedIn();
    bool IsWaiting();
    void SetGame(std::shared_ptr<Game>& game);
    


private:
    boost::asio::ip::tcp::socket socket;
    boost::asio::io_context& io_context;

    Message temporary_message;
    GameMessage temporary_game_message;
    ThreadSafeMessagesQueue outcoming_messages;
    ThreadSafeMessagesQueue& incoming_messages;
    ThreadSafeGameMessagesQueue& incoming_game_messages;
    PlayerColor player_color;
    std::string nickname;
    size_t rating;
    bool logged_in;
    bool is_waiting;
    std::vector<uint8_t> message_size;

    std::shared_ptr<Game> game; //Pointer to Game object provides the ability to send game messages

};
