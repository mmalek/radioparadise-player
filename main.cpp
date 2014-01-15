#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>

#include "mpris2.hpp"
#include "config.hpp"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QQmlApplicationEngine engine(QUrl("qrc:///qml/main.qml"));
	app.setWindowIcon(QIcon("qrc:///images/logo.png"));

#ifdef BUILD_MPRIS
	Mpris2 mpris2(engine);
#endif // BUILD_MPRIS

	return app.exec();
}
