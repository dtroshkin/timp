#ifndef APISERVICE_H
#define APISERVICE_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMutex>

/**
 * @brief Класс ApiService реализует API-клиент для общения с сервером.
 *
 * Поддерживает авторизацию, регистрацию, отправку сообщений, получение истории чата и списка онлайн-пользователей.
 * Работает как синглтон, чтобы обеспечить единый доступ ко всем сетевым операциям из разных частей UI.
 */
class ApiService : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Получает экземпляр синглтона ApiService.
     * @return Указатель на ApiService.
     */
    static ApiService* getInstance();

    /// Удаляем конструктор копирования
    ApiService(const ApiService&) = delete;
    /// Удаляем оператор присваивания
    ApiService& operator=(const ApiService&) = delete;

    /**
     * @brief Подключается к серверу по адресу и порту.
     * @param host IP-адрес или хост.
     * @param port Порт подключения.
     */
    void connectToServer(const QString& host, quint16 port);

    /**
     * @brief Отправляет запрос на вход.
     * @param username Имя пользователя.
     * @param password Пароль.
     */
    void sendLoginRequest(const QString& username, const QString& password);

    /**
     * @brief Отправляет запрос на регистрацию.
     * @param username Имя пользователя.
     * @param password Пароль.
     */
    void sendRegisterRequest(const QString& username, const QString& password);

    /**
     * @brief Отправляет сообщение на сервер.
     * @param message Текст сообщения.
     */
    void sendMessage(const QString& message);

    /**
     * @brief Запрашивает историю сообщений.
     * @param limit Количество сообщений (по умолчанию 50).
     */
    void requestMessageHistory(int limit = 50);

    /**
     * @brief Запрашивает список онлайн-пользователей.
     */
    void requestOnlineUsers();

    /**
     * @brief Проверяет, установлено ли соединение с сервером.
     * @return true если подключено, иначе false.
     */
    bool isConnected() const;

signals:
    // --- Сигналы, связанные с авторизацией ---
    /**
     * @brief Ошибка соединения с сервером.
     * @param error Текст ошибки.
     */
    void connectionError(const QString& error);

    /**
     * @brief Успешный вход.
     * @param message Сопровождающее сообщение.
     */
    void loginSuccess(const QString& message);

    /**
     * @brief Ошибка при входе.
     * @param error Описание ошибки.
     */
    void loginError(const QString& error);

    /**
     * @brief Успешная регистрация.
     * @param message Сопровождающее сообщение.
     */
    void registerSuccess(const QString& message);

    /**
     * @brief Ошибка регистрации.
     * @param error Описание ошибки.
     */
    void registerError(const QString& error);

    /**
     * @brief Сигнал об успешном подключении к серверу.
     */
    void connected();

    // --- Сигналы, связанные с чатом ---
    /**
     * @brief Получено новое сообщение от пользователя.
     * @param sender Отправитель.
     * @param content Содержание.
     * @param timestamp Метка времени.
     */
    void messageReceived(const QString& sender, const QString& content, const QString& timestamp);

    /**
     * @brief Получено системное сообщение.
     * @param content Содержание.
     * @param timestamp Метка времени.
     */
    void systemMessageReceived(const QString& content, const QString& timestamp);

    /**
     * @brief Получена история сообщений.
     * @param messages Массив сообщений.
     */
    void historyReceived(const QJsonArray& messages);

    /**
     * @brief Получен список онлайн-пользователей.
     * @param users Список имён.
     */
    void onlineUsersReceived(const QStringList& users);

    /**
     * @brief Ошибка при отправке сообщения.
     * @param error Описание ошибки.
     */
    void messageSendError(const QString& error);

private slots:
    /**
     * @brief Обрабатывает событие успешного соединения.
     */
    void handleConnected();

    /**
     * @brief Обрабатывает отключение от сервера.
     */
    void handleDisconnected();

    /**
     * @brief Обрабатывает сетевую ошибку.
     * @param error Тип ошибки.
     */
    void handleError(QAbstractSocket::SocketError error);

    /**
     * @brief Обрабатывает получение данных от сервера.
     */
    void handleReadyRead();

private:
    /// Приватный конструктор для реализации синглтона
    explicit ApiService(QObject *parent = nullptr);

    /// Деструктор
    ~ApiService();

    static ApiService* instance; ///< Статический указатель на экземпляр
    static QMutex mutex;         ///< Мьютекс для потокобезопасности

    QTcpSocket* socket;          ///< TCP-сокет клиента

    /**
     * @brief Отправляет JSON-запрос на сервер.
     * @param request JSON-объект запроса.
     */
    void sendJsonRequest(const QJsonObject& request);

    /**
     * @brief Обрабатывает JSON-ответ от сервера.
     * @param response Объект ответа.
     */
    void processResponse(const QJsonDocument& response);
};

#endif // APISERVICE_H
