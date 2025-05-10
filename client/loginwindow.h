#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include "chatcontroller.h"
#include "dialog.h"

namespace Ui {
class LoginWindow;
}

/**
 * @brief Класс LoginWindow представляет окно авторизации пользователя.
 *
 * Отвечает за:
 * - Подключение к серверу
 * - Вход и регистрацию пользователя
 * - Переход к главному окну чата после успешной авторизации
 */
class LoginWindow : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Конструктор окна авторизации.
     * @param parent Родительский виджет.
     * @param controller Указатель на ChatController.
     */
    explicit LoginWindow(QWidget *parent = nullptr, ChatController *controller = nullptr);

    /// Деструктор
    ~LoginWindow();

private slots:
    // --- Слоты кнопок ---

    /**
     * @brief Обработка клика по кнопке "Войти".
     */
    void onLoginClicked();

    /**
     * @brief Обработка клика по кнопке "Зарегистрироваться".
     */
    void onRegisterClicked();

    /**
     * @brief Обработка клика по кнопке "Подключиться".
     */
    void onConnectClicked();

    // --- Слоты ChatController ---

    /**
     * @brief Обработка успешного подключения к серверу.
     */
    void onConnectionEstablished();

    /**
     * @brief Обработка ошибки подключения к серверу.
     * @param error Сообщение об ошибке.
     */
    void onConnectionFailed(const QString &error);

    /**
     * @brief Обработка успешного входа.
     * @param username Имя пользователя.
     */
    void onLoginSuccessful(const QString &username);

    /**
     * @brief Обработка ошибки входа.
     * @param error Сообщение об ошибке.
     */
    void onLoginFailed(const QString &error);

    /**
     * @brief Обработка успешной регистрации.
     */
    void onRegistrationSuccessful();

    /**
     * @brief Обработка ошибки регистрации.
     * @param error Сообщение об ошибке.
     */
    void onRegistrationFailed(const QString &error);

private:
    Ui::LoginWindow *ui; ///< Указатель на UI
    ChatController *chatController; ///< Контроллер логики чата
    Dialog *chatDialog; ///< Главное окно чата

    /**
     * @brief Проверяет корректность введённых данных.
     * @return true, если данные валидны.
     */
    bool validateInput();

    /**
     * @brief Открывает окно чата после успешного входа.
     */
    void openChatDialog();

    /**
     * @brief Подключает сигналы ChatController к слотам LoginWindow.
     */
    void connectControllerSignals();
};

#endif // LOGINWINDOW_H
