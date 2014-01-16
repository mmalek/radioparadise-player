#ifndef ARTWORKDOWNLOADER_HPP
#define ARTWORKDOWNLOADER_HPP

#include <QObject>
#include <QNetworkAccessManager>

class QNetworkReply;
class QQuickWindow;
class QTemporaryFile;

class ArtworkDownloader : public QObject
{
	Q_OBJECT
public:
	explicit ArtworkDownloader(QQuickWindow& window, QObject *parent = 0);

private Q_SLOTS:
	void onArtworkUrlChanged();
	void onDownloadFinished(QNetworkReply* reply);

private:
	QQuickWindow& window_;
	QNetworkAccessManager networkAccessManager_;
	QTemporaryFile* temporaryFile_;
};

#endif // ARTWORKDOWNLOADER_HPP
