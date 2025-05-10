#ifndef MESSAGEFORMATTER_H
#define MESSAGEFORMATTER_H

#include <QString>

/**
 * @brief Утилитный класс для форматирования сообщений чата.
 *
 * Поддерживает базовый markdown-подобный синтаксис:
 * - `*курсив*` — преобразуется в *курсив*
 * - `**жирный**` — преобразуется в **жирный**
 *
 * Преобразование выполняется с помощью HTML-тегов (`<i>`, `<b>`).
 */
class MessageFormatter {
public:
    /**
     * @brief Форматирует входное сообщение, применяя стили (курсив и жирный).
     * @param message Входной текст сообщения.
     * @return Отформатированное HTML-сообщение.
     */
    static QString formatMessage(const QString& message);

private:
    /**
     * @brief Обрабатывает markdown-теги и преобразует их в HTML.
     * @param text Входной текст.
     * @return Текст с применённым HTML-форматированием.
     */
    static QString processMarkdown(const QString& text);

    /**
     * @brief Обрабатывает курсив (`*текст*`) и преобразует в `<i>текст</i>`.
     * @param text Входной текст.
     * @return Текст с применённым курсивом.
     */
    static QString processItalics(const QString& text);

    /**
     * @brief Обрабатывает жирный (`**текст**`) и преобразует в `<b>текст</b>`.
     * @param text Входной текст.
     * @return Текст с применённым жирным стилем.
     */
    static QString processBold(const QString& text);
};

#endif // MESSAGEFORMATTER_H
