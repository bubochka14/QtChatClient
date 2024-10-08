import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

ColoredFrame {
    id: root
    property int selectedRoomID: -1
    property int selectedRoomIndex: -1
    property alias roomIndex: listView.currentIndex
    property alias roomModel: listView.model
    property alias listView: listView
    ListView {
        id: listView
        currentIndex: -1
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        height: parent.height
        width: parent.width
        delegate: StandardDelegate {
            id: delegate
            required property int index
            required property var name
            required property int id
            width: listView.width
            icon:Rectangle {
                height: 50
                width: 50
                radius: 20
                color: "#19182a"
                Image {
                    id: icon
                    anchors.centerIn: parent
                    source: "pics/users.svg"
                    sourceSize.height: 30
                    sourceSize.width: 30
                }
            }
            label.text: delegate.name
            label.font.bold: true
            onClicked:{
                console.log(selectedRoomID, delegate.id)
                selectedRoomID = delegate.id
                selectedRoomIndex = index
                listView.currentIndex = index
            }
        }
    }
}
