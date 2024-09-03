#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QByteArray>

class server : public QTcpServer {
    Q_OBJECT

public:
    server();
    QList<QTcpSocket*> Sockets;
    void mainLogic(){
        emit toSetComunication(this);
    }

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void slotsReadyRead();
    void handleDisconnection();
    void SendIdentificator();
    void ConectionHub();
    void fromSetComunication();

private:
    void SendToClient(QTcpSocket *clientSocket, const QString& str);
    QByteArray Data;

signals:
   void toSetComunication( server *s);
};

#endif // SERVER_H
