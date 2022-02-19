#include "tasksmodel.h"

TasksModel::TasksModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int TasksModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!

    //simply return 100 (but then we should made smth better)
    return 100;
}

QVariant TasksModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!

    //my
    switch (role){
        case DoneRole:
        return QVariant(false);
    case DescriptionRole:
        return QVariant(QStringLiteral("Test description"));
    }
    //
    return QVariant();
}

bool TasksModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags TasksModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

QHash<int, QByteArray> TasksModel::roleNames() const
{
   QHash<int, QByteArray> names;

   names[DoneRole] = "done";
   names[DescriptionRole] = "description";

   return names;
}
