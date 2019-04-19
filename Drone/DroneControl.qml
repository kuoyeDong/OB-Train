import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import "../Component"
import ".."
import "../Sensor/DroneSensor/UartData"
import "./DronePage"

import RegisterMyType 1.0
import RegisterDataFresh 1.0

Page {
    id: page


    width: 1920
    height: 1080
    font.pointSize: 20

    property StackView stack: null
    property int cnt_timer: 0;

    background: Image {
        source: "../images/background.png"
    }

    Button {
        height: 32
        width: 120
        text: "<-"
        anchors.left: parent.left
        anchors.top: parent.top
        onClicked: {
            if(true === myclassExposeByRegType.getserialOpenFlag()){
                //myclassExposeByRegType.closePort();
                qmlToggleButton.toggleRight();
                //                qmlToggleButton.state = "right";
                //                qmlToggleButton.color = "#CCCCCC";

            }
            myclassExposeByRegType.setCurrentPage(0);//清除页面设置
            stack.pop()
        }
    }

    ComboBox {
        id: comboBox
        objectName: "portComboBox"
        x: 146
        y: 48
        font.bold: true
        font.pointSize: 20

        model:comboModel.comboList
    }

    QmlToggleButton{
        id: qmlToggleButton
//        anchors.centerIn: parent
        height: 40
        radius: 20
        x:305
        y:48
        border.width: 1
        width: 106
    }

    Button {
        id: button7
        x: 437
        y: 48
        text: qsTr("配置无线地址")
        font.bold: true
        font.pointSize: 20
        onClicked:
        {
            rfsetpage.show();
        }

    }

    Button {
        id: button1
        x: 653
        y: 50
        text: qsTr("教学资料")
        font.bold: true
        font.pointSize: 20

        onClicked:
        {
            droneinfopage.show();
        }

    }

    Button {
        id: button2
        x: 821
        y: 48
        text: qsTr("工程实例")
        font.bold: true
        font.pointSize: 20

        onClicked:
        {
            droneprojectdeveloppage.active;
            droneprojectdeveloppage.show();
        }
    }

    Connections {
        target: qmlToggleButton
        onToggleRight: {
            myclassExposeByRegType.closePort();

        }

    }

    Connections {
        target: qmlToggleButton
        onToggleLeft: {
            var portName;
            var keys = Object.keys(comboBox);
            for(var i = 0; i < keys.length; i++) {
                var key = keys[i];

                if (key === "currentIndex") {
                    //                    console.log("key FOUND:" + comboBox[key]);
                    portName = comboModel.getElement(comboBox[key]);

                    break;
                }
            }
            myclassExposeByRegType.openAndSetPort(portName,3,3,0,0,0);


        }
    }

    MyClassType
    {
        id:myclassExposeByRegType
    }

    MyDataFresh{
        id:mydataFresh
    }

    Label {
        id: textEdit_read_acc_X
        x: 256
        y: 614
        width: 80
        height: 26
        text: qsTr("0")
        font.bold: true
        font.pixelSize: 30
    }

    Label {
        id: textEdit_read_acc_Y
        x: 256
        y: 666
        width: 86
        height: 26
        text: qsTr("0")
        font.bold: true
        font.pixelSize: 30
    }

    Label {
        id: textEdit_read_acc_Z
        x: 256
        y: 707
        width: 91
        height: 33
        text: qsTr("0")
        font.bold: true
        font.pixelSize: 30
    }


    Label {
        id: label_read_acc_X
        x: 8
        y: 6
        width: 122
        height: 26
        text: qsTr("X轴分量Ax：")
        font.bold: true
        anchors.centerIn: parent
        font.pixelSize: 30
        anchors.verticalCenterOffset: 87
        anchors.horizontalCenterOffset: -804
    }

    Label {
        id: label_read_acc_Y
        x: 4
        y: 10
        width: 120
        height: 32
        text: qsTr("Y轴分量Ay：")
        font.bold: true
        anchors.centerIn: parent
        font.pixelSize: 30
        anchors.horizontalCenterOffset: -805
        anchors.verticalCenterOffset: 136
    }

    Label {
        id: label_read_acc_Z
        x: -2
        y: 2
        width: 120
        height: 24
        text: qsTr("Z轴分量Az：")
        font.bold: true
        anchors.centerIn: parent
        font.pixelSize: 30
        anchors.verticalCenterOffset: 179
        anchors.horizontalCenterOffset: -805
    }


    Rectangle {
        x:779
        y:122
        width: 715
        height: 816
        color: "#00000000"
        border.width: 1

//        IMUDataApplicatePage{
//            id:imudataapplicatepage
//            x:4
//            y:2
//            width: 575
//            height: 390
//        }
    }



    Rectangle {
        //        id: page
        x: 132
        y: 131
        width: 400
        height: 400
        color: "#00000000"
        border.color: "black"

        DRONE2D {
//            id: mydrone2d
            width: 400
            height: 400

        }
    }
//    Component.onCompleted: {

//        myclassExposeByRegType.getPortInfo();
//        comboModel.setComboList(myclassExposeByRegType.receivePort());

//    }


//    MyClassType
//    {
//        id:myclassExposeByRegType
//    }

    RFSetPage{
        id:rfsetpage
    }

    DroneProjectDevelopPage{
        id:droneprojectdeveloppage
    }

    DroneInfoPage{
        id:droneinfopage
    }

//    AccInfoPage_Tips{
//        id:accinfopage_tips
//    }



    Button {
        id: button_print
        x: 799
        y: 878
        text: qsTr("开始打印")
        font.bold: true
        font.pointSize: 22

        onPressed: {
            if(mydataFresh.serialDataPrintFlag == true){
                mydataFresh.serialDataPrintFlag = false;
                button_save.enabled = true;
                button_print.text = qsTr("开始打印");
                timer_print.stop();
            }
            else{
                //开始打印
                mydataFresh.serialDataPrintFlag = true;
                button_save.enabled = false;
                button_print.text = qsTr("停止打印");
                myclassExposeByRegType.clearserialSaveAndApp();
                timer_print.start();
                cnt_timer = 0;
            }
        }

    }


    Button {
        id: button_save
        x: 989
        y: 878
        text: qsTr("保存")
        font.bold: true
        font.pointSize: 22
        enabled: true

        onPressed: {
            mydataFresh.buttonSaveClick("\\Content_resource\\drone\\data\\", textArea.text);
            datasavecompletedw.show();
        }
    }

    DataSaveCompletedPage{
        id:datasavecompletedw
    }

    ScrollView {
        x: 799
        y: 143
        width: 675
        height: 729
        TextArea {
            id: textArea
            x: -3
            y: -6
            width: 546
            height: 719
            text: ""
            font.family: "Times New Roman"
            font.bold: true
            placeholderText: qsTr("整机-数据保存与应用")
            //            objectName: "MagDateAppPageText"
//            font.pointSize: 9
            font.pixelSize: 20
            //text: myserialtest1.receivedata
            readOnly: true
            selectByKeyboard: true
            selectByMouse: true
            //background: ""

        }
    }


    Timer{
        id:timer_print
        interval: 500
        repeat: true
        triggeredOnStart: false


        onTriggered: {

            cnt_timer++;
            if(cnt_timer>=40)
            {
                if(mydataFresh.serialDataPrintFlag == true){
                    mydataFresh.serialDataPrintFlag = false;
                    button_save.enabled = true;
                    button_print.text = qsTr("开始打印");
                    timer_print.stop();
                }
            }
           if(myclassExposeByRegType.getCurrentPage() === 7)
           {
                textArea.text = myclassExposeByRegType.getserialSaveAndApp();
                textArea.cursorPosition = textArea.text.length;
           }

        }
    }

    Timer{
        id:timer_fresh
        interval: 300
        repeat: true
        triggeredOnStart: false

        running: true
        onTriggered: {

           if(myclassExposeByRegType.getCurrentPage() === 7)
           {
               textEdit_read_acc_X.text = myclassExposeByRegType.getAccX();
               textEdit_read_acc_Y.text = myclassExposeByRegType.getAccY();
               textEdit_read_acc_Z.text = myclassExposeByRegType.getAccZ();
               textEdit_read_acc_X1.text = myclassExposeByRegType.getGgyrox();
               textEdit_read_acc_Y1.text = myclassExposeByRegType.getGgyroy();
               textEdit_read_acc_Z1.text = myclassExposeByRegType.getGgyroz();
           }

        }

    }

    Button {
        id: button_open
        x: 1148
        y: 878
        text: qsTr("打开保存文件夹")
        font.bold: true
        font.pointSize: 22
        enabled: true
        onPressed: {
            mydataFresh.buttonOpenFolderClick("\\Content_resource\\drone\\data\\");
        }
    }

    Image {
        id: refresh_button
        source: "../Component/refresh.png"
        x: 84
        y: 50
        height: 38
        width: 36
        MouseArea{
            anchors.fill: parent
            onClicked: {
                myclassExposeByRegType.getPortInfo();
                comboModel.setComboList(myclassExposeByRegType.receivePort());
            }
        }
    }

    Label {
        id: textEdit_read_acc_X1
        x: 579
        y: 614
        width: 80
        height: 26
        text: qsTr("0")
        font.bold: true
        font.pixelSize: 30
    }

    Label {
        id: textEdit_read_acc_Y1
        x: 579
        y: 660
        width: 86
        height: 26
        text: qsTr("0")
        font.bold: true
        font.pixelSize: 30
    }

    Label {
        id: textEdit_read_acc_Z1
        x: 579
        y: 707
        width: 91
        height: 33
        text: qsTr("0")
        font.bold: true
        font.pixelSize: 30
    }

    Label {
        id: label_read_acc_X1
        x: 17
        y: 15
        width: 122
        height: 26
        text: qsTr("X轴分量Gx：")
        anchors.verticalCenterOffset: 87
        font.bold: true
        anchors.horizontalCenterOffset: -480
        anchors.centerIn: parent
        font.pixelSize: 30
    }

    Label {
        id: label_read_acc_Y1
        x: 13
        y: 19
        width: 120
        height: 32
        text: qsTr("Y轴分量Gy：")
        anchors.verticalCenterOffset: 136
        font.bold: true
        anchors.horizontalCenterOffset: -481
        anchors.centerIn: parent
        font.pixelSize: 30
    }

    Label {
        id: label_read_acc_Z1
        x: 7
        y: 11
        width: 120
        height: 24
        text: qsTr("Z轴分量Gz：")
        anchors.verticalCenterOffset: 179
        font.bold: true
        anchors.horizontalCenterOffset: -481
        anchors.centerIn: parent
        font.pixelSize: 30
    }

    Label {
        id: label_read_acc_X2
        x: 2
        y: -2
        width: 122
        height: 26
        text: qsTr("加速度计：")
        anchors.verticalCenterOffset: 46
        font.bold: true
        anchors.horizontalCenterOffset: -804
        anchors.centerIn: parent
        font.pixelSize: 30
    }

    Label {
        id: label_read_acc_X3
        x: 9
        y: -1
        width: 122
        height: 26
        text: qsTr("陀螺仪：")
        anchors.verticalCenterOffset: 46
        font.bold: true
        anchors.horizontalCenterOffset: -480
        anchors.centerIn: parent
        font.pixelSize: 30
    }

    Button {
        id: button_cal_acc
        x: 95
        y: 867
        width: 133
        height: 80
        text: qsTr("传感器\n校准")
        font.bold: true
        font.pointSize: 22

        onClicked:
        {
            myclassExposeByRegType.sendCMD("01", "808080800800", "0000000000000000");
        }
    }

    Slider {
        id: sliderspeed1
        x: 84
        y: 809
        width: 626
        height: 40
        stepSize: 1
        to: 25
        from: -25
        value: 0

    }
    Label {
        id: label5
        x: 95
        y: 773
        width: 325
        height: 30
        text: qsTr("实验平台角度：") + sliderspeed1.value + qsTr(" °")
        font.pixelSize: 30
        font.bold: true
    }

    Button {
        id: buttonstartwork
        x: 271
        y: 867
        width: 123
        height: 80
        text: qsTr("实验平台\n角度设置")
        font.bold: true
        font.pointSize: 22
        onClicked: {
            myclassExposeByRegType.setPlatformRollAngle(-sliderspeed1.value.toLocaleString(), 15);

        }
    }

    Button {
        id: button3
        x: 437
        y: 867
        width: 123
        height: 80
        text: qsTr("实验平台\n复位")
        font.bold: true
        font.pointSize: 22
        onClicked: {
            myclassExposeByRegType.setPlatformRollAngle(0, 20);
            sliderspeed1.value = 0;
        }
    }

}
