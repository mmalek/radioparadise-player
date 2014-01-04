import QtQuick 2.0
import QtMultimedia 5.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: rectangle1
    width: 360
    height: 360
    color: palette.window

    SystemPalette { id: palette; colorGroup: SystemPalette.Active }

    Audio {
        id: stream
        source: "http://stream-uk1.radioparadise.com/mp3-32"
    }

    Image {
        id: backgroundImage
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        visible: hdSwitch.checked
    }

    Image {
        id: coverImage
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 10
        anchors.bottomMargin: 10
        width: 64
        height: 64
    }

    Text {
        anchors.verticalCenter: coverImage.verticalCenter
        anchors.left: coverImage.right
        anchors.leftMargin: 10
        id: title
        font.pointSize: 18
        style: Text.Outline; styleColor: "#FFFFFF"
        onLinkActivated: Qt.openUrlExternally( link )
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onPressed:{ mouse.accepted = false }
        }
    }

    Button {
        id: playButton
        text: qsTr("Play")
        anchors.right: stopButton.left
        anchors.bottom: parent.bottom
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
        onClicked: stream.play()
    }

    Button {
        id: stopButton
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
        text: qsTr("Stop")
        onClicked: stream.stop()
    }

    Text {
        text: "HD"
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

                    var titleText = "<a href=\"http://www.radioparadise.com/rp2-content.php?name=Music&file=songinfo&song_id==%1\">%2 — %3</a>";
                    title.text = titleText.arg( songId ).arg( artist ).arg( songTitle );
                }
            }
            req.send();
        }
    }
}

