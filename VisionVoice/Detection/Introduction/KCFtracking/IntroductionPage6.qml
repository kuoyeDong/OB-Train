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

                    x: 497
                    y: 421
                    width: 1092
                    height: 540
                    anchors.horizontalCenterOffset: 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    source:"../../../images/framework.png"
                }

                Text {
                    id:label
                    x: 117
                    y: 181
                    width: 1686
                    height: 170
                    text: qsTr("    深度学习的概念源于人工神经网络的研究。深度学习通过组合低层特征形成更加抽象的高层表示属性类别或特征，以发现数据的分布式特征表示。
    CNN（卷积神经网络）：包含卷积、池化等计算且具有深度结构的前馈神经网络（Feedforward Neural Networks），是深度学习（deep learning）的代表算法之一。
    ")
                    verticalAlignment: Text.AlignTop
                    wrapMode: Text.WordWrap
                    elide: Text.ElideNone
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: 40
                }

                Text {
                    id:title
                    x: 796
                    y: 67
                    text: qsTr("深度学习与CV")
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
                        page7.visible = true;
                        page7.stack = stack_intro;
                        stack_intro.push(page7);
                    }
                }

                IntroductionPage7{
                    id:page7
                    visible: false
                    stack: stack_intro
                }

}
