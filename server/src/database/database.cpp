#include "database.h"

#include <QDateTime>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Database::Database(QObject *parent) : QObject(parent) {
    db = QSqlDatabase::addDatabase("QSQLITE");
}

Database::~Database() {
    if (db.isOpen()) {
        db.close();
    }
}

bool Database::init(const QString &dbPath) {
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "failed to open database" << db.lastError().text();
        return false;
    }

    return createTables();
}

bool Database::createTables() {
    QSqlQuery usersQuery;
    if (!usersQuery.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE NOT NULL, "
        "password TEXT NOT NULL, "
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)")) {
        qDebug() << "failed to create users table" << usersQuery.lastError().text();
        return false;
    }

    QSqlQuery messagesQuery;
    if (!messagesQuery.exec(
        "CREATE TABLE IF NOT EXISTS messages ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "sender TEXT NOT NULL, "
        "content TEXT NOT NULL, "
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "FOREIGN KEY (sender) REFERENCES users(username))")) {
        qDebug() << "failed to create messages table" << messagesQuery.lastError().text();
        return false;
    }

    return true;
}

bool Database::registerUser(const QString &username, const QString &password) {
    if (username.isEmpty() || password.isEmpty()) {
        qDebug() << "invalid username or password";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
    query.addBindValue(username);
    query.addBindValue(password);

    if (!query.exec()) {
        qDebug() << "failed to register users" << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::checkCredentials(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = ?");
    query.addBindValue(username);

    if (!query.exec() || !query.next()) {
        qDebug() << "user not found";
        return false;
    }

    const QString correctPassword = query.value(0).toString();

    return correctPassword == password;
}

bool Database::saveMessage(const QString &sender, const QString &content) {
    QSqlQuery query;
    query.prepare("INSERT INTO messages (sender, content) VALUES (?, ?)");
    query.addBindValue(sender);
    query.addBindValue(content);

    if (!query.exec()) {
        qDebug() << "failed to save message" << query.lastError().text();
        return false;
    }

    return true;
}

QList<QVariantMap> Database::getRecentMessages(const int limit) {
    QList<QVariantMap> messages;

    QSqlQuery query;
    query.prepare("SELECT id, sender, content, timestamp FROM messages ORDER BY timestamp DESC LIMIT ?");
    query.addBindValue(limit);

    if (!query.exec()) {
        qDebug() << "failed to get recent messages" << query.lastError().text();
        return messages;
    }

    while (query.next()) {
        QVariantMap message;
        message["id"] = query.value("id").toInt();
        message["sender"] = query.value("sender").toString();
        message["content"] = query.value("content").toString();
        message["timestamp"] = query.value("timestamp").toDateTime().toString(Qt::ISODate);

        messages.append(message);
    }

    return messages;
}