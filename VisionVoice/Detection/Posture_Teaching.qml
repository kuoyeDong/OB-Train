import QtQuick 2.9
import QtQuick.Controls 2.2
import "../../Component"

import RegisterMyType 1.0
import RegisterSystemOpen 1.0

Page {
    id: page

    property StackView stack: null

    background: Image {
        source: "../../images/background.png"
    }

    Label{
        id: label
        x: 64
        width: 360
        text: qsTr("目前支持的手势姿态")
        font.bold: true
        anchors.horizontalCenterOffset: 1
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 35
        anchors.horizontalCenter: parent.horizontalCenter
        height: 43
        font.pixelSize: 40
    }

    Button {
        id:back
        height: 32
        width: 120
        text: "<-"
        anchors.left: parent.left
        anchors.top: parent.top
        onClicked: {
                   stack.pop();
               }
    }

//    Button {
//        id: person_detection
//        x: 568
//        y: 647
//        width: 106
//        height: 40
//        text: qsTr("返回")
//        anchors.bottom: parent.bottom
//        anchors.bottomMargin: 10
//        anchors.horizontalCenter: parent.horizontalCenter

//        onClicked:
//        {
//            stack.pop()
//        }
//    }

    Image {
        id: image
        x: 233
        y: 101
        width: 1352
        height: 839
        anchors.horizontalCenter: parent.horizontalCenter
        fillMode: Image.PreserveAspectFit
        source: "../images/posture2.png"
    }

    TextEdit {
        id: textEdit
        x: 422
        width: 812
        height: 30
        text: qsTr("在画面中做出以上五种姿态，相应的姿态名称会显示在画面中")
        font.bold: true
        anchors.horizontalCenterOffset: 1
        anchors.top: image.bottom
        anchors.topMargin: 26
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 30
    }
}




























































/*##^## Designer {
    D{i:0;autoSize:true;height:1080;width:1920}D{i:3;anchors_x:123;anchors_y:239}D{i:4;anchors_x:252;anchors_y:86}
D{i:5;anchors_x:123;anchors_y:563}
}
 ##^##*/
