
#include "songlistdownloader.hpp"
#include "metadata.hpp"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>

namespace
{
	void parseSongTag(Metadata& metadata, QXmlStreamReader& reader)
	{
		while(reader.readNextStartElement())
		{
			if(reader.name() == "album")
				metadata.albumTitle = reader.readElementText();
			else if(reader.name() == "artist")
				metadata.artistName = reader.readElementText();
			else if(reader.name() == "lyrics")
				metadata.lyrics = reader.readElementText();
			else if(reader.name() == "med_cover")
				metadata.artworkUrl = reader.readElementText();
			else if(reader.name() == "prev_song")
				metadata.prevSongId = reader.readElementText().toInt();
			else if(reader.name() == "refresh_interval")
				metadata.songLength = reader.readElementText().toInt();
			else if(reader.name() == "rating")
				metadata.rating = reader.readElementText().toFloat();
			else if(reader.name() == "songid")
				metadata.songId = reader.readElementText().toInt();
			else if(reader.name() == "user_rating")
				metadata.userRating = reader.readElementText().toInt();
			else if(reader.name() == "title")
				metadata.songTitle = reader.readElementText();
			else
				reader.skipCurrentElement();
		}
	}

} // namespace

SongListDownloader::SongListDownloader(
	SongList& songList,
	QNetworkAccessManager& networkAccessManager,
	QObject *parent)
:
	QObject(parent),
	songList_(songList),
	networkAccessManager_(networkAccessManager)
{
	for(int i = 0; i < songList_.modelSize(); ++i )
	{
		replies_.push_back(nullptr);
		QTimer* timer = new QTimer(this);
		timer->setSingleShot(true);
		timer->setInterval(5000);
		connect(timer, SIGNAL(timeout()), &timeoutMapper_, SLOT(map()));
		timeoutMapper_.setMapping(timer,i);
		timeoutTimers_.push_back(timer);
	}

	connect(&timer_,SIGNAL(timeout()),SLOT(fetchCurrSongInfo()));
	timer_.setSingleShot(true);
	timer_.start(0);

	connect(&finishedMapper_, SIGNAL(mapped(int)), SLOT(onSongFetched(int)));
	connect(&timeoutMapper_, SIGNAL(mapped(int)), SLOT(onFetchingFailed(int)));
}

void SongListDownloader::fetchCurrSongInfo()
{
	fetchSongInfo(0, 0);
}

void SongListDownloader::fetchSongInfo(int i, int songId)
{
	if(replies_[i])
	{
		replies_[i]->abort();
		replies_[i]->deleteLater();
	}
	timeoutTimers_[i]->stop();

	QUrl url = QString("http://radioparadise.com/ajax_xml_song_info.php?song_id=%1").arg(songId);
	replies_[i] = networkAccessManager_.get(QNetworkRequest(url));
	connect(replies_[i], SIGNAL(finished()), &finishedMapper_, SLOT(map()));
	finishedMapper_.setMapping(replies_[i],i);
	timeoutTimers_[i]->start();
}

void SongListDownloader::onSongFetched(int i)
{
	QNetworkReply* reply = replies_.at(i).data();
	if(reply->error())
	{
		qWarning() << "Cannot download song info " << reply->url() << ": " << reply->errorString();
		onFetchingFailed(i);
	}
	else
	{
		timeoutTimers_.at(i)->stop();
		Metadata metadata;

		QXmlStreamReader reader(reply);
		while(reader.readNextStartElement())
		{
			if(reader.name() == "song")
				parseSongTag(metadata, reader);
			else
				reader.skipCurrentElement();
		}

		if(i < songList_.modelSize() - 1 && metadata.prevSongId > 3)
		{
			fetchSongInfo(i+1, metadata.prevSongId);
		}

		if(i == 0)
		{
			timer_.setInterval(1000*metadata.songLength);
			timer_.start();

			if(metadata.songId > 3 && (songList_.size() == 0 || songList_.at(0).songId != metadata.songId))
			{
				songList_.push_front(metadata);
			}
		}
		else if(i > 0)
		{
			if(i < songList_.size())
				songList_.set(i, metadata);
			else
				songList_.push_back(metadata);
		}
	}
	reply->deleteLater();
}

void SongListDownloader::onFetchingFailed(int i)
{
	if(networkAccessManager_.networkAccessible() != QNetworkAccessManager::NotAccessible)
	{
		// try again
		fetchSongInfo(i, getSongId(i));
	}
}

int SongListDownloader::getSongId(int i) const
{
	return i > 0 ? songList_.at(i-1).prevSongId : 0;
}
