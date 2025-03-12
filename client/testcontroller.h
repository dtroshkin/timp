#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include <QObject>
#include <QtTest/QtTest>
#include "messageformatter.h"
#include "emojiconverter.h"

class TestController : public QObject
{
    Q_OBJECT

private slots:
    // простые тесты для форматтера сообщений
    void testBold();
    void testItalic();

    // тест для эмодзи
    void testEmoji();
};

#endif // TESTCONTROLLER_H
