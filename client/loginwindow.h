#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include "chatcontroller.h"
#include "dialog.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr, ChatController *controller = nullptr);
    ~LoginWindow();

private slots:
    // Слоты для кнопок
    void onLoginClicked();
    void onRegisterClicked();
    void onConnectClicked();

    // Слоты для событий от контроллера
    void onConnectionEstablished();
    void onConnectionFailed(const QString &error);
    void onLoginSuccessful(const QString &username);
    void onLoginFailed(const QString &error);
    void onRegistrationSuccessful();
    void onRegistrationFailed(const QString &error);

private:
    Ui::LoginWindow *ui;
    ChatController *chatController;
    Dialog *chatDialog;

    bool validateInput();
    void openChatDialog();
    void connectControllerSignals();
};

#endif // LOGINWINDOW_H
