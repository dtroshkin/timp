#include "testcontroller.h"

void TestController::testBold()
{
    // проверяем преобразование **жирного**
    QString input = "это **жирный** текст";
    QString expected = "это <b>жирный</b> текст";
    QCOMPARE(MessageFormatter::formatMessage(input), expected);
}

void TestController::testItalic()
{
    // проверяем преобразование *курсива*
    QString input = "это *курсив* текст";
    QString expected = "это <i>курсив</i> текст";
    QCOMPARE(MessageFormatter::formatMessage(input), expected);
}

void TestController::testEmoji()
{
    // проверяем замену эмодзи
    QString input = "привет :smile:";
    QString result = EmojiConverter::convertEmojis(input);

    // тут проверяем что ":smile:" заменился на эмодзи
    QVERIFY(result.contains("😊"));
    QVERIFY(!result.contains(":smile:"));
}

// это для запуска тестов
QTEST_APPLESS_MAIN(TestController)
