#include <QCoreApplication>
#include <QThread>
#include <QDebug>
#include "server.h"
#include "setcomunication.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    server* s = new server();
    QThread* threadServer = new QThread();
    s->moveToThread(threadServer);

    setComunication* com = new setComunication();
    QThread* threadCommunication = new QThread();
    com->moveToThread(threadCommunication);

    // Соединяем сигналы и слоты
    QObject::connect(s, &server::toSetComunication, com, &setComunication::fromMain);

    // Запускаем потоки
    threadServer->start();
    threadCommunication->start();

    // Запускаем логику сервера
    s->mainLogic();

    return a.exec();
}
