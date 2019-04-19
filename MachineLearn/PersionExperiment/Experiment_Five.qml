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
            img_tracking.source = "../images/src.png";
            feature_show.source = "../images/pro.png";
            count.running = false;
            ExpImage.setAppRunStatusClose();
            stack.pop();
        }
    }

    Label {
        id: title
        x: 719
        width: 1052
        height: 56
        text: qsTr("实验五  不同环境噪声中的人形跟踪算法的应用")
        anchors.horizontalCenterOffset: 0
        anchors.top: parent.top
        anchors.topMargin: 82
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 50
    }

    Image{
        id:img_tracking
        x: 43
        width:1260
        height:707
        anchors.top: setRect.bottom
        anchors.topMargin: 25
        visible: true
        source:"../images/src.png";
        cache:false;
    }

    Image {
        id: feature_show
        x: 1375
        y: 293
        width: 460
        height: 460
        visible: true
        cache: false
        source: "../images/pro.png"
    }

    TextField {
        id: pathInput
        width: 176
        height: 54
        text: qsTr("")
        anchors.top: readpathtitle.bottom
        anchors.topMargin: 5
        anchors.left: readpathtitle.left
        anchors.leftMargin: 0
        font.pointSize: 20
    }

    Label {
        id: readpathtitle
        width: 262
        height: 51
        text: qsTr("图像序列存储路径")
        anchors.left: img_tracking.right
        anchors.leftMargin: 20
        anchors.top: setRect.top
        anchors.topMargin: 0
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 32
    }

    Button {
        id: openSeqpath
        y: 209
        width: 64
        height: 59
        anchors.verticalCenterOffset: 0
        anchors.left: pathInput.right
        anchors.leftMargin: 25
        anchors.verticalCenter: pathInput.verticalCenter
        anchors.right: camera_close.right
        anchors.rightMargin: 0
        //        text: qsTr("打开图像存储路径")
        //        font.pointSize: 20

        background: Image {
            id:saveicon
            source: "../images/folder.png"
        }

        onClicked: {
            openImgSaved.open()
        }
    }

    FileDialog{
        id:openImgSaved
        title:qsTr("图像序列存储文件夹")
        folder: shortcuts.desktop
        selectExisting:true
        selectFolder:true
        selectMultiple:true
        onAccepted: {
            if(gray.checkState != Qt.Checked && hog.checkState != Qt.Checked && rgb.checkState != Qt.Checked)
            {
                statusText.text = qsTr("请先设置参数");
            }
            else
            {
                pathInput.text = fileUrl
                if(gray.checkState == Qt.Checked)
                {
                    ExpImage.exp5Param(0, 0, 0);
                }
                else
                {
                    ExpImage.exp5Param(0, 1, 1);
                }
                ExpImage.setPath(fileUrl, 2);
                ExpImage.setEx_id(5, 0);
                ExpImage.expStart();
                while(!ExpImage.appRunStatus()){}
                ExpImage.setAppRunStatusClose();
                if(!ExpImage.appConfigStatus())
                {
                    statusText.text = qsTr("图像序列载入失败");
                }
                else
                {
                    statusText.text = qsTr("图像序列载入成功");
                    img_tracking.source = "";
                    img_tracking.source = ExpImage.get_tmp_path(0);
                }
           }
        }
    }

    Label {
        id: savepathtitle
        y: 187
        width: 262
        height: 51
        text: qsTr("运算结果保存路径")
        verticalAlignment: Text.AlignVCenter
        anchors.top: readpathtitle.top
        font.pixelSize: 32
        anchors.left: readpathtitle.right
        horizontalAlignment: Text.AlignHCenter
        anchors.topMargin: 0
        anchors.leftMargin: 30
    }

    TextField {
        id: pathOutput
        y: 244
        width: 176
        height: 55
        text: qsTr("")
        anchors.left: savepathtitle.left
        anchors.leftMargin: 0
        anchors.verticalCenter: pathInput.verticalCenter
        font.pointSize: 20
    }

    Button {
        id: openRespath
        y: 244
        width: 64
        height: 59
        anchors.verticalCenter: pathOutput.verticalCenter
        anchors.left: pathOutput.right
        anchors.rightMargin: 0
        background: Image {
            id: saveicon1
            source: "../images/folder.png"
        }
        anchors.right: camera_close.right
        anchors.leftMargin: 25
        anchors.verticalCenterOffset: 0

        onClicked: {
            openResSaved.open()
        }
    }

    FileDialog{
        id:openResSaved
        title:qsTr("数据保存文件夹")
        folder: shortcuts.desktop
        selectExisting:true
        selectFolder:true
        selectMultiple:true
        onAccepted: {
            ExpImage.setPath(fileUrl, 1);
            pathOutput.text = openResSaved.fileUrl
        }
    }

    Button {
        id: firstframe
        x: 1404
        width: 422
        height: 60
        text: qsTr("标定第一帧目标位置")
        anchors.top: statusRect.bottom
        anchors.topMargin: 6
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: statusRect.horizontalCenter
        highlighted: true
        font.pointSize: 30

        onClicked: {
            ExpImage.setEx_id(5, 1);
            ExpImage.expStart();
            while(!ExpImage.appRunStatus()){}
            ExpImage.setAppRunStatusClose();
            if(!ExpImage.appConfigStatus())
            {
                statusText.text = qsTr("行人检测器初始化失败
请手动设置目标框");
            }
            else
            {
                statusText.text = qsTr("检测到行人");
                img_tracking.source = "";
                img_tracking.source = ExpImage.get_tmp_path(0);
            }
        }
    }

    Button {
        id: start
        width: 422
        height: 60
        text: qsTr("运行人形跟踪算法")
        anchors.top: firstframe.bottom
        anchors.topMargin: 5
        anchors.left: firstframe.left
        anchors.leftMargin: 0
        font.pointSize: 30
        highlighted: true

        onClicked: {
            if(gray.checkState != Qt.Checked && hog.checkState != Qt.Checked && rgb.checkState != Qt.Checked)
            {
                statusText.text = qsTr("请先设置参数
并载入测试图像序列");
            }
            else
            {
                count.running=true;
//                ExpImage.setEx_id(5, 4);
//                ExpImage.expStart();
//                img_tracking.source = "";
//                img_tracking.source = ExpImage.get_tmp_path(0);
            }
        }
    }

    Timer{
        id:count;
        interval: 50;
        running: false;
        repeat: true
        onTriggered: {
            if(!ExpImage.appRunStatus())
            {
                statusText.text = qsTr("开始运行跟踪算法");
                ExpImage.setEx_id(5, 4);
                ExpImage.expStart();
                img_tracking.source = "";
                img_tracking.source = ExpImage.get_tmp_path(0);
                feature_show.source = "";
                feature_show.source = ExpImage.get_tmp_path(1);
            }
            else
            {
                img_tracking.source = "../images/src.png";
                feature_show.source = "../images/pro.png";
                statusText.text = qsTr("运行结束");
                count.running = false;
            }
        }
    }

    Rectangle {
        id: statusRect
        y: 762
        width: 557
        height: 108
        color: "#00000000"
        anchors.left: pathInput.left
        anchors.leftMargin: 5
        border.width: 3
    }


    Label {
        id: statusText
        x: 1338
        y: 359
        width: 534
        height: 84
        text: qsTr("相关状态")
        anchors.verticalCenter: statusRect.verticalCenter
        anchors.horizontalCenterOffset: 0
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.horizontalCenter: statusRect.horizontalCenter
        font.pointSize: 25
    }

    Rectangle {
        id: setRect
        y: 158
        width: 568
        height: 110
        color: "#00000000"
        anchors.left: paramSetTitle.right
        anchors.leftMargin: 16
        border.width: 3
    }

    Button {
        id: reset
        y: 936
        width: 164
        height: 67
        text: qsTr("重置跟踪器")
        font.pointSize: 20
        anchors.left: gray.right
        anchors.leftMargin: 15
        anchors.verticalCenter: setRect.verticalCenter
        highlighted: true

        onClicked: {
            ExpImage.setEx_id(5, 3);
            ExpImage.expStart();
            while(!ExpImage.appRunStatus()){}
            ExpImage.setAppRunStatusClose();
            statusText.text = qsTr("参数已重置
请重新载入测试图像序列")
        }
    }

    CheckBox {
        id: hog
        y: 929
        width: 105
        height: 54
        text: qsTr("HOG")
        anchors.verticalCenter: setRect.verticalCenter
        anchors.top: param_rectangle.top
        anchors.topMargin: 0
        anchors.left: setRect.left
        anchors.leftMargin: 15
        font.bold: true
        font.pointSize: 25
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
        anchors.leftMargin: 15
        anchors.verticalCenter: hog.verticalCenter
        font.bold: true
        font.pointSize: 25
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
        anchors.leftMargin: 15
        anchors.verticalCenter: rgb.verticalCenter
        font.bold: true
        font.pointSize: 25
        onCheckStateChanged: {
            if(gray.checkState == Qt.Checked)
            {
                hog.checkState = Qt.Unchecked;
                rgb.checkState = Qt.Unchecked;
            }
        }
    }

    Rectangle {
        id: rect_rectangle
        y: 901
        width: 274
        height: 110
        color: "#00000000"
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: setRect.verticalCenter
        anchors.leftMargin: 274
        border.width: 3
        anchors.left: setRect.right

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
                if(gray.checkState != Qt.Checked && hog.checkState != Qt.Checked && rgb.checkState != Qt.Checked)
                {
                    statusText.text = qsTr("请先设置参数
并载入测试图像序列");
                }
                else
                {
                    if(gray.checkState == Qt.Checked)
                    {
                        ExpImage.exp5Param(0, 0, 0);
                    }
                    else
                    {
                        ExpImage.exp5Param(0, 1, 1);
                    }
                    ExpImage.setEx_id(5, 2);
                    ExpImage.expStart();
                    while(!ExpImage.appRunStatus()){}
                    img_tracking.source = "";
                    img_tracking.source = ExpImage.get_tmp_path(0);
                    ExpImage.setAppRunStatusClose();
                }
            }
        }

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
                if(gray.checkState != Qt.Checked && hog.checkState != Qt.Checked && rgb.checkState != Qt.Checked)
                {
                    statusText.text = qsTr("请先设置参数");
                }
                else
                {
                    if(gray.checkState == Qt.Checked)
                    {
                        ExpImage.exp5Param(1, 0, 0);
                    }
                    else
                    {
                        ExpImage.exp5Param(1, 1, 1);
                    }
                    ExpImage.setEx_id(5, 2);
                    ExpImage.expStart();
                    while(!ExpImage.appRunStatus()){}
                    img_tracking.source = "";
                    img_tracking.source = ExpImage.get_tmp_path(0);
                    ExpImage.setAppRunStatusClose();
                }
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
                if(gray.checkState != Qt.Checked && hog.checkState != Qt.Checked && rgb.checkState != Qt.Checked)
                {
                    statusText.text = qsTr("请先设置参数");
                }
                else
                {
                    if(gray.checkState == Qt.Checked)
                    {
                        ExpImage.exp5Param(2, 0, 0);
                    }
                    else
                    {
                        ExpImage.exp5Param(2, 1, 1);
                    }
                    ExpImage.setEx_id(5, 2);
                    ExpImage.expStart();
                    while(!ExpImage.appRunStatus()){}
                    img_tracking.source = "";
                    img_tracking.source = ExpImage.get_tmp_path(0);
                    ExpImage.setAppRunStatusClose();
                }
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
                if(gray.checkState != Qt.Checked && hog.checkState != Qt.Checked && rgb.checkState != Qt.Checked)
                {
                    statusText.text = qsTr("请先设置参数");
                }
                else
                {
                    if(gray.checkState == Qt.Checked)
                    {
                        ExpImage.exp5Param(3, 0, 0);
                    }
                    else
                    {
                        ExpImage.exp5Param(3, 1, 1);
                    }
                    ExpImage.setEx_id(5, 2);
                    ExpImage.expStart();
                    while(!ExpImage.appRunStatus()){}
                    img_tracking.source = "";
                    img_tracking.source = ExpImage.get_tmp_path(0);
                    ExpImage.setAppRunStatusClose();
                }
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
                if(gray.checkState != Qt.Checked && hog.checkState != Qt.Checked && rgb.checkState != Qt.Checked)
                {
                    statusText.text = qsTr("请先设置参数");
                }
                else
                {
                    if(gray.checkState == Qt.Checked)
                    {
                        ExpImage.exp5Param(4, 0, 0);
                    }
                    else
                    {
                        ExpImage.exp5Param(4, 1, 1);
                    }
                    ExpImage.setEx_id(5, 2);
                    ExpImage.expStart();
                    while(!ExpImage.appRunStatus()){}
                    img_tracking.source = "";
                    img_tracking.source = ExpImage.get_tmp_path(0);
                    ExpImage.setAppRunStatusClose();
                }
            }
        }
    }

    Label {
        id: paramSetTitle
        x: 43
        y: 158
        width: 128
        height: 112
        text: qsTr("参数
设置")
        font.pointSize: 30
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Label {
        id: paramSetTitle2
        x: 864
        y: 158
        width: 137
        height: 112
        text: qsTr("设置
矩形框")
        font.pointSize: 30
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}

























































































































































































/*##^## Designer {
    D{i:4;anchors_x:43}D{i:5;anchors_x:1352;anchors_y:244}D{i:6;anchors_x:1352;anchors_y:158}
D{i:7;anchors_x:1370}D{i:10;anchors_x:1623;anchors_y:187}D{i:11;anchors_x:1352;anchors_y:244}
D{i:12;anchors_x:1817}D{i:15;anchors_y:582}D{i:16;anchors_x:1396;anchors_y:461}D{i:17;anchors_x:1396;anchors_y:580}
D{i:18;anchors_width:557;anchors_x:1327;anchors_y:342}D{i:19;anchors_x:43;anchors_y:917}
D{i:20;anchors_x:516}D{i:24;anchors_x:639}
}
 ##^##*/
