#ifndef TASKSLIST_H
#define TASKSLIST_H

#include <QObject>
#include <QVector>
#include <QDate>

struct TaskItem{
    bool done;
    QString description;
    QDate date;

    bool operator ==(const TaskItem &other) const
    {
       return std::tie(other.done, other.description, other.date) ==
               std::tie(done, description, date);
    }

    bool operator < (const TaskItem &other) const
    {
       return std::tie(other.done, other.description, other.date) <
               std::tie(done, description, date);
    }
};

class TasksList : public QObject
{
    Q_OBJECT
public:
    explicit TasksList(QObject *parent = nullptr);

    QVector<TaskItem> items() const;

    bool setItemAt(int index, const TaskItem &item);

    Q_INVOKABLE
    const int getTotalTasksCount() const;

    Q_INVOKABLE
    void setTotalTasksCount(int);

    Q_INVOKABLE
    void newTaskIsAccepted(QString);

    void debug_debug(const QVector<TaskItem>&, bool);

signals:
    void on_preItemAppended();
    void on_postItemAppended();

    void on_preItemRemoved(int index);
    void on_postItemRemoved();

public slots:
    void appendItem(QDate);
    void removeCompletedItems();

    void writeDataToSQLiteBase();

    void updateCurrentItems(QDate);

private:
    QVector<TaskItem> mFullDataItems;
    QVector<TaskItem> mCurrentItems;

    int totalTasksCount;

    void getDataFromDB();
    void updateFullDataItems();



};

#endif // TASKSLIST_H
