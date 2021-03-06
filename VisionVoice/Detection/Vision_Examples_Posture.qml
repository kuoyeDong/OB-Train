﻿import QtQuick 2.9
import QtQuick.Controls 2.2
import "../../Component"
import "./"
import "./Introduction/KCFtracking"

import RegisterMyType 1.0
import RegisterSystemOpen 1.0

Page {
    id: page

    property StackView stack: null

    background: Image {
        source: "../../images/background.png"
    }

    MySystemOpen
    {
        id:system_open_visual_program
    }



    Label{
        id: label
        width: 410
        text: qsTr("手势姿态识别算法实验")
        anchors.left: back.right
        anchors.leftMargin: -30
        anchors.top: back.bottom
        anchors.topMargin: 71
        height: 55
        font.pixelSize: 50
    }

    Button {
        id: camera_connect
        width: 190
        height: 80
        text: qsTr("连接图传")
        anchors.left: label.left
        anchors.leftMargin: 0
        anchors.top: label.bottom
        anchors.topMargin: 65
        font.pointSize: 18

        MouseArea{
            id:open_camera
            x: 0
            y: 0
            width: 190
            height: 80
            onPressed: {
                if(!CodeImage_Posture.isOpened())
                {
                    count.running=true;
                    CodeImage_Posture.camera_open();
                    CodeImage_Posture.setCameraOpen();
                }
            }
        }
    }

    Button {
        id: introduction
        x: 755
        y: 103
        width: 410
        height: 80
        text: qsTr("AI简介与计算机视觉")
        anchors.horizontalCenterOffset: -235
        anchors.bottom: img_tracking.top
        anchors.bottomMargin: 40
        anchors.horizontalCenter: img_tracking.horizontalCenter
        font.pointSize: 20

        onClicked: {
            //stack.pop();
            intro_page1.visible = true;
            intro_page1.stack = stack;
            stack.push(intro_page1);
        }
    }

    Button {
        id: camera_close
        width: 190
        height: 80
        text: qsTr("断开图传")
        anchors.top: label.bottom
        anchors.topMargin: 65
        font.pointSize: 18
        anchors.left: camera_connect.right
        anchors.leftMargin: 30

        MouseArea{
            id:close_camera_area
            x: 0
            y: 0
            width: 190
            height: 80
            anchors.left: camera_connect.right
            anchors.leftMargin: 30
            onPressed: {
                count.running=false;
                if(CodeImage_Posture.isOpened())
                {
                    CodeImage_Posture.setAlgorithmStop();
                    CodeImage_Posture.setCameraClose();
                    CodeImage_Posture.camera_close();
                    img_tracking.source="../images/connection_1.png";
                }
            }
        }
    }

    Button {
        id: visual_tracking
        width: 410
        height: 110
        text: qsTr("Step1：了解现有的手势姿态")
        anchors.left: camera_connect.left
        anchors.leftMargin: 0
        font.pointSize: 18
        anchors.top: camera_connect.bottom
        anchors.topMargin: 75

        MouseArea{
            id:tracking_project
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.fill: parent
            onClicked: {
                pagePos_Teaching.visible = true;
                pagePos_Teaching.stack = stack;
                stack.push(pagePos_Teaching);
            }
        }
    }

//    Rectangle {
//          id: rec
//          width:1260;
//          height:707;
//          color: "black";
//          anchors.top: parent.top
//          anchors.topMargin: 223
//          anchors.left: camera_close.right
//          anchors.leftMargin: 81
//          Text
//          {
//              //文本
//              text: "请打开摄像头"
//              //字体
//              font.family: "Helvetica"
//              //字大小
//              font.pointSize: 30
//              //颜色
//              color: "white"
//          }
//      }

    Image{
           id:img_tracking;
           width:1260
           height:707
           anchors.top: parent.top
           anchors.topMargin: 223
           anchors.left: camera_close.right
           anchors.leftMargin: 81
           visible: true
           source:"../images/connection_1.png";
           cache:false;
    }

    Timer{
          id:count;
          interval: 5;
          running: false;
          repeat: true
          onTriggered: {
          CodeImage_Posture.setImage();
          }
    }

    Button {
        id:back
        height: 32
        width: 120
        text: "<-"
        anchors.left: parent.left
        anchors.top: parent.top
        onClicked: {
            count.running=false;
            if(CodeImage_Posture.isOpened())
            {
                CodeImage_Posture.setAlgorithmStop();
                CodeImage_Posture.setCameraClose();
                CodeImage_Posture.camera_close();
                img_tracking.source="../images/connection_1.png";
            }
            stack.pop();
        }
    }

    Connections{
           target: CodeImage_Posture
           onCallQmlRefeshImg:{
               if(0 === CodeImage_Posture.connectStatus())
               {
                   //img_tracking.source = "";
                   img_tracking.source = "../images/connection_2.png";
               }
               else
               {
                   img_tracking.source = "";
                   img_tracking.source = "image://CodeImg_Posture";
                   console.log("no");
               }
//                   img_tracking.source = "";
//                   img_tracking.source = "image://CodeImg_Posture";
//                   console.log("no");
           }
    }

    Button {
        id: button
        width: 410
        height: 110
        text: qsTr("Step2：开启手势姿态识别算法，
并做出相应手势姿态")
        font.pointSize: 17
        anchors.left: camera_connect.left
        anchors.leftMargin: 0
        anchors.top: visual_tracking.bottom
        anchors.topMargin: 75
        MouseArea{
            id:start
            width: 410
            height: 110
            onPressed: {
                if(CodeImage_Posture.isOpened())
                {
                    count.running=true;
                    CodeImage_Posture.openPosture();
                }
            }
        }
    }

     Button {
         id: button1
         width: 410
         height: 110
         text: qsTr("Step3：停止手势姿态识别")
         font.pointSize: 18
         anchors.left: camera_connect.left
         anchors.leftMargin: 0
         anchors.top: button.bottom
         anchors.topMargin: 75
         MouseArea{
             id:stop
             width: 410
             height: 110
             onPressed: {
                 CodeImage_Posture.closePosture();
                 CodeImage_Posture.setAlgorithmStop();
             }
         }
     }


     Button {
         id: tracking_video
         x: 1431
         y: 103
         width: 410
         height: 80
         text: qsTr("无人机手势姿态识别示例")
         anchors.horizontalCenterOffset: 297
         anchors.bottom: img_tracking.top
         anchors.bottomMargin: 40
         anchors.horizontalCenter: img_tracking.horizontalCenter
         font.pointSize: 20

         onClicked: {
             system_open_visual_program.openFile("/Content_resource/Posture/video/Posture.MOV");
         }
     }

     IntroductionPage1{
         id:intro_page1
         visible: false
         stack: stack
     }

     Posture_Teaching{
         id:pagePos_Teaching
         visible: false
         stack: stack
     }

}















































































/*##^## Designer {
    D{i:0;autoSize:true;height:1080;width:1920}D{i:5;anchors_y:108}D{i:4;anchors_x:140;anchors_y:271}
D{i:6;anchors_y:108}D{i:7;anchors_y:271}D{i:10;anchors_y:108}D{i:9;anchors_x:140;anchors_y:108}
D{i:12;anchors_x:300;anchors_y:130}D{i:11;anchors_x:595;anchors_y:271}D{i:15;anchors_x:140;anchors_y:592}
D{i:17;anchors_x:140}
}
 ##^##*/
