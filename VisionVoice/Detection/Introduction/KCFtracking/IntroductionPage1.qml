import QtQuick 2.9
import QtQuick.Controls 2.2
import ".."
import "../../../../Component"

Page {

    property StackView stack: null

    StackView {
        id: stack_intro
        initialItem: page_intro
        anchors.fill: parent
    }

    Page{
        id: page_intro
        width: 1920
        height: 1080


        background: Image {
            source: "../../../../images/background.png"
        }

        Button {
            height: 32
            width: 120
            text: "<-"
            anchors.left: parent.left
            anchors.top: parent.top
            onClicked: stack.pop()
        }

        Text {
            id:label
            x: 74
            y: 338
            width: 1017
            height: 328
            text: qsTr("    人工智能是计算机科学的一个分支，它企图了解智能的实质，并生产出一种新的能以人类智能相似的方式做出反应的智能机器.
        2016年3月15日，谷歌人工智能AlphaGo与围棋世界冠军李世石的人机大战最后一场落下了帷幕。人机大战第五场经过长达5个小时搏杀，最终李世石与AlphaGo总比分定格在1比4，以李世石认输结束。以此为契机，全球范围内掀起了一场人工智能（AI）的热潮。
    ")
            font.letterSpacing: 0.3
            font.wordSpacing: 0
            verticalAlignment: Text.AlignTop
            wrapMode: Text.WordWrap
            elide: Text.ElideNone
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: 40
        }

        Text {
            id:title
            x: 508
            y: 245
            text: qsTr("AI简介")
            anchors.bottom: label.top
            anchors.bottomMargin: 50
            font.pixelSize: 50
        }

        Image {
            id:img
            x: 497
            y: 162
            width: 661
            height: 680
            anchors.horizontalCenterOffset: 533
            anchors.horizontalCenter: parent.horizontalCenter
            source: "../../../images/AlphaGo.jpg"
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
                page2.visible = true;
                page2.stack = stack_intro;
                stack_intro.push(page2);
            }
        }

        IntroductionPage2{
            id: page2
            visible: false
            stack: stack_intro
        }


    }

}
