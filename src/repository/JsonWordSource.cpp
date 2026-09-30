#include "repository/JsonWordSource.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>

namespace {

QString formatTranslations(const QJsonArray &translations)
{
    QStringList parts;
    for (const QJsonValue &value : translations) {
        const QJsonObject item = value.toObject();
        const QString type = item.value(QStringLiteral("type")).toString().trimmed();
        const QString translation = item.value(QStringLiteral("translation")).toString().trimmed();
        if (translation.isEmpty())
            continue;
        if (type.isEmpty())
            parts.append(translation);
        else
            parts.append(type + QStringLiteral(". ") + translation);
    }
    return parts.join(QLatin1Char(' '));
}

}

std::vector<Word> JsonWordSource::load()
{
    QFile file(QStringLiteral(":/words/postgraduate.json"));
    if (!file.open(QIODevice::ReadOnly))
        return {};

    const QJsonArray array = QJsonDocument::fromJson(file.readAll()).array();
    std::vector<Word> words;
    words.reserve(static_cast<size_t>(array.size()));
    for (const QJsonValue &value : array) {
        const QJsonObject object = value.toObject();
        Word word;
        word.word = object.value(QStringLiteral("word")).toString().toStdString();
        word.translation = formatTranslations(object.value(QStringLiteral("translations")).toArray()).toStdString();
        if (!word.word.empty())
            words.push_back(std::move(word));
    }
    return words;
}
