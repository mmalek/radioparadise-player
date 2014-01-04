#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlEngine>

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	app.setApplicationName( QObject::tr("Radio Paradise") );

	QQuickView view;
	view.connect(view.engine(), SIGNAL(quit()), SLOT(close()));
	view.setResizeMode(QQuickView::SizeRootObjectToView);
	view.setSource( QUrl("qrc:///qml/main.qml") );
	view.show();

	return app.exec();
}
