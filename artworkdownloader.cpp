#include "artworkdownloader.hpp"
#include "songlist.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QQuickWindow>
#include <QTemporaryFile>

ArtworkDownloader::ArtworkDownloader(
	QQuickWindow& window,
	SongList& songList,
	QNetworkAccessManager& networkAccessManager,
	QObject *parent)
:
	QObject(parent),
	window_(window),
	songList_(songList),
	networkAccessManager_( networkAccessManager ),
	temporaryFile_(0)
{
	connect(&songList, SIGNAL(curSongChanged()), SLOT(onCurSongChanged()));
}

void ArtworkDownloader::onCurSongChanged()
{
	if(	!songList_.isEmpty() && songList_.at(0).artworkUrl.isValid() )
	{
		QNetworkReply* reply = networkAccessManager_.get(QNetworkRequest(songList_.at(0).artworkUrl));
		connect(reply, SIGNAL(finished()), SLOT(onDownloadFinished()));
	}
}

void ArtworkDownloader::onDownloadFinished()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
	if(reply->error())
	{
		qWarning() << "Cannot download artwork " << reply->url() << ": " << reply->errorString();
	}
	else
	{
		if( temporaryFile_ )
		{
			temporaryFile_->deleteLater();
		}

		temporaryFile_ = new QTemporaryFile( this );
		if( temporaryFile_->open() )
		{
			temporaryFile_->write(reply->readAll());
			temporaryFile_->close();
			window_.setProperty("artworkLocalFile", QUrl::fromLocalFile(temporaryFile_->fileName()));
		}
		else
		{
			qWarning() << "Cannot open temporary file!";
		}
	}
	reply->deleteLater();
}
