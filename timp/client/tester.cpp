#include "tester.h"

Tester::Tester(QObject *parent) : QObject(parent) {
    manager = new NetworkManager(this);

    // цепляем сигналы
    connect(manager, &NetworkManager::connectionError,
            this, [](const QString& error) { qDebug() << "ФЕЙЛ КОННЕКТА:" << error; });

    connect(manager, &NetworkManager::loginSuccess,
            this, [](const QString& msg) { qDebug() << "ЛОГИН ЗАШЕЛ:" << msg; });

    // остальные connect как были...

    manager->connectToServer("25.63.209.3", 1234);

    QTimer::singleShot(1000, this, [this]() {
        qDebug() << "ПОГНАЛИ логиниться...";
        manager->sendLoginRequest("vasya", "123456");
    });
}
