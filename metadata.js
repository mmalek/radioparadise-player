
var req = new XMLHttpRequest();

function onRequest(interval)
{
	if( req.readyState === XMLHttpRequest.DONE && req.responseXML ) {
		var root = req.responseXML.documentElement;

		var albumTitle;
		var artworkUrl;
		var artistName;
		var lyrics;
		var rating;
		var songId;
		var songLength;
		var songTitle;
		var userRating;

		for( var node = root.firstChild; node; node = node.nextSibling ) {
			if( node.nodeName === "album" ) {
				albumTitle = node.firstChild.nodeValue;
			} else if( node.nodeName === "artist" ) {
				artistName = node.firstChild.nodeValue;
			} else if( node.nodeName === "lyrics" ) {
				lyrics = node.firstChild.nodeValue;
			} else if( node.nodeName === "med_cover" ) {
				artworkUrl = node.firstChild.nodeValue;
			} else if( node.nodeName === "refresh_interval" ) {
				songLength = parseInt( node.firstChild.nodeValue );
				songInfoTimer.interval = songLength * 1000;
			} else if( node.nodeName === "rating" ) {
				rating = parseFloat( node.firstChild.nodeValue );
			} else if( node.nodeName === "songid" ) {
				songId = parseInt( node.firstChild.nodeValue );
			} else if( node.nodeName === "user_rating" && node.firstChild ) {
				userRating = parseInt( node.firstChild.nodeValue );
			} else if( node.nodeName === "title" ) {
				songTitle = node.firstChild.nodeValue;
			}
		}

		if( songId > 3 ) {
			progressTimer.stop();
			window.albumTitle = albumTitle;
			window.artistName = artistName;
			window.artworkUrl = artworkUrl;
			window.lyrics = lyrics;
			window.rating = rating;
			window.songId = songId;
			window.songLength = songLength;
			window.songPosition = 0;
			window.songTitle = songTitle;
			window.userRating = userRating ? userRating : 0;

			historyModel.insert( 0, { "artistName": artistName, "songTitle": songTitle, "songId": songId } );
			if( historyModel.count > 4 ) {
				historyModel.remove( 4, historyModel.count - 4 );
			}
			progressTimer.start();
		}
	}
}

function fetchMetadata()
{
	req.abort();
	req.open( "GET", "http://radioparadise.com/ajax_xml_song_info.php?song_id=now" );
	req.onreadystatechange = onRequest; //function() { onRequest() }
	songInfoTimer.interval = 5000; // timeout
	req.send();
}
