import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import ChatClient.Network
import ChatClient.Core
import QuickFuture
import ObjectConverter

RowLayout {
    id: root
    spacing: 0
    required property ControllerManager manager
    property bool roomSelected: false
    property alias selectedRoomID : chatBox.roomID
    Component.onCompleted:
    {
        manager.groupController.load(2);
    }

    RoomList {
        id: roomList
        // Layout.minimumWidth: 120
        Layout.fillHeight: true
        Layout.preferredWidth: Math.max(220, parent.width / 4)
        Layout.maximumWidth: 420
        roomModel: manager.groupController.model
        onSelectedRoomChanged: {
            if (!roomSelected)
                roomSelected = true
            chatBox.roomID = roomList.selectedRoom.id
            roomHeader.title = roomList.selectedRoom.name
        }
        listView.footer: StandardDelegate {
            id: createRoomFooter
            label.text: qsTr("Create")
            icon: Rectangle {
                height: 50
                width: 50
                radius: 20
                color: "#19182a"
                Label {
                    text: "+"
                    color: "white"
                    font.pixelSize: 40
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 3
                }
            }
            width: roomList.listView.width
            onClicked: createRoomDialog.open()
        }
    }
    ColumnLayout {
        id: chatColumn
        spacing: 0
        Layout.fillWidth: true
        // Layout.minimumWidth: 350
        ColoredFrame {
            id: roomHeader
            property alias title: titleLabel.text
            visible: roomSelected
            implicitHeight: 50
            Layout.fillWidth: true
            Label {
                anchors.centerIn: parent
                font.bold: true
                id: titleLabel
            }
            Row {
                anchors.right: parent.right
                anchors.rightMargin: 15
                spacing: 10
                Button {
                    anchors.verticalCenter: parent.verticalCenter
                    text: "call"
                    onClicked: manager.callController.handler(root.selectedRoomID).join()
                }
                MouseArea {
                    id: addUserBtn
                    anchors.verticalCenter: parent.verticalCenter
                    height: 30
                    width: 30
                    cursorShape: Qt.PointingHandCursor
                    onClicked: selectUserDialog.open()
                    Image {
                        id: addUserIcon
                        anchors.centerIn: parent
                        height: 25
                        width: 25
                        mipmap: true
                        source: Qt.resolvedUrl("pics/add-user.svg")
                    }
                }
            }
        }

            ChatBox {
                id: chatBox
                focus: true
                initalMessage: qsTr("Select ChatRoom to start messaging")
                manager: root.manager
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }

    CreateRoomDialog {
        id: createRoomDialog
        anchors.centerIn: parent
        onAccepted: manager.groupController.create(roomName)
    }
    SelectUserDialog {
        id: selectUserDialog
        anchors.centerIn: parent
        title: qsTr("Add memder:")
        onSeacrhPatternChanged: {
            Future.onFinished(manager.userController.findUsers(
                                  ObjectConverter.toHash({
                                                             "name": seacrhPattern
                                                         }), 5),
                              function (users) {
                                  selectUserDialog.usersModel = users
                              })
        }
        onUserSelected: id => manager.roomController.addUserToRoom(
                            id, roomList.selectedRoom.id)
    }
}
