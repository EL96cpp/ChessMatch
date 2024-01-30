#include "mainwindow.h"
#include "client.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Client client;
    client.ConnectToServer("127.0.0.1", 40000);
    w.show();
    return a.exec();
}
