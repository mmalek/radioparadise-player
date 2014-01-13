
#include "mpris2.hpp"
#include "mpris2_player.h"
#include "mpris2_root.h"

#include <QDBusConnection>

namespace
{
	const char* MPRIS2_SERVICE_NAME = "org.mpris.MediaPlayer2.radioparadise-player";
	const char* MPRIS2_OBJECT_PATH = "/org/mpris/MediaPlayer2";
} // namespace

Mpris2::Mpris2(QObject *parent)
:
	QObject(parent)
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
}

void Mpris2::Raise()
{

}

void Mpris2::Quit()
{

}

QString Mpris2::identity() const
{
	return QString();
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

}

void Mpris2::Play()
{

}

QString Mpris2::playbackStatus() const
{
	// available values: "Playing", "Paused" or "Stopped"
	return QStringLiteral( "Stopped" );
}

QVariantMap Mpris2::metadata() const
{
	return metadata_;
}

double Mpris2::volume() const
{
	return 1.0;
}

void Mpris2::setVolume(double value)
{

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

bool Mpris2::canPlay() const
{
	return false;
}
