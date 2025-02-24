#include "networkmanager.h"
#include <QNetworkProxy>
#include <QDebug>

// тут создаем наш сокет и подключаем все сигналы от него к нашим методам
// нужно подключить все обработчики чтобы ничего не пропустить
NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent), socket(new QTcpSocket(this)) {
    // коннектим все события от сокета к нашим обработчикам
    connect(socket, &QTcpSocket::connected, this, &NetworkManager::handleConnected);
    connect(socket, &QTcpSocket::disconnected, this, &NetworkManager::handleDisconnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &NetworkManager::handleError);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::handleReadyRead);
}

// подключаемся к серверу по хосту и порту
// отключаем прокси, чтобы избежать проблем
void NetworkManager::connectToServer(const QString& host, quint16 port) {
    qDebug() << "пытаемся подключиться к" << host << ":" << port;

    // если уже подключены - отключаемся
    if (socket->state() != QAbstractSocket::UnconnectedState) {
        qDebug() << "сокет уже подключен, отключаемся";
        socket->disconnectFromHost();
        socket->waitForDisconnected();
    }

    socket->setProxy(QNetworkProxy::NoProxy);
    socket->connectToHost(host, port);
}


// запрос на логин - формируем json с нужными полями и отправляем
void NetworkManager::sendLoginRequest(const QString& username, const QString& password) {
    QJsonObject request;
    request["command"] = "login";
    request["username"] = username;
    request["password"] = password;
    sendJsonRequest(request);
}

// запрос на регистрацию - такой же как логин, только команда другая
void NetworkManager::sendRegisterRequest(const QString& username, const QString& password) {
    QJsonObject request;
    request["command"] = "register";
    request["username"] = username;
    request["password"] = password;
    sendJsonRequest(request);
}

// Отправка сообщения в чат
void NetworkManager::sendMessage(const QString& message) {
    QJsonObject request;
    request["command"] = "send_message";
    request["message"] = message;
    sendJsonRequest(request);
}

// Запрос истории сообщений
void NetworkManager::requestMessageHistory(int limit) {
    QJsonObject request;
    request["command"] = "get_history";
    request["limit"] = limit;
    sendJsonRequest(request);
}

// Запрос списка онлайн пользователей
void NetworkManager::requestOnlineUsers() {
    QJsonObject request;
    request["command"] = "get_online_users";
    sendJsonRequest(request);
}

// Проверка подключения к серверу
bool NetworkManager::isConnected() const {
    return socket->state() == QAbstractSocket::ConnectedState;
}

// отправка любого json-запроса на сервер
// сериализуем и добавляем перенос строки в конце
void NetworkManager::sendJsonRequest(const QJsonObject& request) {
    QJsonDocument doc(request);
    socket->write(doc.toJson(QJsonDocument::Compact) + "\n");
}

// ГЛАВНЫЙ метод - обрабатывает данные от сервера
// читаем построчно, парсим json, передаем в обработчик
void NetworkManager::handleReadyRead() {
    qDebug() << "ПРИШЛИ ДАННЫЕ от сервера!";
    while (socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();
        qDebug() << "ПОЛУЧИЛ:" << QString(line);

        // пробуем парсить как json
        QJsonParseError parseError;
        QJsonDocument response = QJsonDocument::fromJson(line, &parseError);

        // если не получилось распарсить - логируем и шлем ошибку
        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "ФЕЙЛ ПАРСИНГА:" << parseError.errorString();
            emit connectionError("Ошибка парсинга JSON");
            return;
        }

        qDebug() << "JSON РАСПАРСЕН:" << response.toJson();
        processResponse(response);
    }
}

// разбираем ответ сервера и эмитим нужные сигналы
// в зависимости от статуса и сообщения
void NetworkManager::processResponse(const QJsonDocument& response) {
    QJsonObject obj = response.object();
    qDebug() << "ПРОЦЕССИМ ОТВЕТ:" << response.toJson();
    // Проверяем наличие полей status и message для определения типа сообщения
    if (obj.contains("status")) {
        QString status = obj["status"].toString();
        QString message = obj["message"].toString();

        qDebug() << "обработка ответа:" << status << message;


        if (status == "ok") {
            // если успешный логин
            if (message.contains("success login")) {
                qDebug() << "эмитим loginSuccess";
                emit loginSuccess("авторизация успешна");
            }
            // если успешная регистрация
            else if (message.contains("success register")) {
                emit registerSuccess("регистрация успешна");
            }
        }
        else if (status == "error") {
            // ошибки логина (неверные данные или юзер уже в системе)
            if (message.contains("invalid login") || message.contains("already online")) {
                emit loginError(message.contains("already online") ?
                                    "этот юзер уже онлайн" :
                                    "неверный логин или пароль");
            }
            // ошибка регистрации
            else if (message.contains("unsuccessful register")) {
                emit registerError("не удалось зарегистрироваться");
            }
            // ошибка отправки сообщения
            else if (message.contains("empty message") || message.contains("not authenticated")) {
                emit messageSendError(message);
            }
            // все остальные ошибки
            else {
                emit connectionError(message);
            }
        }
        return;
    }

    // Проверяем тип сообщения для чата
    if (obj.contains("type")) {
        QString type = obj["type"].toString();

        // Обычное сообщение
        if (type == "message") {
            QString sender = obj["sender"].toString();
            QString content = obj["content"].toString();
            QString timestamp = obj["timestamp"].toString();
            emit messageReceived(sender, content, timestamp);
        }
        // Системное сообщение
        else if (type == "system") {
            QString content = obj["content"].toString();
            QString timestamp = obj["timestamp"].toString();
            emit systemMessageReceived(content, timestamp);
        }
        // История сообщений
        else if (type == "history") {
            QJsonArray messages = obj["messages"].toArray();
            emit historyReceived(messages);
        }
        // Список онлайн-пользователей
        else if (type == "online_users") {
            QJsonArray usersArray = obj["users"].toArray();
            QStringList users;
            for (const QJsonValue &value : usersArray) {
                users.append(value.toString());
            }
            emit onlineUsersReceived(users);
        }
    }
}

// когда подключились к серверу - эмитим сигнал
void NetworkManager::handleConnected() {
    emit connected();
}

// если соединение оборвалось - эмитим ошибку
void NetworkManager::handleDisconnected() {
    emit connectionError("Соединение с сервером потеряно");
}

// обрабатываем сетевые ошибки и эмитим сообщение
void NetworkManager::handleError(QAbstractSocket::SocketError error) {
    emit connectionError("Ошибка сети: " + socket->errorString());
}
