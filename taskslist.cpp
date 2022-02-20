#include "taskslist.h"
#include <QDebug>
#include <sqlite3.h>
//#include "/usr/include/sqlite3.h"
//#include <sqlite3ext.h>
#include <QGuiApplication>
#include <set>
//#include <QSet>


TasksList::TasksList(QObject *parent) : QObject(parent)
{
    //**********
    /*
    mCurrentItems.append( { true, QStringLiteral("Wash the car"), QDate::currentDate() });
    mCurrentItems.append( { false, QStringLiteral("Fix the sink"), QDate::currentDate() });
    */
    //select * where date ==  currentDate

//    updateDataFromSQLiteBase();
    getDataFromDB();

    QDate date { QDate::currentDate() };
    updateCurrentItems(date);
}

QVector<TaskItem> TasksList::items() const
{
    return mCurrentItems;
}

bool TasksList::setItemAt(int index, const TaskItem &item)
{
   if (index < 0 || index >= mCurrentItems.size())
       return false;

   const TaskItem &oldItem = mCurrentItems.at(index);
   //if (item.done == oldItem.done && //*************************
   if (item.done == oldItem.done &&
           //item.description == oldItem.description)
           item.description == oldItem.description &&
                item.date == oldItem.date)
       return false;

   mCurrentItems[index] = item;
   return true;
}

void TasksList::appendItem()
{
    emit on_preItemAppended();

    TaskItem item;

    item.done = false;
    item.date = QDate::currentDate(); //********

    mCurrentItems.append(item);

    //debug
    qDebug() << "***** list vector inside TasksList module *****";
    for (const auto &elem: mCurrentItems)
        qDebug() << "done: " << elem.done << ", descr: " << elem.description <<
                    ", date: " << elem.date;
    qDebug() << "***** end TasksList module *****";
    //end debug

    emit on_postItemAppended();

}

void TasksList::removeCompletedItems()
{
    debug_debug(mCurrentItems, false);
    debug_debug(mFullDataItems, true);

    for (int i{}; i < mCurrentItems.size(); )
    {
        if (mCurrentItems.at(i).done)
        {
            emit on_preItemRemoved(i);

            for (int j{}; j < mFullDataItems.size(); ++j)
            {
                if ((mCurrentItems.at(i).description ==
                      mFullDataItems.at(j).description) &&
                        (mCurrentItems.at(i).date == mFullDataItems.at(j).date))
                {
                    qDebug() << "match";
                    mFullDataItems.remove(j);
                    break;
                }
            }

            mCurrentItems.remove(i);

            emit on_postItemRemoved();
        }
        else
            ++i;
    }

    debug_debug(mCurrentItems, false);
    debug_debug(mFullDataItems, true);
}

void TasksList::writeDataToSQLiteBase()
{

    const QString SQL_QUERY_DROP { "DROP TABLE IF EXISTS ORGANIZER;" };

    const QString SQL_QUERY_CREATE {
            "CREATE TABLE IF NOT EXISTS ORGANIZER (\'done\' INTEGER, \'task\' TEXT, \'date\' TEXT);" };

    const QString SQL_QUERY_INSERT_TEMPLATE { "INSERT INTO ORGANIZER VALUES (" };

    sqlite3 *db { nullptr };
    char *err { nullptr };

    if( sqlite3_open("my_cosy_database.dblite", &db) != SQLITE_OK)
    {
        std::fprintf(stderr, "Ошибка открытия/создания БД: %s\n", sqlite3_errmsg(db));
        return;
    }

    if (sqlite3_exec(db, SQL_QUERY_DROP.toStdString().c_str(), 0, 0, &err) != SQLITE_OK)
    {
        std::fprintf(stderr, "Ошибка SQL: %sn", err);
        sqlite3_free(err);
    }

    if (sqlite3_exec(db, SQL_QUERY_CREATE.toStdString().c_str(), 0, 0, &err) != SQLITE_OK)
    {
        std::fprintf(stderr, "Ошибка SQL: %sn", err);
        sqlite3_free(err);
    }

    updateFullDataItems();

    for (const auto &elem : mFullDataItems)
    {
        if (elem.description.isEmpty() || !elem.date.isValid())
            continue;

        //debug
        qDebug() << elem.date;
        //end debug


        QString SQL_QUERY_STR { SQL_QUERY_INSERT_TEMPLATE + (elem.done?'1':'0') + ", \'" +
                    elem.description + "\', \'" + (elem.date).toString("yyyy-MM-dd") + "\');" };

        qDebug() << SQL_QUERY_STR;

        if (sqlite3_exec(db, SQL_QUERY_STR.toStdString().c_str(), 0, 0, &err) != SQLITE_OK)
        {
            std::fprintf(stderr, "Ошибка SQL: %sn", err);
            sqlite3_free(err);
        }
    }

    sqlite3_close(db);


}

void TasksList::getDataFromDB()
{

    const QString SQL_QUERY_CREATE {
            "CREATE TABLE IF NOT EXISTS ORGANIZER (\'done\' INTEGER, \'task\' TEXT, \'date\' TEXT);" };

    const QString SQL_QUERY_SELECT { "SELECT * FROM ORGANIZER;" };

    sqlite3 *db { nullptr };
    //char *err { nullptr };

    mFullDataItems.clear();
    TaskItem newItem;

    if(sqlite3_open("my_cosy_database.dblite", &db) == SQLITE_OK)
    {

        sqlite3_stmt *stmt { nullptr };

        if (sqlite3_prepare_v2(db, SQL_QUERY_SELECT.toStdString().c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
            while (sqlite3_step(stmt) != SQLITE_DONE) {

                newItem.done = sqlite3_column_int(stmt, 0);
                newItem.description = QString((const char*)sqlite3_column_text(stmt, 1));
                newItem.date = QDate::fromString(QString((const char*)sqlite3_column_text(stmt,2)),
                                                 "yyyy-MM-dd");

                mFullDataItems.append(newItem);
            }
            sqlite3_close(db);
            return;
        }
        else {
            qDebug() << "Table doesnt exists, skip this step";
        }
    }
    else {
    }
    newItem.done = false;
    newItem.description.clear();
    newItem.date = QDate::currentDate();

    mFullDataItems.append(newItem);
}

void TasksList::updateFullDataItems()
{
    std::set <TaskItem> taskItemSet;

    for (const auto &elem : mCurrentItems)
        qDebug() << "current: " << elem.description;

    for (const auto &elem: mCurrentItems)
        taskItemSet.insert(elem);

    for (const auto &elem: mFullDataItems)
        taskItemSet.insert(elem);

    mFullDataItems.clear();

    for (const auto &elem : taskItemSet)
    {
        if (!elem.description.isEmpty())
            mFullDataItems.append(elem);
    }
}

//void TasksList::updateCurrentItems(QDate &date)
void TasksList::updateCurrentItems(QDate date)
{
    qDebug() << date;
    //updateFullDataItems before
    mCurrentItems.clear();

    for (const auto &elem : mFullDataItems)
    {
        if (elem.date == date)
            mCurrentItems.append(elem);
    }

}

void TasksList::debug_debug(const QVector<TaskItem> &vec, bool isGlobal)
{
    qDebug() << "++ " << (isGlobal? "fullGlobalVec":"currentVec");
    qDebug() << "********begin******";
   for (const auto &elem :vec)
   {
      qDebug() << "done: " << elem.done << ", descr: " << elem.description <<
                ", date: " << elem.date;
   }
    qDebug() << "********end******";
}
