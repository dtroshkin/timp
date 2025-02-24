/********************************************************************************
** Form generated from reading UI file 'loginwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LoginWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLineEdit *hostInput;
    QSpinBox *portInput;
    QPushButton *connectButton;
    QPushButton *registerButton;
    QPushButton *loginButton;

    void setupUi(QDialog *LoginWindow)
    {
        if (LoginWindow->objectName().isEmpty())
            LoginWindow->setObjectName("LoginWindow");
        LoginWindow->resize(400, 300);
        verticalLayout = new QVBoxLayout(LoginWindow);
        verticalLayout->setObjectName("verticalLayout");
        usernameInput = new QLineEdit(LoginWindow);
        usernameInput->setObjectName("usernameInput");

        verticalLayout->addWidget(usernameInput);

        passwordInput = new QLineEdit(LoginWindow);
        passwordInput->setObjectName("passwordInput");
        passwordInput->setEchoMode(QLineEdit::EchoMode::PasswordEchoOnEdit);

        verticalLayout->addWidget(passwordInput);

        groupBox = new QGroupBox(LoginWindow);
        groupBox->setObjectName("groupBox");
        groupBox->setMinimumSize(QSize(382, 162));
        groupBox->setMaximumSize(QSize(382, 162));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName("horizontalLayout");
        hostInput = new QLineEdit(groupBox);
        hostInput->setObjectName("hostInput");

        horizontalLayout->addWidget(hostInput);

        portInput = new QSpinBox(groupBox);
        portInput->setObjectName("portInput");

        horizontalLayout->addWidget(portInput);

        connectButton = new QPushButton(groupBox);
        connectButton->setObjectName("connectButton");

        horizontalLayout->addWidget(connectButton);


        verticalLayout->addWidget(groupBox);

        registerButton = new QPushButton(LoginWindow);
        registerButton->setObjectName("registerButton");

        verticalLayout->addWidget(registerButton);

        loginButton = new QPushButton(LoginWindow);
        loginButton->setObjectName("loginButton");

        verticalLayout->addWidget(loginButton);


        retranslateUi(LoginWindow);

        QMetaObject::connectSlotsByName(LoginWindow);
    } // setupUi

    void retranslateUi(QDialog *LoginWindow)
    {
        LoginWindow->setWindowTitle(QCoreApplication::translate("LoginWindow", "Dialog", nullptr));
        usernameInput->setPlaceholderText(QCoreApplication::translate("LoginWindow", "\320\233\320\276\320\263\320\270\320\275", nullptr));
        passwordInput->setPlaceholderText(QCoreApplication::translate("LoginWindow", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        groupBox->setTitle(QCoreApplication::translate("LoginWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270 \321\201\320\265\321\200\320\262\320\265\321\200\320\260", nullptr));
        hostInput->setPlaceholderText(QCoreApplication::translate("LoginWindow", "\320\230\320\274\321\217 \321\205\320\276\321\201\321\202\320\260", nullptr));
        connectButton->setText(QCoreApplication::translate("LoginWindow", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\270\321\202\321\214\321\201\321\217", nullptr));
        registerButton->setText(QCoreApplication::translate("LoginWindow", "\320\240\320\265\320\263\320\270\321\201\321\202\321\200\320\260\321\206\320\270\321\217", nullptr));
        loginButton->setText(QCoreApplication::translate("LoginWindow", "\320\222\321\205\320\276\320\264", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWindow: public Ui_LoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
