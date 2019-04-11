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
            img_tracking.source="../images/src.png";
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
        x: 42
        width:1260
        height:707
        anchors.top: parent.top
        anchors.topMargin: 187
        anchors.left: camera_close.right
        anchors.leftMargin: 81
        visible: true
        source:"../images/src.png";
        cache:false;
    }

    TextField {
        id: pathInput
        width: 176
        height: 59
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
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.left: img_tracking.right
        anchors.leftMargin: 25
        anchors.topMargin: 0
        anchors.top: img_tracking.top
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
                pathInput.text = openImgSaved.fileUrl
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
        anchors.top: img_tracking.top
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
        height: 59
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
        y: 590
        width: 422
        height: 78
        text: qsTr("标定第一帧目标位置")
        anchors.horizontalCenter: statusRect.horizontalCenter
        anchors.bottom: start.top
        anchors.bottomMargin: 35
        highlighted: true
        font.pointSize: 30
    }

    Button {
        id: start
        y: 461
        width: 422
        height: 78
        text: qsTr("运行人形跟踪算法")
        anchors.bottom: labelTarget.top
        anchors.bottomMargin: 35
        anchors.left: firstframe.left
        anchors.leftMargin: 0
        font.pointSize: 30
        highlighted: true
    }

    Button {
        id: labelTarget
        y: 580
        width: 422
        height: 78
        text: qsTr("标定剩余图像")
        anchors.bottom: img_tracking.bottom
        anchors.bottomMargin: 0
        anchors.left: start.left
        font.pointSize: 30
        highlighted: true
        anchors.leftMargin: 0
    }

    Rectangle {
        id: statusRect
        width: 557
        height: 222
        color: "#00000000"
        anchors.top: pathInput.bottom
        anchors.topMargin: 40
        anchors.left: pathInput.left
        anchors.leftMargin: 0
        border.width: 3
    }


    Label {
        id: statusText
        x: 1338
        y: 362
        width: 534
        height: 188
        text: qsTr("相关状态")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.horizontalCenter: statusRect.horizontalCenter
        font.pointSize: 25
    }

    Rectangle {
        id: setRect
        width: 568
        height: 110
        color: "#00000000"
        anchors.left: img_tracking.left
        anchors.leftMargin: 0
        anchors.top: img_tracking.bottom
        border.width: 3
        anchors.topMargin: 7
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
//            if(gray.checkState == Qt.Checked)
//            {
//                ExpImage.exp4Param(0, 0, 0);
//            }
//            else
//            {
//                ExpImage.exp4Param(0, 1, 1);
//            }
            status.text = qsTr("参数已重置")
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
        width: 478
        height: 110
        color: "#00000000"
        anchors.verticalCenterOffset: 0
        anchors.verticalCenter: setRect.verticalCenter
        anchors.leftMargin: 15
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
                    statusText.text = qsTr("请先设置参数");
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
                    img.source = "";
                    img.source = ExpImage.get_tmp_path(0);
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
                    img.source = "";
                    img.source = ExpImage.get_tmp_path(0);
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
                    img.source = "";
                    img.source = ExpImage.get_tmp_path(0);
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
                    img.source = "";
                    img.source = ExpImage.get_tmp_path(0);
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
                    img.source = "";
                    img.source = ExpImage.get_tmp_path(0);
                    ExpImage.setAppRunStatusClose();
                }
            }
        }
    }
}

































































































































/*##^## Designer {
    D{i:5;anchors_x:1352;anchors_y:244}D{i:6;anchors_x:1352;anchors_y:187}D{i:7;anchors_x:1370}
D{i:10;anchors_x:1623;anchors_y:187}D{i:11;anchors_x:1352;anchors_y:244}D{i:12;anchors_x:1817}
D{i:16;anchors_x:1396;anchors_y:461}D{i:17;anchors_x:1396;anchors_y:580}D{i:18;anchors_x:1396;anchors_y:580}
D{i:19;anchors_width:557;anchors_x:1327;anchors_y:342}D{i:20;anchors_x:1396;anchors_y:917}
D{i:21;anchors_x:516}D{i:25;anchors_x:639}
}
 ##^##*/
