#include "client.h"

Client::Client(QObject *parent) : QObject{parent}, socket(new QTcpSocket(this)) {

    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);

}

void Client::ConnectToServer(const QString &address, const quint16 &port) {

    socket->connectToHost(address, port);
    qDebug() << socket->state();

    if (!socket->waitForConnected(40000)) {

        qDebug() << "Connection error!";

    } else {

        qDebug() << "Connected!";

    }

}

void Client::onReadyRead() {



}
