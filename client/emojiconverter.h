#ifndef EMOJICONVERTER_H
#define EMOJICONVERTER_H

#include <QString>
#include <QHash>

/**
 * @brief Утилитный класс для замены текстовых смайликов на emoji-символы.
 *
 * Поддерживает базовую подстановку, например:
 * - ":)" → 😊
 * - ":(" → 😞
 * - ":D" → 😄
 * - ";)" → 😉
 */
class EmojiConverter {
public:
    /**
     * @brief Преобразует текстовые смайлики в emoji.
     *
     * Метод заменяет известные текстовые выражения на соответствующие emoji-символы,
     * используя предопределенную карту.
     *
     * @param text Входной текст, содержащий смайлики.
     * @return Текст с заменёнными emoji.
     */
    static QString convertEmojis(const QString& text);

private:
    /**
     * @brief Инициализирует карту соответствия смайликов и emoji.
     *
     * Внутренний метод, вызывается один раз.
     * @return Хеш-таблица с парами "смайлик → emoji".
     */
    static QHash<QString, QString> initEmojiMap();

    /**
     * @brief Предоставляет доступ к общей emoji-карте.
     *
     * Использует ленивую инициализацию.
     * @return Ссылка на карту emoji.
     */
    static const QHash<QString, QString>& getEmojiMap();
};

#endif // EMOJICONVERTER_H
