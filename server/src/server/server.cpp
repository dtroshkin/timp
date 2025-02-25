#include "server.h"

#include <QJsonArray>

#include "command_handler.h"
#include "database/database.h"

#include <QJsonDocument>
#include <QJsonObject>


Server::Server(QObject *parent) : QObject(parent), server(new QTcpServer(this)) {
    commandHandler = new CommandHandler(this);
}

Server::~Server() {
    delete commandHandler;
}


bool Server::startServer(const quint16 port) const {
    if (!server->listen(QHostAddress::Any, port)) {
        qDebug() << "failed to start server. error: " << server->errorString();
        return false;
    }

    connect(server, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
    qDebug() << "server is listening on port" << port;
    return true;
}

void Server::handleNewConnection() {
    QTcpSocket *socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(handleClientDisconnected()));
    clients.append(socket);
    qDebug() << "new client connected: " << socket->peerAddress().toString();
}

void Server::handleReadyRead() const {
    const auto socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) return;

    while (socket->canReadLine()) {
        const QByteArray jsonData = socket->readLine().trimmed();
        qDebug() << "got data from client:" << jsonData;

        const QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (doc.isNull()) {
            qDebug() << "failed to parse json";
            continue;
        }

        const QJsonObject json = doc.object();

        commandHandler->processCommand(socket, json);
    }
}

void Server::handleClientDisconnected() {
    const auto socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) return;

    // check if user was authenticated
    if (connectedUsers.contains(socket)) {
        const QString username = connectedUsers[socket];
        // broadcast leaving
        broadcastSystemMessage(username + " has left the chat");
        connectedUsers.remove(socket);

        // send updated user list
        QJsonObject usersResponse;
        usersResponse["type"] = "online_users";
        QJsonArray usersArray;
        for (const QString &user : getOnlineUsers()) {
            usersArray.append(user);
        }
        usersResponse["users"] = usersArray;
        broadcastJSON(usersResponse);
    }

    clients.removeOne(socket);
    socket->deleteLater();
    qDebug() << "client disconnected";
}

void Server::sendResponse(QTcpSocket *socket, const QJsonObject &jsonResponse) {
    qDebug() << "sending response: " << jsonResponse;
    socket->write(QJsonDocument(jsonResponse).toJson(QJsonDocument::Compact) + "\n");
    socket->flush();
}

void Server::sendCommandResponse(QTcpSocket *socket, const CommandResponse &response) {
    QJsonObject jsonResponse;
    jsonResponse["status"] = response.status;
    jsonResponse["message"] = response.message;
    sendResponse(socket, jsonResponse);
}

bool Server::isUserOnline(const QString &username) const {
    return connectedUsers.values().contains(username);
}

void Server::addConnectedUser(QTcpSocket *socket, const QString &username) {
    connectedUsers[socket] = username;
}

QString Server::getUserBySocket(QTcpSocket *socket) const {
    return connectedUsers.value(socket, QString());
}

void Server::broadcastJSON(const QJsonObject &json) {
    const QByteArray data = QJsonDocument(json).toJson(QJsonDocument::Compact) + "\n";
    for (QTcpSocket *client: clients) {
        if (connectedUsers.contains(client)) {
            client->write(data);
            client->flush();
        }
    }
}

void Server::broadcastMessage(const QString &sender, const QString &message) {
    QJsonObject jsonMessage;
    jsonMessage["type"] = "message";
    jsonMessage["sender"] = sender;
    jsonMessage["content"] = message;
    jsonMessage["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    broadcastJSON(jsonMessage);
}

void Server::broadcastSystemMessage(const QString &message) {
    QJsonObject jsonMessage;
    jsonMessage["type"] = "system";
    jsonMessage["content"] = message;
    jsonMessage["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    broadcastJSON(jsonMessage);
}

QStringList Server::getOnlineUsers() const {
    QStringList users = connectedUsers.values();
    users.sort();
    return users;
}
