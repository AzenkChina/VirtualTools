#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include "virtualinput.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    VirtualInput input;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("input", &input);
    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:///main.qml")));
    input.ui = component.create();

    return app.exec();
}
