import QtQuick 2.9
import QtQuick.Controls 2.2
import ".."
import "../../../../Component"

Page {
    width: 1920
    height: 1080

    id: intro_page3

    property StackView stack: null

    background: Image {
        source: "../../../../images/background.png"
    }

    Button {
        height: 32
        width: 120
        text: "<-"
        anchors.left: parent.left
        anchors.top: parent.top
        onClicked: stack_intro.pop(stack_intro.initialItem)
    }

    Image {
        x: 888
        y: 243
        width: 935
        height: 594
        source: "../../../images/ML.jpg"
    }

    Text {
        id:label
        x: 82
        y: 243
        width: 659
        height: 291
        text: qsTr("传统机器学习
       拥有严谨的数学论证
       计算开销较小
深度学习
       缺乏直接的数学证明
       计算开销大
       效果普遍好于传统机器学习")
        font.pixelSize: 40
    }

     Text {
         id:title
         x: 332
         y: 150
         width: 160
         height: 42
         text: qsTr("机器学习")
         anchors.horizontalCenter: label.horizontalCenter
         anchors.bottom: label.top
         anchors.bottomMargin: 50
         font.pixelSize: 40
     }

     Button {
         id: button
         x: 1642
         y: 887
         width: 181
         height: 52
         text: qsTr("继续>>>>")
         font.family: "华文楷体"
         display: AbstractButton.TextBesideIcon
         anchors.right: parent.horizontalCenter
         anchors.rightMargin: -863
         anchors.bottom: parent.verticalCenter
         anchors.bottomMargin: -399
         spacing: 7
         font.pixelSize: 35
         focusPolicy: Qt.NoFocus
         onClicked: {
             page4.visible = true;
             page4.stack = stack_intro;
             stack_intro.push(page4);
         }
     }

     IntroductionPage4{
         id:page4
         visible: false
         stack: stack_intro
     }

}
