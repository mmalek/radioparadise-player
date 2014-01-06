#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QQmlApplicationEngine engine(QUrl("qrc:///qml/main.qml"));
	app.setWindowIcon(QIcon("qrc:///images/logo.png"));
	return app.exec();
}
