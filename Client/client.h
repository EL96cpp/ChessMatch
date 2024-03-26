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

signals:
    void LoggedIn(const QString& nickname, const QString& rating, const QString& games_played, const QMap<QString, QString>& rating_values);
    void Loggedout();
    void Registered(const QString& nickname);
    void StartWaitingForOpponentAccepted();
    void GameStarted(const QString& player_color);
    void MakeMove(const QString& letter_from, const QString& index_from, const QString& letter_to, const QString& index_to);
    void ShowErrorMessage(const QString& title, const QString& error_description);

private:
    void SendMessage(const std::shared_ptr<Message>& message);
    void WriteHeader();
    void WriteBody();
    void ReadHeader();
    void ReadBody();
    void AddToIncomingMessages();
    void ProcessMessages();


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
