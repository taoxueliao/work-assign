#pragma once

#include <QObject>
#include <QStringList>
#include <QUrl>

#include "viewmodel/AssignmentListModel.h"

class AssignController : public QObject {
    Q_OBJECT
    Q_PROPERTY(AssignmentListModel *assignments READ assignments CONSTANT)
    Q_PROPERTY(bool hasResult READ hasResult NOTIFY hasResultChanged)
    Q_PROPERTY(QUrl exportDirectory READ exportDirectory NOTIFY exportDirectoryChanged)
    Q_PROPERTY(QString exportBaseName READ exportBaseName NOTIFY exportBaseNameChanged)

public:
    explicit AssignController(QObject *parent = nullptr);

    AssignmentListModel *assignments();
    bool hasResult() const;
    QUrl exportDirectory() const;
    QString exportBaseName() const;

    Q_INVOKABLE void assign(int count, const QString &month, const QString &round);
    Q_INVOKABLE bool exportTo(const QUrl &directory);

signals:
    void hasResultChanged();
    void exportDirectoryChanged();
    void exportBaseNameChanged();

private:
    AssignmentListModel m_assignments;
    QStringList m_words;
    QStringList m_translations;
    QString m_month;
    QString m_round;
};
