import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2

//import QtQuick.Scene3D 2.0

import "../Component"
import ".."
import "../Sensor/DroneSensor/UartData"
import "./Carmotor"


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

    property int datanum: 0

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
        y: 50
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
        x: 665
        y: 50
        text: qsTr("教学资料")
        font.bold: true
        font.pointSize: 20

        onClicked:
        {
            carmotorinfopage.show();
        }

    }

    Button {
        id: button2
        x: 827
        y: 50
        text: qsTr("工程实例")
        font.bold: true
        font.pointSize: 20

        onClicked:
        {
            carmotorprojectdeveloppage.active;
            carmotorprojectdeveloppage.show();
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

    CarmotorProjectDevelopPage{
        id:carmotorprojectdeveloppage
    }

    CarmotorInfoPage{
        id:carmotorinfopage
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


//    Slider {
//        id: sliderspeed1
//        x: 185
//        y: 699
//        width: 584
//        height: 40
//        stepSize: 0.1
//        to: 5
//        from: 0
//        value: 0

//    }

    Label {
        id: label3
        x: 285
        y: 689
        width: 91
        height: 30
        font.pixelSize: 30
        font.bold: true
        text: qsTr("Ki = 0")/* + label3.text *///+ sliderspeed1.value.toFixed(1).toLocaleString() //+ qsTr(" ")
    }

    Button {
        id: button3
        x: 552
        y: 165
        width: 190
        height: 47
        text: qsTr("清除设置")
        font.bold: true
        font.pointSize: 22
        onClicked: {
            //sliderspeed4.value=1;
            //sliderspeed5.value=0;
            //sliderspeed1.value=0;
           // sliderspeed2.value=0;
            label3.text=qsTr("Kp = "+0)
            label13.text=qsTr("Ki = "+0)
            label5.text=qsTr("Kd = "+0)
            sliderspeed6.value=0;
            sliderspeed7.value=0;

            myclassExposeByRegType.sendCMD("33", "000000000000", "0300000000000000");//发送复位信息
            datanum = 0;
            //复位指令为byte4 0x01
        }
    }


//====================================================ace


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
            mydataFresh.buttonSaveClick("\\Content_resource\\robotcar_motor\\data\\", textCar.text);
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
            id: textCar
            x: 12
            y: -1
            width: 485
            height: 353
            font.family: "Times New Roman"
            font.bold: true
            placeholderText: qsTr("data与应用")
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
        x: 1259
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

            textCar.text = myclassExposeByRegType.getserialSaveAndApp();
            textCar.cursorPosition = textCar.text.length;

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
                mydataFresh.buttonOpenFolderClick("\\Content_resource\\robotcar_motor\\data\\");//("\\Content_resource\\ESC模组\\data\\", textESC.text);//
            }
        }

        Timer{
            id:timer1
            interval: 1000
            repeat: true
            triggeredOnStart: false
            running: true

            onTriggered: {
                label10.text = myclassExposeByRegType.getESC_Speed();// + qsTr("RPM");
            }
        }
        Timer{
            id:timer2
            interval: 1000
            repeat: true
            triggeredOnStart: false
            running: true

            onTriggered: {
                label11.text = myclassExposeByRegType.getESC_Hall() ;//+ qsTr("%");
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
        width: 214
        height: 33
        text: qsTr("当前转速(RPM)：\n")
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: 30
        font.bold: true
    }

    Label {
        id: label10
        x: 1349
        y: 247
        width: 230
        height: 28
        text: "0"
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 30
        font.bold: true
    }

//    Slider {
//        id: sliderspeed2
//        x: 185
//        y: 805
//        width: 591
//        height: 40
//        from: 0
//        to: 5
//        value: 0
//        stepSize: 0.1
//    }

    Label {
        id: label5
        x: 583
        y: 691
        width: 103
        height: 30
        text: qsTr("Kd = 0") //+ sliderspeed2.value.toFixed(1).toLocaleString() //+ qsTr("")
        font.pixelSize: 30
        font.bold: true
    }

    Label {
        id: label8
        x: 1132
        y: 172
        width: 342
        height: 33
        text: qsTr("机器车电机状态量：\n")
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: 40
        font.bold: true
    }
    Label {
        id: label12
        x: 185
        y: 255
        width: 79
        height: 30
        text: qsTr("实验：") //+ comboBox1.currentIndex+1//sliderspeed4.value
        font.pixelSize: 30
        font.bold: true
    }
    ComboBox {
        id: comboBox1
        x: 276
        y: 250
        opacity: 3
        font.bold: true
        font.pointSize: 20
        model:[1, 2]
    }

    Label {
        id: label9
        x: 175
        y: 165
        width: 306
        height: 33
        text: qsTr("机器车电机控制量：\n")
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: 40
        font.bold: true
    }

    Button {
        id: button4
        x: 324
        y: 806
        width: 279
        height: 68
        text: qsTr("使用当前配置")
        font.pointSize: 26
        font.bold: true
        onClicked: {
            //myclassExposeByRegType.setCarMotorInfo(sliderspeed6.value,(10*sliderspeed6.value) ,(10*sliderspeed6.value) ,(10*sliderspeed6.value) ,(10*sliderspeed7.value),comboBox1.currentIndex+1 );
//            myclassExposeByRegType.setCarMotorInfo(sliderspeed6.value,'0' ,'0' ,'0' ,(10*sliderspeed7.value),comboBox1.currentIndex+1 );
            //comboBox1.currentIndex,
            //myclassExposeByRegType.sendCMD("2d", "000000000100", "0000000000000000");//发送复位信息
            //发送实验x、频率、转速、角度、加速度
            //comboBox1.currentIndex+1//转换成字从0开始的
            //comboBox.currentText;//字符串
            if(datanum == 1)
            {
                myclassExposeByRegType.setCarMotorInfo(sliderspeed6.value,'1' ,'0' ,'0' ,(10*sliderspeed7.value),comboBox1.currentIndex+1 );
            }
            else if(datanum == 2)
            {
                myclassExposeByRegType.setCarMotorInfo(sliderspeed6.value,'2' ,'0' ,'0' ,(10*sliderspeed7.value),comboBox1.currentIndex+1 );
            }
            else if(datanum == 3)
            {
                myclassExposeByRegType.setCarMotorInfo(sliderspeed6.value,'3' ,'0' ,'0' ,(10*sliderspeed7.value),comboBox1.currentIndex+1 );
            }
            else
            {
                 myclassExposeByRegType.setCarMotorInfo(sliderspeed6.value,'0' ,'0' ,'0' ,(10*sliderspeed7.value),comboBox1.currentIndex+1 );
            }
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
//-------------------------------------------------设置Kp参数
    Label {
        id: label13
        x: 437
        y: 691
        width: 99
        height: 28
        text: qsTr("Kp = 0") //+ sliderspeed5.value.toFixed(1).toLocaleString()
        //sliderspeed5.value.toFixed(1).toLocaleString()
        font.pixelSize: 30
        font.bold: true

//        TextInput {
//            id: textEdit_set_RPM_X1
//            x: 568
//            y: 475
//            width: 64
//            height: 36
//            text: qsTr("0")
//            clip: false
//            font.bold: true
//            font.pixelSize: 30
//            focus: true
//            validator: IntValidator {
//                top: 85
//                bottom: -85
//            }
//        }
    }

//    Slider {
//        id: sliderspeed5
//        x: 185
//        y: 582
//        width: 591
//        height: 40
//        from: 0
//        to: 5
//        stepSize: 0.1
//        value: 0
//    }
//----------------------------------------------------------
    //===============================================显示里程计值
    Label {
        id: label6
        x: 1132
        y: 314
        width: 136
        height: 33
        text: qsTr("里程计(mm)：")
        font.bold: true
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: 30
    }

    Label {
        id: label11
        x: 1349
        y: 317
        width: 230
        height: 28
        text: "0"
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 30
    }

    Label {
        id: label14
        x: 185
        y: 319
        width: 431
        height: 28
        text: qsTr("设置电机转速（RPM）：") + sliderspeed6.value //+qsTr("RPM");
//        TextInput {
//            id: textEdit_set_RPM_X2
//            x: 568
//            y: 475
//            width: 64
//            height: 36
//            text: +("0")
//            clip: false
//            font.bold: true
//            font.pixelSize: 30
//            focus: true
//            validator: IntValidator {
//                top: 19000
//                bottom: 0
//            }
//        }
        font.bold: true
        font.pixelSize: 30
    }

    Slider {
        id: sliderspeed6
        x: 185
        y: 362
        width: 591
        height: 40
        from: 10000
        stepSize: 100
        to: 20000
        value: 0
    }

    Label {
        id: label15
        x: 185
        y: 417
        width: 445
        height: 28
        text: qsTr("设置行走距离(m)：") + sliderspeed7.value.toFixed(1).toLocaleString()//+qsTr("M");
//text: qsTr("电压：") + sliderspeed5.value.toFixed(1).toLocaleString() + qsTr(" V")
//        TextInput {
//            id: textEdit_set_RPM_X3
//            x: 568
//            y: 475
//            width: 64
//            height: 36
//            text: qsTr("0")
//            clip: false
//            font.bold: true
//            font.pixelSize: 30
//            focus: true
//            validator: IntValidator {
//                top: 10
//                bottom: 0
//            }
//        }
        font.bold: true
        font.pixelSize: 30
    }

    Slider {
        id: sliderspeed7
        x: 185
        y: 462
        width: 591
        height: 40
        from: 0
        stepSize: 0.1
        to: 10
        value: 0
    }

    Button {
        id: button5
        x: 175
        y: 561
        width: 190
        height: 47
        text: qsTr("数据一")
        font.bold: true
        font.pointSize: 22
        onClicked: {
            label3.text=qsTr("Kp = "+1.5)
            label13.text=qsTr("Ki = "+0)
            label5.text=qsTr("Kd = "+0)
//            myclassExposeByRegType.sendCMD("33", "001000000000", "0300000000000000");//发送按键1
            datanum = 1;
        }
    }

    Button {
        id: button6
        x: 405
        y: 561
        width: 190
        height: 47
        text: qsTr("数据二")
        font.bold: true
        font.pointSize: 22
        onClicked: {
            label3.text=qsTr("Kp = "+1.5)
            label13.text=qsTr("Ki = "+0.1)
            label5.text=qsTr("Kd = "+0)
//            myclassExposeByRegType.sendCMD("33", "002000000000", "0300000000000000");//发送按键2
            datanum = 2;
        }
    }
    Rectangle {
        x:246
        y:661
        width: 484
        height: 89
        color: "#00000000"
        border.width: 3

    }
    Button {
        id: button8
        x: 636
        y: 561
        width: 190
        height: 47
        text: qsTr("数据三")
        font.bold: true
        font.pointSize: 22
        onClicked: {
            label3.text=qsTr("Kp = "+1.5)
            label13.text=qsTr("Ki = "+0.1)
            label5.text=qsTr("Kd = "+1)
//            myclassExposeByRegType.sendCMD("33", "003000000000", "0300000000000000");//发送按键3
            datanum = 3;
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



}
