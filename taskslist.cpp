#include "taskslist.h"

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
    //Сообщим, чтоб будем добавлять задание
    emit on_preItemAppended();

    TaskItem item;
    item.done = false;
    mItems.append(item);

    //Сообщим, что добавили
    emit on_postItemAppended();
}

void TasksList::removeCompletedItems()
{
    for (int i{}; i < mItems.size(); )
    {
        //Если работа выполнена, то удаляем
        if (mItems.at(i).done)
        {
         //Кричим на всю округу, что будем удалять
        emit on_preItemRemoved(i);

        mItems.remove(i);

        //
        emit on_postItemRemoved();
        }
        else //Если галочка стоит, то пропускаем
            ++i;
    }
}
