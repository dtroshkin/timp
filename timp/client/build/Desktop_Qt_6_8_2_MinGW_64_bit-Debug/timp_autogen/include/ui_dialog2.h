/********************************************************************************
** Form generated from reading UI file 'dialog2.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG2_H
#define UI_DIALOG2_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLineEdit *messageEdit;
    QPushButton *sendButton;
    QTextBrowser *onlineUser;
    QTextBrowser *messageHistory;

    void setupUi(QWidget *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(590, 431);
        messageEdit = new QLineEdit(Dialog);
        messageEdit->setObjectName("messageEdit");
        messageEdit->setGeometry(QRect(10, 380, 331, 41));
        QFont font;
        font.setPointSize(12);
        messageEdit->setFont(font);
        messageEdit->setCursorPosition(13);
        sendButton = new QPushButton(Dialog);
        sendButton->setObjectName("sendButton");
        sendButton->setGeometry(QRect(350, 380, 81, 41));
        onlineUser = new QTextBrowser(Dialog);
        onlineUser->setObjectName("onlineUser");
        onlineUser->setGeometry(QRect(440, 10, 141, 411));
        messageHistory = new QTextBrowser(Dialog);
        messageHistory->setObjectName("messageHistory");
        messageHistory->setGeometry(QRect(10, 10, 421, 361));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QWidget *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        messageEdit->setInputMask(QString());
        messageEdit->setText(QCoreApplication::translate("Dialog", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \321\202\320\265\320\272\321\201\321\202", nullptr));
        sendButton->setText(QCoreApplication::translate("Dialog", "\320\236\321\202\320\277\321\200\320\260\320\262\320\270\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG2_H
