import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
import MetaOS.Theme 1.0

ApplicationWindow {
    id: rootWindow
    visible: true
    width: 800
    height: 600
    title: "MetaHub"
    flags: Qt.Window
    color: "transparent"

    // background: Image {
    //     id: wallpaperImage
    //     sourceSize: Qt.size(rootWindow.width, rootWindow.height)

    //     GaussianBlur {
    //         anchors.fill: parent
    //         source: wallpaperImage
    //         radius: 16
    //     }
    // }

    SplitView {
        id: splitView
        anchors.fill: parent

        Frame {

            id: sidebarFrame

            SplitView.preferredWidth: rootWindow.width * 0.25
            SplitView.minimumWidth: 150
            SplitView.maximumWidth: 600

            background: Rectangle {
                color: Theme.sidebarColor
            }
            padding: 0

            ListView {
                id: menuListView
                anchors.fill: parent

                model: ["CI/CD Module", "VPN Controller", "Matrix Integration"]

                delegate: ItemDelegate {
                    width: menuListView.width
                    text: modelData
                    padding: 10

                    background: Rectangle {
                        color: parent.ListView.isCurrentItem ? Theme.accentColor : (parent.hovered ? Qt.rgba(1,1,1,0.1) : "transparent")
                        radius: 4
                        Behavior on color { ColorAnimation { duration: 150 } }
                    }

                    contentItem: Row {
                        spacing: 10
                        /*
                        Image {
                            source: "qrc:/icons/" + modelData + ".svg"
                            width: 20; height: 20
                        }
                        */
                        Label {
                            text: parent.parent.text
                            color: parent.ListView.isCurrentItem ? "white" : Theme.textColor
                            font.bold: parent.ListView.isCurrentItem
                        }
                    }

                    onClicked: {
                        infoLabel.text = "Selected: " + modelData
                        menuListView.currentIndex = index
                    }
                }
            }
        }

        Rectangle {

            id: mainContent

            SplitView.fillWidth: true
            SplitView.minimumWidth: 300

            width: parent.width * 0.8
            height: 1
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            color: Qt.rgba(1,1,1,0.05)

            Label {
                id: infoLabel
                text: "Information Panel"
                color: "white"
                font.pixelSize: 20
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 20
            }

            Button {
                id : fetchButton
                text : "Fetch"
                visible: infoLabel.text === "Selected: CI/CD Module"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: infoLabel.bottom
                anchors.topMargin: 20
            }
        }
    }
}