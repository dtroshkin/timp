#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <QTcpSocket>
#include <QJsonObject>
#include <functional>
#include <QString>
#include <QHash>

class Server;

/**
 * @brief Структура CommandResponse содержит ответ сервера на команду.
 */
struct CommandResponse {
    QString status;  ///< Статус ответа (например, "ok" или "error")
    QString message; ///< Сообщение или дополнительная информация
};

/**
 * @brief Тип функции-обработчика команды.
 *
 * Принимает указатель на сервер, сокет клиента и JSON-объект команды.
 */
using CommandHandlerFunction = std::function<void(Server *, QTcpSocket *, const QJsonObject &)>;

/**
 * @brief Класс CommandHandler обрабатывает команды клиентов в формате JSON.
 *
 * Каждая команда (login, register, send_message, get_history, get_online_users)
 * имеет соответствующую функцию-обработчик. Также используется механизм регистрации
 * хендлеров в хэш-таблице для динамического вызова.
 */
class CommandHandler {
public:
    /**
     * @brief Конструктор CommandHandler.
     * @param serverInstance Указатель на объект сервера.
     */
    explicit CommandHandler(Server *serverInstance);

    /**
     * @brief Регистрирует все доступные обработчики команд.
     */
    void registerHandlers();

    /**
     * @brief Обрабатывает входящую команду от клиента.
     *
     * Ищет соответствующий хендлер по ключу "type" и вызывает его.
     * @param socket Сокет клиента.
     * @param json JSON-объект, содержащий команду.
     */
    void processCommand(QTcpSocket *socket, const QJsonObject &json);

private:
    Server *server; ///< Указатель на объект сервера

    /// Хэш-таблица соответствий команд и их обработчиков
    QHash<QString, CommandHandlerFunction> handlers;

    /**
     * @brief Обрабатывает команду входа (login).
     */
    static void handleLogin(Server *server, QTcpSocket *socket, const QJsonObject &json);

    /**
     * @brief Обрабатывает команду регистрации (register).
     */
    static void handleRegister(Server *server, QTcpSocket *socket, const QJsonObject &json);

    /**
     * @brief Обрабатывает отправку сообщения (send_message).
     */
    static void handleSendMessage(Server *server, QTcpSocket *socket, const QJsonObject &json);

    /**
     * @brief Обрабатывает запрос истории сообщений (get_history).
     */
    static void handleGetHistory(Server *server, QTcpSocket *socket, const QJsonObject &json);

    /**
     * @brief Обрабатывает запрос списка онлайн-пользователей (get_online_users).
     */
    static void handleGetOnlineUsers(Server *server, QTcpSocket *socket, const QJsonObject &json);

    /**
     * @brief Рассылает всем клиентам актуальный список онлайн-пользователей.
     */
    static void broadcastOnlineUsers(Server *server);
};

#endif // COMMAND_HANDLER_H
