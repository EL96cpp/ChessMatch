#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    void ConnectToServer(const QString& address, const quint16& port);

public slots:
    void onReadyRead();

signals:


private:
    QTcpSocket* socket;

};

#endif // CLIENT_H
