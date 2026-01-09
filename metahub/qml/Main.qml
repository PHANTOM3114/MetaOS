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
                    }

                    contentItem: Label {
                        text: parent.text
                        color: parent.ListView.isCurrentItem ? "white" : Theme.textColor
                        font.bold: parent.ListView.isCurrentItem
                    }

                    onClicked: {
                        menuListView.currentIndex = index
                    }
                }
            }
        }

        Rectangle {
            id: mainContent
            SplitView.fillWidth: true
            SplitView.minimumWidth: 300
            color: "transparent"

            Item {
                id: cicdView
                anchors.fill: parent
                visible: menuListView.currentIndex === 0

                Button {
                    id: refreshButton
                    text: "Refresh Pipelines"
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.margins: 20
                    onClicked: CiCDViewModel.fetchCicdData()
                }

                ListView {
                    id: pipelineList
                    anchors.top: refreshButton.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 20
                    spacing: 10
                    clip: true

                    model: CiCDViewModel

                    delegate: Rectangle {
                        width: pipelineList.width
                        height: 60
                        color: Qt.rgba(1, 1, 1, 0.1)
                        radius: 6
                        border.color: Qt.rgba(1, 1, 1, 0.2)
                        border.width: 1

                        Row {
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.leftMargin: 15
                            spacing: 15

                            Rectangle {
                                width: 12
                                height: 12
                                radius: 6
                                color: model.conclusion === "success" ? "#4caf50" : 
                                       model.conclusion === "failure" ? "#f44336" : "#ffeb3b"
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            Column {
                                anchors.verticalCenter: parent.verticalCenter
                                spacing: 4

                                Text {
                                    text: model.name
                                    color: "white"
                                    font.bold: true
                                    font.pixelSize: 16
                                }

                                Text {
                                    text: "#" + model.id
                                    color: "#aaaaaa"
                                    font.pixelSize: 12
                                }
                            }
                        }

                        Text {
                            text: model.status
                            color: "#dddddd"
                            font.pixelSize: 14
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.rightMargin: 15
                        }
                    }
                }
            }

            Text {
                visible: menuListView.currentIndex !== 0
                text: "Module not implemented yet"
                color: "grey"
                anchors.centerIn: parent
                font.pixelSize: 24
            }
        }
    }
}