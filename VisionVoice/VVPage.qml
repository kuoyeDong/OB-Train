/*!
 *@file Page1.qml
 *@brief Page1
 *@version 1.0
 *@section LICENSE Copyright (C) 2003-2103 CamelSoft Corporation
 *@author zhengtianzuo
*/
import QtQuick 2.8
import QtQuick.Controls 2.1


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
            text: qsTr("人脸解锁")
            display: AbstractButton.IconOnly

            background: Image {
                source: "images/FaceUnlock.png"
            }

            onClicked: {
                page1.visible = true;
                page1.stack = stack;
                stack.push(page1)
                myclassExposeByRegType.setCurrentPage(8);//打开人脸识别界面
            }
        }

        Button {
            id: button1
            x: 827
            y: 134
            width: 250
            height: 250
            text: qsTr("姿态人型控制")
            display: AbstractButton.IconOnly

            background: Image {
                source: "images/PersonControl.png"

            }


            onClicked: {
                page2.visible = true;
                page2.stack = stack;
                stack.push(page2)
                myclassExposeByRegType.setCurrentPage(9);//打开人形跟随界面
            }
        }

        Button {
            id: button5
            x: 1332
            y: 134
            width: 250
            height: 250
            text: qsTr("语音识别播放")
            display: AbstractButton.IconOnly

            background: Image {
                source: "images/posture_logo.png"
            }

            onClicked: {
                page3.visible = true;
                page3.stack = stack;
                stack.push(page3)
                myclassExposeByRegType.setCurrentPage(10);//打开姿态识别界面
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
            id: element
            x: 358
            y: 788
            text: qsTr("传感模组")
            font.bold: true
            font.pixelSize: 40
        }

        Text {
            id: element1
            x: 358
            y: 390
            text: qsTr("人脸解锁")
            font.bold: true
            font.pixelSize: 40
        }

        Text {
            id: element2
            x: 880
            y: 390
            text: qsTr("人形跟随")
            font.pixelSize: 40
            font.bold: true
        }

        Text {
            id: element3
            x: 840
            y: 788
            text: qsTr("配件软硬接口")
            font.pixelSize: 40
            font.bold: true
        }

        Text {
            id: element4
            x: 1387
            y: 788
            text: qsTr("APP实现")
            font.pixelSize: 40
            font.bold: true
        }

        Text {
            id: element5
            x: 1377
            y: 390
            text: qsTr("姿态识别")
            font.pixelSize: 40
            font.bold: true
        }


    }

    FaceGuidePage {
        id: page1
        visible: false
    }

    PersonGuidePage {
        id: page2
        visible: false
    }

    GestureGuidePage{
        id: page3
        visible: false
    }

//    Voice {
//        id: page3
//        visible: false
//    }

    Model {
        id: page4
        visible: false
    }

    Interface{
        id: page5
        visible: false
    }

    APP {
        id: page6
        visible: false
    }
}







/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
