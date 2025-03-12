#ifndef DIALOG_H
#define DIALOG_H

#include <QDateTime>
#include <QMainWindow>
#include "chatcontroller.h"

namespace Ui {
class Dialog;
}

class Dialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit Dialog(QMainWindow *parent = nullptr, ChatController *controller = nullptr);
    ~Dialog();

private slots:
    // Слоты для UI
    void onSendButtonClicked();
    void onMessageEditReturnPressed();

    // Слоты для обработки событий от ChatController
    void onMessageReceived(const QString &sender, const QString &content, const QString &timestamp);
    void onSystemMessageReceived(const QString &content, const QString &timestamp);
    void onHistoryLoaded(const QJsonArray &messages);
    void onOnlineUsersUpdated(const QStringList &users);
    void onMessageFailedToSend(const QString &error);
    void onConnectionFailed(const QString &error);

private:
    Ui::Dialog *ui;
    ChatController *chatController;

    // Вспомогательные методы
    void displayMessage(const QString &sender, const QString &content, const QString &timestamp);
    void displaySystemMessage(const QString &content, const QString &timestamp);
    void updateOnlineUsers(const QStringList &users);
    QString formatTimestamp(const QString &isoTimestamp);

    // При старте запрашиваем историю и список пользователей
    void requestInitialData();

    // Подключение сигналов контроллера
    void connectControllerSignals();
};

#endif // DIALOG_H
