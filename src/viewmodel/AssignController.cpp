#include "viewmodel/AssignController.h"

#include <QDir>
#include <QFile>
#include <QSettings>
#include <QStandardPaths>

#include "repository/JsonWordSource.h"
#include "service/RandomAssigner.h"

namespace {

QString sanitizeFileName(QString name)
{
    const QString invalid = QStringLiteral("\\/:*?\"<>|");
    for (const QChar ch : invalid)
        name.replace(ch, QLatin1Char('_'));
    return name;
}

bool writeLines(const QString &path, const QStringList &lines)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    QByteArray content;
    content.append("\xEF\xBB\xBF");
    content.append(lines.join(QLatin1Char('\n')).toUtf8());
    if (!lines.isEmpty())
        content.append('\n');
    return file.write(content) == content.size();
}

}

AssignController::AssignController(QObject *parent)
    : QObject(parent)
{
}

AssignmentListModel *AssignController::assignments()
{
    return &m_assignments;
}

bool AssignController::hasResult() const
{
    return !m_words.isEmpty();
}

QUrl AssignController::exportDirectory() const
{
    const QString documents = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString directory = QSettings().value(QStringLiteral("export/directory"), documents).toString();
    if (!QDir(directory).exists())
        directory = documents;
    return QUrl::fromLocalFile(directory);
}

QString AssignController::exportBaseName() const
{
    if (m_month.isEmpty() || m_round.isEmpty())
        return {};
    return sanitizeFileName(m_month + QLatin1Char('_') + m_round);
}

void AssignController::assign(int count, const QString &month, const QString &round)
{
    JsonWordSource source;
    RandomAssigner assigner;
    const std::vector<Assignment> assigned = assigner.assign(source.load(), count);

    m_words.clear();
    m_translations.clear();
    for (const Assignment &item : assigned) {
        m_words.append(QString::fromStdString(item.word));
        m_translations.append(QString::fromStdString(item.translation));
    }
    m_month = month;
    m_round = round;
    m_assignments.setAssignments(assigned);
    emit hasResultChanged();
    emit exportBaseNameChanged();
}

bool AssignController::exportTo(const QUrl &directory)
{
    const QString directoryPath = directory.toLocalFile();
    if (directoryPath.isEmpty() || !hasResult() || exportBaseName().isEmpty())
        return false;
    if (!QDir(directoryPath).exists())
        return false;

    const QString baseName = exportBaseName();
    const QString wordPath = QDir(directoryPath).filePath(baseName + QStringLiteral(".txt"));
    const QString translationPath = QDir(directoryPath).filePath(baseName + QStringLiteral("_translation.txt"));
    if (!writeLines(wordPath, m_words) || !writeLines(translationPath, m_translations))
        return false;

    QSettings().setValue(QStringLiteral("export/directory"), QDir(directoryPath).absolutePath());
    emit exportDirectoryChanged();
    return true;
}
