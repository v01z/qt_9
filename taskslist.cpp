#include "taskslist.h"
#include <QDebug>
#include <sqlite3.h>
//#include "/usr/include/sqlite3.h"
//#include <sqlite3ext.h>

TasksList::TasksList(QObject *parent) : QObject(parent)
{
    //**********
    mItems.append( { true, QStringLiteral("Wash the car"), QDate::currentDate() });
    mItems.append( { false, QStringLiteral("Fix the sink"), QDate::currentDate() });
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
   //
    qDebug() << "here";
    //debug
    qDebug() << "***** list vector inside TasksList module *****";
    for (const auto &elem: mItems)
        qDebug() << "done: " << elem.done << ", descr: " << elem.description;
    qDebug() << "***** end TasksList module *****";
    //end debug

    //https://www.sqlitetutorial.net/sqlite-autoincrement/

    const QString SQL_QUERY_CREATE =
            //"CREATE TABLE IF NOT EXISTS ORGANIZER (\"done\" BOOL, \"task\" VARCHAR, \"date\" VARCHAR)";
            //"CREATE TABLE IF NOT EXISTS ORGANIZER (\'done\' BOOL, \'task\' VARCHAR, \'date\' VARCHAR)";
            "CREATE TABLE IF NOT EXISTS ORGANIZER (\'done\' INTEGER, \'task\' TEXT, \'date\' TEXT);";


    const QString SQL_QUERY_INSERT_TEMPLATE { "INSERT INTO ORGANIZER VALUES (" };

    sqlite3 *db = 0; // хэндл объекта соединение к БД
    char *err = 0;

    // открываем соединение
    if( sqlite3_open("my_cosy_database.dblite", &db) )
    {
        std::fprintf(stderr, "Ошибка открытия/создания БД: %s\n", sqlite3_errmsg(db));
        return;
    }

    if (sqlite3_exec(db, SQL_QUERY_CREATE.toStdString().c_str(), 0, 0, &err))
        {
            std::fprintf(stderr, "Ошибка SQL: %sn", err);
            sqlite3_free(err);
        }


    // выполняем SQL
    for (const auto &elem : mItems)
    {
        //QString SQL_QUERY_STR { SQL_QUERY_INSERT_TEMPLATE + (elem.done?"1":"0") + ", \"" +
        QString SQL_QUERY_STR { SQL_QUERY_INSERT_TEMPLATE + (elem.done?'1':'0') + ", \'" +
            //elem.description + "\", \"" + (elem.date).toString("yyyy-MM-dd") + "\")" };
            //elem.description + "\", \'" + (elem.date).toString("yyyy-MM-dd") + "\')" };
            elem.description + "\', \'" + (elem.date).toString("yyyy-MM-dd") + "\');" };
        qDebug() << SQL_QUERY_STR;

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


   const QString SQL_QUERY_CREATE =
            //"CREATE TABLE IF NOT EXISTS ORGANIZER (\"done\" BOOL, \"task\" VARCHAR, \"date\" VARCHAR)";
            //"CREATE TABLE IF NOT EXISTS ORGANIZER (\'done\' BOOL, \'task\' VARCHAR, \'date\' VARCHAR)";
            "CREATE TABLE IF NOT EXISTS ORGANIZER (\'done\' INTEGER, \'task\' TEXT, \'date\' TEXT);";


    const QString SQL_QUERY_SELECT_TEMPLATE { "SELECT * FROM ORGANIZER VALUES (" };

    sqlite3 *db = 0; // хэндл объекта соединение к БД
    char *err = 0;

    // открываем соединение
    if( sqlite3_open("my_cosy_database.dblite", &db) )
    {
        std::fprintf(stderr, "Ошибка открытия/создания БД: %s\n", sqlite3_errmsg(db));
        return;
    }

    if (sqlite3_exec(db, SQL_QUERY_CREATE.toStdString().c_str(), 0, 0, &err))
        {
            std::fprintf(stderr, "Ошибка SQL: %sn", err);
            sqlite3_free(err);
        }
     //sqlite3_stmt *stmt;
//sqlite3_prepare_v2(db, "select * from demo where date == ", -1, &stmt, NULL);

//sqlite3_bind_int(stmt, 1, 16);

    /*
while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    std::printf("%s is %d years old\n", sqlite3_column_text(stmt, 0), sqlite3_column_int(stmt, 1));
}
*/

    sqlite3_stmt *res;
    //int rc = sqlite3_prepare_v2(db, "SELECT * FROM ORGANIZER WHERE date == 2022-02-20", -1, &res, 0);

    char *err_msg = 0;
    //const char *sql = "SELECT * FROM ORGANIZER";
    const char *sql = "SELECT * FROM ORGANIZER;";

    int rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        //say error
        //qDebug() << "an error happened during sqlte3_prepare";
        qDebug() << "an error happened during sqlite3_exec";
        std::fprintf(stderr, "SQL error: %s\n", err_msg);
//        qDebug() << err_msg;
        sqlite3_close(db);
        return;
    }
    /*
    rc = sqlite3_step(res);
    if (rc == SQLITE_ROW) {
            std::printf("%s\n", sqlite3_column_text(res, 0));
        }
        */
//    int i{};
    /*
    while (sqlite3_step(res) == SQLITE_ROW)
    {
        qDebug() << "row " << i++ << ": " << sqlite3_column_text(res,1);
    }
    */

/// ////

    // выполняем SQL
    /*
    for (const auto &elem : mItems)
    {
        QString SQL_QUERY_STR { SQL_QUERY_INSERT_TEMPLATE + (elem.done?"1":"0") + ", \"" +
            elem.description + "\", \"" + (elem.date).toString("yyyy-MM-dd") + "\")" };
        qDebug() << SQL_QUERY_STR;

        if (sqlite3_exec(db, SQL_QUERY_STR.toStdString().c_str(), 0, 0, &err))
        {
            std::fprintf(stderr, "Ошибка SQL: %sn", err);
            sqlite3_free(err);
        }
    }
    */
    // закрываем соединение

    sqlite3_close(db);



    //debug
   mItems.clear();
   mItems.append( { false, QStringLiteral("New task number one"), QDate::currentDate() });
   mItems.append( { true, QStringLiteral("New task number two"), QDate::currentDate() });

   qDebug() << "Should be updated";
   qDebug() << mItems.size();
    for (const auto &elem: mItems)
    {
        qDebug() << "done: "<< elem.done << " | description: " << elem.description <<
                    ", date: " << elem.date;
    }
    //end debug
}

int callback(void *NotUsed, int argc, char **argv,
             char **azColName) {

    static int row_count{};
    std::printf("************ row number %d: *********** \n", row_count);

    for (int i{}; i < argc; i++)
    {
        std::printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    }
