#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QMap>
#include <QDebug>
#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <thread>

#include "message.h"


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent);

    void ConnectToServer(const std::string& address, const quint16& port);
    void Disconnect();
    void Login(const QString& nickname, const QString& password);

public slots:
    void onReadyRead();

signals:
    void LoggedIn(const QString& nickname, const QString& rating, const QString& games_played, QMap<QString,int>& rating_values);

private:
    void SendMessage();

private:
    Message temporary_message;
    boost::asio::io_context io_context;
    std::thread context_thread;
    boost::asio::ip::tcp::socket socket;

};

#endif // CLIENT_H
