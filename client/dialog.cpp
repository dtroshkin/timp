#include "dialog.h"
#include <QMessageBox>
#include <QScrollBar>
#include "ui_dialog.h"
#include "messageformatter.h"

Dialog::Dialog(QMainWindow *parent, ChatController *controller)
    : QMainWindow(parent)
    , ui(new Ui::Dialog)
    , chatController(controller)
{
    ui->setupUi(this);

    // Устанавливаем заголовок окна
    setWindowTitle("Общий чат");

    // Подключаем сигналы кнопок
    connect(ui->sendButton, &QPushButton::clicked, this, &Dialog::onSendButtonClicked);
    connect(ui->messageEdit, &QLineEdit::returnPressed, this, &Dialog::onMessageEditReturnPressed);

    // Очищаем placeholder при фокусе
    connect(ui->messageEdit, &QLineEdit::textEdited, [this]() {
        static bool cleared = false;
        if (!cleared && ui->messageEdit->text() == "Введите текст") {
            ui->messageEdit->clear();
            cleared = true;
        }
    });

    // Если нам передали контроллер, подключаем его сигналы
    if (chatController) {
        connectControllerSignals();

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

void Dialog::connectControllerSignals() {
    // Подключаем сигналы ChatController к слотам Dialog
    connect(chatController, &ChatController::messageReceived,
            this, &Dialog::onMessageReceived);
    connect(chatController, &ChatController::systemMessageReceived,
            this, &Dialog::onSystemMessageReceived);
    connect(chatController, &ChatController::historyLoaded,
            this, &Dialog::onHistoryLoaded);
    connect(chatController, &ChatController::onlineUsersUpdated,
            this, &Dialog::onOnlineUsersUpdated);
    connect(chatController, &ChatController::messageFailedToSend,
            this, &Dialog::onMessageFailedToSend);
    connect(chatController, &ChatController::connectionFailed,
            this, &Dialog::onConnectionFailed);
}

void Dialog::onSendButtonClicked()
{
    QString message = ui->messageEdit->text().trimmed();
    if (message.isEmpty() || message == "Введите текст") {
        return; // не отправляем пустые сообщения
    }

    // Отправляем сообщение через контроллер
    if (chatController && chatController->isConnected()) {
        chatController->sendMessage(message);
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

void Dialog::onHistoryLoaded(const QJsonArray &messages)
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

void Dialog::onOnlineUsersUpdated(const QStringList &users)
{
    updateOnlineUsers(users);
}

void Dialog::onMessageFailedToSend(const QString &error)
{
    QMessageBox::warning(this, "Ошибка отправки", error);
}

void Dialog::onConnectionFailed(const QString &error)
{
    QMessageBox::critical(this, "Ошибка соединения", error);
    // Можно также закрыть диалог и вернуться на экран логина
    // this->close();
    // parent()->show();
}

void Dialog::displayMessage(const QString &sender, const QString &content, const QString &timestamp) {
    QString time = formatTimestamp(timestamp);
    // Форматируем только контент сообщения, не трогая отправителя
    QString formattedContent = MessageFormatter::formatMessage(content);
    QString formattedMessage = QString("<b>[%1] %2:</b> %3").arg(time, sender, formattedContent);
    ui->messageHistory->append(formattedMessage);

    // Прокручиваем к новому сообщению
    QScrollBar *scrollBar = ui->messageHistory->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void Dialog::displaySystemMessage(const QString &content, const QString &timestamp)
{
    QString time = formatTimestamp(timestamp);
    QString formattedMessage = QString("<i>[%1] %2</i>").arg(time, content);
    ui->messageHistory->append(formattedMessage);

    // Прокручиваем к новому сообщению
    QScrollBar *scrollBar = ui->messageHistory->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
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
    if (chatController && chatController->isConnected()) {
        chatController->requestHistory(50); // последние 50 сообщений
        chatController->requestOnlineUsers();
    }
}
