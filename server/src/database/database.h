#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlQuery>

/**
 * @brief Класс Database управляет подключением к базе данных и выполняет основные запросы.
 *
 * Этот класс реализован в виде синглтона и обеспечивает:
 * - Инициализацию и подключение к базе данных SQLite
 * - Создание таблиц при первом запуске
 * - Регистрацию пользователей и проверку их данных
 * - Сохранение и получение сообщений
 */
class Database final : public QObject {
    Q_OBJECT

public:

    /**
        * @brief Получить единственный экземпляр класса Database (паттерн Singleton).
        * @return Ссылка на экземпляр Database.
        */
    static Database &instance() {
        static Database instance;
        return instance;
    }

    /// Удаляем копирование (Singleton)
    Database(const Database &) = delete;
    /// Удаляем присваивание (Singleton)
    Database &operator=(const Database &) = delete;
    /**
         * @brief Инициализация базы данных.
         *
         * Открывает соединение с базой данных SQLite.
         * @param dbPath Путь к файлу базы данных. По умолчанию "database.sqlite".
         * @return true если инициализация прошла успешно, иначе false.
         */
    bool init(const QString &dbPath = "database.sqlite");
    /**
         * @brief Создаёт таблицы в базе данных (если они ещё не существуют).
         * @return true если успешно, иначе false.
         */
    static bool createTables();
    /**
         * @brief Регистрирует нового пользователя.
         *
         * Хэширует пароль перед сохранением.
         * @param username Имя пользователя.
         * @param password Пароль (в незашифрованном виде).
         * @return true если регистрация успешна, иначе false.
         */
    // db queries
    static bool registerUser(const QString &username, const QString &password);
    /**
        * @brief Проверяет логин и пароль пользователя.
        *
        * Сравнивает имя и хэш пароля с данными в БД.
        * @param username Имя пользователя.
        * @param password Пароль (в незашифрованном виде).
        * @return true если логин и пароль корректны, иначе false.
        */
    static bool checkCredentials(const QString &username, const QString &password);
    /**
         * @brief Сохраняет новое сообщение от пользователя.
         * @param sender Имя отправителя.
         * @param content Текст сообщения.
         * @return true если успешно сохранено, иначе false.
         */
    static bool saveMessage(const QString &sender, const QString &content);
    /**
        * @brief Получает список последних сообщений.
        * @param limit Количество сообщений, по умолчанию 50.
        * @return Список сообщений в виде QVariantMap (ключи: sender, content, timestamp).
        */
    static QList<QVariantMap> getRecentMessages(int limit = 50);

private:
    /**
     * @brief Приватный конструктор (Singleton).
     * @param parent Родительский QObject.
     */
    explicit Database(QObject *parent = nullptr);
    /// Деструктор
    ~Database() override;
    /// Объект базы данных SQLite
    QSqlDatabase db;
};


#endif //DATABASE_H
