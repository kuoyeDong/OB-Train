import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2

//import QtQuick.Scene3D 2.0

import "../Component"
import "."
import "../Sensor/DroneSensor/UartData"
import "../Sensor/DroneSensor/MagPage"
import"./Doubler"

import RegisterMyType 1.0
import RegisterDataFresh 1.0

Page {


    width: 1920
    height: 1080
    font.pointSize: 20

    property StackView stack: null

//    property int timerprintcnt: 0
//    property int timerprinttimeout: 30
//    property int functionchoice: 1
    property int cnt_timer: 0
    property bool timer_set_lab_flag: false
    property int cnt_timer_step: 5

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
            if(timer_set_lab_flag === true)
            {
                timer_set_lab.running = false;
                timer_set_lab_flag = false;
                button.text = "运行演示";
            }
            myclassExposeByRegType.setCurrentPage(0);//清除页面设置
            stack.pop();
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

    Connections {
        target: qmlToggleButton
        onToggleRight: {
            myclassExposeByRegType.closePort();
            if(timer_set_lab_flag === true)
            {
                timer_set_lab.running = false;
                timer_set_lab_flag = false;
                button.text = "运行演示";
            }

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

    CompassUiBig
    {
        id: compassui
        x: 160
        y: 142
        width: 700
        height: 700
    }

    IMU2D
    {
        id: imu2d
        x: 1029
        y: 142
        width: 700
        height: 700
    }

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
    Timer {

        id:timer_set_lab

        interval: 1000
//        running: true
        repeat: true
        triggeredOnStart: false
        onTriggered: {

            cnt_timer+=1;
            if(cnt_timer==(1* cnt_timer_step))
            {
                myclassExposeByRegType.setMagCorner(270, 0);
                sliderspeed3.value = 270;
                sliderspeed2.value = 0;
            }
            else if(cnt_timer==(2* cnt_timer_step))
            {
                myclassExposeByRegType.setMagCorner(180, 0);
                sliderspeed3.value = 180;
                sliderspeed2.value = 0;
            }
            else if(cnt_timer==(3* cnt_timer_step))
            {
                myclassExposeByRegType.setMagCorner(90, 0);
                sliderspeed3.value = 90;
                sliderspeed2.value = 0;
            }
            else if(cnt_timer==(4* cnt_timer_step))
            {
                myclassExposeByRegType.setMagCorner(0, 0);
                sliderspeed3.value = 0;
                sliderspeed2.value = 0;
            }
            else if(cnt_timer==(5* cnt_timer_step))
            {
                myclassExposeByRegType.setMagCorner(0, 45);
                sliderspeed3.value = 0;
                sliderspeed2.value = 45;
            }
            else if(cnt_timer==(6* cnt_timer_step))
            {
                myclassExposeByRegType.setMagCorner(90, 45);
                sliderspeed3.value = 90;
                sliderspeed2.value = 45;
            }
            else if(cnt_timer==(7* cnt_timer_step))
            {
                myclassExposeByRegType.setMagCorner(180, 45);
                sliderspeed3.value = 180;
                sliderspeed2.value = 45;
            }
            else if(cnt_timer==(8* cnt_timer_step))
            {
                myclassExposeByRegType.setMagCorner(270, 45);
                sliderspeed3.value = 270;
                sliderspeed2.value = 45;
            }
            else if(cnt_timer==(9* cnt_timer_step))
            {
                myclassExposeByRegType.setMagCorner(270, 70);
                sliderspeed3.value = 270;
                sliderspeed2.value = 70;
            }
            else if(cnt_timer==(10* cnt_timer_step))
            {
                myclassExposeByRegType.setMagCorner(180, 70);
                sliderspeed3.value = 180;
                sliderspeed2.value = 70;
            }
            else if(cnt_timer==(11* cnt_timer_step))
            {
                myclassExposeByRegType.setMagCorner(90, 70);
                sliderspeed3.value = 90;
                sliderspeed2.value = 70;
            }
            else if(cnt_timer==(12* cnt_timer_step))
            {
                myclassExposeByRegType.setMagCorner(0, 70);
                sliderspeed3.value = 0;
                sliderspeed2.value = 70;
            }
            else if(cnt_timer>=(13* cnt_timer_step))
            {
//                timer_set_lab_acc.stop();
//                accinfopage_tips.close();
//                if(mydataFresh.serialDataPrintFlag == true){
//                    mydataFresh.serialDataPrintFlag = false;
//                    button_save.enabled = true;
//                    button_print.text = qsTr("开始打印");
//                    timer_print.stop();
//                }
                cnt_timer = 0;
            }
        }
    }

    MyClassType
    {
        id:myclassExposeByRegType
    }

    RFSetPage{
        id:rfsetpage
    }




    MyDataFresh{
        id:mydataFresh
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

    Button {
        id: button
        x: 675
        y: 48
        text: qsTr("运行演示")
        font.capitalization: Font.AllUppercase
        font.pointSize: 20
        font.bold: true
        onClicked: {
            if(timer_set_lab_flag === true)
            {
                timer_set_lab.running = false;
                timer_set_lab_flag = false;
                button.text = "运行演示";
                sliderspeed3.enabled = true;
                sliderspeed2.enabled = true;
            }
            else
            {
                timer_set_lab.running = true;
                timer_set_lab_flag = true;
                button.text = "停止演示";
                cnt_timer = 0;
                sliderspeed3.enabled = false;
                sliderspeed2.enabled = false;
            }
        }

    }

    Slider {
        id: sliderspeed2
        x: 398
        y: 865
        width: 1308
        height: 40
        from: -70
        to: 70
        stepSize: 1
        value: 0
        onValueChanged: {
            myclassExposeByRegType.setMagCorner(sliderspeed3.value.toLocaleString(), sliderspeed2.value.toLocaleString());
        }
    }

    Slider {
        id: sliderspeed3
        x: 398
        y: 925
        width: 1308
        height: 40
        from: 0
        to: 270
        stepSize: 1
        value: 0
        onValueChanged: {
            myclassExposeByRegType.setMagCorner(sliderspeed3.value.toLocaleString(), sliderspeed2.value.toLocaleString());
        }
    }

    Label {
        id: label5
        x: 38
        y: 930
        width: 325
        height: 30
        text: qsTr("实验平台水平角度：") + sliderspeed3.value + qsTr(" °")
        font.pixelSize: 30
        font.bold: true
    }

    Label {
        id: label6
        x: 38
        y: 865
        width: 156
        height: 35
        text: qsTr("实验平台垂直角度：") + sliderspeed2.value + qsTr(" °")
        font.pixelSize: 30
        font.bold: true
    }

}
