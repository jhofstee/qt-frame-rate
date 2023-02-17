import QtQuick
import QtQuick.Controls
import Test

Window {
	width: 640
	height: 480
	visible: true
	title: "Blocked gui thread"

	Rectangle {
		color: "blue"
		width: 100
		height: 100
		anchors.centerIn: parent

		RotationAnimation on rotation {
			loops: Animation.Infinite
			from: 0
			to: 360
			duration: 1000
		}
	}

	CheckBox {
		id: checkbox
		text: "queued"
		checked: Test.queued
		anchors {
			top: parent.top
			left: parent.left
		}
		onCheckedChanged: Test.queued = checked
	}

	Text {
		id: fpsText
		text: "fps"
		anchors {
			top: checkbox.bottom
			left: parent.left
		}
	}

	Text {
		id: latencyText
		anchors {
			top: fpsText.bottom
			left: parent.left
		}
		text: "latency"
	}

	onFrameSwapped: Test.frameUpdate()

	Timer {
		interval: 1000
		running: true
		repeat: true

		onTriggered: {
			fpsText.text = "fps: " + Test.fps() + "Hz";
			latencyText.text = "latency: " + Test.latency() + "ms";
		}
	}
}
