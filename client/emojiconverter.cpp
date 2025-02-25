#include "emojiconverter.h"

QString EmojiConverter::convertEmojis(const QString& text) {
    QString result = text;
    const auto& emojiMap = getEmojiMap();

    // ищем все :что-то: в тексте
    for (auto it = emojiMap.begin(); it != emojiMap.end(); ++it) {
        result.replace(":" + it.key() + ":", it.value());
    }

    return result;
}

const QHash<QString, QString>& EmojiConverter::getEmojiMap() {
    static const QHash<QString, QString> emojiMap = initEmojiMap();
    return emojiMap;
}

QHash<QString, QString> EmojiConverter::initEmojiMap() {
    // тут базовый набор эмодзи
    return {
        {"smile", "😊"},
        {"heart", "❤️"},
        {"laugh", "😂"},
        {"wink", "😉"},
        {"sad", "😢"},
        {"angry", "😠"},
        {"cool", "😎"},
        {"fire", "🔥"},
        {"ok", "👍"},
        {"no", "👎"},
        {"party", "🎉"},
        {"think", "🤔"},
        {"cat", "😺"},
        {"dog", "🐶"}
    };
}
