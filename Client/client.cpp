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

void Client::OnLogin(const QString &nickname, const QString &password) {


    QJsonObject json_message;

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

    json_message[QStringLiteral("Action")] = QStringLiteral("Start_waiting");
    QByteArray byte_array = QJsonDocument(json_message).toJson();
    byte_array.append("\n");

    uint32_t size = byte_array.size();

    std::shared_ptr<Message> message = std::make_shared<Message>();
    message->message_size = size;
    message->body = byte_array;

    SendMessage(message);

}

void Client::OnStopWaitingForOpponent() {

    QJsonObject json_message;
    json_message[QStringLiteral("Action")] = QStringLiteral("Stop_waiting");

    QByteArray byte_array = QJsonDocument(json_message).toJson();
    byte_array.append("\n");

    uint32_t size = byte_array.size();

    std::shared_ptr<Message> message = std::make_shared<Message>();
    message->message_size = size;
    message->body = byte_array;

    qDebug() << byte_array;

    SendMessage(message);

}

void Client::OnMakeMove(const QString &letter_from, const QString &index_from, const QString &letter_to, const QString &index_to) {

    QJsonObject json_message;
    json_message[QStringLiteral("Action")] = QStringLiteral("Make_move");
    json_message[QStringLiteral("Letter_from")] = letter_from;
    json_message[QStringLiteral("Index_from")] = index_from;
    json_message[QStringLiteral("Letter_to")] = letter_to;
    json_message[QStringLiteral("Index_to")] = index_to;

    QByteArray byte_array = QJsonDocument(json_message).toJson();
    byte_array.append("\n");

    uint32_t size = byte_array.size();

    std::shared_ptr<Message> message = std::make_shared<Message>();
    message->message_size = size;
    message->body = byte_array;

    qDebug() << byte_array;

    SendMessage(message);

}

void Client::OnEatFigure(const QString &letter_from, const QString &index_from, const QString &letter_to,
                         const QString &index_to, const QString& transformation_type) {

    QJsonObject json_message;
    json_message[QStringLiteral("Action")] = QStringLiteral("Eat_figure");
    json_message[QStringLiteral("Letter_from")] = letter_from;
    json_message[QStringLiteral("Index_from")] = index_from;
    json_message[QStringLiteral("Letter_to")] = letter_to;
    json_message[QStringLiteral("Index_to")] = index_to;
    json_message[QStringLiteral("Transformation_type")] = transformation_type;


    QByteArray byte_array = QJsonDocument(json_message).toJson();
    byte_array.append("\n");

    uint32_t size = byte_array.size();

    std::shared_ptr<Message> message = std::make_shared<Message>();
    message->message_size = size;
    message->body = byte_array;

    SendMessage(message);

}

void Client::OnMakeCastling(const QString &letter_from, const QString &index_from, const QString &letter_to, const QString &index_to) {

    QJsonObject json_message;
    json_message[QStringLiteral("Action")] = QStringLiteral("Make_castling");
    json_message[QStringLiteral("Letter_from")] = letter_from;
    json_message[QStringLiteral("Index_from")] = index_from;
    json_message[QStringLiteral("Letter_to")] = letter_to;
    json_message[QStringLiteral("Index_to")] = index_to;

    QByteArray byte_array = QJsonDocument(json_message).toJson();
    byte_array.append("\n");

    uint32_t size = byte_array.size();

    std::shared_ptr<Message> message = std::make_shared<Message>();
    message->message_size = size;
    message->body = byte_array;

    qDebug() << byte_array;

    SendMessage(message);

}

void Client::OnTransformPawn(const QString &letter_from, const QString &index_from, const QString &letter_to, const QString &index_to, const QString &figure_type) {

    QJsonObject json_message;
    json_message[QStringLiteral("Action")] = QStringLiteral("Transform_pawn");
    json_message[QStringLiteral("Letter_from")] = letter_from;
    json_message[QStringLiteral("Index_from")] = index_from;
    json_message[QStringLiteral("Letter_to")] = letter_to;
    json_message[QStringLiteral("Index_to")] = index_to;
    json_message[QStringLiteral("Figure_type")] = figure_type;

    QByteArray byte_array = QJsonDocument(json_message).toJson();
    byte_array.append("\n");

    uint32_t size = byte_array.size();

    std::shared_ptr<Message> message = std::make_shared<Message>();
    message->message_size = size;
    message->body = byte_array;

    qDebug() << byte_array;

    SendMessage(message);

}

void Client::OnOfferDraw() {

    QJsonObject json_message;
    json_message[QStringLiteral("Action")] = QStringLiteral("Offer_draw");

    QByteArray byte_array = QJsonDocument(json_message).toJson();
    byte_array.append("\n");

    uint32_t size = byte_array.size();

    std::shared_ptr<Message> message = std::make_shared<Message>();
    message->message_size = size;
    message->body = byte_array;

    qDebug() << byte_array;

    SendMessage(message);

}

void Client::OnDrawAccepted() {

    QJsonObject json_message;
    json_message[QStringLiteral("Action")] = QStringLiteral("Accept_draw");

    QByteArray byte_array = QJsonDocument(json_message).toJson();
    byte_array.append("\n");

    uint32_t size = byte_array.size();

    std::shared_ptr<Message> message = std::make_shared<Message>();
    message->message_size = size;
    message->body = byte_array;

    qDebug() << byte_array;

    SendMessage(message);

}

void Client::OnResign() {

    QJsonObject json_message;
    json_message[QStringLiteral("Action")] = QStringLiteral("Resign");

    QByteArray byte_array = QJsonDocument(json_message).toJson();
    byte_array.append("\n");

    uint32_t size = byte_array.size();

    std::shared_ptr<Message> message = std::make_shared<Message>();
    message->message_size = size;
    message->body = byte_array;

    qDebug() << byte_array;

    SendMessage(message);

}

void Client::SendMessage(const std::shared_ptr<Message>& message) {

    bool writing_message = !outcoming_messages.empty();

    outcoming_messages.push_back(message);

    if (!writing_message) {

        WriteHeader();

    }

}

void Client::WriteHeader() {

    boost::asio::async_write(socket, boost::asio::buffer(&outcoming_messages.front()->message_size, sizeof(uint32_t)),
                             [this](std::error_code ec, size_t length) {

        if (!ec) {

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

    });

}

void Client::AddToIncomingMessages() {

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

            QJsonValue action_value = json_message_object.value(QLatin1String("Action"));
            QJsonValue code_value = json_message_object.value(QLatin1String("Code"));

            if (action_value.toString() == "Login") {

                if (code_value.toString() == "200") {

                    QString nickname = json_message_object.value(QLatin1String("Nickname")).toString();
                    QString rating = json_message_object.value(QLatin1String("Rating")).toString();
                    QString games_played = json_message_object.value(QLatin1String("Games_played")).toString();
                    QJsonArray rating_array = json_message_object.value(QLatin1String("Top_players")).toArray();

                    this->nickname = nickname;

                    QList<QPair<QString, QString>> rating_map;

                    for (auto line : rating_array) {

                        foreach(const QString& player_nickname, line.toObject().keys()) {

                            QJsonValue value = line.toObject().value(player_nickname);
                            rating_map.push_back(QPair(player_nickname, value.toString()));

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
                QString opponent_nickname = json_message_object.value(QLatin1String("Opponent_nickname")).toString();
                qDebug() << player_color << " game started";
                emit GameStarted(player_color, nickname, opponent_nickname);

            } else if (action_value.toString() == "Stop_waiting_accepted") {

                emit StopWaitingForOpponentAccepted();

            } else if (action_value.toString() == "Stop_waiting_error") {

                emit ShowErrorMessage("Stop waiting", "Stop waiting for opponent error!");

            } else if (action_value.toString() == "Move_accepted") {

                QString letter_from = json_message_object.value(QLatin1String("Letter_from")).toString();
                QString index_from = json_message_object.value(QLatin1String("Index_from")).toString();
                QString letter_to = json_message_object.value(QLatin1String("Letter_to")).toString();
                QString index_to = json_message_object.value(QLatin1String("Index_to")).toString();

                emit MakeMoveAccepted(letter_from, index_from, letter_to, index_to);
                qDebug() << "Move accepted";

            } else if (action_value.toString() == "Make_castling_accepted") {

                QString letter_from = json_message_object.value(QLatin1String("Letter_from")).toString();
                QString index_from = json_message_object.value(QLatin1String("Index_from")).toString();
                QString letter_to = json_message_object.value(QLatin1String("Letter_to")).toString();
                QString index_to = json_message_object.value(QLatin1String("Index_to")).toString();

                emit MakeCastlingAccepted(letter_from, index_from, letter_to, index_to);
                qDebug() << "Make castling accepted!";

            } else if (action_value.toString() == "Transform_pawn_accepted") {

                QString letter_from = json_message_object.value(QLatin1String("Letter_from")).toString();
                QString index_from = json_message_object.value(QLatin1String("Index_from")).toString();
                QString letter_to = json_message_object.value(QLatin1String("Letter_to")).toString();
                QString index_to = json_message_object.value(QLatin1String("Index_to")).toString();
                QString figure_type_str = json_message_object.value(QLatin1String("Figure_type")).toString();
                FigureType figure_type = GetFigureTypeFromString(figure_type_str);

                emit TransformPawnAccepted(letter_from, index_from, letter_to, index_to, figure_type);

            } else if (action_value.toString() == "Move_error") {

                QString letter_from = json_message_object.value(QLatin1String("Letter_from")).toString();
                QString index_from = json_message_object.value(QLatin1String("Index_from")).toString();
                QString letter_to = json_message_object.value(QLatin1String("Letter_to")).toString();
                QString index_to = json_message_object.value(QLatin1String("Index_to")).toString();

                emit ShowErrorMessage("Incorrect move", "Move " + letter_from + index_from + "-" +
                                      letter_to + index_to + " is not allowed!");

            } else if (action_value.toString() == "Eat_figure_accepted") {

                QString letter_from = json_message_object.value(QLatin1String("Letter_from")).toString();
                QString index_from = json_message_object.value(QLatin1String("Index_from")).toString();
                QString letter_to = json_message_object.value(QLatin1String("Letter_to")).toString();
                QString index_to = json_message_object.value(QLatin1String("Index_to")).toString();
                QString transformation_figure_type = json_message_object.value(QLatin1String("Transformation_type")).toString();

                emit EatFigureAccepted(letter_from, index_from, letter_to, index_to, transformation_figure_type);


            } else if (action_value.toString() == "Eat_figure_error") {

                QString letter_from = json_message_object.value(QLatin1String("Letter_from")).toString();
                QString index_from = json_message_object.value(QLatin1String("Index_from")).toString();
                QString letter_to = json_message_object.value(QLatin1String("Letter_to")).toString();
                QString index_to = json_message_object.value(QLatin1String("Index_to")).toString();

                emit ShowErrorMessage("Eat figure error", "Eat figure at " +
                                      letter_to + index_to + " is not allowed!");

            } else if (action_value.toString() == "Make_castling_error") {

                QString letter_from = json_message_object.value(QLatin1String("Letter_from")).toString();
                QString index_from = json_message_object.value(QLatin1String("Index_from")).toString();
                QString letter_to = json_message_object.value(QLatin1String("Letter_to")).toString();
                QString index_to = json_message_object.value(QLatin1String("Index_to")).toString();

                emit ShowErrorMessage("Castling error", "Castling " + letter_from + index_from + "-" +
                                      letter_to + index_to + " is not allowed!");

            } else if (action_value.toString() == "Draw_offered") {

                QString nickname = json_message_object.value(QLatin1String("Nickname")).toString();

                qDebug() << "Draw offered by " << nickname;

                if (this->nickname != nickname) {

                    emit DrawOffered();

                }

            } else if (action_value.toString() == "Game_over") {

                QString result = json_message_object.value(QLatin1String("Result")).toString();

                emit GameOver(result);

            } else if (action_value.toString() == "Update_rating") {

                QString new_rating = json_message_object.value(QLatin1String("New_rating")).toString();

                emit UpdatePlayerRatingAndGamesPlayed(new_rating);

            } else if (action_value.toString() == "Update_top_players_rating") {

                QJsonArray rating_array = json_message_object.value(QLatin1String("Top_players")).toArray();

                this->nickname = nickname;

                QList<QPair<QString, QString>> rating_map;

                for (auto line : rating_array) {

                    foreach(const QString& player_nickname, line.toObject().keys()) {

                        QJsonValue value = line.toObject().value(player_nickname);
                        rating_map.push_back(QPair(player_nickname, value.toString()));

                    }

                }

                emit UpdateTopPlayersRating(rating_map);

            }

        }

    }

}

FigureType Client::GetFigureTypeFromString(const QString &figure_type) {

    if (figure_type == "Pawn") {

        return FigureType::PAWN;

    } else if (figure_type == "Rook") {

        return FigureType::ROOK;

    } else if (figure_type == "Knight") {

        return FigureType::KNIGHT;

    } else if (figure_type == "Bishop") {

        return FigureType::BISHOP;

    } else if (figure_type == "Queen") {

        return FigureType::QUEEN;

    } else if (figure_type == "King") {

        return FigureType::KING;

    } else {

        return FigureType::EMPTY;

    }

}

void Client::OnExitApplication() {

    QJsonObject json_message;

    json_message[QStringLiteral("Action")] = QStringLiteral("Disconnect");

    QByteArray byte_array = QJsonDocument(json_message).toJson();
    byte_array.append("\n");

    uint32_t size = byte_array.size();

    std::shared_ptr<Message> message = std::make_shared<Message>();
    message->message_size = size;
    message->body = byte_array;

    qDebug() << byte_array;

    SendMessage(message);

    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    io_context.stop();

    if (context_thread.joinable()) {

        context_thread.join();

    }

}


