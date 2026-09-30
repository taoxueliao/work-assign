#include <QCoreApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "viewmodel/AssignController.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QCoreApplication::setOrganizationName(QStringLiteral("WordAssign"));
    QCoreApplication::setApplicationName(QStringLiteral("app"));
    QQmlApplicationEngine engine;
    AssignController controller;
    engine.rootContext()->setContextProperty(QStringLiteral("assignController"), &controller);
    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));
    return app.exec();
}
