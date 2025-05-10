#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

#include "command_handler.h"

class CommandHandler;

/**
 * @brief Класс Server реализует TCP-сервер для обработки клиентских соединений.
 *
 * Сервер:
 * - Принимает подключения по TCP
 * - Обменивается JSON-сообщениями с клиентами
 * - Хранит список подключённых пользователей
 * - Делегирует обработку команд объекту CommandHandler
 */
class Server final : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Конструктор сервера.
     * @param parent Родительский QObject.
     */
    explicit Server(QObject *parent = nullptr);

    /// Деструктор сервера
    ~Server() override;

    /**
     * @brief Запускает сервер на заданном порту.
     * @param port Порт, на котором будет слушать сервер. По умолчанию 1234.
     * @return true если сервер успешно запущен, иначе false.
     */
    [[nodiscard]] bool startServer(quint16 port = 1234) const;

    /**
     * @brief Отправляет клиенту стандартный ответ (статус и сообщение).
     * @param socket Сокет клиента.
     * @param response Структура ответа.
     */
    static void sendCommandResponse(QTcpSocket *socket, const CommandResponse &response);

    /**
     * @brief Проверяет, онлайн ли пользователь.
     * @param username Имя пользователя.
     * @return true если пользователь онлайн, иначе false.
     */
    [[nodiscard]] bool isUserOnline(const QString &username) const;

    /**
     * @brief Добавляет пользователя в список подключённых.
     * @param socket Сокет клиента.
     * @param username Имя пользователя.
     */
    void addConnectedUser(QTcpSocket *socket, const QString &username);

    /**
     * @brief Возвращает имя пользователя по его сокету.
     * @param socket Сокет клиента.
     * @return Имя пользователя или пустая строка, если не найден.
     */
    QString getUserBySocket(QTcpSocket *socket) const;

    /**
     * @brief Отправляет произвольный JSON-ответ клиенту.
     * @param socket Сокет клиента.
     * @param jsonResponse JSON-объект с ответом.
     */
    static void sendResponse(QTcpSocket *socket, const QJsonObject &jsonResponse);

    /**
     * @brief Рассылает текстовое сообщение всем клиентам.
     * @param sender Имя отправителя.
     * @param message Текст сообщения.
     */
    void broadcastMessage(const QString &sender, const QString &message);

    /**
     * @brief Рассылает JSON-объект всем клиентам.
     * @param json JSON-объект для рассылки.
     */
    void broadcastJSON(const QJsonObject &json);

    /**
     * @brief Рассылает системное сообщение всем клиентам.
     * @param message Текст системного сообщения.
     */
    void broadcastSystemMessage(const QString &message);

    /**
     * @brief Возвращает список имён всех онлайн-пользователей.
     * @return Список имён.
     */
    QStringList getOnlineUsers() const;

private slots:
    /**
     * @brief Обрабатывает новое входящее соединение.
     */
    void handleNewConnection();

    /**
     * @brief Обрабатывает готовность клиента к чтению данных.
     */
    void handleReadyRead() const;

    /**
     * @brief Обрабатывает отключение клиента.
     */
    void handleClientDisconnected();

private:
    QTcpServer *server;                          ///< Серверный сокет
    QList<QTcpSocket *> clients;                 ///< Список клиентских сокетов
    QHash<QTcpSocket *, QString> connectedUsers; ///< Ассоциация сокетов и имён пользователей

    CommandHandler *commandHandler;              ///< Обработчик команд
};

#endif // SERVER_H
