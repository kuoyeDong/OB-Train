import QtQuick 2.9
import QtQuick.Controls 2.2
import "."

Page {
    id: page
    width: 1920
    height: 1080

    property StackView stack: null

    background: Image {
        source: "../../images/background.png"
    }

    Button {
        height: 32
        width: 120
        text: "<-"
        anchors.left: parent.left
        anchors.top: parent.top
        onClicked: stack.pop()
    }

    Label {
        id: title
        x: 719
        width: 756
        height: 56
        text: qsTr("人形跟踪与手势姿态识别相关实验")
        anchors.horizontalCenterOffset: 0
        anchors.top: parent.top
        anchors.topMargin: 82
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 50
    }

    Button {
        id: ex1
        x: 440
        width: 1016
        height: 80
        text: qsTr("实验一  使用图传模组采集测试用视频数据")
        highlighted: true
        autoRepeat: false
        flat: false
        anchors.top: title.bottom
        anchors.topMargin: 65
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 35

        onClicked: {
            ex_page_1.visible = true;
            ex_page_1.stack = stack;
            stack.push(ex_page_1)
        }
    }

    Button {
        id: ex2
        x: 440
        width: 1016
        height: 80
        text: qsTr("实验二  人形跟踪使用的图像特征与模型")
        highlighted: true
        anchors.top: ex1.bottom
        anchors.topMargin: 65
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 35

        onClicked: {
            ex_page_2.visible = true;
            ex_page_2.stack = stack;
            stack.push(ex_page_2)
        }
    }

    Button {
        id: ex3
        x: 440
        width: 1016
        height: 80
        text: qsTr("实验三  人形跟踪算法模型的训练")
        flat: false
        highlighted: true
        anchors.top: ex2.bottom
        anchors.topMargin: 65
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 35

        onClicked: {
            ex_page_3.visible = true;
            ex_page_3.stack = stack;
            stack.push(ex_page_3)
        }
    }

    Button {
        id: ex4
        x: 382
        width: 1016
        height: 80
        text: qsTr("实验四  人形跟踪中的模型在人形识别中的测试")
        highlighted: true
        anchors.top: ex3.bottom
        anchors.topMargin: 65
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 35

        onClicked: {
            ex_page_4.visible = true;
            ex_page_4.stack = stack;
            stack.push(ex_page_4)
        }
    }

    Button {
        id: ex5
        x: 382
        width: 1016
        height: 80
        text: qsTr("实验五  不同环境噪声中的人形跟踪算法的应用")
        highlighted: true
        anchors.top: ex4.bottom
        anchors.topMargin: 65
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 35

        onClicked: {
            ex_page_5.visible = true;
            ex_page_5.stack = stack;
            stack.push(ex_page_5)
        }
    }

    Experiment_One{
        id:ex_page_1
        visible: false
    }

    Experiment_Two{
        id:ex_page_2
        visible: false
    }

    Experiment_Three{
        id:ex_page_3
        visible: false
    }

    Experiment_Four{
        id:ex_page_4
        visible: false
    }

    Experiment_Five{
        id:ex_page_5
        visible: false
    }

}
