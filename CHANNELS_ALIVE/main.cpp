#include "main.h"


int main(int argc, char *argv[])
{

	QGuiApplication::setApplicationName("Channels A-Live");
	QGuiApplication::setOrganizationName("DSTU");

	QGuiApplication app(argc, argv);

	BackAPI API;

	QIcon::setThemeName("Channels A-Live");

	QQuickStyle::setStyle("Material");

	QQmlApplicationEngine engine;
	const QUrl url(u"qrc:/CHANNELSALIVE/main.qml"_qs);
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
			 &app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
	}, Qt::QueuedConnection);
	engine.rootContext()->setContextProperty("API", &API);
	engine.load(url);

	return app.exec();
}
