import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2

//import QtQuick.Scene3D 2.0

import "../../Component"
import ".."
import "./UartData"
import "./GyroPage"


import RegisterMyType 1.0
import RegisterDataFresh 1.0

Page {


    width: 1920
    height: 1080
    font.pointSize: 20

    property StackView stack: null

    property int timerprintcnt: 0
    property int timerprinttimeout: 30
    property int functionchoice: 1

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
            if(true === myclassExposeByRegType.getserialOpenFlag()){
                //myclassExposeByRegType.closePort();
                qmlToggleButton.toggleRight();
                //                qmlToggleButton.state = "right";
                //                qmlToggleButton.color = "#CCCCCC";

            }
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
            gyroinfopage.show();
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
            gyroprojectdeveloppage.active;
            gyroprojectdeveloppage.show();
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



//    Component.onCompleted: {

//        myclassExposeByRegType.getPortInfo();
//        comboModel.setComboList(myclassExposeByRegType.receivePort());
//        //        console.log(positionSource.position.coordinate.latitude,
//        //                    positionSource.position.coordinate.longitude)
//    }



//    Timer {
//        id:timer1
//        interval: 100
//        repeat: true
//        triggeredOnStart: false
//        running: false//true

//        onTriggered: {
//            //            compassuiimu.setBearing(myclassExposeByRegType.getAngleYawNum());
//            //            console.log(myclassExposeByRegType.getAngleYawNum());
//        }
//    }

    MyClassType
    {
        id:myclassExposeByRegType
    }

    RFSetPage{
        id:rfsetpage
    }

    GyroProjectDevelopPage{
        id:gyroprojectdeveloppage
    }

    GyroInfoPage{
        id:gyroinfopage
    }

    Button {
        id: buttonstartwork
        x: 465
        y: 793
        width: 123
        height: 80
        text: qsTr("开始转动\n实验平台")
        font.bold: true
        font.pointSize: 22
        onClicked: {
//            myclassExposeByRegType.sendCMD("28", "000020000000", "0000000000b40000");
            myclassExposeByRegType.setPlatformYawAngle(sliderspeed2.value, sliderspeed1.value);

            timerprinttimeout = sliderspeed2.value / sliderspeed1.value + 2;
            functionchoice = 1;
            platformWorking();
//            functionchoice = 1;
//            myclassExposeByRegType.sendCMD("27", "808080800800", "0000000000000000");
//            timerprintcnt = 0;
//            timerPlatformReset.start();

        }
    }

    function platformWorking(){
        if(mydataFresh.serialDataPrintFlag == true){
            mydataFresh.serialDataPrintFlag = false;
            buttonsave.enabled = true;
//            buttonstartprint.text = qsTr("开始打印");
            buttonstartwork.enabled = true;
            buttonstartwork1.enabled = true;
            buttonstartwork2.enabled = true;
            button3.enabled = true;
            button4.enabled = true;
            timerShowtext.stop();
        }
        else{
            //开始打印
            mydataFresh.serialDataPrintFlag = true;
            buttonsave.enabled = false;
            buttonstartwork.enabled = false;
            button3.enabled = false;
            buttonstartwork1.enabled = false;
            buttonstartwork2.enabled = false;
            button4.enabled = false;
//            buttonstartprint.text = qsTr("停止打印");
            myclassExposeByRegType.clearserialSaveAndApp();
            myclassExposeByRegType.clearTIMESet();
            timerprintcnt = 0;
            timerShowtext.start();
        }
    }


    Slider {
        id: sliderspeed1
        x: 119
        y: 704
        width: 886
        height: 40
        stepSize: 1
        to: 50
        from: 10
        value: 20

    }

    Label {
        id: label3
        x: 119
        y: 668
        width: 325
        height: 30
        font.pixelSize: 30
        font.bold: true
        text: qsTr("实验平台转速：") + sliderspeed1.value + qsTr(" °/s")
    }

    Button {
        id: button3
        x: 285
        y: 793
        width: 123
        height: 80
        text: qsTr("实验平台\n复位")
        font.bold: true
        font.pointSize: 22
        onClicked: {
            myclassExposeByRegType.sendCMD("27", "808080800800", "0000000000000000");
        }
    }

    Button {
        id: buttonsave
        x: 1075
        y: 868
        text: qsTr("保存数据")
        font.bold: true
        font.pointSize: 22
        enabled: true

        onPressed: {
            //fileDialog.open();
            //timer1.stop();
            mydataFresh.buttonSaveClick("\\Content_resource\\IMU模组\\数据保存\\", textArea.text);
            //timer1.start();
            datasavecompletedw.show();
        }
    }

    DataSaveCompletedPage{
        id:datasavecompletedw
    }

    ScrollView {
        x: 1075
        y: 157
        width: 542
        height: 697
        TextArea {
            id: textArea
            x: -10
            y: -6
            width: 487
            height: 319
            font.family: "Times New Roman"
            font.bold: true
            placeholderText: qsTr("陀螺仪-数据保存与应用")
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

//    FileDialog{
//        id: fileDialog
//        title: qsTr("文件保存")
//        nameFilters: "*.txt"
//        selectExisting: false
//        //fileMode: FileDialog.SaveFile
//        onAccepted: {

//        }
//    }


    MyDataFresh{
        id:mydataFresh
    }


    Timer{
        id:timerShowgyro
        interval: 1000
        repeat: true
        triggeredOnStart: false
        running: true

        onTriggered: {
            if(myclassExposeByRegType.getCurrentPage() === 2)
            {
                label10.text = myclassExposeByRegType.getGgyroz() + qsTr(" °/s");
            }
//            console.log(myclassExposeByRegType.getGgyroz());
        }
    }


    Timer{
        id:timerShowtext
        interval: 1000
        repeat: true
        triggeredOnStart: false

        onTriggered: {
            timerprintcnt++;
            if(timerprintcnt==4)
            {
                if(functionchoice == 2)
                {
                    myclassExposeByRegType.setPlatformYawAngle(200, 30);
                    sliderspeed1.value = 30;
                }
                if(functionchoice == 3)
                {
                    myclassExposeByRegType.setPlatformYawAngle(200, 10);
                    sliderspeed1.value = 10;
                }
            }
            if(timerprintcnt > timerprinttimeout){
                platformWorking();
            }
            textArea.text = myclassExposeByRegType.getserialSaveAndApp();
            textArea.cursorPosition = textArea.text.length;


        }
    }

    Timer{
        id:timerPlatformReset
        interval: 1000
        repeat: true
        triggeredOnStart: false

        onTriggered: {
            timerprintcnt++;
            if(timerprintcnt>=5)
            {
                timerPlatformReset.stop();
                timerprintcnt = 0;
                if(functionchoice == 2)
                {
                    timerprinttimeout = 12;
                }
                if(functionchoice == 3)
                {
                    timerprinttimeout = 14;
                }
                platformWorking();
//                console.log(functionchoice);
                if(functionchoice == 2)
                {
                    myclassExposeByRegType.setPlatformYawAngle(70, 10);
                    sliderspeed2.value = 200;
                    sliderspeed1.value = 10;
                }
                if(functionchoice == 3)
                {
                    myclassExposeByRegType.setPlatformYawAngle(180, 30);
                    sliderspeed2.value = 200;
                    sliderspeed1.value = 30;
                }
            }
        }
    }


    Button {
        id: opensavefolder
        x: 1234
        y: 868
        text: qsTr("打开保存文件夹")
        font.bold: true
        font.pointSize: 22
        enabled: true
        onPressed: {
            mydataFresh.buttonOpenFolderClick("\\Content_resource\\IMU模组\\数据保存\\");
        }
    }

    Label {
        id: label4
        x: 380
        y: 303
        width: 376
        height: 33
        text: qsTr("陀螺仪测量的转速：\n")
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 30
        font.bold: true
    }

    Label {
        id: label10
        x: 380
        y: 342
        width: 376
        height: 28
        text: "0" + qsTr(" °/s")
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 30
        font.bold: true
    }

    Slider {
        id: sliderspeed2
        x: 119
        y: 593
        width: 886
        height: 40
        from: 90
        to: 260
        value: 180
        stepSize: 1
    }

    Label {
        id: label5
        x: 119
        y: 557
        width: 325
        height: 30
        text: qsTr("实验平台角度：") + sliderspeed2.value + qsTr(" °")
        font.pixelSize: 30
        font.bold: true
    }

    Button {
        id: button4
        x: 119
        y: 793
        width: 123
        height: 80
        text: qsTr("陀螺仪\n校准")
        font.pointSize: 22
        font.bold: true
        onClicked: {
            myclassExposeByRegType.sendCMD("01", "808080800800", "0000000000000000");
        }
    }

    Button {
        id: buttonstartwork1
        x: 654
        y: 793
        width: 123
        height: 80
        text: qsTr("实验平台\n加速实验")
        font.bold: true
        font.pointSize: 22
        onClicked: {
//            myclassExposeByRegType.setPlatformYawAngle(70, 10);
//            sliderspeed1.value = 10;
//            timerprinttimeout = 12;
            functionchoice = 2;
            myclassExposeByRegType.sendCMD("27", "808080800800", "0000000000000000");
            timerprintcnt = 0;
            timerPlatformReset.start();
//            platformWorking();
        }
    }

    Button {
        id: buttonstartwork2
        x: 839
        y: 793
        width: 123
        height: 80
        text: qsTr("实验平台\n减速实验")
        font.bold: true
        font.pointSize: 22
        onClicked: {
//            myclassExposeByRegType.setPlatformYawAngle(180, 30);
//            sliderspeed1.value = 30;
//            timerprinttimeout = 12;
            functionchoice = 3;
            myclassExposeByRegType.sendCMD("27", "808080800800", "0000000000000000");
            timerprintcnt = 0;
            timerPlatformReset.start();
//            platformWorking();
        }
    }
    Image {
        id: refresh_button
        source: "../../Component/refresh.png"
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

}