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
        id:img1
                    x: 1007
                    y: 446
                    width: 625
                    height: 502
                    source: "../../../images/od_intro.jpg"
                }

    Image {
        id:img2
                    x: 320
                    y: 446
                    width: 542
                    height: 502
                    source: "../../../images/posture_intro.jpg"
                }

                Text {
                    id:label
                    x: 603
                    y: 191
                    width: 715
                    height: 209
                    text: qsTr("在计算机视觉领域，CNN目前主要用于：
物体识别
    样例：使用CNN进行姿态识别
物体检测
    样例：使用CNN进行人体检测")
                    verticalAlignment: Text.AlignTop
                    wrapMode: Text.WordWrap
                    elide: Text.ElideNone
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: 40
                }

                Text {
                    id:title
                    x: 311
                    y: 153
                    text: qsTr("深度学习与CV")
                    anchors.horizontalCenter: label.horizontalCenter
                    anchors.bottom: label.top
                    anchors.bottomMargin: 50
                    font.pixelSize: 40
                }

}
