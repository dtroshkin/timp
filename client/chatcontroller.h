#ifndef CHATCONTROLLER_H
#define CHATCONTROLLER_H

#include <QObject>
#include <QJsonArray>
#include <QStringList>
#include "apiservice.h"

// Класс контроллера чата, управляющий бизнес-логикой
class ChatController : public QObject {
    Q_OBJECT
public:
    explicit ChatController(QObject *parent = nullptr);
    ~ChatController();

    // Методы для работы с сервером через ApiService
    void connectToServer(const QString& host, quint16 port);
    void login(const QString& username, const QString& password);
    void registerUser(const QString& username, const QString& password);
    void sendMessage(const QString& message);
    void requestHistory(int limit = 50);
    void requestOnlineUsers();

    // Проверка состояний
    bool isConnected() const;
    bool isLoggedIn() const;

    // Получение текущего пользователя
    QString getCurrentUser() const;

signals:
    // Сигналы для UI
    void connectionEstablished();
    void connectionFailed(const QString& error);
    void loginSuccessful(const QString& username);
    void loginFailed(const QString& error);
    void registrationSuccessful();
    void registrationFailed(const QString& error);
    void messageSent();
    void messageFailedToSend(const QString& error);
    void messageReceived(const QString& sender, const QString& content, const QString& timestamp);
    void systemMessageReceived(const QString& content, const QString& timestamp);
    void historyLoaded(const QJsonArray& messages);
    void onlineUsersUpdated(const QStringList& users);

private slots:
    // Обработчики событий от ApiService
    void onApiConnected();
    void onApiConnectionError(const QString& error);
    void onApiLoginSuccess(const QString& message);
    void onApiLoginError(const QString& error);
    void onApiRegisterSuccess(const QString& message);
    void onApiRegisterError(const QString& error);
    void onApiMessageReceived(const QString& sender, const QString& content, const QString& timestamp);
    void onApiSystemMessageReceived(const QString& content, const QString& timestamp);
    void onApiHistoryReceived(const QJsonArray& messages);
    void onApiOnlineUsersReceived(const QStringList& users);
    void onApiMessageSendError(const QString& error);

private:
    ApiService* apiService;
    bool loggedIn;
    QString currentUser;

    // Подключение сигналов ApiService
    void connectApiSignals();
};

#endif // CHATCONTROLLER_H
