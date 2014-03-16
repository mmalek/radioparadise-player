#ifndef METADATA_HPP
#define METADATA_HPP

#include <QString>
#include <QUrl>

struct Metadata
{
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
};

#endif // METADATA_HPP
