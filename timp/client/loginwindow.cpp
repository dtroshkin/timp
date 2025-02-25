#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>

    LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow),
    networkManager(new NetworkManager(this)),
    chatDialog(nullptr) {

    ui->setupUi(this);

    // Устанавливаем заголовок окна
    setWindowTitle("Авторизация");

    // настройки по дефолту для подключения
    ui->hostInput->setText("25.63.209.3");
    ui->portInput->setValue(33333);
    ui->portInput->setRange(1, 65535);

    // кнопочки логина и реги ПОКА ВЫРУБАЕМ
    ui->loginButton->setEnabled(false);
    ui->registerButton->setEnabled(false);

    // цепляем основные кнопки к обработчикам
    connect(ui->connectButton, &QPushButton::clicked, this, &LoginWindow::onConnectClicked);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);

    // подключаем все ответы от сервака
    connect(networkManager, &NetworkManager::loginSuccess, this, &LoginWindow::onLoginSuccess);
    connect(networkManager, &NetworkManager::loginError, this, &LoginWindow::onLoginError);
    connect(networkManager, &NetworkManager::registerSuccess, this, &LoginWindow::onRegisterSuccess);
    connect(networkManager, &NetworkManager::registerError, this, &LoginWindow::onRegisterError);
    connect(networkManager, &NetworkManager::connectionError, this, &LoginWindow::onConnectionError);
}

LoginWindow::~LoginWindow() {
    delete ui;
    // chatDialog будет удален Qt автоматически, т.к. он имеет родителя
}

void LoginWindow::onConnectClicked() {
    disconnect(networkManager, &NetworkManager::connectionError, this, nullptr);
    disconnect(networkManager, &NetworkManager::connected, this, nullptr);

    const QString host = ui->hostInput->text();
    const int port = ui->portInput->value();

    if (host.isEmpty()) {
        QMessageBox::warning(this, "ошибка", "укажи хост!");
        return;
    }

    // обработка ошибок коннекта
    connect(networkManager, &NetworkManager::connectionError, this, [this](const QString &error) {
        QMessageBox::critical(this, "фейл", "не могу подключиться: " + error);
        ui->loginButton->setEnabled(false);
        ui->registerButton->setEnabled(false);
    });

    // если подключились - активим кнопки
    connect(networkManager, &NetworkManager::connected, this, [this]() {
        qDebug() << "CONNECTED сигнал получен!";
        QMessageBox::information(this, "коннект", "подключились к серверу!");
        ui->loginButton->setEnabled(true);
        ui->registerButton->setEnabled(true);
        ui->connectButton->setEnabled(false);
    });


    networkManager->connectToServer(host, port);
}


void LoginWindow::onLoginClicked() {
    if (!validateInput()) return;

    networkManager->sendLoginRequest(
        ui->usernameInput->text(),
        ui->passwordInput->text()
        );
}

void LoginWindow::onRegisterClicked() {
    if (!validateInput()) return;

    networkManager->sendRegisterRequest(
        ui->usernameInput->text(),
        ui->passwordInput->text()
        );
}

bool LoginWindow::validateInput() {
    const QString username = ui->usernameInput->text();
    const QString password = ui->passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "ошибка", "введи логин и пароль!");
        return false;
    }

    return true;
}

void LoginWindow::onLoginSuccess(const QString &message) {
    QMessageBox::information(this, "успех", message);
    // Открываем окно чата
    openChatDialog();
}

void LoginWindow::onLoginError(const QString &error) {
    QMessageBox::warning(this, "ошибка входа", error);
}

void LoginWindow::onRegisterSuccess(const QString &message) {
    QMessageBox::information(this, "успех", message);
    // После успешной регистрации можно предложить сразу войти
    if (QMessageBox::question(this, "Вход", "Хотите войти сейчас?",
                              QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
        onLoginClicked();
    }
}

void LoginWindow::onRegisterError(const QString &error) {
    QMessageBox::warning(this, "ошибка регистрации", error);
}


    void LoginWindow::onConnectionError(const QString &error) {
    QMessageBox::critical(this, "ошибка соединения", error);
}

void LoginWindow::openChatDialog() {
    // Создаем окно чата, если его еще нет
    if (!chatDialog) {
        chatDialog = new Dialog(nullptr, networkManager);
    }

    // Скрываем окно логина и показываем окно чата
    this->hide();
    chatDialog->show(); // используем exec(), чтобы блокировать поток до закрытия диалога
}
