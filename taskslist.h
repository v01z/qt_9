#ifndef TASKSLIST_H
#define TASKSLIST_H

#include <QObject>
#include <QVector>
#include <QDate>

struct TaskItem{
    bool done;
    QString description;
    QDate date;
};

class TasksList : public QObject
{
    Q_OBJECT
public:
    explicit TasksList(QObject *parent = nullptr);

    QVector<TaskItem> items() const;

    bool setItemAt(int index, const TaskItem &item);

    //int callback(void *, int, char **, char **);

signals:
    void on_preItemAppended();
    void on_postItemAppended();

    void on_preItemRemoved(int index);
    void on_postItemRemoved();

public slots:
    void appendItem();
    void removeCompletedItems();

    void writeDataToSQLiteBase(); // *************
    void updateDataFromSQLiteBase();// ***********


private:
    QVector<TaskItem> mItems;


};

//int callback(void *, int, char **, char **);

#endif // TASKSLIST_H
