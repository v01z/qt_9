#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "taskslist.h"
#include "tasksmodel.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    //change to smth like URL-mode
    qmlRegisterType<TasksModel>("Tasks", 1, 0, "TasksModel");
    qmlRegisterUncreatableType<TasksList>("Tasks", 1, 0, "TasksList",
        QStringLiteral("TasksList should not be created in QML"));

    TasksList tasksList;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(QStringLiteral("tasksList"),
        &tasksList);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
