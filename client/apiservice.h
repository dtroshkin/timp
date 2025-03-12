#ifndef APISERVICE_H
#define APISERVICE_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMutex>

// класс для работы с API сервера
class ApiService : public QObject {
    Q_OBJECT
public:
    // синглтон - публичный статический метод для получения инстанса
    static ApiService* getInstance();
    // запрет копирования и присваивания
    ApiService(const ApiService&) = delete;
    ApiService& operator=(const ApiService&) = delete;

    // методы для работы с сервером
    void connectToServer(const QString& host, quint16 port);
    void sendLoginRequest(const QString& username, const QString& password);
    void sendRegisterRequest(const QString& username, const QString& password);
    void sendMessage(const QString& message);
    void requestMessageHistory(int limit = 50);
    void requestOnlineUsers();

    // проверка соединения
    bool isConnected() const;

signals:
    // сигналы для авторизации
    void connectionError(const QString& error);
    void loginSuccess(const QString& message);
    void loginError(const QString& error);
    void registerSuccess(const QString& message);
    void registerError(const QString& error);
    void connected();

    // сигналы для чата
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
    // приватный конструктор (для синглтона)
    explicit ApiService(QObject *parent = nullptr);
    ~ApiService();

    // статический указатель на инстанс
    static ApiService* instance;
    // мьютекс для безопасного создания инстанса
    static QMutex mutex;

    QTcpSocket* socket;
    void sendJsonRequest(const QJsonObject& request);
    void processResponse(const QJsonDocument& response);
};

#endif // APISERVICE_H
