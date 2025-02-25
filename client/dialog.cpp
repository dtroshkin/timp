#include "dialog.h"
#include <QMessageBox>
#include <QScrollBar>
#include "ui_dialog.h"
#include "messageformatter.h"

Dialog::Dialog(QMainWindow *parent, NetworkManager *netManager)
    : QMainWindow(parent)
    , ui(new Ui::Dialog)
    , networkManager(netManager)
{
    ui->setupUi(this);

    // Установим заголовок окна
    setWindowTitle("Общий чат");

    // Кнопка отправки
    connect(ui->sendButton, &QPushButton::clicked, this, &Dialog::onSendButtonClicked);

    // Отправка по Enter
    connect(ui->messageEdit, &QLineEdit::returnPressed, this, &Dialog::onMessageEditReturnPressed);

    // Очищаем placeholder при фокусе
    connect(ui->messageEdit, &QLineEdit::textEdited, [this]() {
        static bool cleared = false;
        if (!cleared && ui->messageEdit->text() == "Введите текст") {
            ui->messageEdit->clear();
            cleared = true;
        }
    });

    // Если нам передали менеджер сети - подключаем его сигналы
    if (networkManager) {
        connect(networkManager, &NetworkManager::messageReceived, this, &Dialog::onMessageReceived);
        connect(networkManager,
                &NetworkManager::systemMessageReceived,
                this,
                &Dialog::onSystemMessageReceived);
        connect(networkManager, &NetworkManager::historyReceived, this, &Dialog::onHistoryReceived);
        connect(networkManager,
                &NetworkManager::onlineUsersReceived,
                this,
                &Dialog::onOnlineUsersReceived);
        connect(networkManager,
                &NetworkManager::messageSendError,
                this,
                &Dialog::onMessageSendError);
        connect(networkManager, &NetworkManager::connectionError, this, &Dialog::onConnectionError);

        // Запрашиваем начальные данные
        requestInitialData();
    }

    // Очищаем браузеры сообщений и пользователей
    ui->messageHistory->clear();
    ui->onlineUser->clear();

    // Фокус на ввод сообщения
    ui->messageEdit->setFocus();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::onSendButtonClicked()
{
    QString message = ui->messageEdit->text().trimmed();
    if (message.isEmpty() || message == "Введите текст") {
        return; // не отправляем пустые сообщения
    }

    // Отправляем сообщение, если есть соединение
    if (networkManager && networkManager->isConnected()) {
        networkManager->sendMessage(message);
        ui->messageEdit->clear();
        ui->messageEdit->setFocus();
    } else {
        QMessageBox::warning(this, "Ошибка", "Нет соединения с сервером");
    }
}

void Dialog::onMessageEditReturnPressed()
{
    onSendButtonClicked(); // переиспользуем тот же код
}

void Dialog::onMessageReceived(const QString &sender,
                               const QString &content,
                               const QString &timestamp)
{
    qDebug() << "ДИАЛОГ: получил сообщение от" << sender << ":" << content;
    displayMessage(sender, content, timestamp);
}

void Dialog::onSystemMessageReceived(const QString &content, const QString &timestamp)
{
    qDebug() << "ДИАЛОГ: получил системное сообщение:" << content;
    displaySystemMessage(content, timestamp);
}

void Dialog::onHistoryReceived(const QJsonArray &messages)
{
    qDebug() << "ПОЛУЧИЛИ ИСТОРИЮ В ДИАЛОГЕ:" << messages.size() << "сообщений";
    // Очищаем историю перед заполнением
    ui->messageHistory->clear();

    // Перебираем все сообщения и добавляем их в порядке от старых к новым
    for (const QJsonValue &value : messages) {
        QJsonObject message = value.toObject();
        QString type = message["type"].toString();
        QString content = message["content"].toString();
        QString timestamp = message["timestamp"].toString();

        if (type == "message") {
            QString sender = message["sender"].toString();
            displayMessage(sender, content, timestamp);
        } else if (type == "system") {
            displaySystemMessage(content, timestamp);
        }
    }

    // Прокручиваем до последнего сообщения
    QScrollBar *scrollBar = ui->messageHistory->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void Dialog::onOnlineUsersReceived(const QStringList &users)
{
    updateOnlineUsers(users);
}

void Dialog::onMessageSendError(const QString &error)
{
    QMessageBox::warning(this, "Ошибка отправки", error);
}

void Dialog::onConnectionError(const QString &error)
{
    QMessageBox::critical(this, "Ошибка соединения", error);
    // Можно также закрыть диалог и вернуться на экран логина
    // this->reject();
}

void Dialog::displayMessage(const QString &sender, const QString &content, const QString &timestamp) {
    QString time = formatTimestamp(timestamp);
    // форматируем только контент сообщения, не трогая отправителя
    QString formattedContent = MessageFormatter::formatMessage(content);
    QString formattedMessage = QString("<b>[%1] %2:</b> %3").arg(time, sender, formattedContent);
    ui->messageHistory->append(formattedMessage);
    ui->messageHistory->repaint();
}


void Dialog::displaySystemMessage(const QString &content, const QString &timestamp)
{
    QString time = formatTimestamp(timestamp);
    QString formattedMessage = QString("<i>[%1] %2</i>").arg(time, content);
    ui->messageHistory->append(formattedMessage);
}

void Dialog::updateOnlineUsers(const QStringList &users)
{
    ui->onlineUser->clear();
    ui->onlineUser->append("<b>Пользователи онлайн:</b>");

    for (const QString &user : users) {
        ui->onlineUser->append(user);
    }
}

QString Dialog::formatTimestamp(const QString &isoTimestamp)
{
    QDateTime dateTime = QDateTime::fromString(isoTimestamp, Qt::ISODate);
    return dateTime.toString("HH:mm:ss");
}

void Dialog::requestInitialData()
{
    // Запрашиваем историю сообщений и список онлайн-пользователей
    if (networkManager && networkManager->isConnected()) {
        networkManager->requestMessageHistory(50); // последние 50 сообщений
        networkManager->requestOnlineUsers();
    }
}
