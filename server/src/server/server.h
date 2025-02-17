#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>


class Server final : public QObject {
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);

    bool startServer(quint16 port = 1234) const;

private slots:
    void handleNewConnection();

    void handleReadyRead();

    void handleClientDisconnected();

private:
    struct CommandResponse {
        QString status;
        QString message;
    };

    // command handlers
    CommandResponse handleLoginCommand(QTcpSocket *socket, const QJsonObject &json);

    static CommandResponse handleRegisterCommand(const QJsonObject &json);

    // helpers
    static void sendResponse(QTcpSocket *socket, const CommandResponse &response);

    bool isUserOnline(const QString &username) const;

    QTcpServer *server;
    QList<QTcpSocket *> clients;
    QHash<QTcpSocket *, QString> connectedUsers;
};


#endif //SERVER_H
