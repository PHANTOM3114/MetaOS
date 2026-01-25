import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: authPopup
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    anchors.centerIn: Overlay.overlay
    property string authUrl: ""
    property string userCode: "----"

    background: Rectangle {
        color: "#2E3440"
        border.color: "#4C566A"
        border.width: 1
        radius: 8
    }

    contentItem: ColumnLayout {
        spacing: 20
        width: 400

        Label {
            text: "GitHub Authorization"
            font.pixelSize: 20
            font.bold: true
            color: "#ECEFF4"
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: "Follow the link and enter the code:"
            color: "#D8DEE9"
            font.pixelSize: 14
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            text: authPopup.authUrl
            color: "#88C0D0"
            font.pixelSize: 14
            font.underline: true
            Layout.alignment: Qt.AlignHCenter
            
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: Qt.openUrlExternally(authPopup.authUrl)
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: "#3B4252"
            radius: 4
            border.color: "#4C566A"

            Text {
                text: authPopup.userCode
                anchors.centerIn: parent
                color: "#EBCB8B"
                font.pixelSize: 32
                font.family: "Monospace"
                font.bold: true
                font.letterSpacing: 4
            }
        }

        Button {
            text: "Copy Code"
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 150
            
            contentItem: Text {
                text: parent.text
                font: parent.font
                color: "#2E3440"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: parent.down ? "#81A1C1" : "#88C0D0"
                radius: 4
            }

            onClicked: {
                clipboardHelper.text = authPopup.userCode
                clipboardHelper.selectAll()
                clipboardHelper.copy()
                text = "Copied!"
                timer.start()
            }

            Timer {
                id: timer
                interval: 2000
                onTriggered: parent.text = "Copy Code"
            }
        }
        
        TextEdit {
            id: clipboardHelper
            visible: false
        }
    }
}