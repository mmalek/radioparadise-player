﻿
#include "mpris2.hpp"
#include "mpris2_player.h"
#include "mpris2_root.h"

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
} // namespace

Mpris2::Mpris2(QQuickWindow& window, qint64 pid, QObject* parent)
:
	QObject(parent),
	window_(window)
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
	connect( &window_, SIGNAL(albumTitleChanged()), SLOT(onAlbumTitleChanged()) );
	connect( &window_, SIGNAL(artistNameChanged()), SLOT(onArtistNameChanged()) );
	connect( &window_, SIGNAL(artworkChanged()), SLOT(onArtworkChanged()) );
	connect( &window_, SIGNAL(lyricsChanged()), SLOT(onLyricsChanged()) );
	connect( &window_, SIGNAL(ratingChanged()), SLOT(onRatingChanged()) );
	connect( &window_, SIGNAL(songTitleChanged()), SLOT(onSongTitleChanged()) );
	connect( &window_, SIGNAL(userRatingChanged()), SLOT(onUserRatingChanged()) );
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
	return QString();
}

void Mpris2::Next()
{
	// Implement PSD (Play Something Different) feature here
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
	metaData_["xesam:album"] = window_.property("albumTitle");
	metaData_["xesam:artist"] = window_.property("artistName");
//	metaData_["mpris:artUrl"] = window_.property("artwork");
	metaData_["xesam:asText"] = window_.property("lyrics");
	metaData_["xesam:autoRating"] = window_.property("rating").toDouble()/10.0;
	metaData_["xesam:title"] = window_.property("songTitle");
	metaData_["xesam:userRating"] = window_.property("userRating").toDouble()/10.0;
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
	return 0;
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

void Mpris2::onAlbumTitleChanged()
{
	metaData_["xesam:album"] = window_.property("albumTitle");
	EmitPropertiesChanged("Metadata", metaData_);
}

void Mpris2::onArtistNameChanged()
{
	metaData_["xesam:artist"] = window_.property("artistName");
	EmitPropertiesChanged("Metadata", metaData_);
}

void Mpris2::onArtworkChanged()
{
//	metaData_["mpris:artUrl"] = window_.property("artwork");
//	EmitPropertiesChanged("Metadata", metaData_);
}

void Mpris2::onLyricsChanged()
{
	metaData_["xesam:asText"] = window_.property("lyrics");
	EmitPropertiesChanged("Metadata", metaData_);
}

void Mpris2::onRatingChanged()
{
	metaData_["xesam:autoRating"] = window_.property("rating").toDouble()/10.0;
	EmitPropertiesChanged("Metadata", metaData_);
}

void Mpris2::onSongTitleChanged()
{
	metaData_["xesam:title"] = window_.property("songTitle");
	EmitPropertiesChanged("Metadata", metaData_);
}

void Mpris2::onUserRatingChanged()
{
	metaData_["xesam:userRating"] = window_.property("userRating").toDouble()/10.0;
	EmitPropertiesChanged("Metadata", metaData_);
}
