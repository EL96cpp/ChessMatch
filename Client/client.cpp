#include "client.h"
#include "message.h"

Client::Client(QObject *parent) : QObject{parent}, socket(io_context) {

    ConnectToServer("127.0.0.0", 40000);

    outcoming_messages.clear();
    incoming_messages.clear();

    context_thread = std::thread([this]() { io_context.run(); });

    message_processing_thread = std::thread(&Client::ProcessMessages, this);
    message_processing_thread.detach();

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
    json_message[QStringLiteral("Action")] = QStringLiteral("Login");
    json_message[QStringLiteral("Nickname")] = nickname;
    json_message[QStringLiteral("Password")] = password;
    QByteArray byte_array = QJsonDocument(json_message).toJson();
    byte_array.append("\n");

    uint32_t size = byte_array.size();

    std::shared_ptr<Message> message = std::make_shared<Message>();
    message->message_size = size;
    message->body = byte_array;

    qDebug() << size << " will send to the Server";

    SendMessage(message);

}

void Client::OnLogout(const QString &nickname) {

    QJsonObject json_message;
    json_message[QStringLiteral("Method")] = QStringLiteral("POST");
    json_message[QStringLiteral("Action")] = QStringLiteral("Logout");
    json_message[QStringLiteral("Nickname")] = nickname;
    QByteArray byte_array = QJsonDocument(json_message).toJson();
    byte_array.append("\n");

    uint32_t size = byte_array.size();

    std::shared_ptr<Message> message = std::make_shared<Message>();
    message->message_size = size;
    message->body = byte_array;

    qDebug() << size << " will send to the Server";

    SendMessage(message);

}

void Client::OnRegister(const QString &nickname, const QString &password) {

    QJsonObject json_message;
    json_message[QStringLiteral("Method")] = QStringLiteral("POST");
    json_message[QStringLiteral("Action")] = QStringLiteral("Register");
    json_message[QStringLiteral("Nickname")] = nickname;
    json_message[QStringLiteral("Password")] = password;
    QByteArray byte_array = QJsonDocument(json_message).toJson();
    byte_array.append("\n");

    uint32_t size = byte_array.size();

    std::shared_ptr<Message> message = std::make_shared<Message>();
    message->message_size = size;
    message->body = byte_array;

    qDebug() << size << " will send to the Server";

    SendMessage(message);

}

void Client::OnStartWaitingForOpponent() {

    QJsonObject json_message;
    json_message[QStringLiteral("Method")] = QStringLiteral("POST");
    json_message[QStringLiteral("Action")] = QStringLiteral("Start_waiting");
    QByteArray byte_array = QJsonDocument(json_message).toJson();
    byte_array.append("\n");

    uint32_t size = byte_array.size();

    std::shared_ptr<Message> message = std::make_shared<Message>();
    message->message_size = size;
    message->body = byte_array;

    SendMessage(message);

}

void Client::SendMessage(const std::shared_ptr<Message>& message) {

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

    boost::asio::async_write(socket, boost::asio::buffer(&outcoming_messages.front()->message_size, sizeof(uint32_t)),
                             [this](std::error_code ec, size_t length) {

        if (!ec) {

            qDebug() << "Write message header";

            if (outcoming_messages.front()->body.size() > 0) {

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

    boost::asio::async_write(socket, boost::asio::buffer(outcoming_messages.front()->body.data(),
                                                         outcoming_messages.front()->body.size()),
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

            qDebug() << "Read header handler function";

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

            qDebug() << "Read message body";
            AddToIncomingMessages();

        } else {

            qDebug() << "Error while reading message body " << ec.message();

        }

    });

}

void Client::AddToIncomingMessages() {

    qDebug() << "message : " << incoming_temporary_message.body;

    bool messages_are_processing = !incoming_messages.empty();
    incoming_messages.push_back(std::make_shared<Message>(incoming_temporary_message));

    if (messages_are_processing) {



    }


    ReadHeader();

}

void Client::ProcessMessages() {

    while (true) {

        incoming_messages.wait();

        while (!incoming_messages.empty()) {

            std::shared_ptr<Message> message = incoming_messages.pop_front();

            QJsonParseError parse_error;
            QJsonDocument json_document_message = QJsonDocument::fromJson(message->body, &parse_error);

            if (parse_error.error != QJsonParseError::NoError) {

                qDebug() << "parse error!";
                qDebug () << parse_error.errorString();

            }

            QJsonObject json_message_object = json_document_message.object();

            QJsonValue method_value = json_message_object.value(QLatin1String("Method"));
            QJsonValue action_value = json_message_object.value(QLatin1String("Action"));
            QJsonValue code_value = json_message_object.value(QLatin1String("Code"));

            if (method_value.toString() == "POST") {

                if (action_value.toString() == "Login") {

                    if (code_value.toString() == "200") {

                        QString nickname = json_message_object.value(QLatin1String("Nickname")).toString();
                        QString rating = json_message_object.value(QLatin1String("Rating")).toString();
                        QString games_played = json_message_object.value(QLatin1String("Games_played")).toString();
                        QJsonArray rating_array = json_message_object.value(QLatin1String("Top_players")).toArray();

                        QMap<QString, QString> rating_map;

                        for (auto line : rating_array) {

                            foreach(const QString& player_nickname, line.toObject().keys()) {

                                QJsonValue value = line.toObject().value(player_nickname);
                                rating_map[player_nickname] = value.toString();

                            }

                        }

                        emit LoggedIn(nickname, rating, games_played, rating_map);

                    } else if (code_value.toString() == "403") {

                        QString error_description = json_message_object.value(QLatin1String("Error_description")).toString();
                        emit ShowErrorMessage("Login error", error_description);

                    }

                } else if (action_value.toString() == "Logout") {

                    if (code_value.toString() == "200") {

                        emit Loggedout();

                    } else if (code_value.toString() == "403") {

                        QString error_description = json_message_object.value(QLatin1String("Error_description")).toString();
                        emit ShowErrorMessage("Logout error", error_description);

                    }


                } else if (action_value.toString() == "Register") {

                    if (code_value.toString() == "200") {

                        QString nickname = json_message_object.value(QLatin1String("Nickname")).toString();
                        emit Registered(nickname);

                    } else {

                        QString error_description = json_message_object.value(QLatin1String("Error_description")).toString();
                        emit ShowErrorMessage("Register error", error_description);

                    }

                } else if (action_value.toString() == "Start_waiting") {

                    if (code_value.toString() == "200") {

                        emit StartWaitingForOpponentAccepted();

                    } else {

                        QString error_description = json_message_object.value(QLatin1String("Error_description")).toString();
                        emit ShowErrorMessage("Start game error", error_description);

                    }

                } else if (action_value.toString() == "Start_game") {

                    QString player_color = json_message_object.value(QLatin1String("Player_color")).toString();
                    qDebug() << player_color << " game started";
                    emit GameStarted(player_color);

                } else if (action_value.toString() == "Move_accepted") {



                } else if (action_value.toString() == "Move_error") {

                    QString letter_from = json_message_object.value(QLatin1String("Letter_from")).toString();
                    QString index_from = json_message_object.value(QLatin1String("Index_from")).toString();
                    QString letter_to = json_message_object.value(QLatin1String("Letter_to")).toString();
                    QString index_to = json_message_object.value(QLatin1String("Index_to")).toString();

                    emit ShowErrorMessage("Incorrect move", "Move " + letter_from + index_from + "-" +
                                          letter_to + index_to + " is not allowed!");

                }

            }

        }


    }

}


