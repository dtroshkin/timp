#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

#include "command_handler.h"

class CommandHandler;

class Server final : public QObject {
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);

    ~Server() override;

    [[nodiscard]] bool startServer(quint16 port = 1234) const;

    static void sendCommandResponse(QTcpSocket *socket, const CommandResponse &response);

    [[nodiscard]] bool isUserOnline(const QString &username) const;

    void addConnectedUser(QTcpSocket *socket, const QString &username);

    QString getUserBySocket(QTcpSocket *socket) const;

    static void sendResponse(QTcpSocket *socket, const QJsonObject &jsonResponse);

    void broadcastMessage(const QString &sender, const QString &message);

    void broadcastJSON(const QJsonObject &json);

    void broadcastSystemMessage(const QString &message);

    QStringList getOnlineUsers() const;

private slots:
    void handleNewConnection();

    void handleReadyRead() const;

    void handleClientDisconnected();

private:
    QTcpServer *server;
    QList<QTcpSocket *> clients;
    QHash<QTcpSocket *, QString> connectedUsers;

    CommandHandler *commandHandler;
};

#endif //SERVER_H
