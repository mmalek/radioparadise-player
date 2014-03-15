#ifndef SONGLIST_HPP
#define SONGLIST_HPP

#include <QList>
#include <QObject>
#include <QQmlListProperty>

#include "metadata.hpp"

class SongList : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int modelSize READ modelSize CONSTANT)
	Q_PROPERTY(QQmlListProperty<Metadata> model READ model NOTIFY modelChanged)

public:
	int modelSize() const { return modelSize_; }

	QQmlListProperty<Metadata> model() { return QQmlListProperty<Metadata>(this, model_); }

	void push_front(Metadata* metadata)
	{
		if( metadata )
		{
			model_.push_front(metadata);
			while( model_.size() > modelSize_ )
			{
				model_.back()->deleteLater();
				model_.pop_back();
			}
			Q_EMIT modelChanged();
		}
	}

	void push_back(Metadata* metadata)
	{
		if( metadata )
		{
			model_.push_back(metadata);
			while( model_.size() > modelSize_ )
			{
				model_.back()->deleteLater();
				model_.pop_back();
			}
			Q_EMIT modelChanged();
		}
	}

	Metadata& at(int i) const { return *model_.at(i); }
	bool isEmpty() const { return model_.isEmpty(); }
	int size() const { return model_.size(); }

Q_SIGNALS:
	void modelChanged();

private:
	const int modelSize_ = 4;
	QList<Metadata*> model_;
};

#endif // SONGLIST_HPP
