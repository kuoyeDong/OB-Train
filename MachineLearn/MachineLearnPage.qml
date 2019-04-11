﻿/*!
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
            id:button1
            x: 313
            y: 134
            width: 250
            height: 250
            text: qsTr("模型构建")

            background: Image {
                source: "images/ModelGenerate.png"
            }

            onClicked: {
                page1.visible = true;
                page1.stack = stack;
                stack.push(page1)
            }
        }

        Button {
            id: button2
            x: 827
            y: 134
            width: 250
            height: 250
            text: qsTr("数据训练集")

            background: Image {
                source: "images/DataTrain.png"

            }

            onClicked: {
                page2.visible = true;
                page2.stack = stack;
                stack.push(page2)
            }
        }

        Button {
            id: button3
            x: 1332
            y: 134
            width: 250
            height: 250
            text: qsTr("训练验证")

            background: Image {
                source: "images/TrainVerify.png"
            }

            onClicked: {
                page3.visible = true;
                page3.stack = stack;
                stack.push(page3)
            }
        }


    }

    VisionExperiment{
        id: page1
        visible: false
    }

    DataTrain{
        id: page2
        visible: false
    }


    TrainVerify {
        id: page3
        visible: false
    }

}




