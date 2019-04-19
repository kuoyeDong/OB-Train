import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import ".."

Page {
    id: page
    width: 1920
    height: 1080
    property alias durationFontwordSpacing: duration.font.wordSpacing

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
            count.running=false;
            if(ExpFaceImage.isOpened())
            {
                ExpFaceImage.camera_close();
                img_tracking.source="../../VisionVoice/images/connection_1.png";
            }
            stack.pop();
         }
    }

    Label {
        id: title
        x: 719
        width: 948
        height: 56
        text: qsTr("实验一  使用图传模组采集图像数据实验")
        anchors.horizontalCenterOffset: 0
        anchors.top: parent.top
        anchors.topMargin: 82
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 50
    }

    Button {
        id: camera_connect
        x: 72
        y: 223
        width: 190
        height: 80
        text: qsTr("连接图传")
        anchors.left: label.left
        anchors.leftMargin: 0
        anchors.top: label.bottom
        anchors.topMargin: 65
        font.pointSize: 25

        MouseArea{
            id:open_camera
            x: 0
            y: 0
            width: 190
            height: 80
            onPressed: {
                if(!ExpFaceImage.isOpened())
                {
                    count.running=true;
                    ExpFaceImage.camera_open();
                }
            }
        }
    }

    Button {
        id: camera_close
        width: 190
        height: 80
        text: qsTr("断开图传")
        anchors.top: camera_connect.top
        anchors.topMargin: 0
        font.pointSize: 25
        anchors.left: camera_connect.right
        anchors.leftMargin: 42

        MouseArea{
            id:close_camera_area
            x: 0
            y: 0
            width: 190
            height: 80
            anchors.left: camera_connect.right
            anchors.leftMargin: 30
            onPressed: {
                count.running=false;
                if(ExpFaceImage.isOpened())
                {
                    ExpFaceImage.camera_close();
                    img_tracking.source="../../VisionVoice/images/connection_1.png";
                }
            }
        }
    }

    Image{
           id:img_tracking;
           width:1260
           height:707
           anchors.top: parent.top
           anchors.topMargin: 223
           anchors.left: camera_close.right
           anchors.leftMargin: 81
           visible: true
           source:"../../VisionVoice/images/connection_1.png";
           cache:false;
    }

    Timer{
        id:count;
        interval: 5;
        running: false;
        repeat: true
        onTriggered: {
            ExpFaceImage.setImage();
        }
    }

    Connections{
        target: ExpFaceImage
        onCallQmlRefeshImg:{
            if(0 === ExpFaceImage.connectStatus())
            {
                //img_tracking.source = "";
                img_tracking.source = "../../VisionVoice/images/connection_2.png";
            }
            else
            {
                img_tracking.source = "";
                img_tracking.source = "image://ExpFaceImg";
            }

            if(ExpFaceImage.faceDetRunStatus())
            {
                recordpath.text = qsTr("采集保存中......")
                playicon.source = "../images/recording.png"
            }
            else
            {
                recordpath.text = qsTr("开始采集图像")
                playicon.source = "../images/play.png"
            }
        }
    }

    TextField {
        id: pathInput
        width: 332
        height: 80
        text: qsTr("")
        font.pointSize: 20
        anchors.left: pathtitle.left
        anchors.leftMargin: 0
        anchors.top: pathtitle.bottom
        anchors.topMargin: 10
    }

    Label {
        id: pathtitle
        width: 422
        height: 65
        text: qsTr("请选择图像存储路径
（为空则不会录制）：")
        horizontalAlignment: Text.AlignHCenter
        anchors.left: cbtitle2.right
        anchors.leftMargin: -190
        anchors.topMargin: 60
        anchors.top: cbtitle2.bottom
        font.pixelSize: 30
    }

    Button {
        id: record
        x: 72
        width: 152
        height: 153
        text: qsTr("")
        anchors.horizontalCenter: pathtitle.horizontalCenter
        anchors.top: pathInput.bottom
        anchors.topMargin: 25

        background: Image {
            id:playicon
            source: "../images/play.png"
        }

        onClicked:{
            if(ExpFaceImage.isOpened())
            {
                recordpath.text = qsTr("采集保存中......")
                playicon.source = "../images/recording.png"
                ExpFaceImage.getParamsForFaceExp1(pathInput.text,duration.currentText, interval.currentText);
                ExpFaceImage.openFaceDetbyMTCNN();

//                ExpFaceImage.exp1Param(duration.currentText, fps.currentText);
//                ExpFaceImage.setEx_id(1, 1);
            }
        }
    }

    Label {
        id: recordpath
        x: 49
        width: 211
        text: qsTr("开始采集图像")
        anchors.horizontalCenterOffset: 1
        anchors.bottom: img_tracking.bottom
        anchors.bottomMargin: -30
        anchors.top: record.bottom
        anchors.topMargin: 6
        anchors.horizontalCenter: record.horizontalCenter
        font.pixelSize: 35
    }

    Label {
        id: cbtitle
        width: 190
        height: 42
        text: qsTr("采集数量：")
        anchors.top: camera_connect.bottom
        anchors.topMargin: 54
        anchors.left: camera_connect.left
        anchors.leftMargin: 0
        font.pixelSize: 40
    }

    ComboBox {
        id: duration
        width: 112
        height: 68
        anchors.verticalCenter: second.verticalCenter
        font.pointSize: 25
        anchors.top: camera_close.bottom
        anchors.topMargin: 41
        anchors.left: camera_close.left
        anchors.leftMargin: 0

        model: ["100", "200", "300", "400"]
    }

    Label {
        id: second
        x: 419
        y: 357
        width: 46
        height: 42
        text: qsTr("帧")
        anchors.leftMargin: 32
        anchors.topMargin: 54
        anchors.top: camera_connect.bottom
        font.pixelSize: 40
        anchors.left: duration.right
    }

    Label {
        id: cbtitle2
        width: 190
        height: 42
        text: qsTr("采集数字：")
        anchors.leftMargin: 0
        anchors.topMargin: 80
        anchors.top: cbtitle.bottom
        font.pixelSize: 40
        anchors.left: cbtitle.left
    }

    ComboBox {
        id: interval
        x: 304
        y: 460
        width: 112
        height: 66
        anchors.verticalCenter: fpstitle.verticalCenter
        font.pointSize: 25
        anchors.leftMargin: 0
        anchors.topMargin: 55
        model: ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]
        anchors.top: duration.bottom
        anchors.left: duration.left
    }

    Label {
        id: fpstitle
        x: 448
        y: 479
        width: 46
        height: 42
        text: qsTr("")
        anchors.leftMargin: 40
        anchors.topMargin: 80
        anchors.top: second.bottom
        font.pixelSize: 40
        anchors.left: fps.right
    }

    Button {
        id: button
        x: 420
        y: 656
        width: 74
        height: 80
        z: 1
        anchors.bottom: pathInput.bottom
        anchors.bottomMargin: 0
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
        title:qsTr("选择保存路径")
        folder: shortcuts.desktop
        selectExisting:true
        selectFolder:true
        selectMultiple:true
        onAccepted: {
            pathInput.text = openImgSaved.fileUrl
        }
    }

}































































/*##^## Designer {
    D{i:6;anchors_y:162}D{i:13;anchors_x:72}D{i:19;anchors_x:72;anchors_y:480}
}
 ##^##*/
