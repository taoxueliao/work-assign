// Qt 列表模型，把 Assignment 交给 QML 列表。

#pragma once

#include <QAbstractListModel>

#include <vector>

#include "domain/Assignment.h"

class AssignmentListModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit AssignmentListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setAssignments(const std::vector<Assignment> &assignments);

private:
    std::vector<Assignment> m_assignments;
};
