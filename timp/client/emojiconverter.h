#ifndef EMOJICONVERTER_H
#define EMOJICONVERTER_H

#include <QString>
#include <QHash>

class EmojiConverter {
public:
    static QString convertEmojis(const QString& text);

private:
    static QHash<QString, QString> initEmojiMap();
    static const QHash<QString, QString>& getEmojiMap();
};

#endif // EMOJICONVERTER_H
