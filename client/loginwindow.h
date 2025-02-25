#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include "networkmanager.h"
#include "dialog.h" // Добавляем включение Dialog

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    // слоты для кнопок
    void onLoginClicked();
    void onRegisterClicked();
    void onConnectClicked();

    // слоты для ответов сервера
    void onLoginSuccess(const QString &message);
    void onLoginError(const QString &error);
    void onRegisterSuccess(const QString &message);
    void onRegisterError(const QString &error);
    void onConnectionError(const QString &error);

private:
    Ui::LoginWindow *ui;
    NetworkManager *networkManager;
    Dialog *chatDialog; // Указатель на окно чата
    bool validateInput();

    // Приватный метод для открытия окна чата
    void openChatDialog();
};

#endif // LOGINWINDOW_H
