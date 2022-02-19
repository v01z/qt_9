#ifndef TASKSLIST_H
#define TASKSLIST_H

#include <QObject>
#include <QVector>

struct TaskItem{
    bool done;
    QString description;
};

class TasksList : public QObject
{
    Q_OBJECT
public:
    explicit TasksList(QObject *parent = nullptr);

    QVector<TaskItem> items() const;

    bool setItemAt(int index, const TaskItem &item);


signals:
    void on_preItemAppended();
    void on_postItemAppended();

    void on_preItemRemoved(int index);
    void on_postItemRemoved();

public slots:
    void appendItem();
    void removeCompletedItems();


private:
    QVector<TaskItem> mItems;
};

#endif // TASKSLIST_H
