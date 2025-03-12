#include "command_handler.h"

#include <QJsonDocument>

#include "server.h"
#include "database/database.h"

#include "QJsonArray"

CommandHandler::CommandHandler(Server *serverInstance) : server(serverInstance) {
    registerHandlers();
}

void CommandHandler::registerHandlers() {
    handlers["login"] = &CommandHandler::handleLogin;
    handlers["register"] = &CommandHandler::handleRegister;
    handlers["send_message"] = &CommandHandler::handleSendMessage;
    handlers["get_history"] = &CommandHandler::handleGetHistory;
    handlers["get_online_users"] = &CommandHandler::handleGetOnlineUsers;
}

void CommandHandler::processCommand(QTcpSocket *socket, const QJsonObject &json) {
    const auto command = json["command"].toString();

    if (handlers.contains(command)) {
        handlers[command](server, socket, json);
        return;
    }

    server->sendCommandResponse(socket, {"error", "unknown command"});
}

void CommandHandler::handleLogin(Server *server, QTcpSocket *socket, const QJsonObject &json) {
    const QString username = json["username"].toString();
    const QString password = json["password"].toString();

    if (!Database::checkCredentials(username, password)) {
        server->sendCommandResponse(socket, {"error", "invalid login or password"});
        return;
    }

    if (server->isUserOnline(username)) {
        server->sendCommandResponse(socket, {"error", "user already online"});
        return;
    }

    server->addConnectedUser(socket, username);
    qDebug() << "user connected: " << username;

    server->broadcastSystemMessage(username + " has joined the chat");

    // send message history after user logged in
    QJsonObject historyCmd;
    historyCmd["command"] = "get_history";
    historyCmd["limit"] = 50;
    handleGetHistory(server, socket, historyCmd);

    // send updated online user list
    broadcastOnlineUsers(server);

    server->sendCommandResponse(socket, {"ok", "success login"});
}

void CommandHandler::handleRegister(Server *server, QTcpSocket *socket, const QJsonObject &json) {
    const QString username = json["username"].toString();
    const QString password = json["password"].toString();

    if (Database::registerUser(username, password)) {
        qDebug() << "user registered";
        server->sendCommandResponse(socket, {"ok", "success register"});
        return;
    }

    server->sendCommandResponse(socket, {"error", "unsuccessful register"});
}

void CommandHandler::handleSendMessage(Server *server, QTcpSocket *socket, const QJsonObject &json) {
    const QString message = json["message"].toString().trimmed();

    if (message.isEmpty()) {
        server->sendCommandResponse(socket, {"error", "empty message"});
        return;
    }

    // get socket username
    const QString sender = server->getUserBySocket(socket);
    if (sender.isEmpty()) {
        server->sendCommandResponse(socket, {"error", "not authenticated"});
        return;
    }

    // save message to db
    if (!Database::saveMessage(sender, message)) {
        server->sendCommandResponse(socket, {"error", "failed to save message"});
        return;
    }

    // broadcast message
    server->broadcastMessage(sender, message);
}

void CommandHandler::handleGetHistory(Server *server, QTcpSocket *socket, const QJsonObject &json) {
    const QString username = server->getUserBySocket(socket);
    if (username.isEmpty()) {
        server->sendCommandResponse(socket, {"error", "not authenticated"});
        return;
    }

    int limit = json.value("limit").toInt(50);
    if (limit <= 0 || limit > 200) {
        limit = 50;
    }

    QList<QVariantMap> messages = Database::getRecentMessages(limit);
    std::ranges::reverse(messages);

    QJsonArray messagesArray;
    for (const auto &msg: messages) {
        QJsonObject msgObj;
        msgObj["type"] = "message";
        msgObj["sender"] = msg["sender"].toString();
        msgObj["content"] = msg["content"].toString();
        msgObj["timestamp"] = msg["timestamp"].toString();
        messagesArray.append(msgObj);
    }

    QJsonObject historyResponse;
    historyResponse["type"] = "history";
    historyResponse["messages"] = messagesArray;

    server->sendResponse(socket, historyResponse);
}

void CommandHandler::handleGetOnlineUsers(Server *server, QTcpSocket *socket, const QJsonObject &json) {
    const QString username = server->getUserBySocket(socket);
    if (username.isEmpty()) {
        server->sendCommandResponse(socket, {"error", "not authenticated"});
        return;
    }

    QStringList onlineUsers = server->getOnlineUsers();

    QJsonObject usersResponse;
    usersResponse["type"] = "online_users";

    QJsonArray usersArray;
    for (const QString &user: onlineUsers) {
        usersArray.append(user);
    }

    usersResponse["count"] = onlineUsers.size();
    usersResponse["users"] = usersArray;

    server->sendResponse(socket, usersResponse);
}

void CommandHandler::broadcastOnlineUsers(Server *server) {
    QStringList onlineUsers = server->getOnlineUsers();

    QJsonObject usersResponse;
    usersResponse["type"] = "online_users";

    QJsonArray usersArray;
    for (const QString &user: onlineUsers) {
        usersArray.append(user);
    }

    usersResponse["count"] = onlineUsers.size();
    usersResponse["users"] = usersArray;

    server->broadcastJSON(usersResponse);
}
