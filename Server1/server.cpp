#include "server.h"
#include "setcomunication.h"
#include <QDebug>
#include <QTimer>

server::server() {
    if (this->listen(QHostAddress::Any, 2323)) {
        qDebug() << "Server started on port 2323";
    } else {
        qDebug() << "Error starting server";
    }

    // Таймер для проверки соединений
    QTimer *timer = new QTimer(this);
    timer->start(1000); // Интервал 5 секунд
    connect(timer, &QTimer::timeout, this, &server::ConectionHub);
}

void server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* socket = new QTcpSocket(this);
    if (socket->setSocketDescriptor(socketDescriptor)) {
        QString clientIP = socket->peerAddress().toString();
        qDebug() << "Client connected from IP:" << clientIP;

        Sockets.push_back(socket);

        connect(socket, &QTcpSocket::readyRead, this, &server::slotsReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &server::handleDisconnection);
        qDebug() << "Client connected, socket descriptor:" << socketDescriptor;
    } else {
        delete socket; // Удаляем сокет, если не удалось установить дескриптор
        qDebug() << "Error setting socket descriptor";
    }
}

void server::slotsReadyRead() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_6_0);

        if (in.status() == QDataStream::Ok) {
            qDebug() << "Reading data...";
            QString str;
            in >> str;
            qDebug() << "Received:" << str;
            SendToClient(socket, str);
        } else {
            qDebug() << "DataStream error";
        }
    }
}

void server::SendToClient(QTcpSocket* clientSocket, const QString& str) {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << str;

    if (clientSocket->state() == QAbstractSocket::ConnectedState) {
        clientSocket->write(data);
    } else {
        // Обработка закрытых сокетов
        clientSocket->disconnectFromHost();
        Sockets.removeAll(clientSocket);
        clientSocket->deleteLater();
    }
}


void server::handleDisconnection() {

    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    if (!socket) {
        qDebug() << "Error: Null socket in handleDisconnection";
        return;
    }

    QByteArray dataToRemove;
    QDataStream outToRemove(&dataToRemove, QIODevice::WriteOnly);
    outToRemove.setVersion(QDataStream::Qt_6_0);

    // Створення повідомлення для видалення
    QString identifierToRemove = QString("tOrEmUvE %1, %2")
                                     .arg(socket->peerAddress().toString())
                                     .arg(socket->peerPort());

    outToRemove << identifierToRemove;

    // Відправлення повідомлення всім підключеним клієнтам
    for (auto socketTarget : Sockets) {
        if(socket!=socketTarget){
        socketTarget->write(dataToRemove);
        qDebug() << "Sent delete command to" << socketTarget->peerAddress().toString();
        }
    }

    if (socket) {
        qDebug() << "Client disconnected, removing socket";
        Sockets.removeAll(socket);
        socket->deleteLater();
    }
}

void server::SendIdentificator() {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);

    QList<QTcpSocket*> activeSockets;

    for (QTcpSocket* socket : Sockets) {
        if (socket->state() == QAbstractSocket::ConnectedState) {
            activeSockets.append(socket);
        } else {
            socket->deleteLater();
        }
    }

    for (QTcpSocket* socketTarget : activeSockets) {
        for (QTcpSocket* socketToSend : activeSockets) {
            QString identifier = (socketToSend == socketTarget)
            ? QString("mYthEinDeNtIfIcAtOr %1, %2").arg(socketToSend->peerAddress().toString()).arg(socketToSend->peerPort())
            : QString("thEinDeNtIfIcAtOr %1, %2").arg(socketToSend->peerAddress().toString()).arg(socketToSend->peerPort());

            QByteArray data;
            QDataStream out(&data, QIODevice::WriteOnly);
            out << identifier;

            if (socketTarget->write(data) == -1) {
                qDebug() << "Error writing to socket" << socketTarget->errorString();
            }
        }
    }

    Sockets = activeSockets;
}

void server::ConectionHub()
{
    for(auto chakingSocket : Sockets) {
        if(chakingSocket->state() == QAbstractSocket::ConnectedState){
            SendIdentificator();
            // qDebug()<<chakingSocket;
        }
        else if(chakingSocket->state() == QAbstractSocket::UnconnectedState){
            handleDisconnection();
        }
    }
}

void server::fromSetComunication()
{

}


