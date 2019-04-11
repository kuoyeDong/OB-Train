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

    Text {
        id:label
        x: 73
        y: 130
        width: 933
        height: 454
        text: qsTr("常见的传统机器学习算法：
    线性回归（LR）
    支持向量机（SVM）
优缺点:
    LR：
    优点：模型简单（如：y = ax + b），计算简单
    缺点：需要大量数据才会有较好效果
    SVM：
    优点：适合数据量较少的场景
    缺点：模型复杂（多元、无固定解），计算复杂
样例：视觉跟踪算法")
        verticalAlignment: Text.AlignTop
        wrapMode: Text.WrapAnywhere
        elide: Text.ElideNone
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: 40
    }

    Text {
        id:title
        x: 73
        y: 61
        text: qsTr("传统机器学习与CV")
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
            page6.visible = true;
            page6.stack = stack_intro;
            stack_intro.push(page6);
        }
    }

    IntroductionPage6{
        id:page6
        visible: false
        stack: stack_intro
    }

}
