#include "apiservice.h"
#include <QNetworkProxy>
#include <QDebug>

// Инициализация статических членов
ApiService* ApiService::instance = nullptr;
QMutex ApiService::mutex;

// Реализация метода получения инстанса (синглтон)
ApiService* ApiService::getInstance() {
    // Используем double-checked locking для эффективности
    if (!instance) {
        mutex.lock();
        if (!instance) {
            instance = new ApiService();
        }
        mutex.unlock();
    }
    return instance;
}

// Приватный конструктор
ApiService::ApiService(QObject *parent)
    : QObject(parent), socket(new QTcpSocket(this)) {

    // Подключаем сигналы сокета
    connect(socket, &QTcpSocket::connected, this, &ApiService::handleConnected);
    connect(socket, &QTcpSocket::disconnected, this, &ApiService::handleDisconnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &ApiService::handleError);
    connect(socket, &QTcpSocket::readyRead, this, &ApiService::handleReadyRead);

    qDebug() << "ApiService создан";
}

ApiService::~ApiService() {
    if (socket) {
        socket->disconnectFromHost();
        socket->deleteLater();
    }
    qDebug() << "ApiService уничтожен";
}

// Подключение к серверу
void ApiService::connectToServer(const QString& host, quint16 port) {
    qDebug() << "Попытка подключения к" << host << ":" << port;

    // Отключаемся, если уже подключены
    if (socket->state() != QAbstractSocket::UnconnectedState) {
        qDebug() << "Сокет уже подключен, отключаемся";
        socket->disconnectFromHost();
        socket->waitForDisconnected();
    }

    socket->setProxy(QNetworkProxy::NoProxy);
    socket->connectToHost(host, port);
}

// Отправка запроса авторизации
void ApiService::sendLoginRequest(const QString& username, const QString& password) {
    QJsonObject request;
    request["command"] = "login";
    request["username"] = username;
    request["password"] = password;
    sendJsonRequest(request);
}

// Отправка запроса регистрации
void ApiService::sendRegisterRequest(const QString& username, const QString& password) {
    QJsonObject request;
    request["command"] = "register";
    request["username"] = username;
    request["password"] = password;
    sendJsonRequest(request);
}

// Отправка сообщения
void ApiService::sendMessage(const QString& message) {
    QJsonObject request;
    request["command"] = "send_message";
    request["message"] = message;
    sendJsonRequest(request);
}

// Запрос истории сообщений
void ApiService::requestMessageHistory(int limit) {
    QJsonObject request;
    request["command"] = "get_history";
    request["limit"] = limit;
    sendJsonRequest(request);
}

// Запрос списка онлайн пользователей
void ApiService::requestOnlineUsers() {
    QJsonObject request;
    request["command"] = "get_online_users";
    sendJsonRequest(request);
}

// Проверка статуса подключения
bool ApiService::isConnected() const {
    return socket->state() == QAbstractSocket::ConnectedState;
}

// Отправка JSON-запроса
void ApiService::sendJsonRequest(const QJsonObject& request) {
    QJsonDocument doc(request);
    socket->write(doc.toJson(QJsonDocument::Compact) + "\n");
}

// Обработка входящих данных
void ApiService::handleReadyRead() {
    qDebug() << "Получены данные от сервера";
    while (socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();
        qDebug() << "Получено:" << QString(line);

        // Парсинг JSON
        QJsonParseError parseError;
        QJsonDocument response = QJsonDocument::fromJson(line, &parseError);

        // Проверка ошибок парсинга
        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "Ошибка парсинга JSON:" << parseError.errorString();
            emit connectionError("Ошибка парсинга JSON");
            return;
        }

        qDebug() << "JSON разобран:" << response.toJson();
        processResponse(response);
    }
}


// Обработка ответа сервера
void ApiService::processResponse(const QJsonDocument& response) {
    QJsonObject obj = response.object();
    qDebug() << "Обработка ответа:" << response.toJson();

    // Проверка на наличие полей status и message
    if (obj.contains("status")) {
        QString status = obj["status"].toString();
        QString message = obj["message"].toString();

        qDebug() << "Статус ответа:" << status << message;

        if (status == "ok") {
            // Успешный логин
            if (message.contains("success login")) {
                qDebug() << "Отправка сигнала loginSuccess";
                emit loginSuccess("Авторизация успешна");
            }
            // Успешная регистрация
            else if (message.contains("success register")) {
                emit registerSuccess("Регистрация успешна");
            }
        }
        else if (status == "error") {
            // Ошибки логина
            if (message.contains("invalid login") || message.contains("already online")) {
                emit loginError(message.contains("already online") ?
                                    "Этот пользователь уже онлайн" :
                                    "Неверный логин или пароль");
            }
            // Ошибка регистрации
            else if (message.contains("unsuccessful register")) {
                emit registerError("Не удалось зарегистрироваться");
            }
            // Ошибка отправки сообщения
            else if (message.contains("empty message") || message.contains("not authenticated")) {
                emit messageSendError(message);
            }
            // Прочие ошибки
            else {
                emit connectionError(message);
            }
        }
        return;
    }

    // Проверка на тип сообщения для чата
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
        // Список пользователей онлайн
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

// Обработка успешного подключения
void ApiService::handleConnected() {
    qDebug() << "Подключение установлено";
    emit connected();
}

// Обработка разрыва соединения
void ApiService::handleDisconnected() {
    qDebug() << "Соединение разорвано";
    emit connectionError("Соединение с сервером потеряно");
}

// Обработка ошибок сокета
void ApiService::handleError(QAbstractSocket::SocketError error) {
    qDebug() << "Ошибка сокета:" << error << socket->errorString();
    emit connectionError("Ошибка сети: " + socket->errorString());
}
