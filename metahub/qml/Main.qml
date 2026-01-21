import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import "."

ApplicationWindow {
    id: window
    visible: true
    width: 1280
    height: 800
    title: "Metahub"
    color: Theme.polarNight2

    property real uiScale: 1.0
        Shortcut { sequence: "Ctrl+="; onActivated: window.uiScale = Math.min(window.uiScale + 0.1, 2.0) }
        Shortcut { sequence: "Ctrl+-"; onActivated: window.uiScale = Math.max(window.uiScale - 0.1, 0.5) }
        Shortcut { sequence: "Ctrl+0"; onActivated: window.uiScale = 1.0 }

    property bool isGithubAuthenticated: false

    Connections {
        target: AuthManager 
        
        function onTokenReceived(token) {
            console.log("Token synced with Daemon. UI updated to Connected state.")
            window.isGithubAuthenticated = true
        }

        function onTokenErrorReceived(error) {
            console.warn("Auth Error: " + error)
            window.isGithubAuthenticated = false
        }
    }

        Item {
            anchors.centerIn: parent
            width: parent.width / window.uiScale
            height: parent.height / window.uiScale
            scale: window.uiScale
            transformOrigin: Item.Center

            SplitView {
                anchors.fill: parent
                handle: Rectangle { implicitWidth: 1; color: Theme.polarNight1 }

                Rectangle {
                    SplitView.preferredWidth: 250
                    SplitView.minimumWidth: 200
                    SplitView.maximumWidth: 350
                    color: Theme.polarNight1

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 0

                        Rectangle {
                            Layout.fillWidth: true; height: 80; color: "transparent"
                            Text {
                                text: "METAHUB"
                                color: Theme.snowStorm3
                                font.pixelSize: 20; font.bold: true
                                anchors.centerIn: parent
                            }
                        }

                        ListView {
                            id: sidebarList
                            Layout.fillWidth: true; Layout.fillHeight: true
                            clip: true
                            model: ListModel {
                                ListElement { name: "Dashboard";}
                                ListElement { name: "CI/CD Module"; }
                                ListElement { name: "VPN Module"; }
                                ListElement { name: "Matrix Module"; }
                            }

                            delegate: Rectangle {
                                width: ListView.view.width
                                height: 48
                                color: ListView.isCurrentItem ? Theme.polarNight3 : "transparent"

                                Rectangle {
                                    width: 4; height: parent.height
                                    color: Theme.frost2
                                    visible: parent.ListView.isCurrentItem
                                }

                                Text {
                                    text: model.name
                                    color: parent.ListView.isCurrentItem ? Theme.snowStorm2 : Theme.snowStorm1
                                    font.pixelSize: 14
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left; anchors.leftMargin: 24
                                }
                                MouseArea { anchors.fill: parent; onClicked: sidebarList.currentIndex = index }
                            }
                        }
                    }
                }

                Rectangle {
                    SplitView.fillWidth: true
                    color: Theme.polarNight2

                    StackLayout {
                        anchors.fill: parent
                        currentIndex: sidebarList.currentIndex

                        Item { Text { text: "Dashboard Placeholder"; color: Theme.snowStorm1; anchors.centerIn: parent } }

                        Item {
                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: Theme.defaultMargin
                                spacing: 16

                                RowLayout {
                                    Layout.fillWidth: true
                                    Text {
                                        text: "Build Pipelines"
                                        color: Theme.snowStorm3
                                        font.pixelSize: 24; font.bold: true
                                        Layout.fillWidth: true
                                    }

                                    Button {
                                        id: authBtn
                                        text: window.isGithubAuthenticated ? "Connected" : "GitHub Login"
                                        flat: true

                                        contentItem: Text {
                                            text: parent.text
                                            color: window.isGithubAuthenticated ? Theme.auroraGreen : Theme.auroraYellow
                                            font.bold: true
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }

                                        background: Rectangle {
                                            color: parent.pressed ? Theme.polarNight1 : "transparent"
                                            radius: 6
                                            border.width: 1
                                            border.color: window.isGithubAuthenticated ? Theme.auroraGreen : Theme.auroraYellow
                                        }

                                        onClicked: {
                                            if (!window.isGithubAuthenticated) {
                                                console.log("Starting OAuth flow...")
                                                AuthManager.startAuth()
                                            } else {
                                                console.log("Already authenticated. (Add logout logic here if needed)")
                                            }
                                        }
                                    }

                                    Button {
                                        id: refreshBtn
                                        text: "⟳ Refresh"
                                        flat: true

                                        scale: pressed ? 0.95: (hovered ? 1.02 : 1.0)
                                        Behavior on scale { NumberAnimation { duration: 100; easing.type: Easing.OutQuad } }

                                        contentItem: Text {
                                            text: parent.text
                                            color: parent.pressed ? Theme.polarNight1 : Theme.frost2
                                            font.bold: true
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }

                                        background: Rectangle {
                                            color: parent.pressed ? Theme.frost2: (parent.hovered ? Theme.polarNight3 : "transparent")
                                            radius: 6
                                            border.width: 1
                                            border.color: parent.hovered ? Theme.frost2 : Theme.polarNight4

                                            Behavior on color { ColorAnimation { duration: 150 } }
                                            Behavior on border.color { ColorAnimation { duration: 150 } }
                                        }
                                        onClicked: CiCDViewModel.fetchCicdData()
                                    }
                                }

                                ListView {
                                    id: pipelineListView
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    clip: true
                                    spacing: 8
                                    model: CiCDViewModel

                                    add: Transition {
                                        NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 300 }
                                        NumberAnimation { property: "y"; from: 20; to: 0; duration: 400; easing.type: Easing.OutQuart }
                                    }
                                    displaced: Transition { NumberAnimation { properties: "x, y"; duration: 200 } }

                                    delegate: Rectangle {
                                        id: cardDelegate
                                        width: pipelineListView.width
                                        height: 60
                                        radius: Theme.defaultRadius

                                        color: mouseArea.containsMouse ? Theme.polarNight4 : Theme.polarNight3
                                        Behavior on color { ColorAnimation { duration: 150 } }

                                        MouseArea {
                                            id: mouseArea
                                            anchors.fill: parent
                                            hoverEnabled: true
                                            onClicked: console.log("Clicked pipeline " + model.id)
                                        }

                                        RowLayout {
                                            anchors.fill: parent
                                            anchors.leftMargin: 16
                                            anchors.rightMargin: 16
                                            spacing: 16

                                            Rectangle {
                                                width: 24; height: 24
                                                radius: 12
                                                color: model.conclusion === "success" ? Theme.auroraGreen :
                                                model.conclusion === "failure" ? Theme.auroraRed : Theme.auroraYellow

                                                scale: 1.0
                                                Component.onCompleted: { scale = 0; animScale.start() }
                                                NumberAnimation on scale { id: animScale; to: 1.0; duration: 400; easing.type: Easing.OutBack }

                                                Shape {
                                                    anchors.centerIn: parent
                                                    width: 10; height: 10
                                                    visible: model.conclusion === "success"
                                                    ShapePath {
                                                        strokeWidth: 2; strokeColor: Theme.iconOnLight
                                                        fillColor: "transparent"; capStyle: ShapePath.RoundCap
                                                        startX: 1; startY: 5; PathLine { x: 4; y: 8 } PathLine { x: 9; y: 1 }
                                                    }
                                                }

                                                Text {
                                                    anchors.centerIn: parent
                                                    text: "✕"
                                                    color: Theme.iconOnLight
                                                    font.pixelSize: 12; font.bold: true
                                                    visible: model.conclusion === "failure"
                                                }
                                            }

                                            ColumnLayout {
                                                Layout.fillWidth: true
                                                spacing: 2
                                                Text {
                                                    text: model.name ? model.name : "Unknown"
                                                    color: Theme.snowStorm2
                                                    font.pixelSize: 14; font.bold: true
                                                }
                                                Text {
                                                    text: "ID: " + model.id + " • " + model.status
                                                    color: Theme.snowStorm1
                                                    font.pixelSize: 11; opacity: 0.6
                                                }
                                            }

                                            Button {
                                                text: "Logs"
                                                flat: true
                                                Layout.preferredHeight: 28

                                                scale: pressed ? 0.95 : 1.0
                                                Behavior on scale { NumberAnimation { duration: 50 } }

                                                contentItem: Text {
                                                    text: parent.text
                                                    color: parent.hovered ? Theme.snowStorm3 : Theme.frost3
                                                    font.pixelSize: 11; font.bold: true
                                                    horizontalAlignment: Text.AlignHCenter
                                                    verticalAlignment: Text.AlignVCenter
                                                }

                                                background: Rectangle {
                                                    color: parent.pressed ? Theme.polarNight1: (parent.hovered ? Theme.polarNight4 : "transparent")
                                                    radius: 4
                                                    border.width: 1
                                                    border.color: parent.hovered ? Theme.polarNight4 : Theme.polarNight4

                                                    Behavior on color { ColorAnimation { duration: 100 } }
                                                }
                                                onClicked: console.log("Logs requested")
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        Item { Text { text: "VPN Placeholder"; color: Theme.snowStorm1; anchors.centerIn: parent } }
                        Item { Text { text: "Matrix Placeholder"; color: Theme.snowStorm1; anchors.centerIn: parent } }
                    }
                }
            }
        }
    }
