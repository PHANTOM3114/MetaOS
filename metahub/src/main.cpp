#include <QDBusConnection>
#include <QDBusInterface>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "auth_manager.hpp"
#include "cicd_view_model.hpp"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    AuthManager authManager;
    authManager.setupGithub();

    CiCDViewModel cicd_view_model;

    engine.rootContext()->setContextProperty("CiCDViewModel", &cicd_view_model);
    engine.rootContext()->setContextProperty("AuthManager", &authManager);

    qmlRegisterSingletonType(QUrl("qrc:/Theme.qml"), "MetaOS.Theme", 1, 0, "Theme");

    const QUrl url(QStringLiteral("qrc:/Main.qml"));

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}