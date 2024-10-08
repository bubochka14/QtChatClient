import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Window {
    id: root
    signal registerUser(string username, string passw)
    signal loginUser(string username, string passw)
    property alias errorString: authPage.errorString
    property alias loadingProgress : loadingPage.progress
    property alias loadingStatus: loadingPage.status
    property alias state:rootItem.state
    width: 325
    height: 425
    flags: root.flags| Qt.FramelessWindowHint
    Item {
        id: rootItem
        anchors.fill: parent
        states: [
            State {
                name: "inital"
                PropertyChanges {
                    target: stackLayout
                    currentIndex: 0
                }
            },
            State {
                name: "loading"
                PropertyChanges {
                    target: stackLayout
                    currentIndex: 1
                }
            }
        ]
        state: "inital"
        StackLayout {
            id: stackLayout
            currentIndex: 1
            anchors.fill: parent
            AuthorizationPage    {
                id: authPage
                focus: true
                Layout.fillWidth: true
                Layout.fillHeight: true
                onRegisterUser: (user, pss)=>{root.registerUser(user,pss)}
                onLoginUser: (user, pss)=>{root.loginUser(user,pss)}
            }
            LoadingPage {
                id: loadingPage
                focus: true
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
