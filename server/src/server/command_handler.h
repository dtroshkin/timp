#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <QTcpSocket>
#include <QJsonObject>
#include <functional>
#include <QString>
#include <QHash>

class Server;

struct CommandResponse {
    QString status;
    QString message;
};

using CommandHandlerFunction = std::function<void(Server *, QTcpSocket *, const QJsonObject &)>;

class CommandHandler {
public:
    explicit CommandHandler(Server *serverInstance);

    void registerHandlers();

    void processCommand(QTcpSocket *socket, const QJsonObject &json);

private:
    Server *server;
    QHash<QString, CommandHandlerFunction> handlers;

    static void handleLogin(Server *server, QTcpSocket *socket, const QJsonObject &json);

    static void handleRegister(Server *server, QTcpSocket *socket, const QJsonObject &json);

    static void handleSendMessage(Server *server, QTcpSocket *socket, const QJsonObject &json);

    static void handleGetHistory(Server *server, QTcpSocket *socket, const QJsonObject &json);

    static void handleGetOnlineUsers(Server *server, QTcpSocket *socket, const QJsonObject &json);

    static void broadcastOnlineUsers(Server *server);
};

#endif
