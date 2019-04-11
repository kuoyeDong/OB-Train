import QtQuick 2.9
import QtQuick.Controls 2.2

import QtQuick.Window 2.2

import QtMultimedia 5.4
import ".."
import "./PresentPage"
import "../Component"
//import "./PresentPage/FaceUseGuide.qml"


import RegisterMyType 1.0
import RegisterSystemOpen 1.0
import RegisterFaceName 1.0

Page {

    property StackView stack: null
    visible: true

    background: Image {
        source: "../images/background.png"
    }

    MySaveFaceName
    {
        id:saveLocalName
    }


    Label{
        id: label
        y: 150
        text: qsTr("人脸识别")
        anchors.verticalCenterOffset: -453
        anchors.horizontalCenterOffset: 1
        height: 62
        width: 241
        anchors.centerIn: parent
        font.pixelSize: 60
    }

    MyClassType
    {
        id:myclassExposeByRegType
    }

    MySystemOpen
    {
        id:mySystemOpenReg
    }

    Button {
        id: button
        x: 150
        y: 200
        width: 200
        text: qsTr("简介")
        font.bold: true
        font.pointSize: 20
        MouseArea{
            id:mouseArea
            anchors.fill: parent
            onPressed: {
                page1.visible = true;
                page1.stack = stack;
                stack.push(page1);
            }
        }
    }
    Button {
        id: button1
        x: 150
        y: 300
        width: 200
        text: qsTr("使用教程")
        font.bold: true
        font.pointSize: 20
        onClicked:
        {
            useguidepage.show();
        }
//        MouseArea{
//            id:mouseAreaUseGuide
//            anchors.rightMargin: 0
//            anchors.bottomMargin: 0
//            anchors.leftMargin: 0
//            anchors.topMargin: 0
//            anchors.fill: parent
//            onPressed: {
//                page3.visible = true;
//                page3.stack = stack;
//                stack.push(page3);
//            }
//        }
    }
    FaceUseGuide
    {
        id:useguidepage
    }


    Button {
        id: button2
        x: 150
        y: 400
        width: 200
        text: qsTr("启动人脸检测 ")
        font.bold: true
        font.pointSize: 20
        MouseArea{
            id:mouseProjectArea
            anchors.fill: parent
            onPressed: {
                if(CodeImage_Face.isOpened())
                {
                    count.running=true;
                    CodeImage_Face.openFaceDet();
                }
            }
        }
    }

    Button {
        id: button3
        x: 150
        y: 500
        width: 200
        text: qsTr("启动人脸识别 ")
        font.bold: true
        font.pointSize: 20
        MouseArea{
            id:mouseProjectArea2
            anchors.fill: parent
            onPressed: {
                if(CodeImage_Face.isOpened())
                {
                    count.running=true;
                    CodeImage_Face.openFaceRec();
                }
            }
        }
    }
	Button {
        id: button4
        x: 150
        y: 600
        width: 200
        text: qsTr("启动人脸变换 ")
        font.bold: true
        font.pointSize: 20
        MouseArea{
            id:mouseProjectArea3
            anchors.fill: parent
            onPressed: {
                if(CodeImage_Face.isOpened())
                {
                    count.running=true;
                    CodeImage_Face.setCameraOpen();
                    facechangepage.show();
                    //CodeImage_Face.openFaceChange();
                }
            }
        }
    }
    FaceChangePage
    {
        id:facechangepage
    }

    Button {
        id: button5
        x: 150
        y: 700
        width: 200
        text: qsTr("启动人脸注册 ")
        font.bold: true
        font.pointSize: 20
        onClicked:
        {
            if(CodeImage_Face.isOpened())
            {
                count.running=true;
                CodeImage_Face.setCameraOpen();
                faceregisterpage.show();
            }
        }

    }
    FaceRegisterPage
    {
        id:faceregisterpage
    }

    FacePresent {
        id: page1
        visible: false
        stack: stack
    }
    VideoPlayerDBW {
        id: page2
        visible: false
        stack: stack
    }
    UseGuide {
        id: page3
        visible: false
        stack: stack
    }

//    Rectangle {
//        id: rec
//        x: 500;
//        y: 200;
//        width:1260;
//        height:707;
//        color: "black";
//        Text
//        {
//            //文本
//            text: "请打开摄像头"
//            //字体
//            font.family: "Helvetica"
//            //字大小
//            font.pointSize: 30
//            //颜色
//            color: "white"
//        }
//    }

   Image
   {
       id:img;
       x: 500;
       y: 200;
       width:1260;
       height:707;
       visible: true
       source:"./images/connection_1.png";
       cache:false;
   }
   Timer
   {
      id:count;
      interval: 5;
      running: false;
      repeat: true
      onTriggered: {
      CodeImage_Face.setImage();
      }
   }

   Button
   {
       id: button6
       x: 500
       y: 150
       width: 180
       text: qsTr("打开摄像头 ")
       font.bold: true
       font.pointSize: 20
       MouseArea
       {
           anchors.fill: parent
           onPressed:
           {
               if(!CodeImage_Face.isOpened())
               {
                   count.running=true;
                   CodeImage_Face.camera_open();
                   CodeImage_Face.setCameraOpen();
               }
//               count.running=true;
//               CodeImage_Face.camera_open();
//               CodeImage_Face.setCameraOpen();
           }
       }
   }
   Button
   {
       id: button7
       x: 700
       y: 150
       width: 180
       text: qsTr("关闭摄像头 ")
       font.bold: true
       font.pointSize: 20
       MouseArea
       {
           anchors.fill: parent
           onPressed:
           {
               count.running=false;
               if(CodeImage_Face.isOpened())
               {
                   CodeImage_Face.setAlgorithmStop();
                   CodeImage_Face.setFaceDetStop();
                   CodeImage_Face.setCameraClose();
                   CodeImage_Face.camera_close();
                   img.source="./images/connection_1.png";
               }
//               CodeImage_Face.setFaceDetStop();
//               CodeImage_Face.camera_close();
//               img.source="";
           }
       }
   }



   Connections
   {
       target: CodeImage_Face
       onCallQmlRefeshImg:
       {
           if(0 === CodeImage_Face.connectStatus())
           {
               //img_tracking.source = "";
               img.source = "./images/connection_2.png";
           }
           else
           {
               img.source = "";
               img.source = "image://CodeImg_Face";
               console.log("no");
           }
//           img.source = "";
//           img.source = "image://CodeImg_Face";
//           console.log("no");
       }
   }

    Button
    {
        height: 32
        width: 120
        text: "<-"
        anchors.left: parent.left
        anchors.top: parent.top
//        onPressed:
//        {
//            count.running=false;
//            CodeImage_Face.setCameraClose();
//            CodeImage_Face.camera_close();
//            img.source="";
//        }
        onClicked:
        {
            count.running=false;
            if(CodeImage_Face.isOpened())
            {
                CodeImage_Face.setAlgorithmStop();
                CodeImage_Face.setCameraClose();
                CodeImage_Face.camera_close();
                img.source="./images/connection_1.png";
            }
            stack.pop();
        }
    }


}

