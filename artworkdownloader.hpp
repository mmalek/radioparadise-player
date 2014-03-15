#ifndef ARTWORKDOWNLOADER_HPP
#define ARTWORKDOWNLOADER_HPP

#include <QObject>
#include <QNetworkAccessManager>

class QNetworkAccessManager;
class QNetworkReply;
class QQuickWindow;
class QTemporaryFile;
class SongList;

class ArtworkDownloader : public QObject
{
	Q_OBJECT
public:
	explicit ArtworkDownloader(
		QQuickWindow& window,
		SongList& songList,
		QNetworkAccessManager& networkAccessManager,
		QObject *parent = 0);

private Q_SLOTS:
	void onSongListChanged();
	void onDownloadFinished();

private:
	QQuickWindow& window_;
	SongList& songList_;
	QNetworkAccessManager& networkAccessManager_;
	QTemporaryFile* temporaryFile_;
};

#endif // ARTWORKDOWNLOADER_HPP
