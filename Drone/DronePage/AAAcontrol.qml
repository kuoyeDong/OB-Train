import QtQuick 2.9
import QtQuick.Controls 2.2

import RegisterMyType 1.0


Item {
    id: aaaCongrolPage
    width: 600
    height: 150
    antialiasing: false
    z: 0
    rotation: 0
    transformOrigin: Item.Center

//        title: "电子罗盘-实验操作"


    Label{
        id: label
        text: qsTr("X轴控制")
        font.bold: true
        anchors.verticalCenterOffset: -61
        anchors.horizontalCenterOffset: -219
        height: 30
        width: 110
        anchors.centerIn: parent
        font.pixelSize: 30
    }

    Label {
        id: label1
        x: 24
        y: 83
        width: 112
        height: 35
        font.pixelSize: 30
        text: qsTr("Z轴控制")
        font.bold: true
    }

    Button {
        id: button
        x: 446
        y: 19
        text: qsTr("复位")
        font.bold: true
        font.pointSize: 22
        onClicked:
        {
            slider.value = 0;
            slider1.value = 0;
//                myclassExposeByRegType.sendto("ff5580808080080027000000000000000000");
            myclassExposeByRegType.sendCMD("27", "808080800800", "0000000000000000");
        }
//                MouseArea{
//            id:mouseResetArea
//            anchors.fill: parent
//            onPressed: {
//                slider.value = 0;
//                slider1.value = 0;
////                myclassExposeByRegType.sendto("ff5580808080080027000000000000000000");
//                myclassExposeByRegType.sendCMD("27", "808080800800", "0000000000000000");
//            }
//        }
    }

    Label {
        id: label2
        x: 334
        y: 24
        width: 49
        height: 30
        font.pixelSize: 30
        text: slider1.value + qsTr(" °")
    }

    Label {
        id: label3
        x: 343
        y: 85
        width: 51
        height: 30
        font.pixelSize: 30
        text: slider.value + qsTr(" °")
    }

    Slider {
        id: slider1
        x: 131
        y: 19
        stepSize: 1
        to: 90
        from: -90
        value: 0

        onValueChanged: {
            myclassExposeByRegType.setMagCorner(slider.value,value);

        }
    }

    Slider {
        id: slider
        x: 133
        y: 80
        width: 207
        height: 40
        stepSize: 1
        to: 360
        value: 0

        onValueChanged: {
            myclassExposeByRegType.setMagCorner(value,slider1.value);
        }
    }

    Timer {

        id:timera

        property int timett: 44

        interval: 1000
//        running: true
        repeat: true
        triggeredOnStart: false

        onTriggered: {
            timett--;
            if(timett < 0)
            {
                mymagcalibrationpage.timet = 44;
                timera.stop();
                mymagcalibrationpage.close();
            }
            else
            {
                mymagcalibrationpage.timet = timett;
            }

//            console.info("close");
        }
    }

    MyClassType
    {
        id:myclassExposeByRegType
    }



}
