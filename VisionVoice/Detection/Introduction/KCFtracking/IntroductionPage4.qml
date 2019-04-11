import QtQuick 2.9
import QtQuick.Controls 2.2
import ".."
import "../../../../Component"

Page {
    width: 1920
    height: 1080

    id: intro_page1

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
                    x: 686
                    y: 252
                    width: 1137
                    height: 577
                    source: "../../../images/feature_hand.jpg"
                }

    Text {
        id:label
                    x: 73
                    y: 252
                    width: 595
                    height: 206
                    text: qsTr("特征的构造：
基本上为人工构造特征
       人工特征的优缺点：
       针对某些场景高度优化
       泛化能力（普适性）较差")
                    verticalAlignment: Text.AlignTop
                    wrapMode: Text.WrapAnywhere
                    elide: Text.ElideNone
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: 40
                }

    Text {
        id:title
                    x: 211
                    y: 187
                    text: qsTr("传统机器学习与CV")
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
            page5.visible = true;
            page5.stack = stack_intro;
            stack_intro.push(page5);
        }
    }

    IntroductionPage5{
        id:page5
        visible: false
        stack: stack_intro
    }
}
