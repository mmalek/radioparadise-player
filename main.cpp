#include <QIcon>
#include <QApplication>
#include <QtQml>
#include <QQuickWindow>

#include "artworkdownloader.hpp"
#include "metadata.hpp"
#include "songlist.hpp"
#include "songlistdownloader.hpp"
#include "config.hpp"

#include "moc_metadata.cpp"
#include "moc_songlist.cpp"

#ifdef BUILD_MPRIS
#include "mpris2.hpp"
#endif // BUILD_MPRIS

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName(QStringLiteral("Radio Paradise"));
	app.setApplicationVersion("1.0");

	qmlRegisterType<Metadata>();
	qmlRegisterType<SongList>("com.paradise.SongList", 1, 0, "SongList");

	QQmlApplicationEngine engine(QUrl("qrc:///qml/main.qml"));

	QQuickWindow* window = qobject_cast<QQuickWindow*>(engine.rootObjects().front());
	if(!window)
	{
		qFatal( "Cannot obtain access to the main window!" );
		return 1;
	}

	SongList* songList = window->findChild<SongList*>("songList");
	if(!songList)
	{
		qFatal( "Cannot obtain access to the song list object!" );
		return 1;
	}

	window->setIcon(QIcon(":/images/logo.png"));
	window->setTitle(app.applicationName());

	QNetworkAccessManager networkAccessManager;

	ArtworkDownloader artworkDownloader(*window, *songList, networkAccessManager);

	SongListDownloader songListDownloader(*songList, networkAccessManager);

#ifdef BUILD_MPRIS
	Mpris2 mpris2(*window, *songList, app.applicationPid());
#endif // BUILD_MPRIS

	return app.exec();
}
