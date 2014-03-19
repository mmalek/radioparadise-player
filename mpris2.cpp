
#include "mpris2.hpp"
#include "mpris2_player.h"
#include "mpris2_root.h"
#include "songlist.hpp"

#include <QQuickItem>
#include <QQuickWindow>
#include <QDBusConnection>
#include <QMediaPlayer>

namespace
{
	const char* MPRIS2_SERVICE_NAME = "org.mpris.MediaPlayer2.radioparadise-player-%1";
	const char* MPRIS2_OBJECT_PATH = "/org/mpris/MediaPlayer2";

	void EmitPropertiesChanged(const QString& name, const QVariant& val, const QString& mprisEntity = "org.mpris.MediaPlayer2.Player")
	{
		QDBusMessage msg = QDBusMessage::createSignal(MPRIS2_OBJECT_PATH, "org.freedesktop.DBus.Properties", "PropertiesChanged");
		QVariantMap map;
		map.insert(name, val);
		QVariantList args = QVariantList() << mprisEntity << map << QStringList();
		msg.setArguments(args);
		QDBusConnection::sessionBus().send(msg);
	}

	QVariantMap toMetadata( SongList const& songList )
	{
		QVariantMap metadata;

		if( !songList.isEmpty() )
		{
			Metadata const& songMetadata = songList.at(0);

			metadata["xesam:album"] = songMetadata.albumTitle;
			metadata["xesam:artist"] = songMetadata.artistName;
			metadata["xesam:asText"] = songMetadata.lyrics;
			metadata["mpris:length"] = static_cast<long long int>(songMetadata.songLength) * 1000000ll;
			metadata["xesam:title"] = songMetadata.songTitle;

			const double rating = songMetadata.rating/10.0;
			if( rating > 0.0 )
			{
				metadata["xesam:autoRating"] = rating;
			}

			const double userRating = static_cast<double>(songMetadata.userRating)/10.0;
			if( userRating > 0.0 )
			{
				metadata["xesam:userRating"] = userRating;
			}
			else if( rating > 0.0 )
			{
				metadata["xesam:userRating"] = rating;
			}
		}

		return metadata;
	}

} // namespace

Mpris2::Mpris2(QQuickWindow& window, SongList& songList, qint64 pid, QObject* parent)
:
	QObject(parent),
	window_(window),
	songList_(songList)
{
	new Mpris2Root(this);
	new Mpris2Player(this);

	const QString serviceName = QString(MPRIS2_SERVICE_NAME).arg(pid);
	if(!QDBusConnection::sessionBus().registerService(serviceName))
	{
		qWarning() << "Cannot register D-BUS service " << serviceName;
		return;
	}

	if(!QDBusConnection::sessionBus().registerObject(MPRIS2_OBJECT_PATH, this))
	{
		qWarning() << "Cannot register D-BUS object " << MPRIS2_OBJECT_PATH;
		return;
	}

	connect( &window_, SIGNAL(playbackStateChanged()), SLOT(onPlaybackStateChanged()) );
	connect( &window_, SIGNAL(volumeChanged()), SLOT(onVolumeChanged()) );
	connect( &window_, SIGNAL(artworkLocalFileChanged()), SLOT(onArtworkLocalFileChanged()) );
	connect( &songList_, SIGNAL(curSongChanged()), SLOT(onCurSongChanged()) );
}

void Mpris2::Raise()
{
	window_.requestActivate();
}

void Mpris2::Quit()
{
	window_.close();
}

QString Mpris2::identity() const
{
	return window_.title();
}

QString Mpris2::desktopEntry() const
{
	return QStringLiteral("radioparadise-player");
}

void Mpris2::Next()
{
	// Implement PSD (Play Something Different) feature here
}

void Mpris2::PlayPause()
{
	QMetaObject::invokeMethod( &window_, "play" );
}

void Mpris2::Stop()
{
	QMetaObject::invokeMethod( &window_, "stop" );
}

void Mpris2::Play()
{
	QMetaObject::invokeMethod( &window_, "play" );
}

QString Mpris2::playbackStatus() const
{
	switch( window_.property("playbackState").toInt() )
	{
		case QMediaPlayer::PlayingState:
			return QStringLiteral( "Playing" );
		case QMediaPlayer::PausedState:
			return QStringLiteral( "Paused" );
		default:
			return QStringLiteral( "Stopped" );
	}
}

QVariantMap Mpris2::metadata()
{
	metaData_ = toMetadata(songList_);
	metaData_["mpris:artUrl"] = window_.property("artworkLocalFile").toString();
	return metaData_;
}

double Mpris2::volume() const
{
	return window_.property("volume").toDouble();
}

void Mpris2::setVolume(double value)
{
	window_.setProperty("volume", QVariant(value));
}

qlonglong Mpris2::position() const
{
	return static_cast<long long>( songList_.curSongPosition() ) * 1000000ll;
}

bool Mpris2::canGoNext() const
{
	// return true here when PSD feature will be available
	return false;
}

void Mpris2::onPlaybackStateChanged()
{
	EmitPropertiesChanged("PlaybackStatus", playbackStatus());
}

void Mpris2::onVolumeChanged()
{
	EmitPropertiesChanged("Volume", volume());
}

void Mpris2::onArtworkLocalFileChanged()
{
	metaData_["mpris:artUrl"] = window_.property("artworkLocalFile").toString();
	EmitPropertiesChanged("Metadata", metaData_);
}

void Mpris2::onCurSongChanged()
{
	metaData_ = toMetadata(songList_);
	EmitPropertiesChanged("Metadata", metaData_);
}
