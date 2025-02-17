#include "server.h"
#include "database/database.h"

#include <QJsonDocument>
#include <QJsonObject>


Server::Server(QObject *parent) : QObject(parent), server(new QTcpServer(this)) {
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

void Server::handleReadyRead() {
    const auto socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) return;

    const QByteArray jsonData = socket->readAll();

    // trying to parse json
    const QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull()) {
        qDebug() << "failed to read json data";
        return;
    }

    qDebug() << "received data from client" << jsonData;

    const QJsonObject json = doc.object();
    const auto command = json["command"].toString();

    CommandResponse response;
    if (command == "login") {
        response = handleLoginCommand(socket, json);
    } else if (command == "register") {
        response = handleRegisterCommand(json);
    } else {
        response = {"error", "unknown command"};
    }

    sendResponse(socket, response);
}

void Server::handleClientDisconnected() {
    const auto socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) return;

    clients.removeOne(socket);
    socket->deleteLater();
    qDebug() << "client disconnected";
}

Server::CommandResponse Server::handleLoginCommand(QTcpSocket *socket, const QJsonObject &json) {
    const QString username = json["username"].toString();
    const QString password = json["password"].toString();

    if (!Database::instance().checkCredentials(username, password)) {
        return {"error", "invalid login or password"};
    }

    if (isUserOnline(username)) {
        return {"error", "user already online"};
    }

    connectedUsers[socket] = username;
    qDebug() << "user connected: " << username;
    return {"ok", "success"};
}

Server::CommandResponse Server::handleRegisterCommand(const QJsonObject &json) {
    const QString username = json["username"].toString();
    const QString password = json["password"].toString();

    if (Database::instance().registerUser(username, password)) {
        qDebug() << "user registered";
        return {"ok", "success"};
    }

    return {"error", "unsuccessful request"};
}


void Server::sendResponse(QTcpSocket *socket, const CommandResponse &response) {
    QJsonObject jsonResponse;
    jsonResponse["status"] = response.status;
    jsonResponse["message"] = response.message;
    socket->write(QJsonDocument(jsonResponse).toJson(QJsonDocument::Compact));
}

bool Server::isUserOnline(const QString &username) const {
    return connectedUsers.values().contains(username);
}
