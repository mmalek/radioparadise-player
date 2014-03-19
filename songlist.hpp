#ifndef SONGLIST_HPP
#define SONGLIST_HPP

#include "metadata.hpp"

#include <QAbstractListModel>
#include <QList>

class QTimer;

class SongList : public QAbstractListModel
{
	Q_OBJECT

	Q_PROPERTY(int modelSize READ modelSize CONSTANT)
	Q_PROPERTY(int curSongLength READ curSongLength NOTIFY curSongChanged)
	Q_PROPERTY(int curSongPosition READ curSongPosition NOTIFY curSongPositionChanged)

public:
	enum MetadataRoles
	{
		AlbumTitle = Qt::UserRole + 1,
		ArtworkUrl,
		ArtistName,
		Lyrics,
		Rating,
		SongId,
		SongLength,
		SongTitle,
		UserRating,
		PrevSongId
	};

public:
	explicit SongList(QObject *parent = 0);

	int modelSize() const { return modelSize_; }
	int curSongLength() const { return !songs_.isEmpty() ? songs_.front().songLength : 0; }
	int curSongPosition() const { return curSongPosition_; }

	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	virtual QHash<int, QByteArray> roleNames() const;

	void push_front(Metadata const& metadata);
	void push_back(Metadata const& metadata);
	Metadata const& at(int i) const { return songs_.at(i); }
	bool isEmpty() const { return songs_.isEmpty(); }
	int size() const { return songs_.size(); }
	void set(int i, Metadata const& metadata);

Q_SIGNALS:
	void curSongChanged();
	void curSongPositionChanged();

private Q_SLOTS:
	void onProgressTimerTimeout();

private:
	void removeRedundantElements();

private:
	const int modelSize_ = 4;
	int curSongPosition_;
	QTimer* progressTimer_;
	QList<Metadata> songs_;
};

#endif // SONGLIST_HPP
