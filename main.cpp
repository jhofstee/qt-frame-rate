#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "test.hpp"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	Test test;

	QQmlApplicationEngine engine;
	qmlRegisterSingletonInstance("Test", 1, 0, "Test", &test);


	const QUrl url(u"qrc:/qt-frame-rate/main.qml"_qs);
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
					 &app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
	}, Qt::QueuedConnection);
	engine.load(url);

	return app.exec();
}
