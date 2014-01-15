#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuick/QQuickWindow>

#include "mpris2.hpp"
#include "config.hpp"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QQmlApplicationEngine engine(QUrl("qrc:///qml/main.qml"));

	if(QQuickWindow* window = qobject_cast<QQuickWindow*>(engine.rootObjects().front()))
	{
		window->setIcon(QIcon("qrc:///images/logo.png"));

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
