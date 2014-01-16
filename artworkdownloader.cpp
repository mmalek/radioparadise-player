#include "artworkdownloader.hpp"

#include <QNetworkReply>
#include <QQuickWindow>
#include <QTemporaryFile>

ArtworkDownloader::ArtworkDownloader(QQuickWindow& window, QObject *parent)
:
	QObject(parent),
	window_(window),
	temporaryFile_(0)
{
	connect(&window_, SIGNAL(artworkUrlChanged()), SLOT(onArtworkUrlChanged()));
	connect(&networkAccessManager_, SIGNAL(finished(QNetworkReply*)), SLOT(onDownloadFinished(QNetworkReply*)));
}

void ArtworkDownloader::onArtworkUrlChanged()
{
	const QUrl artworkUrl = window_.property("artworkUrl").toUrl();

	if( artworkUrl.isValid() )
	{
		networkAccessManager_.get(QNetworkRequest(artworkUrl));
	}
}

void ArtworkDownloader::onDownloadFinished(QNetworkReply* reply)
{
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
}
