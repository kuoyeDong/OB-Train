import QtQuick 2.9
import QtQuick.Controls 2.2

Item {
    id: imu2dpage

    width: 700
    height: 700

//    function setimuBearing(headingIn)
//    {
//        imu2dimage.angle = headingIn
//    }

    Image {
        id: imu2dimage

        x: 300
        y: 300
        width: 100
        height: 100

        source: "../../Component/qipao.png"

        transform: Rotation {
            id: imuRotation
            angle: 0
            origin.x: imu2dimage.width / 2; origin.y: imu2dimage.height / 2;
//                Behavior on angle {
//                    SpringAnimation { spring: 2; damping: 0.2; modulus: 360 }
////                    SpringAnimation { spring: 2; damping: 0.2; modulus: 360 }
//                }
        }
    }

    Image {
        id: imubackground

        x: 50
        y: 50
        width: 600
        height: 600
        source: "../../Component/timg.png"
    }

    Text {
        id: element2
        x: 433
        y: 656
        text: qsTr("Y轴夹角：")
        font.pixelSize: 40
        font.bold: true
    }

    Text {
        id: rolltext
        x: 607
        y: 656
        text: qsTr("0°")
        font.pixelSize: 40
        font.bold: true
    }

    Text {
        id: element
        x: 15
        y: 656
        text: qsTr("X轴夹角：")
        font.pixelSize: 40
        font.bold: true
    }

    Text {
        id: pitchtext
        x: 192
        y: 656
        text: qsTr("0°")
        font.pixelSize: 40
        font.bold: true
    }

    Timer{
        interval: 100
        repeat: true
        triggeredOnStart: false
        running: true

        onTriggered: {
//            imu2dimage.x = 175 + myclassExposeByRegType.getOffsetX();
//            imu2dimage.y = 175 + myclassExposeByRegType.getOffsetY();
//            pitchtext.text = myclassExposeByRegType.getAnglePitch();
//            rolltext.text = myclassExposeByRegType.getAngleRoll();
            if(myclassExposeByRegType.getCurrentPage() === 15)
            {
                //使用三轴加速计得到水平两轴的倾角
                imu2dimage.x = 300 - 2 * myclassExposeByRegType.getOffsetX_Acc();
                imu2dimage.y = 300 + 2 * myclassExposeByRegType.getOffsetY_Acc();
                pitchtext.text = myclassExposeByRegType.getAccAngleX();
                rolltext.text = myclassExposeByRegType.getAccAngleY();
            }

        }
    }

}
