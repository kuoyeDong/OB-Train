/*!
 *@file Page1.qml
 *@brief Page1
 *@version 1.0
 *@section LICENSE Copyright (C) 2003-2103 CamelSoft Corporation
 *@author zhengtianzuo
*/
import QtQuick 2.8
import QtQuick.Controls 2.1
import "."

import RegisterMyType 1.0


Page {

    StackView {
        id: stack
        initialItem: mainView
        anchors.fill: parent


    }

    Page {
        id: mainView
        width: 1920
        height: 1080

        background: Image {
            source: "../images/background.png"
        }

        Button {
            x: 313
            y: 134
            width: 250
            height: 250
            text: qsTr("移动机器人")
            display: AbstractButton.IconOnly

            background: Image {
                source: "images/Robot.png"

            }


            onClicked: {
            myclassExposeByRegType.setCurrentPage(14);//打开无刷电调界面
                page1.visible = true;
                page1.stack = stack;
                stack.push(page1)
            }
        }

        Button {
            id: button1
            x: 827
            y: 152
            width: 250
            height: 250
            text: qsTr("机械臂")
            display: AbstractButton.IconOnly

            background: Image {
                source: "images/Arm.png"
            }

            onClicked: {
                page2.visible = true;
                page2.stack = stack;
                stack.push(page2)
            }
        }

        Button {
            id: button5
            x: 1332
            y: 134
            width: 250
            height: 250
            text: qsTr("眼手协作")
            display: AbstractButton.IconOnly

            background: Image {
                source: "images/Connection.png"
            }

            onClicked: {
                page3.visible = true;
                page3.stack = stack;
                stack.push(page3)
            }
        }

        Button {
            id: button3
            x: 313
            y: 532
            width: 250
            height: 250
            text: qsTr("传感模组")
            display: AbstractButton.IconOnly

            background: Image {
                source: "images/Model.png"
            }

            onClicked: {
            myclassExposeByRegType.setCurrentPage(12);//步进电机整机界面
                page4.visible = true;
                page4.stack = stack;
                stack.push(page4)
            }
        }

        Button {
            id: button4
            x: 827
            y: 532
            width: 250
            height: 250
            text: qsTr("配件软硬接口")
            transformOrigin: Item.Center
            display: AbstractButton.IconOnly

            background: Image {
                source: "images/Interface.png"
            }

            onClicked: {
            myclassExposeByRegType.setCurrentPage(13);//直流电机无人机整机界面
                page5.visible = true;
                page5.stack = stack;
                stack.push(page5)
            }
        }



        Button {
            id: button2
            x: 1332
            y: 526
            width: 250
            height: 250
            text: qsTr("APP实现")
            display: AbstractButton.IconOnly

            background: Image {
                source: "images/APP.png"
            }

            onClicked: {
                page6.visible = true;
                page6.stack = stack;
                stack.push(page6)
            }
        }

        Text {
            id: element2
            x: 358
            y: 797
            text: qsTr("轴臂电机")
            font.pixelSize: 40
            font.bold: true
        }

        Text {
            id: element3
            x: 338
            y: 396
            text: qsTr("机器车电机")
            font.pixelSize: 40
            font.bold: true
        }

        Text {
            id: element4
            x: 872
            y: 788
            text: qsTr("有刷电机")
            font.pixelSize: 40
            font.bold: true
        }

        Text {
            id: element5
            x: 1387
            y: 788
            text: qsTr("APP控制")
            font.pixelSize: 40
            font.bold: true
        }

        Text {
            id: element6
            x: 892
            y: 396
            text: qsTr("机械臂")
            font.pixelSize: 40
            font.bold: true
        }

        Text {
            id: element7
            x: 1377
            y: 390
            text: qsTr("手眼协作")
            font.pixelSize: 40
            font.bold: true
        }


    }

    MyClassType
    {
        id:myclassExposeByRegType
    }

//    Robot {
//        id: page1
//        visible: false
//    }
    Carmotor {
        id: page1
        visible: false
    }
//    MovingRobot {
//        id: page1
//        visible: false
//    }

    Arm {
        id: page2
        visible: false
    }

    Connection {
        id: page3
        visible: false
    }

//    Model {
//        id: page4
//        visible: false
//    }

    BDCmotor{
        id: page5
        visible: false
    }

    APP {
        id: page6
        visible: false
    }

    Stepmotor{
        id:page4
        visible: false
    }
}







/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
