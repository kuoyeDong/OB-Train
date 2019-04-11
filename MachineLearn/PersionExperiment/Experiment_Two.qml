import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import ".."

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
        onClicked: {
            ExpImage.setEx_id(0, -1);
            ExpImage.setAppRunStatusClose();
            img.source = "../images/src.png"
            feature_show.source = "../images/pro.png"
            stack.pop();
        }
    }

    Label {
        id: title
        x: 719
        width: 904
        height: 56
        text: qsTr("实验二  人形跟踪使用的图像特征与模型")
        anchors.horizontalCenterOffset: 0
        anchors.top: parent.top
        anchors.topMargin: 82
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 50
    }

    Image{
        id:img
        x: 34
        y: 336
        width:1158
        height:650
        visible: true
        source:"../images/src.png";
        cache:false;
    }

    Label {
        id: pathtitle
        x: 34
        y: 145
        width: 366
        height: 44
        text: qsTr("请选择待测试图片：")
        horizontalAlignment: Text.AlignHCenter
        anchors.left: cbtitle2.right
        anchors.leftMargin: -190
        anchors.topMargin: 60
        anchors.top: cbtitle2.bottom
        font.pixelSize: 40
    }

    Button {
        id: button
        x: 386
        y: 432
        width: 52
        height: 44
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: pathtitle.verticalCenter
        anchors.bottom: pathInput.bottom
        anchors.bottomMargin: 0
        anchors.right: camera_close.right
        anchors.rightMargin: 0

        background: Image {
            id:saveicon
            source: "../images/folder.png"
        }

        onClicked: {
            openImgSaved.open()
        }
    }

    Button {
        id: light
        y: 215
        width: 84
        height: 84
        text: qsTr("")
        anchors.verticalCenter: ld_rectangle.verticalCenter
        anchors.left: ld_rectangle.left
        anchors.leftMargin: 26

        background: Image {
            id:lighticon
            source: "../images/light.png"
        }

        onClicked: {
            ExpImage.setEx_id(2, 1);
            ExpImage.exp2Param(1);
            ExpImage.expStart();
            while(!ExpImage.appRunStatus()){}
            img.source = "";
            img.source = ExpImage.get_tmp_path(0);
            ExpImage.setAppRunStatusClose();
        }
    }

    Button {
        id: dark
        width: 84
        height: 84
        text: qsTr("")
        anchors.left: light.right
        anchors.leftMargin: 43
        anchors.top: light.top
        anchors.topMargin: 0

        background: Image {
            id:darkicon
            source: "../images/dark.png"
        }

        onClicked: {
            ExpImage.setEx_id(2, 1);
            ExpImage.exp2Param(0);
            ExpImage.expStart();
            while(!ExpImage.appRunStatus()){}
            img.source = "";
            img.source = ExpImage.get_tmp_path(0);
            ExpImage.setAppRunStatusClose();
        }
    }

    Button {
        id: flip
        y: 215
        width: 84
        height: 84
        text: qsTr("")
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: flip_rectangle.verticalCenter
        anchors.left: flip_rectangle.left
        anchors.leftMargin: 20

        background: Image {
            id:flipicon
            source: "../images/flip.png"
        }

        onClicked: {
            ExpImage.setEx_id(2, 2);
            ExpImage.expStart();
            while(!ExpImage.appRunStatus()){}
            img.source = "";
            img.source = ExpImage.get_tmp_path(0);
            ExpImage.setAppRunStatusClose();
        }
    }

    Button {
        id: rect
        y: 215
        width: 84
        height: 84
        text: qsTr("")
        anchors.verticalCenter: rect_rectangle.verticalCenter
        anchors.left: rect_rectangle.left
        anchors.leftMargin: 15

        background: Image {
            id:recticon
            source: "../images/rect.png"
        }

        onClicked: {
            ExpImage.exp2Param(0);
            ExpImage.setEx_id(2, 3);
            ExpImage.expStart();
            while(!ExpImage.appRunStatus()){}
            img.source = "";
            img.source = ExpImage.get_tmp_path(0);
            ExpImage.setAppRunStatusClose();
        }
    }


    FileDialog{
        id:openImgSaved
        title:qsTr("打开图片")
        nameFilters: ["Image Files (*.jpg *.png *.gif *.bmp *.ico)", "*.*"]
        folder: shortcuts.desktop
        selectExisting:true
        onAccepted: {
            ExpImage.setPath(fileUrl, 2);
            ExpImage.setEx_id(2, 0);
            ExpImage.expStart();
            img.source = fileUrl
            while(!ExpImage.appRunStatus()){}
            ExpImage.setAppRunStatusClose();
        }
    }

    FileDialog{
        id:openResSaved
        title:qsTr("选择保存路径")
        folder: shortcuts.desktop
        selectExisting:true
        selectFolder:true
        selectMultiple:true
        onAccepted: {
            ExpImage.setPath(fileUrl, 1);
            textField.text = fileUrl;
        }
    }

    Image {
        id: feature_show
        x: 1227
        y: 336
        width: 650
        height: 650
        visible: true
        cache: false
        source: "../images/pro.png"
    }

    Label {
        id: recttitle
        x: 648
        width: 126
        height: 84
        text: qsTr("设置
矩形框")
        horizontalAlignment: Text.AlignHCenter
        anchors.top: rect.top
        anchors.topMargin: 0
        anchors.leftMargin: 10
        anchors.left: rect_rectangle.right
        font.pointSize: 30
    }

    Label {
        id: lighttitle
        width: 166
        height: 84
        text: qsTr("调整
图像亮度")
        horizontalAlignment: Text.AlignHCenter
        anchors.left: dark.right
        anchors.leftMargin: 30
        anchors.top: dark.top
        anchors.topMargin: 0
        font.pointSize: 30
    }

    Rectangle {
        id: ld_rectangle
        y: 202
        width: 260
        height: 110
        color: "#00000000"
        anchors.left: pathtitle.left
        anchors.leftMargin: 0
        border.width: 5
    }

    Rectangle {
        id: rect_rectangle
        x: 524
        y: 202
        width: 263
        height: 110
        color: "#00000000"
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: ld_rectangle.verticalCenter
        anchors.leftMargin: 198
        border.width: 5
        anchors.left: ld_rectangle.right

        Button {
            id: up
            x: 159
            y: 8
            width: 40
            height: 40
            text: qsTr("Button")

            background: Image {
                id:upicon
                source: "../images/up.png"
            }

            onClicked: {
                ExpImage.exp2Param(1);
                ExpImage.setEx_id(2, 3);
                ExpImage.expStart();
                while(!ExpImage.appRunStatus()){}
                img.source = "";
                img.source = ExpImage.get_tmp_path(0);
                ExpImage.setAppRunStatusClose();
            }

        }

        Button {
            id: down
            x: 159
            y: 62
            width: 40
            height: 40
            text: qsTr("Button")

            background: Image {
                id:downicon
                source: "../images/down.png"
            }

            onClicked: {
                ExpImage.exp2Param(2);
                ExpImage.setEx_id(2, 3);
                ExpImage.expStart();
                while(!ExpImage.appRunStatus()){}
                img.source = "";
                img.source = ExpImage.get_tmp_path(0);
                ExpImage.setAppRunStatusClose();
            }
        }

        Button {
            id: right_b
            x: 208
            y: 35
            width: 40
            height: 40
            text: qsTr("Button")

            background: Image {
                id:righticon
                source: "../images/right.png"
            }

            onClicked: {
                ExpImage.exp2Param(3);
                ExpImage.setEx_id(2, 3);
                ExpImage.expStart();
                while(!ExpImage.appRunStatus()){}
                img.source = "";
                img.source = ExpImage.get_tmp_path(0);
                ExpImage.setAppRunStatusClose();
            }
        }

        Button {
            id: left_b
            x: 110
            y: 35
            width: 40
            height: 40
            text: qsTr("Button")

            background: Image {
                id:lefticon
                source: "../images/left.png"
            }

            onClicked: {
                ExpImage.exp2Param(4);
                ExpImage.setEx_id(2, 3);
                ExpImage.expStart();
                while(!ExpImage.appRunStatus()){}
                img.source = "";
                img.source = ExpImage.get_tmp_path(0);
                ExpImage.setAppRunStatusClose();
            }
        }
    }

    Button {
        id: rgbhist
        x: 1227
        y: 213
        width: 264
        height: 110
        text: qsTr("统计RGB三通道
像素值分布")
        highlighted: true
        font.pointSize: 25
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: ld_rectangle.verticalCenter
        anchors.leftMargin: 30
        anchors.left: hogfeature.right

        onClicked: {
            ExpImage.setEx_id(2, 4);
            ExpImage.expStart();
            while(!ExpImage.appRunStatus()){}
            feature_show.source = "";
            feature_show.source = ExpImage.get_tmp_path(1);
            ExpImage.setAppRunStatusClose();
        }
    }

    Button {
        id: hogfeature
        x: 1227
        y: 215
        width: 157
        height: 110
        text: qsTr("计算
HOG特征")
        autoRepeat: false
        flat: false
        highlighted: true
        font.pointSize: 25
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: ld_rectangle.verticalCenter
        anchors.leftMargin: 1193
        anchors.left: ld_rectangle.left

        onClicked: {
            ExpImage.setEx_id(2, 5);
            ExpImage.expStart();
            while(!ExpImage.appRunStatus()){}
            feature_show.source = "";
            feature_show.source = ExpImage.get_tmp_path(1);
            ExpImage.setAppRunStatusClose();
        }
    }

    Button {
        id: geimodel
        y: 215
        width: 169
        height: 110
        text: qsTr("构建
线性模型")
        anchors.left: rgbhist.right
        anchors.leftMargin: 30
        highlighted: true
        font.pointSize: 25
        anchors.verticalCenter: ld_rectangle.verticalCenter
        anchors.verticalCenterOffset: 0

        onClicked: {
            ExpImage.setEx_id(2, 6);
            ExpImage.expStart();
            while(!ExpImage.appRunStatus()){}
            feature_show.source = "";
            feature_show.source = ExpImage.get_tmp_path(1);
            ExpImage.setAppRunStatusClose();
        }
    }

    Label {
        id: pathtitle1
        x: 492
        y: 145
        width: 816
        height: 44
        text: qsTr("数据存储路径：（为空则不会保存实验数据）")
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: pathtitle.verticalCenter
        anchors.top: cbtitle2.bottom
        anchors.topMargin: 60
        font.pixelSize: 40
        anchors.leftMargin: -190
        anchors.left: cbtitle2.right
        horizontalAlignment: Text.AlignHCenter
    }

    Button {
        id: datasave
        y: 145
        width: 52
        height: 44
        anchors.left: textField.right
        anchors.leftMargin: 13
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: pathtitle1.verticalCenter
        anchors.bottomMargin: 0
        anchors.bottom: pathInput.bottom
        anchors.right: camera_close.right
        background: Image {
            id: saveicon1
            source: "../images/folder.png"
        }
        anchors.rightMargin: 0

        onClicked: {
            openResSaved.open()
        }
    }

    Rectangle {
        id: flip_rectangle
        x: 845
        y: 202
        width: 124
        height: 110
        color: "#00000000"
        anchors.verticalCenterOffset: 0
        anchors.left: rect_rectangle.right
        anchors.verticalCenter: ld_rectangle.verticalCenter
        anchors.leftMargin: 204
        border.width: 5
    }

    Label {
        id: fliptitle
        y: 237
        width: 89
        height: 85
        text: qsTr("旋转
图像")
        anchors.verticalCenterOffset: 0
        anchors.left: flip_rectangle.right
        anchors.leftMargin: 10
        anchors.verticalCenter: flip_rectangle.verticalCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 30
    }

    TextField {
        id: textField
        y: 145
        width: 519
        height: 44
        text: qsTr("未选择数据存储路径")
        anchors.left: pathtitle1.right
        anchors.leftMargin: -15
        anchors.verticalCenter: pathtitle1.verticalCenter
        font.bold: true
        font.pointSize: 23
    }


}











































































































































/*##^## Designer {
    D{i:35;anchors_x:1825}D{i:39;anchors_x:1293}
}
 ##^##*/
