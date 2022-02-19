#include "taskslist.h"
#include <QDebug>

TasksList::TasksList(QObject *parent) : QObject(parent)
{
    mItems.append( { true, QStringLiteral("Wash the car") });
    mItems.append( { false, QStringLiteral("Fix the sink") });
}

QVector<TaskItem> TasksList::items() const
{
    return mItems;
}

bool TasksList::setItemAt(int index, const TaskItem &item)
{
   if (index < 0 || index >= mItems.size())
       return false;

   const TaskItem &oldItem = mItems.at(index);
   if (item.done == oldItem.done &&
           item.description == oldItem.description)
       return false;

   mItems[index] = item;
   return true;
}

void TasksList::appendItem()
{
    emit on_preItemAppended();

    TaskItem item;
    item.done = false;
    mItems.append(item);

    emit on_postItemAppended();

}

void TasksList::removeCompletedItems()
{
    for (int i{}; i < mItems.size(); )
    {
        if (mItems.at(i).done)
        {
        emit on_preItemRemoved(i);

        mItems.remove(i);

        emit on_postItemRemoved();
        }
        else
            ++i;
    }
}

void TasksList::writeDataToSQLiteBase()
{
   //
    qDebug() << "here";
}

void TasksList::updateDataFromSQLiteBase()
{
    //debug
   mItems.clear();
   mItems.append( { false, QStringLiteral("New task number one") });
   mItems.append( { true, QStringLiteral("New task number two") });

   qDebug() << "Should be updated";
   qDebug() << mItems.size();
    for (const auto &elem: mItems)
    {
        qDebug() << "done: "<< elem.done << " | description: " << elem.description;
    }
    //end debug
}
