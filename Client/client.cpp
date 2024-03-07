#include "client.h"
#include "message.h"

Client::Client(QObject *parent) : QObject{parent}, socket(io_context) {

    ConnectToServer("127.0.0.0", 40000);

    context_thread = std::thread([this]() { io_context.run(); });

}

void Client::ConnectToServer(const std::string& address, const quint16 &port) {

    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(address, std::to_string(port));

    boost::asio::async_connect(socket, endpoints, [this](std::error_code ec, boost::asio::ip::tcp::endpoint endpoint) {

        if (!ec) {

            qDebug() << "Connection accepted!";

        } else {

            qDebug() << "Connection error " << ec.message();

        }

    });

}

void Client::Disconnect() {

    io_context.stop();

    if (context_thread.joinable()) {

        context_thread.join();

    }

}

void Client::Login(const QString &nickname, const QString &password) {

    QJsonObject message;
    message[QStringLiteral("Method")] = QStringLiteral("POST");
    message[QStringLiteral("Resource")] = QStringLiteral("Login");
    message[QStringLiteral("Nickname")] = nickname;
    message[QStringLiteral("Password")] = password;
    QByteArray byte_array = QJsonDocument(message).toJson();
    byte_array.append("\n");

    uint32_t size = byte_array.size();

    temporary_message.message_size = size;
    temporary_message.body = byte_array;

    qDebug() << size << " send to the Server";

    SendMessage();

}

void Client::onReadyRead() {



}

void Client::SendMessage() {

    qDebug() << "SendMessageBody call";

    socket.write_some(boost::asio::buffer(&temporary_message.message_size, sizeof(temporary_message.message_size)));
    socket.write_some(boost::asio::buffer(temporary_message.body.data(), temporary_message.message_size));

    temporary_message.message_size = 0;
    temporary_message.body.clear();

}

