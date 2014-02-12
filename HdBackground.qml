import QtQuick 2.2

Item {

	states: [
		State {
			name: "bgImage"
			PropertyChanges { target: bgImage; opacity: 1.0 }
			PropertyChanges { target: fgImage; opacity: 0.0 }
		},
		State {
			name: "fgImage"
			PropertyChanges { target: bgImage; opacity: 1.0 }
			PropertyChanges { target: fgImage; opacity: 1.0 }
		}
	]

	transitions: [
		Transition {
			PropertyAnimation { property: "opacity"; easing.type: Easing.Linear }
		}
	]

	Image {
		id: bgImage
		anchors.fill: parent
		fillMode: Image.PreserveAspectCrop
		opacity: 0.0
		onStatusChanged: if( status === Image.Ready ) parent.state = "bgImage"
	}

	Image {
		id: fgImage
		anchors.fill: parent
		fillMode: Image.PreserveAspectCrop
		opacity: 0.0
		onStatusChanged: if( status === Image.Ready ) parent.state = "fgImage"
	}

	Timer {
		interval: 15000;
		running: parent.visible
		repeat: true
		triggeredOnStart: true
		property var req: new XMLHttpRequest()
		onTriggered: {
			req.open( "GET", "http://radioparadise.com/readtxt.php" );
			req.onreadystatechange = function() {
				if( req.readyState === XMLHttpRequest.DONE ) {
					if( parent.state === "bgImage" )
						fgImage.source = req.responseText.trim();
					else
						bgImage.source = req.responseText.trim();
				}
			}
			req.send();
		}
	}
}
