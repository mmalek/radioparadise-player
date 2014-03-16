
#include "songlist.hpp"

SongList::SongList(QObject *parent)
:
	QAbstractListModel(parent)
{
}

int SongList::rowCount(const QModelIndex& /*parent*/) const
{
	return songs_.size();
}

QVariant SongList::data(const QModelIndex& index, int role) const
{
	if(index.row() < songs_.size())
	{
		Metadata const& song = songs_.at(index.row());
		switch( role )
		{
			case AlbumTitle: return song.albumTitle;
			case ArtworkUrl: return song.artworkUrl;
			case ArtistName: return song.artistName;
			case Lyrics: return song.lyrics;
			case Rating: return song.rating;
			case SongId: return song.songId;
			case SongLength: return song.songLength;
			case SongTitle: return song.songTitle;
			case UserRating: return song.userRating;
			case PrevSongId: return song.prevSongId;
		}
	}

	return QVariant();
}

QHash<int, QByteArray> SongList::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[AlbumTitle] = "albumTitle";
	roles[ArtworkUrl] = "artworkUrl";
	roles[ArtistName] = "artistName";
	roles[Lyrics] = "lyrics";
	roles[Rating] = "rating";
	roles[SongId] = "songId";
	roles[SongLength] = "songLength";
	roles[SongTitle] = "songTitle";
	roles[UserRating] = "userRating";
	roles[PrevSongId] = "prevSongId";
	return roles;
}


void SongList::push_front(Metadata const& metadata)
{
	beginInsertRows(QModelIndex(), 0, 0);
	songs_.push_front(metadata);
	endInsertRows();

	removeRedundantElements();

	Q_EMIT curSongChanged();
}

void SongList::push_back(Metadata const& metadata)
{
	beginInsertRows(QModelIndex(), songs_.size(), songs_.size());
	songs_.push_back(metadata);
	endInsertRows();

	removeRedundantElements();
}

void SongList::set(int i, Metadata const& metadata)
{
	songs_[i] = metadata;
	Q_EMIT dataChanged(index(i), index(i));

	if(i == 0)
	{
		Q_EMIT curSongChanged();
	}
}

void SongList::removeRedundantElements()
{
	if( songs_.size() > modelSize_ )
	{
		beginRemoveRows(QModelIndex(), modelSize_, songs_.size()-1);
		while( songs_.size() > modelSize_ )
		{
			songs_.pop_back();
		}
		endRemoveRows();
	}
}
