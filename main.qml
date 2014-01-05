import QtQuick 2.0
import QtMultimedia 5.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: rectangle1
    width: 500
    height: 281.25
    color: palette.window

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

    Image {
        id: coverImage
        anchors.left: parent.left
        anchors.bottom: streamQuality.top
        anchors.leftMargin: 10
        anchors.bottomMargin: 10
        width: 64
        height: 64
    }

    Text {
        anchors.verticalCenter: coverImage.verticalCenter
        anchors.left: coverImage.right
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        id: title
        font.pointSize: 12
        style: Text.Outline; styleColor: "#FFFFFF"
        textFormat: Text.RichText
        elide: Text.ElideRight
        onLinkActivated: Qt.openUrlExternally( link )
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onPressed:{ mouse.accepted = false }
        }
    }

    Slider {
        id: volume

        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 10
        anchors.bottomMargin: 10

        value: 0.5
    }

    ComboBox {
        id: streamQuality

        anchors.left: volume.right
        anchors.bottom: volume.bottom
        anchors.leftMargin: 5

        currentIndex: 1
        model: ListModel {
            id: qualityModel
            ListElement { text: "32k"; quality: 32 }
            ListElement { text: "96k"; quality: 96 }
            ListElement { text: "192k"; quality: 192 }
        }
        onActivated: {
            if( stream.playbackState === Audio.PlayingState )
                playStream(index);
        }
    }

    Button {
        id: stopButton

        anchors.left: streamQuality.right
        anchors.bottom: streamQuality.bottom
        anchors.leftMargin: 5

        text: qsTr("Stop")
        onClicked: stream.stop()
    }

    Button {
        id: playButton

        anchors.left: stopButton.right
        anchors.bottom: stopButton.bottom
        anchors.leftMargin: 5

        text: qsTr("Play")
        onClicked: playStream(streamQuality.currentIndex)
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
            var req = new XMLHttpRequest();
            req.open( "GET", "http://radioparadise.com/ajax_xml_song_info.php?song_id=now" );
            req.onreadystatechange = function() {
                if( req.readyState === XMLHttpRequest.DONE ) {
                    var root = req.responseXML.documentElement;
                    var songId;
                    var songTitle;
                    var artist;
                    for( var node = root.firstChild; node; node = node.nextSibling ) {
                        if( node.nodeName === "refresh_interval" ) {
                            songInfoTimer.interval = parseInt( node.firstChild.nodeValue ) * 1000;
                            print( "interval: " + songInfoTimer.interval );
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

                    var titleText = "<a href='http://www.radioparadise.com/rp2-content.php?name=Music&file=songinfo&song_id==%1' style='color: black; text-decoration: none'>%2 — %3</a>";
                    title.text = titleText.arg( songId ).arg( artist ).arg( songTitle );
                }
            }
            req.send();
        }
    }
}

