#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlQuery>


class Database final : public QObject {
    Q_OBJECT

public:
    static Database &instance() {
        static Database instance;
        return instance;
    }

    // block copy (singleton)
    Database(const Database &) = delete;

    Database &operator=(const Database &) = delete;

    bool init(const QString &dbPath = "database.sqlite");

    static bool createTables();

    // db queries
    static bool registerUser(const QString &username, const QString &password);

    static bool checkCredentials(const QString &username, const QString &password);

    static bool saveMessage(const QString &sender, const QString &content);

    static QList<QVariantMap> getRecentMessages(int limit = 50);

private:
    explicit Database(QObject *parent = nullptr);

    ~Database() override;

    QSqlDatabase db;
};


#endif //DATABASE_H
