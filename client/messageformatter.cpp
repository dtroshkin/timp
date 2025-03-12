#include "messageformatter.h"
#include <QRegularExpression>
#include "emojiconverter.h"

QString MessageFormatter::formatMessage(const QString& message) {
    // сначала обрабатываем жирный текст, потом курсив
    QString formatted = message;
    formatted = EmojiConverter::convertEmojis(formatted);
    formatted = processBold(formatted);
    formatted = processItalics(formatted);
    return formatted;
}

QString MessageFormatter::processBold(const QString& text) {
    // создаем КОПИЮ строки, т.к. будем её менять
    QString result = text;
    QRegularExpression re("\\*\\*(.+?)\\*\\*");
    result.replace(re, "<b>\\1</b>");
    return result;
}

QString MessageFormatter::processItalics(const QString& text) {
    QString result = text;
    QRegularExpression re("\\*(.+?)\\*");
    result.replace(re, "<i>\\1</i>");
    return result;
}
