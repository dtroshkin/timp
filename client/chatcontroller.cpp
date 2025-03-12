#include "chatcontroller.h"
#include <QDebug>

ChatController::ChatController(QObject *parent)
    : QObject(parent),
    apiService(ApiService::getInstance()),
    loggedIn(false),
    currentUser("")
{
    // Подключаем сигналы ApiService к слотам контроллера
    connectApiSignals();
    qDebug() << "ChatController создан";
}

ChatController::~ChatController() {
    qDebug() << "ChatController уничтожен";
}

void ChatController::connectApiSignals() {
    // Подключаем сигналы ApiService к слотам контроллера
    connect(apiService, &ApiService::connected, this, &ChatController::onApiConnected);
    connect(apiService, &ApiService::connectionError, this, &ChatController::onApiConnectionError);
    connect(apiService, &ApiService::loginSuccess, this, &ChatController::onApiLoginSuccess);
    connect(apiService, &ApiService::loginError, this, &ChatController::onApiLoginError);
    connect(apiService, &ApiService::registerSuccess, this, &ChatController::onApiRegisterSuccess);
    connect(apiService, &ApiService::registerError, this, &ChatController::onApiRegisterError);
    connect(apiService, &ApiService::messageReceived, this, &ChatController::onApiMessageReceived);
    connect(apiService, &ApiService::systemMessageReceived, this, &ChatController::onApiSystemMessageReceived);
    connect(apiService, &ApiService::historyReceived, this, &ChatController::onApiHistoryReceived);
    connect(apiService, &ApiService::onlineUsersReceived, this, &ChatController::onApiOnlineUsersReceived);
    connect(apiService, &ApiService::messageSendError, this, &ChatController::onApiMessageSendError);
}

// Методы для взаимодействия с сервером через ApiService

void ChatController::connectToServer(const QString& host, quint16 port) {
    qDebug() << "ChatController: Подключение к серверу" << host << ":" << port;
    apiService->connectToServer(host, port);
}

void ChatController::login(const QString& username, const QString& password) {
    qDebug() << "ChatController: Отправка запроса на логин для" << username;
    currentUser = username; // Сохраняем имя пользователя (будет использоваться, если логин успешен)
    apiService->sendLoginRequest(username, password);
}

void ChatController::registerUser(const QString& username, const QString& password) {
    qDebug() << "ChatController: Отправка запроса на регистрацию для" << username;
    apiService->sendRegisterRequest(username, password);
}

void ChatController::sendMessage(const QString& message) {
    qDebug() << "ChatController: Отправка сообщения";
    if (!isLoggedIn()) {
        emit messageFailedToSend("Вы не авторизованы");
        return;
    }

    apiService->sendMessage(message);
}

void ChatController::requestHistory(int limit) {
    qDebug() << "ChatController: Запрос истории сообщений, лимит:" << limit;
    apiService->requestMessageHistory(limit);
}

void ChatController::requestOnlineUsers() {
    qDebug() << "ChatController: Запрос списка пользователей онлайн";
    apiService->requestOnlineUsers();
}

bool ChatController::isConnected() const {
    return apiService->isConnected();
}

bool ChatController::isLoggedIn() const {
    return loggedIn;
}

QString ChatController::getCurrentUser() const {
    return currentUser;
}

// Обработчики событий от ApiService

void ChatController::onApiConnected() {
    qDebug() << "ChatController: Соединение установлено";
    emit connectionEstablished();
}

void ChatController::onApiConnectionError(const QString& error) {
    qDebug() << "ChatController: Ошибка соединения:" << error;
    loggedIn = false; // Сбрасываем флаг авторизации при ошибке соединения
    emit connectionFailed(error);
}

void ChatController::onApiLoginSuccess(const QString& message) {
    qDebug() << "ChatController: Успешный логин для" << currentUser;
    loggedIn = true;
    emit loginSuccessful(currentUser);
}

void ChatController::onApiLoginError(const QString& error) {
    qDebug() << "ChatController: Ошибка логина:" << error;
    loggedIn = false;
    currentUser = ""; // Сбрасываем текущего пользователя
    emit loginFailed(error);
}


void ChatController::onApiRegisterSuccess(const QString& message) {
    qDebug() << "ChatController: Успешная регистрация";
    emit registrationSuccessful();
}

void ChatController::onApiRegisterError(const QString& error) {
    qDebug() << "ChatController: Ошибка регистрации:" << error;
    emit registrationFailed(error);
}

void ChatController::onApiMessageReceived(const QString& sender, const QString& content, const QString& timestamp) {
    qDebug() << "ChatController: Получено сообщение от" << sender << ":" << content;
    emit messageReceived(sender, content, timestamp);
}

void ChatController::onApiSystemMessageReceived(const QString& content, const QString& timestamp) {
    qDebug() << "ChatController: Получено системное сообщение:" << content;
    emit systemMessageReceived(content, timestamp);
}

void ChatController::onApiHistoryReceived(const QJsonArray& messages) {
    qDebug() << "ChatController: Получена история сообщений";
    emit historyLoaded(messages);
}

void ChatController::onApiOnlineUsersReceived(const QStringList& users) {
    qDebug() << "ChatController: Получен список пользователей онлайн";
    emit onlineUsersUpdated(users);
}

void ChatController::onApiMessageSendError(const QString& error) {
    qDebug() << "ChatController: Ошибка отправки сообщения:" << error;
    emit messageFailedToSend(error);
}
