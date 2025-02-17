#include "database.h"

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
    if (QSqlQuery query; !query.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE NOT NULL , "
        "password TEXT NOT NULL, "
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)")) {
        qDebug() << "failed to create table" << query.lastError().text();
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
