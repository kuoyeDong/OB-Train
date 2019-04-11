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
        width: 1058
        height: 56
        text: qsTr("实验四  人形跟踪中的模型在人形识别中的测试")
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
        width: 312
        height: 44
        text: qsTr("选择测试的图片:")
        horizontalAlignment: Text.AlignHCenter
        anchors.left: cbtitle2.right
        anchors.leftMargin: -190
        anchors.topMargin: 60
        anchors.top: cbtitle2.bottom
        font.pixelSize: 40
    }

    Button {
        id: button
        x: 346
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
            ExpImage.setEx_id(4, 2);
            ExpImage.exp4Param(1, 1, 1);
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
            ExpImage.setEx_id(4, 2);
            ExpImage.exp4Param(0, 1, 1);
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
            ExpImage.setEx_id(4, 3);
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
            ExpImage.exp4Param(0, 1, 1);
            ExpImage.setEx_id(4, 4);
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
            ExpImage.setEx_id(4, 0);
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
        height: 69
        text: qsTr("设置
矩形框")
        horizontalAlignment: Text.AlignHCenter
        anchors.top: rect.top
        anchors.topMargin: 8
        anchors.leftMargin: 14
        anchors.left: rect_rectangle.right
        font.pointSize: 24
    }

    Label {
        id: lighttitle
        width: 166
        height: 71
        text: qsTr("调整
图像亮度")
        horizontalAlignment: Text.AlignHCenter
        anchors.left: dark.right
        anchors.leftMargin: 29
        anchors.top: dark.top
        anchors.topMargin: 6
        font.pointSize: 24
    }

    Rectangle {
        id: ld_rectangle
        y: 202
        width: 260
        height: 110
        color: "#00000000"
        anchors.left: pathtitle.left
        anchors.leftMargin: 0
        border.width: 3
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
        border.width: 3
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
                ExpImage.exp4Param(1, 1, 1);
                ExpImage.setEx_id(4, 4);
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
                ExpImage.exp4Param(2, 1, 1);
                ExpImage.setEx_id(4, 4);
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
                ExpImage.exp4Param(3, 1, 1);
                ExpImage.setEx_id(4, 4);
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
                ExpImage.exp4Param(4, 1, 1);
                ExpImage.setEx_id(4, 4);
                ExpImage.expStart();
                while(!ExpImage.appRunStatus()){}
                img.source = "";
                img.source = ExpImage.get_tmp_path(0);
                ExpImage.setAppRunStatusClose();
            }
        }
    }

    Button {
        id: getmodel
        y: 215
        width: 103
        height: 110
        text: qsTr("测试
模型")
        anchors.left: paramsSetting.right
        anchors.leftMargin: 43
        highlighted: true
        font.pointSize: 20
        anchors.verticalCenter: ld_rectangle.verticalCenter
        anchors.verticalCenterOffset: 0

        onClicked: {
            status.text = qsTr("开始测试")
            ExpImage.setEx_id(4, 6);
            ExpImage.expStart();
            while(!ExpImage.appRunStatus()){}
            feature_show.source = "";
            feature_show.source = ExpImage.get_tmp_path(1);
            ExpImage.setAppRunStatusClose();
        }
    }

    Label {
        id: pathtitle1
        x: 898
        y: 145
        width: 279
        height: 44
        text: qsTr("数据存储路径:")
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
        anchors.leftMargin: 163
        border.width: 3
    }

    Label {
        id: fliptitle
        y: 237
        width: 89
        height: 71
        text: qsTr("旋转
图像")
        anchors.verticalCenterOffset: 0
        anchors.left: flip_rectangle.right
        anchors.leftMargin: 10
        anchors.verticalCenter: flip_rectangle.verticalCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 24
    }

    TextField {
        id: textField
        y: 145
        width: 231
        height: 44
        text: qsTr("未选择数据存储路径")
        anchors.verticalCenterOffset: 0
        anchors.left: pathtitle1.right
        anchors.leftMargin: 16
        anchors.verticalCenter: pathtitle1.verticalCenter
        font.bold: true
        font.pointSize: 18
    }

    CheckBox {
        id: hog
        width: 105
        height: 54
        text: qsTr("HOG")
        anchors.top: param_rectangle.top
        anchors.topMargin: 0
        anchors.left: param_rectangle.left
        anchors.leftMargin: 0
        font.bold: true
        font.pointSize: 20
        onCheckStateChanged: {
            if(hog.checkState == Qt.Checked)
            {
                gray.checkState = Qt.Unchecked;
            }
        }
    }

    CheckBox {
        id: rgb
        y: 202
        width: 105
        height: 54
        text: qsTr("RGB")
        anchors.left: hog.right
        anchors.leftMargin: 20
        anchors.verticalCenter: hog.verticalCenter
        font.bold: true
        font.pointSize: 20
        onCheckStateChanged: {
            if(rgb.checkState == Qt.Checked)
            {
                gray.checkState = Qt.Unchecked;
            }
        }
    }

    CheckBox {
        id: gray
        y: 202
        width: 117
        height: 54
        text: qsTr("GRAY")
        anchors.left: rgb.right
        anchors.leftMargin: 20
        anchors.verticalCenter: rgb.verticalCenter
        font.bold: true
        font.pointSize: 20
        onCheckStateChanged: {
            if(gray.checkState == Qt.Checked)
            {
                hog.checkState = Qt.Unchecked;
                rgb.checkState = Qt.Unchecked;
            }
        }
    }

    Button {
        id: paramsSetting
        y: 202
        width: 103
        height: 110
        text: qsTr("重置
参数")
        anchors.leftMargin: 0
        anchors.left: feature_show.left
        font.pointSize: 20
        anchors.verticalCenterOffset: 0
        highlighted: true
        anchors.verticalCenter: getmodel.verticalCenter

        onClicked: {
            ExpImage.setEx_id(4, 5);
            ExpImage.expStart();
            while(!ExpImage.appRunStatus()){}
            ExpImage.setAppRunStatusClose();
//            if(gray.checkState == Qt.Checked)
//            {
//                ExpImage.exp4Param(0, 0, 0);
//            }
//            else
//            {
//                ExpImage.exp4Param(0, 1, 1);
//            }
            status.text = qsTr("参数已重置
请重新载入模型")
        }
    }

    Rectangle {
        id: param_rectangle
        x: 1168
        y: 258
        width: 374
        height: 54
        color: "#00000000"
        anchors.bottom: getmodel.bottom
        anchors.bottomMargin: 0
        border.width: 3
        anchors.leftMargin: 27
        anchors.left: getmodel.right
    }

    Label {
        id: pathmodel
        x: 427
        y: 145
        width: 393
        height: 44
        text: qsTr("选择保存的模型路径:")
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: pathtitle.verticalCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.top: cbtitle2.bottom
        font.pixelSize: 40
        anchors.leftMargin: -190
        anchors.left: cbtitle2.right
        anchors.topMargin: 60
    }

    Button {
        id: openmodel
        y: 145
        width: 52
        height: 44
        anchors.left: pathmodel.right
        anchors.leftMargin: 5
        anchors.bottom: pathInput.bottom
        anchors.rightMargin: 0
        background: Image {
            id: saveicon2
            source: "../images/humanModel.png"
        }
        anchors.bottomMargin: 0
        anchors.verticalCenter: pathtitle.verticalCenter
        anchors.right: camera_close.right
        anchors.verticalCenterOffset: 0

        onClicked: {
            openModelSaved.open()
        }
    }

    FileDialog{
        id:openModelSaved
        title:qsTr("读取模型")
        folder: shortcuts.desktop
//        nameFilters: ["Image Files (*.jpg *.png *.gif *.bmp *.ico)", "*.*"]
        selectExisting:true
        selectFolder:true
        selectMultiple:true
        onAccepted: {
            if(gray.checkState != Qt.Checked && hog.checkState != Qt.Checked && rgb.checkState != Qt.Checked)
            {
                status.text = qsTr("请先设置参数
再读取模型文件");
            }
            else
            {
                if(gray.checkState == Qt.Checked)
                {
                    ExpImage.exp4Param(0, 0, 0);
                }
                else
                {
                    ExpImage.exp4Param(0, 1, 1);
                }
                ExpImage.setPath(fileUrl, 1);
                ExpImage.setEx_id(4, 1);
                ExpImage.expStart();
                while(!ExpImage.appRunStatus()){}
                ExpImage.setAppRunStatusClose();
                if(!ExpImage.appConfigStatus())
                {
                    status.text = qsTr("模型载入失败
文件缺失、内容无效");
                }
                else
                {
                    status.text = qsTr("模型载入成功");
                }
            }
        }
    }

    Label {
        id: status
        x: 1503
        y: 145
        width: 374
        height: 107
        text: qsTr("模型未载入
参数未设置")
        verticalAlignment: Text.AlignVCenter
        anchors.left: openfunction.right
        anchors.leftMargin: 10
        horizontalAlignment: Text.AlignHCenter
        anchors.top: cbtitle2.bottom
        font.pixelSize: 40
        anchors.topMargin: 60
    }

}





























































































































































































































































/*##^## Designer {
    D{i:31;anchors_x:1373}D{i:33;anchors_x:1825}D{i:37;anchors_x:1293}D{i:38;anchors_x:1227;anchors_y:258}
D{i:39;anchors_x:1449}D{i:40;anchors_x:1577}D{i:41;anchors_x:1227}D{i:42;anchors_x:1470;anchors_y:202}
D{i:43;anchors_y:202}D{i:44;anchors_x:761}D{i:47;anchors_x:1081;anchors_y:202}
}
 ##^##*/
