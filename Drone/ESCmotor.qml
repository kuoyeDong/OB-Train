import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2

//import QtQuick.Scene3D 2.0

import "../Component"
import ".."
import "../Sensor/DroneSensor/UartData"
import "./ESCmotor"


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
        anchors.centerIn: parent
        height: 40
        radius: 20
        border.width: 1
        anchors.verticalCenterOffset: -472
        anchors.horizontalCenterOffset: -614
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
            escinfopage.show();
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
            escprojectdeveloppage.active;
            escprojectdeveloppage.show();
        }
    }

    ESCProjectDevelopPage{
        id:escprojectdeveloppage
    }

    ESCInfoPage{
        id:escinfopage
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
//=============ace for esc
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
        x: 178
        y: 627
        width: 591
        height: 40
        stepSize: 10
        to: 30
        from: 10
        value: 0

    }

    Label {
        id: label3
        x: 191
        y: 567
        width: 365
        height: 30
        font.pixelSize: 30
        font.bold: true
        text: qsTr("加速时间：") + sliderspeed1.value + qsTr(" ")
    }

    Button {
        id: button3
        x: 620
        y: 382
        width: 189
        height: 47
        text: qsTr("清除设置")
        font.bold: true
        font.pointSize: 22
        onClicked: {
            //sliderspeed4.value=1;
            sliderspeed5.value=0;
            sliderspeed1.value=0;
            sliderspeed2.value=0;
            myclassExposeByRegType.sendCMD("2d", "000000000000", "0000000000000000");//发送复位信息
            //复位指令为byte4 0x01
        }
    }

//    Button {
//        id: buttonsave
//        x: 1241
//        y: 875
//        text: qsTr("保存数据")
//        font.bold: true
//        font.pointSize: 22
//        enabled: true

//        onPressed: {
//            //fileDialog.open();
//            //timer1.stop();
//            mydataFresh.buttonSaveClick("\\Content_resource\\IMU模组\\data\\", textArea.text);
//           // mydataFresh.buttonSaveClick("\\Content_resource\\ESC模组\\data\\", textESC.text);
//            //timer1.start();
//            datasavecompletedw.show();
//        }
//    }


//=============ace for esc
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
            mydataFresh.buttonSaveClick("\\Content_resource\\brussless_motor\\data\\", textESC.text);
            //timer1.start();
            datasavecompletedw.show();
        }
    }

    DataSaveCompletedPage{
        id:datasavecompletedw
    }
//=============ace for esc
    ScrollView {
        x: 1075
        y: 462
        width: 542
        height: 376
        TextArea {
            id: textESC
            x: 12
            y: -1
            width: 485
            height: 353
            font.family: "Times New Roman"
            font.bold: true
            placeholderText: qsTr("ESC-数据保存与应用")
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
//=============ace for esc
    Button {
        id: button_print
        x: 1256
        y: 868
        text: qsTr("开始打印")
        font.bold: true
        font.pointSize: 22

        onPressed: {
            if(mydataFresh.serialDataPrintFlag == true){
                mydataFresh.serialDataPrintFlag = false;
                buttonsave.enabled = true;
                button_print.text = qsTr("开始打印");
                timer_print.stop();
            }
            else{
                //开始打印
                mydataFresh.serialDataPrintFlag = true;
                buttonsave.enabled = false;
                button_print.text = qsTr("停止打印");
                myclassExposeByRegType.clearserialSaveAndApp();
                timer_print.start();
            }
        }

    }
//=============ace for esc
    Timer{
        id:timer_print
        interval: 1000
        repeat: true
        triggeredOnStart: false

        onTriggered: {

            textESC.text = myclassExposeByRegType.getserialSaveAndApp();
            textESC.cursorPosition = textESC.text.length;

        }
    }


//    DataSaveCompletedPage{
//        id:datasavecompletedw
//    }
//=============ace for esc
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
            //label10.text = myclassExposeByRegType.getGgyroz() + qsTr(" °/s");
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
                console.log(functionchoice);
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
            x: 1446
            y: 868
            text: qsTr("打开保存文件夹")
            font.bold: true
            font.pointSize: 22
            enabled: true
            onPressed: {
                mydataFresh.buttonOpenFolderClick("\\Content_resource\\brussless_motor\\data\\");//("\\Content_resource\\ESC模组\\data\\", textESC.text);//
            }
        }

        Timer{
            id:timer1
            interval: 1000
            repeat: true
            triggeredOnStart: false
            running: true

            onTriggered: {
                label10.text = myclassExposeByRegType.getESC_Speed() + qsTr("RPM");
            }
        }
        Timer{
            id:timer2
            interval: 1000
            repeat: true
            triggeredOnStart: false
            running: true

            onTriggered: {
                label11.text = myclassExposeByRegType.getESC_Thro() + qsTr("%");
            }
        }
//===================================================================实时发送PPM值
//        Timer{
//            id:timer3
//            interval: 100
//            repeat: true
//            triggeredOnStart: false
//            running: trues

//            onTriggered: {
//                myclassExposeByRegType.setPlatformInfo(sliderspeed5.value);
//                //发送实验x、频率、转速、角度、加速度
//                //comboBox1.currentIndex+1//转换成字从0开始的
//                //comboBox.currentText;//字符串
//            }
//        }

//====================================================================实时发送数据
    Label {
        id: label4
        x: 1132
        y: 242
        width: 267
        height: 33
        text: qsTr("当前转速：\n")
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: 30
        font.bold: true
    }

    Label {
        id: label10
        x: 1405
        y: 247
        width: 230
        height: 28
        text: "RPM"
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 30
        font.bold: true
    }

    Slider {
        id: sliderspeed2
        x: 178
        y: 762
        width: 591
        height: 40
        from: 10
        to: 30
        value: 0
        stepSize: 10
    }

    Label {
        id: label5
        x: 191
        y: 705
        width: 325
        height: 30
        text: qsTr("减速时间：")+ sliderspeed2.value + qsTr(" ")
        font.pixelSize: 30
        font.bold: true
    }

    Label {
        id: label7
        x: 1132
        y: 301
        width: 204
        height: 33
        text: qsTr("油门比例：\n")
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: 30
        font.bold: true
    }

    Label {
        id: label11
        x: 1405
        y: 301
        width: 210
        height: 28
        text: "%"
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 30
        font.bold: true
    }

    Label {
        id: label8
        x: 1132
        y: 172
        width: 332
        height: 33
        text: qsTr("电机状态：\n")
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: 40
        font.bold: true
    }

    Label {
        id: label9
        x: 178
        y: 389
        width: 306
        height: 33
        text: qsTr("无人机旋翼电调控制量：\n")
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: 40
        font.bold: true
    }

    Button {
        id: button4
        x: 312
        y: 824
        width: 300
        height: 92
        text: qsTr("使用当前配置")
        font.pointSize: 26
        font.bold: true
        onClicked: {
            myclassExposeByRegType.setESCMotorInfo(sliderspeed5.value,sliderspeed1.value , sliderspeed2.value,0);

            //myclassExposeByRegType.sendCMD("2d", "000000000100", "0000000000000000");//发送复位信息
            //发送实验x、频率、转速、角度、加速度
            //comboBox1.currentIndex+1//转换成字从0开始的
            //comboBox.currentText;//字符串
        }
    }

//    Slider {
//        id: sliderspeed4
//        x: 164
//        y: 307
//        width: 886
//        height: 40
//        to: 3
//        from: 1
//        stepSize: 1
//        value: 1

//    }

    Label {
        id: label13
        x: 178
        y: 462
        width: 445
        height: 28
        text: qsTr("PPM信号：") + sliderspeed5.value+ qsTr("uS")
        font.pixelSize: 30
        font.bold: true
    }

    Slider {
        id: sliderspeed5
        x: 178
        y: 509
        width: 719
        height: 40
        from: 900
        to: 2000
        stepSize: 1
        value: 0
    }

    Button {
        id: button5
        x: 696
        y: 824
        width: 224
        height: 92
        text: qsTr("模拟飞行")
        font.pointSize: 26
        font.bold: true
        onClicked: {
            myclassExposeByRegType.setESCMotorInfo(sliderspeed5.value,sliderspeed1.value , sliderspeed2.value,2);
            //发送实验x、频率、转速、角度、加速度
            //comboBox1.currentIndex+1//转换成字从0开始的
            //comboBox.currentText;//字符串
        }
    }

    Image {
        id: image
        x: 217
        y: 129
        width: 535
        height: 218
        fillMode: Image.PreserveAspectFit
        source: "images/ESCmotor.gif"
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

}
