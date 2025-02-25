#ifndef MESSAGEFORMATTER_H
#define MESSAGEFORMATTER_H

#include <QString>

class MessageFormatter {
public:
    static QString formatMessage(const QString& message);

private:
    static QString processMarkdown(const QString& text);
    static QString processItalics(const QString& text);
    static QString processBold(const QString& text);
};

#endif // MESSAGEFORMATTER_H
