#include "client.h"
#include "message.h"

Client::Client(QObject *parent) : QObject{parent}, socket(io_context) {

    ConnectToServer("127.0.0.0", 40000);

    outcoming_messages.clear();
    incoming_messages.clear();

    context_thread = std::thread([this]() { io_context.run(); });

}

void Client::ConnectToServer(const std::string& address, const quint16 &port) {

    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(address, std::to_string(port));

    boost::asio::async_connect(socket, endpoints, [this](std::error_code ec, boost::asio::ip::tcp::endpoint endpoint) {

        if (!ec) {

            qDebug() << "Connection accepted!";

            ReadHeader();

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

void Client::OnLogin(const QString &nickname, const QString &password) {

    QJsonObject json_message;
    json_message[QStringLiteral("Method")] = QStringLiteral("POST");
    json_message[QStringLiteral("Resource")] = QStringLiteral("Login");
    json_message[QStringLiteral("Nickname")] = nickname;
    json_message[QStringLiteral("Password")] = password;
    QByteArray byte_array = QJsonDocument(json_message).toJson();
    byte_array.append("\n");

    uint32_t size = byte_array.size();

    Message message;
    message.message_size = size;
    message.body = byte_array;

    qDebug() << size << " will send to the Server";

    SendMessage(message);

}

void Client::OnRegister(const QString &nickname, const QString &password) {

    QJsonObject json_message;
    json_message[QStringLiteral("Method")] = QStringLiteral("POST");
    json_message[QStringLiteral("Resource")] = QStringLiteral("Register");
    json_message[QStringLiteral("Nickname")] = nickname;
    json_message[QStringLiteral("Password")] = password;
    QByteArray byte_array = QJsonDocument(json_message).toJson();
    byte_array.append("\n");

    uint32_t size = byte_array.size();

    Message message;
    message.message_size = size;
    message.body = byte_array;

    qDebug() << size << " will send to the Server";

    SendMessage(message);

}

void Client::SendMessage(const Message& message) {

    /*
    socket.write_some(boost::asio::buffer(&incoming_temporary_message.message_size, sizeof(incoming_temporary_message.message_size)));
    socket.write_some(boost::asio::buffer(incoming_temporary_message.body.data(), incoming_temporary_message.message_size));

    incoming_temporary_message.message_size = 0;
    incoming_temporary_message.body.clear();
    */

    qDebug() << "Send message call";


    bool writing_message = !outcoming_messages.empty();

    qDebug() << writing_message << " writing message";

    outcoming_messages.push_back(message);

    if (!writing_message) {

        qDebug() << "will write message header";

        WriteHeader();

    }

}

void Client::WriteHeader() {

    boost::asio::async_write(socket, boost::asio::buffer(&outcoming_messages.front().message_size, sizeof(uint32_t)),
                             [this](std::error_code ec, size_t length) {

        if (!ec) {

            qDebug() << "Write message header";

            if (outcoming_messages.front().body.size() > 0) {

                WriteBody();

            } else {

                outcoming_messages.pop_back();

                if (!outcoming_messages.empty()) {

                    WriteHeader();

                }

            }

        } else {

            qDebug() << "Write header error " << ec.message();

        }

    });

}

void Client::WriteBody() {

    boost::asio::async_write(socket, boost::asio::buffer(outcoming_messages.front().body.data(),
                                                         outcoming_messages.front().body.size()),
                             [this](std::error_code ec, size_t length) {

        if (!ec) {

            qDebug() << "Write message body";

            outcoming_messages.pop_front();

            if (!outcoming_messages.empty()) {

                WriteHeader();

            }

        } else {

            qDebug() << "Write body error " << ec.message();

        }

    });

}

void Client::ReadHeader() {

    boost::asio::async_read(socket, boost::asio::buffer(&incoming_temporary_message.message_size, sizeof(uint32_t)),
                            [this](std::error_code ec, size_t length) {

        if (!ec) {

            if (incoming_temporary_message.message_size > 0) {

                incoming_temporary_message.body.resize(incoming_temporary_message.message_size);
                ReadBody();

            } else {

                ReadHeader();

            }

        } else {

            qDebug() << "Error while reading header " << ec.message();

        }

    });

}

void Client::ReadBody() {

    boost::asio::async_read(socket, boost::asio::buffer(incoming_temporary_message.body.data(), incoming_temporary_message.body.size()),
                            [this](std::error_code ec, size_t length) {

        if (!ec) {

            AddToIncomingMessages();

        } else {

            qDebug() << "Error while reading message body " << ec.message();

        }

    }
);

}

void Client::AddToIncomingMessages() {

    incoming_messages.push_back(incoming_temporary_message);

    ReadHeader();

}


