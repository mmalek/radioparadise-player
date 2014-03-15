#ifndef SONGLISTDOWNLOADER_HPP
#define SONGLISTDOWNLOADER_HPP

#include "songlist.hpp"

#include <QList>
#include <QObject>
#include <QPointer>
#include <QSignalMapper>
#include <QTimer>

class QNetworkAccessManager;
class QNetworkReply;

class SongListDownloader : public QObject
{
	Q_OBJECT
public:
	explicit SongListDownloader(
		SongList& songList,
		QNetworkAccessManager& networkAccessManager,
		QObject *parent = 0);

public Q_SLOTS:
	void fetchCurrSongInfo();
	void fetchSongInfo(int i, int songId);
	void onSongFetched(int i);
	void onFetchingFailed(int i);

private:
	int getSongId(int i) const;

private:
	QTimer timer_;
	QList<QPointer<QNetworkReply>> replies_;
	QList<QTimer*> timeoutTimers_;
	QSignalMapper finishedMapper_;
	QSignalMapper timeoutMapper_;
	SongList& songList_;
	QNetworkAccessManager& networkAccessManager_;
};

#endif // SONGLISTDOWNLOADER_HPP
