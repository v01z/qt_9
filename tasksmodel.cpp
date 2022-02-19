#include "tasksmodel.h"
#include "taskslist.h"

TasksModel::TasksModel(QObject *parent)
    : QAbstractListModel { parent },
      mList { nullptr }
{
}

int TasksModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mList)
        return 0;


    return mList->items().size();
}

QVariant TasksModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mList)
        return QVariant();


    //my
    const TaskItem item { mList->items().at(index.row()) };
    switch (role){
        case DoneRole:
            return QVariant(item.done);
        case DescriptionRole:
            return QVariant(item.description);

        //case DateTimeRole:
    }
    //
    return QVariant();
}

bool TasksModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //my
    if (!mList)
        return false;

    TaskItem item { mList->items().at(index.row()) };
    switch (role){
        case DoneRole:
            item.done = value.toBool();
            break;
        case DescriptionRole:
            item.description = value.toString();
            break;
        //case DateTimeRole:
    }
    //my

    //if (data(index, role) != value) {
    if (mList->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags TasksModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> TasksModel::roleNames() const
{
   QHash<int, QByteArray> names;

   names[DoneRole] = "done";
   names[DescriptionRole] = "description";

   return names;
}

TasksList *TasksModel::list() const
{
    return mList;
}

void TasksModel::setList(TasksList *list)
{
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    //delete mList;

    mList = list;

    if (mList)
    {
        connect(mList, &TasksList::on_preItemAppended, this, [=]{
            const int index { mList->items().size() };
            beginInsertRows(QModelIndex(), index, index);
        });

        connect(mList, &TasksList::on_postItemAppended, this, [=]{
            endInsertRows();
        });

        connect(mList, &TasksList::on_preItemRemoved, this, [=](int index){
            beginRemoveRows(QModelIndex(), index, index);
        });

        connect(mList, &TasksList::on_postItemRemoved, this, [=]{
            endRemoveRows();
        });

    }
    endResetModel();
}
