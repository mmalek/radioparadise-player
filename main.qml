import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.1
import QtMultimedia 5.0

Window {
    width: 500
    height: 500 / 2.56
    color: palette.window
    visible: true
    title: qsTr("Radio Paradise")
    minimumWidth: controlsRect.width + 10
    minimumHeight: radioParadiseLogo.y + radioParadiseLogo.height + controlsRect.height + 15

    SystemPalette { id: palette; colorGroup: SystemPalette.Active }

    function playStream(qualityIndex) {
        stream.source = "http://stream-tx4.radioparadise.com/rp_%1.ogg".arg( qualityModel.get( qualityIndex ).quality );
        print( "Playing stream " + stream.source );
        stream.play();
    }

    Audio {
        id: stream
        volume: volume.value
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
		anchors.bottomMargin: 5
    
		width: controlsLayout.x + controlsLayout.width + 5
	    height: coverImage.height + controlsLayout.height + 20

		Rectangle {
			id: controlsBackground

			anchors.fill: parent
			color: "black"
			radius: 10
			opacity: 0.5
			visible: hdSwitch.checked
		}

		Image {
			id: coverImage

			anchors.left: parent.left
			anchors.top: parent.top
			anchors.leftMargin: 5
			anchors.topMargin: 5

			width: 64
			height: 64
		}

		Text {
			id: title

			anchors.verticalCenter: coverImage.verticalCenter
			anchors.left: coverImage.right
			anchors.right: songTime.left
			anchors.leftMargin: 10
			anchors.rightMargin: 5

			property int songId;
			property string artist;
			property string songTitle;

			text: ( songId && artist && songTitle ) ?
				"<a href='http://www.radioparadise.com/rp2-content.php?name=Music&file=songinfo&song_id==%1' style='color: %4; text-decoration: none'>%2 — %3</a>".arg( songId ).arg( artist ).arg( songTitle ).arg( color )
				: ""
			font.pointSize: 12
			color: hdSwitch.checked ? "#FFFFFF" : palette.windowText
			textFormat: Text.RichText
			elide: Text.ElideRight
			onLinkActivated: Qt.openUrlExternally( link )
			MouseArea {
				anchors.fill: parent
				cursorShape: Qt.PointingHandCursor
				onPressed:{ mouse.accepted = false }
			}
		}

		Text {
			id: songTime

			anchors.verticalCenter: title.verticalCenter
			anchors.right: parent.right
			anchors.rightMargin: 5

			property int seconds: 0.0

			text: leadingZero( Math.floor( seconds/60 ) ) + ":" + leadingZero( seconds % 60 )
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
			anchors.leftMargin: 5
			anchors.topMargin: 10

			Slider {
				id: volume
				value: 0.5
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
					if( stream.playbackState === Audio.PlayingState )
						playStream(index);
				}
			}

			Button {
				id: stopButton
				text: qsTr("Stop")
				onClicked: stream.stop()
				visible: stream.playbackState === Audio.PlayingState
			}

			Button {
				id: playButton
				text: qsTr("Play")
				onClicked: playStream(streamQuality.currentIndex)
				visible: !stopButton.visible
			}
		}
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
                    var songId;
                    var songTitle;
                    var artist;
                    for( var node = root.firstChild; node; node = node.nextSibling ) {
                        if( node.nodeName === "refresh_interval" ) {
                            print( "interval: " + node.firstChild.nodeValue );
                            interval = parseInt( node.firstChild.nodeValue )
                        } else if( node.nodeName === "med_cover" ) {
                            coverImage.source = node.firstChild.nodeValue;
                        } else if( node.nodeName === "songid" ) {
                            songId = node.firstChild.nodeValue;
                        } else if( node.nodeName === "title" ) {
                            print( "title: " + node.firstChild.nodeValue );
                            songTitle = node.firstChild.nodeValue;
                        } else if( node.nodeName === "artist" ) {
                            print( "artist: " + node.firstChild.nodeValue );
                            artist = node.firstChild.nodeValue;
                        }
                    }

                    songInfoTimer.interval = interval * 1000;
                    songTime.seconds = interval;
                    progressTimer.start();

					title.songId = songId;
					title.artist = artist;
					title.songTitle = songTitle;
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
