#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class NetworkManager : public QObject {
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

    // Базовые методы подключения и авторизации
    void connectToServer(const QString& host, quint16 port);
    void sendLoginRequest(const QString& username, const QString& password);
    void sendRegisterRequest(const QString& username, const QString& password);

    // Новые методы для работы с чатом
    void sendMessage(const QString& message);
    void requestMessageHistory(int limit = 50);
    void requestOnlineUsers();

    // Метод для проверки состояния подключения
    bool isConnected() const;

signals:
    // Сигналы регистрации и авторизации (уже были)
    void connectionError(const QString& error);
    void loginSuccess(const QString& message);
    void loginError(const QString& error);
    void registerSuccess(const QString& message);
    void registerError(const QString& error);
    void connected();

    // Новые сигналы для событий чата
    void messageReceived(const QString& sender, const QString& content, const QString& timestamp);
    void systemMessageReceived(const QString& content, const QString& timestamp);
    void historyReceived(const QJsonArray& messages);
    void onlineUsersReceived(const QStringList& users);
    void messageSendError(const QString& error);

private slots:
    void handleConnected();
    void handleDisconnected();
    void handleError(QAbstractSocket::SocketError error);
    void handleReadyRead();

private:
    QTcpSocket* socket;
    void sendJsonRequest(const QJsonObject& request);
    void processResponse(const QJsonDocument& response);
};

#endif // NETWORKMANAGER_H
