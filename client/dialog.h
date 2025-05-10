#ifndef DIALOG_H
#define DIALOG_H

#include <QDateTime>
#include <QMainWindow>
#include "chatcontroller.h"

namespace Ui {
class Dialog;
}

/**
 * @brief Класс Dialog представляет главное окно чата.
 *
 * Отвечает за:
 * - Отображение истории сообщений и онлайн-пользователей
 * - Отправку сообщений
 * - Обработку сигналов от ChatController
 */
class Dialog : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Конструктор окна.
     * @param parent Родительский QMainWindow.
     * @param controller Указатель на контроллер чата.
     */
    explicit Dialog(QMainWindow *parent = nullptr, ChatController *controller = nullptr);

    /// Деструктор
    ~Dialog();

private slots:
    // --- Слоты UI ---

    /**
     * @brief Обработка нажатия кнопки "Отправить".
     */
    void onSendButtonClicked();

    /**
     * @brief Обработка нажатия Enter в поле ввода сообщения.
     */
    void onMessageEditReturnPressed();

    // --- Слоты ChatController ---

    /**
     * @brief Обработка входящего сообщения.
     * @param sender Отправитель.
     * @param content Содержимое.
     * @param timestamp Метка времени.
     */
    void onMessageReceived(const QString &sender, const QString &content, const QString &timestamp);

    /**
     * @brief Обработка входящего системного сообщения.
     * @param content Содержимое.
     * @param timestamp Метка времени.
     */
    void onSystemMessageReceived(const QString &content, const QString &timestamp);

    /**
     * @brief Загрузка истории сообщений.
     * @param messages JSON-массив сообщений.
     */
    void onHistoryLoaded(const QJsonArray &messages);

    /**
     * @brief Обновление списка онлайн-пользователей.
     * @param users Список имён.
     */
    void onOnlineUsersUpdated(const QStringList &users);

    /**
     * @brief Обработка ошибки при отправке сообщения.
     * @param error Описание ошибки.
     */
    void onMessageFailedToSend(const QString &error);

    /**
     * @brief Обработка ошибки соединения.
     * @param error Описание ошибки.
     */
    void onConnectionFailed(const QString &error);

private:
    Ui::Dialog *ui; ///< Сгенерированный UI-интерфейс
    ChatController *chatController; ///< Контроллер бизнес-логики

    // --- Вспомогательные методы ---

    /**
     * @brief Отображает входящее сообщение в интерфейсе.
     */
    void displayMessage(const QString &sender, const QString &content, const QString &timestamp);

    /**
     * @brief Отображает системное сообщение.
     */
    void displaySystemMessage(const QString &content, const QString &timestamp);

    /**
     * @brief Обновляет список онлайн-пользователей.
     */
    void updateOnlineUsers(const QStringList &users);

    /**
     * @brief Преобразует ISO-дату во формат для отображения.
     * @param isoTimestamp Метка времени ISO 8601.
     * @return Отформатированная строка времени.
     */
    QString formatTimestamp(const QString &isoTimestamp);

    /**
     * @brief Запрашивает начальные данные (история + онлайн).
     */
    void requestInitialData();

    /**
     * @brief Подключает сигналы от контроллера чата.
     */
    void connectControllerSignals();
};

#endif // DIALOG_H
