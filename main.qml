import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.1
import QtMultimedia 5.0

Window {
	id: window
	width: 500
	height: 500 / 2.56
	color: palette.window
	visible: true
	title: qsTr("Radio Paradise")
	minimumWidth: controlsRect.width + 20
	minimumHeight: radioParadiseLogo.y + radioParadiseLogo.height + controlsRect.height + 20

	readonly property int playbackState: player.playbackState
	property real volume: 0.5

	property int songId;
	property string albumTitle;
	property string artistName;
	property string artwork;
	property string lyrics;
	property real rating;
	property string songTitle;
	property int userRating;

	function stop() {
		player.stop();
	}

	function play() {
		playStream(streamQuality.currentIndex);
	}

	function playStream(qualityIndex) {
		player.source = "http://stream-tx4.radioparadise.com/rp_%1.ogg".arg( qualityModel.get( qualityIndex ).quality );
		print( "Playing stream " + player.source );
		player.play();
	}

	SystemPalette { id: palette; colorGroup: SystemPalette.Active }

	Audio {
		id: player
		volume: window.volume
	}

	Image {
		id: backgroundImage

		anchors.fill: parent

		fillMode: Image.PreserveAspectCrop
		visible: hdSwitch.checked
	}

	Image {
		id: radioParadiseLogo

		anchors.left: parent.left
		anchors.top: parent.top
		anchors.leftMargin: 10
		anchors.topMargin: 10

		source: "qrc:///images/logo_hd.png"
		MouseArea {
			anchors.fill: parent
			cursorShape: Qt.PointingHandCursor
			onClicked: Qt.openUrlExternally( "http://www.radioparadise.com" )
		}
	}

	Item {
		id: controlsRect

		anchors.horizontalCenter: parent.horizontalCenter
		anchors.bottom: parent.bottom
		anchors.bottomMargin: 10

		width: controlsLayout.width + 25
		height: coverImage.height + controlsLayout.height + 30

		Rectangle {
			id: controlsBackground

			anchors.fill: parent
			color: "black"
			radius: 10
			opacity: 0.0
		}

		Image {
			id: coverImage

			anchors.left: parent.left
			anchors.top: parent.top
			anchors.leftMargin: 10
			anchors.topMargin: 10

			width: 64
			height: 64

			source: window.artwork
		}

		Text {
			id: title

			anchors.verticalCenter: coverImage.verticalCenter
			anchors.left: coverImage.right
			anchors.right: songTime.left
			anchors.leftMargin: 10
			anchors.rightMargin: 5

			text: ( window.artistName && window.songTitle ) ? "%1 — %2".arg( window.artistName ).arg( window.songTitle ) : ""
			font.pointSize: 12
			font.underline: titleMouseArea.containsMouse
			elide: Text.ElideRight
			MouseArea {
				id: titleMouseArea
				anchors.fill: parent
				cursorShape: Qt.PointingHandCursor
				hoverEnabled: true
				onClicked: { Qt.openUrlExternally( "http://www.radioparadise.com/rp2-content.php?name=Music&file=songinfo&song_id==%1".arg( parent.songId ) ) }
			}
		}

		Text {
			id: songTime

			anchors.verticalCenter: title.verticalCenter
			anchors.right: parent.right
			anchors.rightMargin: 10

			property int seconds: 0.0

			text: "(%1:%2)".arg( leadingZero( Math.floor( seconds/60 ) ) ).arg( leadingZero( seconds % 60 ) )
			font.pointSize: title.font.pointSize
			color: title.color
			textFormat: title.textFormat

			function leadingZero( num ) {
				var str = num.toString();
				if( str.length === 1 )
				{
					str = "0" + str;
				}
				return str;
			}
		}

		RowLayout {
			id: controlsLayout
			spacing: 5
			anchors.left: parent.left
			anchors.top: coverImage.bottom
			anchors.leftMargin: 10
			anchors.topMargin: 10

			Slider {
				id: volumeSlider
				value: window.volume
				onValueChanged: window.volume = value
			}

			ComboBox {
				id: streamQuality
				currentIndex: 1
				model: ListModel {
					id: qualityModel
					ListElement { text: QT_TR_NOOP("32k"); quality: 32 }
					ListElement { text: QT_TR_NOOP("96k"); quality: 96 }
					ListElement { text: QT_TR_NOOP("192k"); quality: 192 }
				}
				onActivated: {
					if( player.playbackState === Audio.PlayingState )
						playStream(index);
				}
			}

			Button {
				id: stopButton
				text: qsTr("Stop")
				onClicked: window.stop()
				visible: player.playbackState === Audio.PlayingState
			}

			Button {
				id: playButton
				text: qsTr("Play")
				onClicked: window.play()
				visible: !stopButton.visible
			}
		}

		states: [
			State {
				name: "HD_DISABLED";
				when: !hdSwitch.checked
				PropertyChanges { target: controlsBackground; opacity: 0.0 }
				PropertyChanges { target: title; color: palette.windowText }
			},
			State {
				name: "HD_ENABLED";
				when: hdSwitch.checked
				PropertyChanges { target: controlsBackground;  opacity: 0.5 }
				PropertyChanges { target: title; color: "#FFFFFF" }
			}
		]

		transitions: [
			Transition { PropertyAnimation { property: "opacity"; easing.type: Easing.Linear } },
			Transition { ColorAnimation { easing.type: Easing.Linear } }
		]
	}

	Text {
		text: qsTr("HD")
		anchors.verticalCenter: hdSwitch.verticalCenter
		anchors.right: hdSwitch.left
		anchors.rightMargin: 5
		style: Text.Outline; styleColor: "#FFFFFF"
	}

	Switch {
		id: hdSwitch
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.rightMargin: 10
		anchors.topMargin: 10
	}

	Timer {
		interval: 15000;
		running: hdSwitch.checked
		repeat: true
		triggeredOnStart: true
		onTriggered: {
			var req = new XMLHttpRequest();
			req.open( "GET", "http://radioparadise.com/readtxt.php" );
			req.onreadystatechange = function() {
				if( req.readyState === XMLHttpRequest.DONE ) {
					backgroundImage.source = req.responseText.trim();
				}
			}
			req.send();
		}
	}

	Timer {
		id: songInfoTimer
		interval: 0
		running: true
		repeat: true
		triggeredOnStart: true
		onTriggered: {
			progressTimer.stop();

			var req = new XMLHttpRequest();
			req.open( "GET", "http://radioparadise.com/ajax_xml_song_info.php?song_id=now" );
			req.onreadystatechange = function() {
				if( req.readyState === XMLHttpRequest.DONE ) {
					var root = req.responseXML.documentElement;
					var interval
					for( var node = root.firstChild; node; node = node.nextSibling ) {
						if( node.nodeName === "album" ) {
							window.albumTitle = node.firstChild.nodeValue;
						} else if( node.nodeName === "artist" ) {
							window.artistName = node.firstChild.nodeValue;
						} else if( node.nodeName === "lyrics" ) {
							window.lyrics = node.firstChild.nodeValue;
						} else if( node.nodeName === "med_cover" ) {
							window.artwork = node.firstChild.nodeValue;
						} else if( node.nodeName === "refresh_interval" ) {
							interval = parseInt( node.firstChild.nodeValue )
						} else if( node.nodeName === "rating" ) {
							window.rating = parseFloat( node.firstChild.nodeValue )
						} else if( node.nodeName === "songid" ) {
							window.songId = node.firstChild.nodeValue;
						} else if( node.nodeName === "user_rating" ) {
							if( node.firstChild )
								window.userRating = parseInt( node.firstChild.nodeValue )
						} else if( node.nodeName === "title" ) {
							window.songTitle = node.firstChild.nodeValue;
						}
					}

					songInfoTimer.interval = interval * 1000;
					songTime.seconds = interval;
					progressTimer.start();
				}
			}
			req.send();
		}
	}

	Timer {
		id: progressTimer
		interval: 1000
		running: false
		repeat: true
		onTriggered: { --songTime.seconds }
	}
}
