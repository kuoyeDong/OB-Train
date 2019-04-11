import QtQuick 2.9
import QtQuick.Controls 2.2
import ".."
import "./PersionExperiment"
import "./FaceExperiment"

Page {
    id: page

    property StackView stack: null

    background: Image {
        source: "../images/background.png"
    }

    Button {
        height: 32
        width: 120
        text: "<-"
        anchors.left: parent.left
        anchors.top: parent.top
        onClicked: stack.pop()
    }

    Button {
        id:button1
        x: 472
        y: 415
        width: 250
        height: 250
        text: qsTr("人脸识别相关实验")
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: title.left
        anchors.rightMargin: 0

        background: Image {
            source: "images/FaceUnlock.png"
        }

        onClicked: {
            page1.visible = true;
            page1.stack = stack;
            stack.push(page1)
        }
    }

    Button {
        id: button2
        y: 415
        width: 250
        height: 250
        text: qsTr("人形检测与跟踪相关实验")
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: title.right
        anchors.leftMargin: 0

        background: Image {
            source: "images/PersonControl.png"

        }

        onClicked: {
            page2.visible = true;
            page2.stack = stack;
            stack.push(page2)
        }
    }

    Label {
        id: title
        x: 719
        width: 482
        height: 66
        text: qsTr("机器视觉相关实验")
        anchors.top: parent.top
        anchors.topMargin: 160
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 60
    }

    Face_Experiment_List{
        id:page1
        visible: false
    }


    Person_Experiment_List{
        id:page2
        visible: false
    }

}












/*##^## Designer {
    D{i:5;anchors_x:1182}D{i:7;anchors_y:158}
}
 ##^##*/
