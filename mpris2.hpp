#ifndef MPRIS2_HPP
#define MPRIS2_HPP

#include <QObject>
#include <QDBusObjectPath>

class QQuickWindow;

class Mpris2 : public QObject
{
	Q_OBJECT
public:
	explicit Mpris2(QQuickWindow& window, QObject *parent = 0);

	//
	// org.mpris.MediaPlayer2 methods
	//

	void Raise();
	void Quit();

	//
	// org.mpris.MediaPlayer2 properties
	//

	Q_PROPERTY( bool CanQuit READ canQuit )
	bool canQuit() const { return true; }

	Q_PROPERTY( bool CanRaise READ canRaise )
	bool canRaise() const { return true; }

	Q_PROPERTY( bool HasTrackList READ hasTrackList )
	bool hasTrackList() const { return false; }

	Q_PROPERTY( QString Identity READ identity )
	QString identity() const;

	Q_PROPERTY( QString DesktopEntry READ desktopEntry )
	QString desktopEntry() const;

	Q_PROPERTY( QStringList SupportedUriSchemes READ supportedUriSchemes )
	QStringList supportedUriSchemes() const { return QStringList(); }

	Q_PROPERTY( QStringList SupportedMimeTypes READ supportedMimeTypes )
	QStringList supportedMimeTypes() const { return QStringList(); }

	Q_PROPERTY( bool Fullscreen READ fullscreen WRITE setFullscreen )
	bool fullscreen() const { return false; }
	void setFullscreen(bool) {}

	Q_PROPERTY( bool CanSetFullscreen READ canSetFullscreen )
	bool canSetFullscreen() const { return false; }

	//
	// org.mpris.MediaPlayer2.Player methods
	//

	void Next();
	void Previous() {}
	void Pause() {}
	void PlayPause() {}
	void Stop();
	void Play();
	void Seek(qlonglong offset) {}
	void SetPosition(const QDBusObjectPath& trackId, qlonglong offset) {}
	void OpenUri(const QString& uri) {}

	//
	// org.mpris.MediaPlayer2.Player properties
	//

	Q_PROPERTY( QString PlaybackStatus READ playbackStatus )
	QString playbackStatus() const;

	Q_PROPERTY( QString LoopStatus READ loopStatus WRITE setLoopStatus )
	QString loopStatus() const { return QStringLiteral( "None" ); }
	void setLoopStatus(const QString& value) {}

	Q_PROPERTY( double Rate READ rate WRITE setRate )
	double rate() const { return 1.0; }
	void setRate(double value) {}

	Q_PROPERTY( bool Shuffle READ shuffle WRITE setShuffle )
	bool shuffle() const { return false; }
	void setShuffle(bool value) {}

	Q_PROPERTY( QVariantMap Metadata READ metadata )
	QVariantMap metadata() const;

	Q_PROPERTY( double Volume READ volume WRITE setVolume )
	double volume() const;
	void setVolume(double value);

	Q_PROPERTY( qlonglong Position READ position )
	qlonglong position() const;

	Q_PROPERTY( double MinimumRate READ minimumRate )
	double minimumRate() const { return 1.0; }

	Q_PROPERTY( double MaximumRate READ maximumRate )
	double maximumRate() const { return 1.0; }

	Q_PROPERTY( bool CanGoNext READ canGoNext )
	bool canGoNext() const;

	Q_PROPERTY( bool CanGoPrevious READ canGoPrevious )
	bool canGoPrevious() const { return false; }

	Q_PROPERTY( bool CanPlay READ canPlay )
	bool canPlay() const { return true; }

	Q_PROPERTY( bool CanPause READ canPause )
	bool canPause() const { return false; }

	Q_PROPERTY( bool CanSeek READ canSeek )
	bool canSeek() const { return false; }

	Q_PROPERTY( bool CanControl READ canControl )
	bool canControl() const { return true; }

Q_SIGNALS:
	void Seeked(qlonglong position);

private Q_SLOTS:
	void onPlaybackStateChanged();
	void onVolumeChanged();

private:
	QQuickWindow& window_;
	QVariantMap metadata_;
};

#endif // MPRIS2_HPP
