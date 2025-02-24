/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QWidget *centralwidget;
    QLineEdit *messageEdit;
    QPushButton *sendButton;
    QTextBrowser *onlineUser;
    QTextBrowser *messageHistory;

    void setupUi(QMainWindow *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(576, 414);
        centralwidget = new QWidget(Dialog);
        centralwidget->setObjectName("centralwidget");
        messageEdit = new QLineEdit(centralwidget);
        messageEdit->setObjectName("messageEdit");
        messageEdit->setGeometry(QRect(0, 370, 331, 41));
        QFont font;
        font.setPointSize(12);
        messageEdit->setFont(font);
        messageEdit->setCursorPosition(13);
        sendButton = new QPushButton(centralwidget);
        sendButton->setObjectName("sendButton");
        sendButton->setGeometry(QRect(340, 370, 81, 41));
        onlineUser = new QTextBrowser(centralwidget);
        onlineUser->setObjectName("onlineUser");
        onlineUser->setGeometry(QRect(430, 0, 141, 411));
        messageHistory = new QTextBrowser(centralwidget);
        messageHistory->setObjectName("messageHistory");
        messageHistory->setGeometry(QRect(0, 0, 421, 361));
        Dialog->setCentralWidget(centralwidget);

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QMainWindow *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "MainWindow", nullptr));
        messageEdit->setInputMask(QString());
        messageEdit->setText(QCoreApplication::translate("Dialog", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \321\202\320\265\320\272\321\201\321\202", nullptr));
        sendButton->setText(QCoreApplication::translate("Dialog", "\320\236\321\202\320\277\321\200\320\260\320\262\320\270\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
