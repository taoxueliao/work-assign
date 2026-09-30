#include "viewmodel/AssignmentListModel.h"

namespace {
enum Roles {
    WordRole = Qt::UserRole + 1,
    TranslationRole
};
}

AssignmentListModel::AssignmentListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int AssignmentListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return static_cast<int>(m_assignments.size());
}

QVariant AssignmentListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
        return {};

    const Assignment &item = m_assignments[static_cast<size_t>(index.row())];
    switch (role) {
    case WordRole:
        return QString::fromStdString(item.word);
    case TranslationRole:
        return QString::fromStdString(item.translation);
    default:
        return {};
    }
}

QHash<int, QByteArray> AssignmentListModel::roleNames() const
{
    return {
        {WordRole, "word"},
        {TranslationRole, "translation"}
    };
}

void AssignmentListModel::setAssignments(const std::vector<Assignment> &assignments)
{
    beginResetModel();
    m_assignments = assignments;
    endResetModel();
}
