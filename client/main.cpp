#include <QApplication>
#include <QStyleFactory>
#include <QSharedMemory>
#include <QMessageBox>
#include "loginwindow.h"
#include "chatcontroller.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Проверка на повторный запуск приложения
    QSharedMemory shared("timp_chat_client_running");
    if (!shared.create(1)) {
        QMessageBox::warning(nullptr,
                             "Упс",
                             "Чат уже запущен!\nНельзя запустить второй экземпляр.");
        return -1;
    }

    // Устанавливаем стиль Fusion
    a.setStyle(QStyleFactory::create("Fusion"));

    // Создаем контроллер чата
    ChatController* chatController = new ChatController(&a);

    // Создаем окно логина, передавая ему контроллер
    LoginWindow w(nullptr, chatController);
    w.show();

    int result = a.exec();

    // При выходе очищаем ресурсы
    shared.detach();
    return result;
}
