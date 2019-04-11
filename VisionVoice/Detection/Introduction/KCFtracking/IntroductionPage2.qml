import QtQuick 2.9
import QtQuick.Controls 2.2
import ".."
import "../../../../Component"

Page {
    width: 1920
    height: 1080

    id: intro_page2

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

    BorderImage {
        x: 1346
        y: 337
        width: 471
        height: 407
        source: "../../../images/AI_ML.jpg"
    }
    BorderImage {
        x: 174
        y: 498
        width: 1073
        height: 388
        source: "../../../images/ML_Human.jpg"
    }

    Text {
        x: 81
        y: 176
        width: 1259
        height: 288
        text: qsTr("    机器学习(Machine Learning, ML)专门研究计算机怎样模拟或实现人类的学习行为。它是人工智能的核心，是使计算机具有智能的根本途径。
    本质：统计学
    方法：对现有的数据（特征）进行分析（训练），进而获得规律（模型），然后进行判断和预测。当前机器学习的主要应用场景：计算机视觉。")
        verticalAlignment: Text.AlignTop
        wrapMode: Text.WrapAnywhere
        elide: Text.ElideNone
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: 40
    }

    Text {
        id: element1
        x: 675
        y: 101
        text: qsTr("机器学习")
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
            page3.visible = true;
            page3.stack = stack_intro;
            stack_intro.push(page3);
        }
    }

    IntroductionPage3{
        id: page3
        visible: false
        stack: stack_intro
    }

}
