#ifndef CHATCONTROLLER_H
#define CHATCONTROLLER_H

#include <QObject>
#include <QJsonArray>
#include <QStringList>
#include "apiservice.h"

/**
 * @brief Класс ChatController управляет логикой взаимодействия между UI и сервером.
 *
 * Используется для:
 * - Управления соединением и авторизацией
 * - Пересылки сообщений через ApiService
 * - Обработки результатов и генерации сигналов для UI
 */
class ChatController : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Конструктор ChatController.
     * @param parent Родительский QObject.
     */
    explicit ChatController(QObject *parent = nullptr);

    /// Деструктор
    ~ChatController();

    /**
     * @brief Подключается к серверу.
     * @param host IP-адрес или домен сервера.
     * @param port Порт подключения.
     */
    void connectToServer(const QString& host, quint16 port);

    /**
     * @brief Выполняет вход пользователя.
     * @param username Имя пользователя.
     * @param password Пароль.
     */
    void login(const QString& username, const QString& password);

    /**
     * @brief Выполняет регистрацию нового пользователя.
     * @param username Имя пользователя.
     * @param password Пароль.
     */
    void registerUser(const QString& username, const QString& password);

    /**
     * @brief Отправляет текстовое сообщение.
     * @param message Текст сообщения.
     */
    void sendMessage(const QString& message);

    /**
     * @brief Запрашивает историю сообщений.
     * @param limit Количество сообщений (по умолчанию 50).
     */
    void requestHistory(int limit = 50);

    /**
     * @brief Запрашивает список онлайн-пользователей.
     */
    void requestOnlineUsers();

    /**
     * @brief Проверяет, установлено ли соединение с сервером.
     * @return true если подключено.
     */
    bool isConnected() const;

    /**
     * @brief Проверяет, авторизован ли пользователь.
     * @return true если да.
     */
    bool isLoggedIn() const;

    /**
     * @brief Возвращает имя текущего пользователя.
     * @return Строка с именем пользователя.
     */
    QString getCurrentUser() const;

signals:
    // --- Сигналы для UI ---
    /**
     * @brief Успешное соединение с сервером.
     */
    void connectionEstablished();

    /**
     * @brief Ошибка соединения.
     * @param error Сообщение об ошибке.
     */
    void connectionFailed(const QString& error);

    /**
     * @brief Успешный вход.
     * @param username Имя вошедшего пользователя.
     */
    void loginSuccessful(const QString& username);

    /**
     * @brief Ошибка входа.
     * @param error Сообщение об ошибке.
     */
    void loginFailed(const QString& error);

    /**
     * @brief Успешная регистрация.
     */
    void registrationSuccessful();

    /**
     * @brief Ошибка регистрации.
     * @param error Сообщение об ошибке.
     */
    void registrationFailed(const QString& error);

    /**
     * @brief Подтверждение отправки сообщения.
     */
    void messageSent();

    /**
     * @brief Ошибка при отправке сообщения.
     * @param error Сообщение об ошибке.
     */
    void messageFailedToSend(const QString& error);

    /**
     * @brief Получено новое сообщение.
     */
    void messageReceived(const QString& sender, const QString& content, const QString& timestamp);

    /**
     * @brief Получено системное сообщение.
     */
    void systemMessageReceived(const QString& content, const QString& timestamp);

    /**
     * @brief Загружена история сообщений.
     */
    void historyLoaded(const QJsonArray& messages);

    /**
     * @brief Получен список онлайн-пользователей.
     */
    void onlineUsersUpdated(const QStringList& users);

private slots:
    // --- Слоты для приёма сигналов от ApiService ---
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
    ApiService* apiService; ///< Указатель на экземпляр ApiService
    bool loggedIn;          ///< Флаг авторизации
    QString currentUser;    ///< Имя текущего пользователя

    /**
     * @brief Подключает сигналы ApiService к слотам ChatController.
     */
    void connectApiSignals();
};

#endif // CHATCONTROLLER_H
