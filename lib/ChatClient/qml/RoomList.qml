import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Frame {
    id: root
    property int selectedRoomID: -1
    property int selectedRoomIndex: -1
    property alias roomIndex: listView.currentIndex
    property alias roomModel: listView.model
    //background :Rectangle{ color: "#6F7D73"}
    ListView {
        id: listView
        currentIndex: -1
        height: parent.height
        width: parent.width
        delegate: roomListDelegate
    }
    Component {
        id: roomListDelegate

        ItemDelegate  {
            width: listView.width
            height: 50
            Label
            {
                text: name
            }

            MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onEntered: listView.currentIndex = index
                onClicked:
                {
                    selectedRoomID = id;
                    selectedRoomIndex   = listView.currentIndex;
                }
               // onExited: listView.currentIndex = -1
            }
        }
    }
}
