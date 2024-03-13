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
    void OnRegister(const QString& nickname, const QString& password);

signals:
    void LoggedIn(const QString& nickname, const QString& rating, const QString& games_played, const QMap<QString, QString>& rating_values);

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

    Message incoming_temporary_message;
    boost::asio::io_context io_context;
    std::thread context_thread;
    std::thread message_processing_thread;
    boost::asio::ip::tcp::socket socket;

};

#endif // CLIENT_H
