#include "taskslist.h"
#include <QDebug>
#include <sqlite3.h>
//#include "/usr/include/sqlite3.h"
//#include <sqlite3ext.h>
#include <QGuiApplication>


TasksList::TasksList(QObject *parent) : QObject(parent)
{
    //**********
    /*
    mItems.append( { true, QStringLiteral("Wash the car"), QDate::currentDate() });
    mItems.append( { false, QStringLiteral("Fix the sink"), QDate::currentDate() });
    */
    //select * where date ==  currentDate

    updateDataFromSQLiteBase();
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
   //if (item.done == oldItem.done && //*************************
   if (item.done == oldItem.done &&
           //item.description == oldItem.description)
           item.description == oldItem.description &&
                item.date == oldItem.date)
       return false;

   mItems[index] = item;
   return true;
}

void TasksList::appendItem()
{
    emit on_preItemAppended();

    TaskItem item;

    item.done = false;
    item.date = QDate::currentDate(); //********

    mItems.append(item);

    //debug
    qDebug() << "***** list vector inside TasksList module *****";
    for (const auto &elem: mItems)
        qDebug() << "done: " << elem.done << ", descr: " << elem.description <<
                    ", date: " << elem.date;
    qDebug() << "***** end TasksList module *****";
    //end debug

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
    const QString SQL_QUERY_CREATE =
            "CREATE TABLE IF NOT EXISTS ORGANIZER (\'done\' INTEGER, \'task\' TEXT, \'date\' TEXT);";

    const QString SQL_QUERY_INSERT_TEMPLATE { "INSERT INTO ORGANIZER VALUES (" };

    sqlite3 *db { nullptr };
    char *err { nullptr };

    if( sqlite3_open("my_cosy_database.dblite", &db) != SQLITE_OK)
    {
        std::fprintf(stderr, "Ошибка открытия/создания БД: %s\n", sqlite3_errmsg(db));
        return;
    }

    if (sqlite3_exec(db, SQL_QUERY_CREATE.toStdString().c_str(), 0, 0, &err) != SQLITE_OK)
        {
            std::fprintf(stderr, "Ошибка SQL: %sn", err);
            sqlite3_free(err);
        }

    for (const auto &elem : mItems)
    {
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
    // закрываем соединение
    sqlite3_close(db);


}

void TasksList::updateDataFromSQLiteBase()
{

    const QString SQL_QUERY_CREATE =
            "CREATE TABLE IF NOT EXISTS ORGANIZER (\'done\' INTEGER, \'task\' TEXT, \'date\' TEXT);";


    const QString SQL_QUERY_SELECT { "SELECT * FROM ORGANIZER;" };

    const QString SQL_QUERY_DROP { "DROP TABLE IF EXISTS ORGANIZER" };

    sqlite3 *db { nullptr };
    char *err { nullptr };

    mItems.clear();
    TaskItem newItem;

    if(sqlite3_open("my_cosy_database.dblite", &db) == SQLITE_OK)
    {

        sqlite3_stmt *stmt { nullptr };

        if (sqlite3_prepare_v2(db, SQL_QUERY_SELECT.toStdString().c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
            while (sqlite3_step(stmt) != SQLITE_DONE) {

                newItem.done = sqlite3_column_int(stmt, 0);
                //newItem.description = QString(sqlite3_column_text(stmt, 1));
                qDebug() << newItem.done;
                newItem.description = QString((const char*)sqlite3_column_text(stmt, 1));
                qDebug() << newItem.description;
                //newItem.date = QDate::fromString(QString(sqlite3_column_text(stmt,2), "yyyy-MM-dd"));
                //newItem.date = QDate::fromString(QString((const char*)sqlite3_column_text(stmt,2), "yyyy-MM-dd"));
                const unsigned char * dateC = sqlite3_column_text(stmt,2);
                //std::printf("printf date: %s\n", sqlite3_column_text(stmt,2));
                std::printf("printf date: %s\n", dateC);
                QString qstr = QString((const char*)dateC);
                qDebug() << "qstr is: " << qstr;
                //newItem.date = QDate::fromString(qstr, Qt::DateFormat("yyyy-MM-dd"));
                //
               // newItem.date = QDate::fromString(qstr, "yyyy-MM-dd");
                //
                newItem.date = QDate::fromString(QString((const char*)sqlite3_column_text(stmt,2)),
                                                 "yyyy-MM-dd");
                //
                //newItem.date = QDate::fromString(QString((const char*)sqlite3_column_text(stmt,2)));
//                newItem.date = QDate::currentDate();
                qDebug() << newItem.date;

                mItems.append(newItem);
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

    mItems.append(newItem);

}
