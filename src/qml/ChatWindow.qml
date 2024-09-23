import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import ChatClient
import QWindowKit 1.0

ApplicationWindow {
    id: root
    width: 650
    height: 600
    visible: false
    signal languageSet(string language)
    signal logout
    color: "transparent"
    required property AbstractChatController controller
        WindowAgent {
        id: windowAgent
    }
    background: Item{}
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        TitleBar {
            id: titleBar
            Layout.fillWidth: true
            agent: windowAgent
            window:root
            Layout.preferredHeight: 32
        }
        RowLayout {
            id: mainLayout
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.minimumWidth: 650
            spacing: 0
            MenuPanel {
                Layout.fillHeight: true
                width: 75
                onLogoutClicked: root.logout()
                onProfileClicked: profileViewer.open()
                onRoomsClicked: stack.currentIndex = 0
                onDirectClicked: stack.currentIndex = 1
                onSettingsClicked: stack.currentIndex = 2
            }
            StackLayout {
                id: stack
                RoomsPage {
                    id: roomsPage
                    controller: root.controller
                }
                DirectPage {
                    id: directPage
                    controller: root.controller
                }

                SettingsPage {
                    id: settingsPage
                }
            }
        }
    }
    function toggleMaximized() {
        if (root.visibility === Window.Maximized) {
            root.showNormal()
        } else {
            root.showMaximized()
        }
    }

    ProfileViewer {
        id: profileViewer
        user: controller.currentUser
        anchors.centerIn: parent
    }

    Component.onCompleted: {
        windowAgent.setup(root)
        windowAgent.setTitleBar(titleBar)
    }
}