#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMap>
#include <queue>
#include <QDebug>
#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <thread>

#include "threadsafequeue.h"
#include "message.h"
#include "chessfigure.h"


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent);

    void ConnectToServer(const std::string& address, const quint16& port);
    void Disconnect();

public slots:
    void OnLogin(const QString& nickname, const QString& password);
    void OnLogout(const QString& nickname);
    void OnRegister(const QString& nickname, const QString& password);
    void OnStartWaitingForOpponent();
    void OnMakeMove(const QString& letter_from, const QString& index_from, const QString& letter_to, const QString& index_to);
    void OnEatFigure(const QString& letter_from, const QString& index_from, const QString& letter_to,
                     const QString& index_to, const QString& transformation_type = "");
    void OnMakeCastling(const QString& letter_from, const QString& index_from, const QString& letter_to, const QString& index_to);
    void OnTransformPawn(const QString& letter_from, const QString& index_from, const QString& letter_to,
                         const QString& index_to, const QString& figure_type);
    void OnOfferDraw();
    void OnResign();

signals:
    void LoggedIn(const QString& nickname, const QString& rating, const QString& games_played, const QMap<QString, QString>& rating_values);
    void Loggedout();
    void Registered(const QString& nickname);
    void StartWaitingForOpponentAccepted();
    void GameStarted(const QString& player_color);
    void MakeMoveAccepted(const QString& letter_from, const QString& index_from, const QString& letter_to, const QString& index_to);
    void EatFigureAccepted(const QString& letter_from, const QString& index_from,
                           const QString& letter_to, const QString& index_to,
                           const QString& transformation_figure_type = "");
    void MakeCastlingAccepted(const QString& letter_from, const QString& index_from, const QString& letter_to, const QString& index_to);
    void TransformPawnAccepted(const QString& letter_from, const QString& index_from, const QString& letter_to,
                               const QString& index_to, const FigureType& figure_type);
    void ShowErrorMessage(const QString& title, const QString& error_description);
    void GameOver(const QString& game_result);
    void DrawOffered();

private:
    void SendMessage(const std::shared_ptr<Message>& message);
    void WriteHeader();
    void WriteBody();
    void ReadHeader();
    void ReadBody();
    void AddToIncomingMessages();
    void ProcessMessages();
    FigureType GetFigureTypeFromString(const QString& figure_type);

private:
    ThreadSafeQueue<std::shared_ptr<Message>> incoming_messages;
    ThreadSafeQueue<std::shared_ptr<Message>> outcoming_messages;

    bool is_playing;

    Message incoming_temporary_message;
    boost::asio::io_context io_context;
    std::thread context_thread;
    std::thread message_processing_thread;
    boost::asio::ip::tcp::socket socket;

};

#endif // CLIENT_H
