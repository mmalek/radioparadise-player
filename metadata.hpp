#ifndef METADATA_HPP
#define METADATA_HPP

#include <QObject>
#include <QString>
#include <QUrl>

class Metadata : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString albumTitle MEMBER albumTitle NOTIFY albumTitleChanged)
	Q_PROPERTY(QUrl artworkUrl MEMBER artworkUrl NOTIFY artworkUrlChanged)
	Q_PROPERTY(QString artistName MEMBER artistName NOTIFY artistNameChanged)
	Q_PROPERTY(QString lyrics MEMBER lyrics NOTIFY lyricsChanged)
	Q_PROPERTY(qreal rating MEMBER rating NOTIFY ratingChanged)
	Q_PROPERTY(int songId MEMBER songId NOTIFY songIdChanged)
	Q_PROPERTY(int songLength MEMBER songLength NOTIFY songLengthChanged)
	Q_PROPERTY(QString songTitle MEMBER songTitle NOTIFY songTitleChanged)
	Q_PROPERTY(int userRating MEMBER userRating NOTIFY userRatingChanged)
	Q_PROPERTY(int prevSongId MEMBER prevSongId NOTIFY prevSongIdChanged)

public:
	QString albumTitle;
	QUrl artworkUrl;
	QString artistName;
	QString lyrics;
	qreal rating;
	int songId;
	int songLength;
	QString songTitle;
	int userRating;
	int prevSongId;

	explicit Metadata(QObject *parent = 0)
	:
		QObject(parent),
		rating(0.0),
		songId(0),
		songLength(0),
		userRating(0),
		prevSongId(0)
	{
	}

	void operator=(Metadata const& rhs)
	{
		albumTitle = rhs.albumTitle;
		artworkUrl = rhs.artworkUrl;
		artistName = rhs.artistName;
		lyrics = rhs.lyrics;
		rating = rhs.rating;
		songId = rhs.songId;
		songLength = rhs.songLength;
		songTitle = rhs.songTitle;
		userRating = rhs.userRating;
		prevSongId = rhs.prevSongId;
	}

Q_SIGNALS:
	void albumTitleChanged();
	void artworkUrlChanged();
	void artistNameChanged();
	void lyricsChanged();
	void ratingChanged();
	void songIdChanged();
	void songLengthChanged();
	void songTitleChanged();
	void userRatingChanged();
	void prevSongIdChanged();
};

#endif // METADATA_HPP
