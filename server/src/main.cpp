#include <QCoreApplication>

#include "database/database.h"
#include "server/server.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    if (!Database::instance().init()) {
        qDebug() << "database initialization failed";
        return 1;
    }

    // ReSharper disable once CppTooWideScopeInitStatement
    const Server server;
    if (!server.startServer(1234)) {
        return 1;
    }

    return QCoreApplication::exec();
}
