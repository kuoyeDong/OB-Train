import QtQuick 2.9
import QtQuick.Controls 2.2
import QtWebEngine 1.1
import QtWebSockets 1.0
import QtWebChannel 1.0
import QtWebView 1.1
import io.decovar.WebSocketTransport 1.0
import RegisterJsApiObject 1.0

import QtQuick.Window 2.10
import Qt.labs.folderlistmodel 2.1
import QtQuick.Dialogs 1.2

import ".."

Page {
    id: page
    width: 1920
    height: 1080

    property StackView stack: null

    background: Image {
        source: "../../images/background.png"
    }


    Button {
        height: 32
        width: 120
        text: "<-"
        anchors.left: parent.left
        anchors.top: parent.top
        onClicked:
        {
            stack.pop()
            count.running=false;
            imgsrc.source = ""
            imgdst.source = ""
            ExpFaceImage.closeFaceExp2();
        }
    }

    Label {
        id: title
        x: 719
        width: 922
        height: 56
        text: qsTr("实验二  卷积神经网络（CNN）与特征提取")
        anchors.horizontalCenterOffset: 0
        anchors.top: parent.top
        anchors.topMargin: 45
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 50
    }

    WebEngineView
    {
        id: webView
        x: 0
        y: 180
        width: 1360;
        height: 720;
        url: "../../module/ConvNetDraw/index.html"
        smooth: true
        visible: true
        onNewViewRequested: request.openIn(webView)
    }

    Rectangle
    {
        id: rectangle
        x: 1392
        y: 180
        width: 230
        height: 230
        color: "#000000"
        Text
        {
            //文本
            text: "输入图像"
            //字体
            anchors.centerIn: rectangle
            font.family: "Helvetica"
            //字大小
            font.pointSize: 20
            //颜色
            color: "white"
        }
    }

    Rectangle
    {
        id: rectangle1
        x: 1652
        y: 180
        width: 230
        height: 230
        color: "#000000"
        Text
        {
            //文本
            text: "输出图像"
            //字体
            anchors.centerIn: rectangle1
            font.family: "Helvetica"
            //字大小
            font.pointSize: 20
            //颜色
            color: "white"
        }
    }



    Image
    {
        id:imgsrc;
        x: 1392
        y: 180
        width: 230
        height: 230
        visible: true
        source: fileDialog.fileurl
        cache:false;
    }

    Image
    {
        id:imgdst;
        x: 1652
        y: 180
        width: 230
        height: 230
        visible: true
        source: fileDialog.fileurl
        cache:false;
    }

    Timer{
        id:count;
        interval: 5;
        running: false;
        repeat: true;
        onTriggered: {
            ExpFaceImage.setImage();
        }
    }

    Connections{
        target: ExpFaceImage
        onCallQmlRefeshImg:{
            if(0 === ExpFaceImage.connectStatus())
            {
                imgdst.source = "image://ExpFaceImg";
            }
            else
            {
                imgdst.source = "";
            }

        }
    }

    FileDialog
    {
        id: fileDialog
        title: qsTr("请选择一张图片")
        folder: shortcuts.desktop
        //nameFilters: ["Photo Files", "Image Files (*.jpg *.png *.gif *.bmp *.ico)", "*.*"]
        nameFilters: ["Image Files (*.jpg *.png)", "*.*"]


        onAccepted:
        {
            imgsrc.source = fileDialog.fileUrl
        }
    }

    Button {
        id: button
        x: 1467
        y: 440
        width: 340
        height: 60
        text: qsTr("请选择一张图像")
        leftPadding: 10
        padding: 5
        font.weight: Font.Light
        font.wordSpacing: -0.1
        font.pointSize: 25
        onClicked:
        {
            imgsrc.source = "";
            imgdst.source = "";
            fileDialog.open()
            count.running=false;
        }
    }

    Label {
        id: label
        x: 1392
        y: 529
        width: 137
        height: 50
        text: qsTr("卷积核1：")
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 25
    }

    ComboBox {
        id: comboBox
        x: 1535
        y: 528
        width: 162
        height: 50
        font.bold: true
        font.pointSize: 20
        model: ["Sobel_H", "Sobel_V", "Laplace"]
    }

    TextField {
        id: textField
        x: 1703
        y: 530
        width: 80
        height: 50
        text: qsTr("0")
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 25
    }

    Label {
        id: label1
        x: 1789
        y: 530
        width: 80
        height: 50
        text: qsTr("层")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 25
    }



    Label {
        id: label2
        x: 1392
        y: 610
        width: 137
        height: 50
        text: qsTr("卷积核2：")
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 25
    }

    ComboBox {
        id: comboBox1
        x: 1535
        y: 610
        width: 162
        height: 50
        padding: 0
        font.bold: true
        font.pointSize: 20
        model: ["Sobel_H", "Sobel_V", "Laplace"]
    }

    TextField {
        id: textField1
        x: 1703
        y: 610
        width: 80
        height: 50
        text: qsTr("0")
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 25
    }

    Label {
        id: label3
        x: 1789
        y: 610
        width: 80
        height: 50
        text: qsTr("层")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 25
    }



    Label {
        id: label4
        x: 1392
        y: 690
        width: 137
        height: 50
        text: qsTr("卷积核3：")
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 25
    }

    ComboBox {
        id: comboBox2
        x: 1535
        y: 690
        width: 162
        height: 50
        font.pointSize: 20
        font.bold: true
        model: ["Sobel_H", "Sobel_V", "Laplace"]
    }

    TextField {
        id: textField2
        x: 1703
        y: 690
        width: 80
        height: 50
        text: qsTr("0")
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 25
    }

    Label {
        id: label5
        x: 1789
        y: 690
        width: 80
        height: 50
        text: qsTr("层")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 25
    }



    Button {
        id: button1
        x: 1467
        y: 770
        width: 340
        height: 60
        text: qsTr("输出特征图像")
        spacing: 5
        focusPolicy: Qt.WheelFocus
        leftPadding: 10
        padding: 5
        font.weight: Font.Light
        font.pointSize: 25
        font.wordSpacing: 0
        onClicked:
        {
            var len = String(fileDialog.fileUrl);
            console.log("length of string(path): ", len.match("f"))
            count.running=true;
            imgdst.source = "";
            if(len.match("f"))
            {
                ExpFaceImage.getParamsForFaceExp2(fileDialog.fileUrl,
                                                  textField.text,
                                                  textField1.text,
                                                  textField2.text,
                                                  comboBox.currentText,
                                                  comboBox1.currentText,
                                                  comboBox2.currentText);
                ExpFaceImage.openFaceExp2();
            }

            //count.running=false;
        }
    }



}
