#include "taskslist.h"
#include <QDebug>
#include <sqlite3.h>
//#include "/usr/include/sqlite3.h"
//#include <sqlite3ext.h>

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

    //debug
    /*
    qDebug() << "***** list vector inside TasksList module *****";
    for (const auto &elem: mItems)
        qDebug() << "done: " << elem.done << ", descr: " << elem.description;
    qDebug() << "***** end TasksList module *****";
    */
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
   //
    qDebug() << "here";
    //debug
    qDebug() << "***** list vector inside TasksList module *****";
    for (const auto &elem: mItems)
        qDebug() << "done: " << elem.done << ", descr: " << elem.description;
    qDebug() << "***** end TasksList module *****";
    //end debug

//    const char* SQL = "CREATE TABLE IF NOT EXISTS foo(a,b,c); INSERT INTO FOO VALUES(1,2,3); INSERT INTO FOO SELECT * FROM FOO;";

    //https://www.sqlitetutorial.net/sqlite-autoincrement/

    const char* SQL =
            //"CREATE TABLE ORGANIZER (\"surname\" VARCHAR,\"name\" VARCHAR, \"id\" INTEGER PRIMARY KEY  NOT NULL ,\"flag\" BOOL)";
            //"CREATE TABLE ORGANIZER (\"id\" INTEGER PRIMARY KEY NOT NULL,\"done\" BOOL, \"task\" VARCHAR)";
            "CREATE TABLE ORGANIZER (\"done\" BOOL, \"task\" VARCHAR)";

    const char* SQL_INSERT =
            "INSERT INTO ORGANIZER VALUES (0, \"my very first task\")";

    const size_t MAX_STR_SIZE { 128 };
//    char SQL_QUERY [MAX_STR_SIZE];

    QString SQL_QUERY_STR_BEGIN { "INSERT INTO ORGANIZER VALUES (" };

    sqlite3 *db = 0; // хэндл объекта соединение к БД
    char *err = 0;

    // открываем соединение
    if( sqlite3_open("my_cosy_database.dblite", &db) )
    {
        std::fprintf(stderr, "Ошибка открытия/создания БД: %s\n", sqlite3_errmsg(db));
        return;
    }
    // выполняем SQL
    //else if (sqlite3_exec(db, SQL, 0, 0, &err))
    for (const auto &elem : mItems)
    {
        QString SQL_QUERY_STR { SQL_QUERY_STR_BEGIN + (elem.done?"1":"0") + ", \"" + elem.description + "\")" };
        qDebug() << SQL_QUERY_STR;

        //if (sqlite3_exec(db, SQL_INSERT, 0, 0, &err))
        if (sqlite3_exec(db, SQL_QUERY_STR.toStdString().c_str(), 0, 0, &err))
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
