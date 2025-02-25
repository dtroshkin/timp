#ifndef DIALOG_H
#define DIALOG_H

#include <QDateTime>
#include <QMainWindow>
#include "networkmanager.h"

namespace Ui {
class Dialog;
}

class Dialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit Dialog(QMainWindow *parent = nullptr, NetworkManager *networkManager = nullptr);
    ~Dialog();

private slots:
    // Слот для отправки сообщения (кнопка "Отправить")
    void onSendButtonClicked();
    // Слот для отправки сообщения через Enter
    void onMessageEditReturnPressed();

    // Слоты для обработки сообщений из NetworkManager
    void onMessageReceived(const QString &sender, const QString &content, const QString &timestamp);
    void onSystemMessageReceived(const QString &content, const QString &timestamp);
    void onHistoryReceived(const QJsonArray &messages);
    void onOnlineUsersReceived(const QStringList &users);
    void onMessageSendError(const QString &error);
    void onConnectionError(const QString &error);

private:
    Ui::Dialog *ui;
    NetworkManager *networkManager;

    // Вспомогательные методы
    void displayMessage(const QString &sender, const QString &content, const QString &timestamp);
    void displaySystemMessage(const QString &content, const QString &timestamp);
    void updateOnlineUsers(const QStringList &users);
    QString formatTimestamp(const QString &isoTimestamp);

    // При старте запрашиваем историю и список пользователей
    void requestInitialData();
};

#endif // DIALOG_H
