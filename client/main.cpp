#include <QApplication>
#include "loginwindow.h"
#include <QStyleFactory>
#include <QSharedMemory>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // создаем объект разделяемой памяти
    QSharedMemory shared("timp_chat_client_running");

    // пробуем захватить память
    if (!shared.create(1)) {
        // если не получилось - значит клиент уже запущен
        QMessageBox::warning(nullptr,
                             "упс",
                             "чат уже запущен!\nнельзя запустить второй экземпляр.");
        return -1;
    }

    a.setStyle(QStyleFactory::create("Fusion"));
    LoginWindow w;  // создаем НАШЕ окно
    w.show();      // показываем его

    return a.exec();
}
