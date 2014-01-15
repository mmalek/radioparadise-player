
#include "mpris2.hpp"
#include "mpris2_player.h"
#include "mpris2_root.h"

#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <QDBusConnection>
#include <QMediaPlayer>
#include <QWidget>

namespace
{
	const char* MPRIS2_SERVICE_NAME = "org.mpris.MediaPlayer2.radioparadise-player";
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

Mpris2::Mpris2(QQmlApplicationEngine& engine, QObject* parent)
:
	QObject(parent),
	engine_(engine),
	window_(qobject_cast<QQuickWindow*>(engine_.rootObjects().front()))
{
	new Mpris2Root(this);
	new Mpris2Player(this);

	if(!QDBusConnection::sessionBus().registerService(MPRIS2_SERVICE_NAME))
	{
		qWarning( "Cannot register D-BUS service %s", MPRIS2_SERVICE_NAME );
		return;
	}

	if(!QDBusConnection::sessionBus().registerObject(MPRIS2_OBJECT_PATH, this))
	{
		qWarning( "Cannot register D-BUS object %s", MPRIS2_SERVICE_NAME );
		return;
	}

	connect( window_, SIGNAL(playbackStateChanged()), SLOT(onPlaybackStateChanged()) );
	connect( window_, SIGNAL(volumeChanged()), SLOT(onVolumeChanged()) );
}

void Mpris2::Raise()
{
	window_->requestActivate();
}

void Mpris2::Quit()
{
	window_->close();
}

QString Mpris2::identity() const
{
	return window_->title();
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
	QMetaObject::invokeMethod( window_, "stop" );
}

void Mpris2::Play()
{
	QMetaObject::invokeMethod( window_, "play" );
}

QString Mpris2::playbackStatus() const
{
	switch( window_->property("playbackState").toInt() )
	{
		case QMediaPlayer::PlayingState:
			return QStringLiteral( "Playing" );
		case QMediaPlayer::PausedState:
			return QStringLiteral( "Paused" );
		default:
			return QStringLiteral( "Stopped" );
	}
}

QVariantMap Mpris2::metadata() const
{
	return metadata_;
}

double Mpris2::volume() const
{
	return window_->property("volume").toDouble();
}

void Mpris2::setVolume(double value)
{
	window_->setProperty("volume", QVariant(value));
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
