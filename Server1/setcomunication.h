#ifndef SETCOMUNICATION_H
#define SETCOMUNICATION_H

#include <QObject>
#include <QThread>
#include <QList>
#include <QTcpSocket>
#include <QTimer>
#include "server.h"

class setComunication : public QObject
{
    Q_OBJECT
public:
    explicit setComunication(QObject *parent = nullptr);
    void workInThread();

signals:

public slots:
    void fromMain(server *s);
private:
    QList<QTcpSocket*>  Sockets;
     QTimer *timer;

};

#endif // SETCOMUNICATION_H
