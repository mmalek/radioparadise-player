#include <QIcon>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

#include "artworkdownloader.hpp"
#include "mpris2.hpp"
#include "config.hpp"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QQmlApplicationEngine engine(QUrl("qrc:///qml/main.qml"));

	if(QQuickWindow* window = qobject_cast<QQuickWindow*>(engine.rootObjects().front()))
	{
		window->setIcon(QIcon(":/images/logo.png"));

		ArtworkDownloader artworkDownloader(*window);

#ifdef BUILD_MPRIS
		Mpris2 mpris2(*window, app.applicationPid());
#endif // BUILD_MPRIS

		return app.exec();
	}
	else
	{
		qFatal( "Cannot obtain access to the main window!" );
		return -1;
	}
}
