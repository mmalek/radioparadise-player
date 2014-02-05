
var req = new XMLHttpRequest();
var HISTORY_COUNT = 4;

function parseSongInfo(document)
{
	var metadata = {
		albumTitle: "",
		artworkUrl: "",
		artistName: "",
		lyrics: "",
		rating: 0.0,
		songId: 0,
		songLength: 0,
		songTitle: "",
		userRating: 0,
		prevSongId: 0
	};

	for( var node = document.firstChild; node; node = node.nextSibling ) {
		if( node.nodeName === "album" ) {
			metadata.albumTitle = node.firstChild.nodeValue;
		} else if( node.nodeName === "artist" ) {
			metadata.artistName = node.firstChild.nodeValue;
		} else if( node.nodeName === "lyrics" ) {
			metadata.lyrics = node.firstChild.nodeValue;
		} else if( node.nodeName === "med_cover" ) {
			metadata.artworkUrl = node.firstChild.nodeValue;
		} else if( node.nodeName === "prev_song" ) {
			metadata.prevSongId = parseInt( node.firstChild.nodeValue );
		} else if( node.nodeName === "refresh_interval" && node.firstChild ) {
			metadata.songLength = parseInt( node.firstChild.nodeValue );
		} else if( node.nodeName === "rating" ) {
			metadata.rating = parseFloat( node.firstChild.nodeValue );
		} else if( node.nodeName === "songid" ) {
			metadata.songId = parseInt( node.firstChild.nodeValue );
		} else if( node.nodeName === "user_rating" && node.firstChild ) {
			metadata.userRating = parseInt( node.firstChild.nodeValue );
		} else if( node.nodeName === "title" ) {
			metadata.songTitle = node.firstChild.nodeValue;
		}
	}

	return metadata;
}

function updatePreviousSong(prevSongIndex,prevSongId)
{
	if(prevSongIndex < HISTORY_COUNT && ( prevSongIndex >= historyModel.count || historyModel.get(prevSongIndex).songId !== prevSongId )) {
		var req = new XMLHttpRequest();
		req.open( "GET", "http://radioparadise.com/ajax_xml_song_info.php?song_id=" + prevSongId );
		req.onreadystatechange = function(){
			if( req.readyState === XMLHttpRequest.DONE && req.responseXML ) {
				var metadata = parseSongInfo(req.responseXML.documentElement);
				historyModel.set(prevSongIndex,metadata);
				updatePreviousSong(prevSongIndex+1,metadata.prevSongId);
			}
		}
		req.send();
	}
}

function setCurrentSong(metadata)
{
	if( metadata.songId > HISTORY_COUNT - 1 && ( historyModel.count === 0 || historyModel.get(0).songId !== metadata.songId ) ) {
		progressTimer.stop();
		window.songPosition = 0;
		historyModel.insert( 0, metadata );
		if( historyModel.count > HISTORY_COUNT ) {
			historyModel.remove( HISTORY_COUNT, historyModel.count - HISTORY_COUNT );
		}
		updatePreviousSong(1,metadata.prevSongId);
		progressTimer.start();
	}

	songInfoTimer.interval = ( metadata.songLength > 0 ? metadata.songLength : 1 ) * 1000;
}

function fetchMetadata()
{
	req.abort();
	req.open( "GET", "http://radioparadise.com/ajax_xml_song_info.php?song_id=now" );
	req.onreadystatechange = function(){
		if( req.readyState === XMLHttpRequest.DONE && req.responseXML ) {
			var metadata = parseSongInfo(req.responseXML.documentElement);
			setCurrentSong(metadata);
		}
	}
	songInfoTimer.interval = 5000; // timeout
	req.send();
}
