#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent, ChatController *controller)
    : QDialog(parent),
    ui(new Ui::LoginWindow),
    chatController(controller),
    chatDialog(nullptr) {

    ui->setupUi(this);

    // Устанавливаем заголовок окна
    setWindowTitle("Авторизация");

    // Настройки по умолчанию для подключения
    ui->hostInput->setText("25.63.209.3");
    ui->portInput->setValue(33333);
    ui->portInput->setRange(1, 65535);

    // Деактивируем кнопки логина и регистрации до установки соединения
    ui->loginButton->setEnabled(false);
    ui->registerButton->setEnabled(false);

    // Подключаем слоты кнопок
    connect(ui->connectButton, &QPushButton::clicked, this, &LoginWindow::onConnectClicked);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);

    // Если передан контроллер, подключаем его сигналы
    if (chatController) {
        connectControllerSignals();
    }
}

LoginWindow::~LoginWindow() {
    delete ui;
    // chatDialog будет удален Qt автоматически, т.к. имеет родителя
}

void LoginWindow::connectControllerSignals() {
    // Подключаем сигналы ChatController к слотам LoginWindow
    connect(chatController, &ChatController::connectionEstablished,
            this, &LoginWindow::onConnectionEstablished);
    connect(chatController, &ChatController::connectionFailed,
            this, &LoginWindow::onConnectionFailed);
    connect(chatController, &ChatController::loginSuccessful,
            this, &LoginWindow::onLoginSuccessful);
    connect(chatController, &ChatController::loginFailed,
            this, &LoginWindow::onLoginFailed);
    connect(chatController, &ChatController::registrationSuccessful,
            this, &LoginWindow::onRegistrationSuccessful);
    connect(chatController, &ChatController::registrationFailed,
            this, &LoginWindow::onRegistrationFailed);
}

void LoginWindow::onConnectClicked() {
    const QString host = ui->hostInput->text();
    const int port = ui->portInput->value();

    if (host.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Укажите хост!");
        return;
    }

    // Отключаем кнопку на время подключения
    ui->connectButton->setEnabled(false);
    ui->connectButton->setText("Подключение...");

    // Пытаемся подключиться через контроллер
    chatController->connectToServer(host, port);
}

void LoginWindow::onConnectionEstablished() {
    QMessageBox::information(this, "Подключение", "Подключение к серверу установлено!");
    ui->loginButton->setEnabled(true);
    ui->registerButton->setEnabled(true);
    ui->connectButton->setEnabled(false);
    ui->connectButton->setText("Подключено");
}

void LoginWindow::onConnectionFailed(const QString &error) {
    QMessageBox::critical(this, "Ошибка подключения", error);
    ui->loginButton->setEnabled(false);
    ui->registerButton->setEnabled(false);
    ui->connectButton->setEnabled(true);
    ui->connectButton->setText("Подключиться");
}

void LoginWindow::onLoginClicked() {
    if (!validateInput()) return;

    ui->loginButton->setEnabled(false);
    ui->loginButton->setText("Вход...");

    chatController->login(ui->usernameInput->text(), ui->passwordInput->text());
}

void LoginWindow::onRegisterClicked() {
    if (!validateInput()) return;

    ui->registerButton->setEnabled(false);
    ui->registerButton->setText("Регистрация...");

    chatController->registerUser(ui->usernameInput->text(), ui->passwordInput->text());
}

bool LoginWindow::validateInput() {
    const QString username = ui->usernameInput->text();
    const QString password = ui->passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите логин и пароль!");
        return false;
    }

    return true;
}


void LoginWindow::onLoginSuccessful(const QString &username) {
    QMessageBox::information(this, "Успех", "Авторизация успешна!");

    // Открываем окно чата
    openChatDialog();

    // Восстанавливаем состояние кнопки
    ui->loginButton->setEnabled(true);
    ui->loginButton->setText("Вход");
}

void LoginWindow::onLoginFailed(const QString &error) {
    QMessageBox::warning(this, "Ошибка входа", error);

    // Восстанавливаем состояние кнопки
    ui->loginButton->setEnabled(true);
    ui->loginButton->setText("Вход");
}

void LoginWindow::onRegistrationSuccessful() {
    QMessageBox::information(this, "Успех", "Регистрация успешна!");

    // Предлагаем войти
    if (QMessageBox::question(this, "Вход", "Хотите войти сейчас?",
                              QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
        onLoginClicked();
    }

    // Восстанавливаем состояние кнопки
    ui->registerButton->setEnabled(true);
    ui->registerButton->setText("Регистрация");
}

void LoginWindow::onRegistrationFailed(const QString &error) {
    QMessageBox::warning(this, "Ошибка регистрации", error);

    // Восстанавливаем состояние кнопки
    ui->registerButton->setEnabled(true);
    ui->registerButton->setText("Регистрация");
}

void LoginWindow::openChatDialog() {
    // Создаем окно чата, если его еще нет
    if (!chatDialog) {
        chatDialog = new Dialog(nullptr, chatController);
    }

    // Скрываем окно логина и показываем окно чата
    this->hide();
    chatDialog->show();
}
