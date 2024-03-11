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


class ClientConnection : public std::enable_shared_from_this<ClientConnection> {

public:
    ClientConnection(boost::asio::ip::tcp::socket&& socket, boost::asio::io_context& io_context, ThreadSafeQueue<std::shared_ptr<Message>>& incoming_messages);

    bool IsConnected();

    void OnLoggedIn(const std::string& nickname, const size_t& rating);
    void SetClientState(const ClientState& state);

    void StartReadingMessage();
    void ReadMessageBody();

    std::string GetNickname();
    size_t GetRating();
    bool LoggedIn();
    ClientState GetClientState();
    void SetGame(std::shared_ptr<Game>& game);


private:
    boost::asio::ip::tcp::socket socket;
    boost::asio::io_context& io_context;

    Message temporary_message;
    ThreadSafeQueue<std::shared_ptr<Message>>& incoming_messages;
    std::string nickname;
    size_t rating;
    bool logged_in;
    ClientState state;
    std::vector<uint8_t> message_size;

    std::shared_ptr<Game> game; //Pointer to Game object provides the ability to send game messages

};
