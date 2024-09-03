#include "setcomunication.h"
#include <QDebug>
#include <QTimer>

setComunication::setComunication(QObject *parent)
    : QObject{parent}, timer(new QTimer(this))
{
    connect(timer, &QTimer::timeout, this, &setComunication::workInThread);
    timer->setInterval(1000); // Интервал в 1 секунду
    timer->start();
}

void setComunication::workInThread()
{
    // qDebug() << "Working in thread:";
    for (auto i : Sockets) {
        qDebug() << i;
    }
}

void setComunication::fromMain(server *s)
{
    // qDebug() << "From Main:";
    Sockets.clear(); // Очистить старые данные
    for (auto socket : s->Sockets) {
        Sockets.append(socket); // Используем append, а не присвоение
        qDebug() << socket;
    }
}
