import QtQuick 2.9
import QtQuick.Controls 2.2

//import RegisterMyType 1.0
import RegisterFaceName 1.0

ApplicationWindow {
    id: faceregisterpage

    x: 350
    y: 200

    width: 400
    height: 200

    title: "人脸注册"

    background: Image {
        source: "../images/background.png"
    }

    Label {
        id: label0
        x: 0
        y: 60
        width: 200
        text: qsTr("请输入注册人名：")
        font.bold: true
        font.pixelSize: 20
    }


    Label {
        id: label1
        width: 200
        text: qsTr("请您正对摄像头")
        font.bold: true
        //anchors.top: parent
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 20
    }


    TextInput {
        id: textEdit_Name
        x: 205
        y: 60
        width: 80
        height: 20
        text: qsTr("")
        font.bold: true
        font.pixelSize: 20
//        validator: IntValidator{bottom: 0; top: 255;}
        focus: true
    }

    Button {
        id: button_set_rfaddr
        x: 60
        y: 120
        text: qsTr("确认")
        font.bold: true
        font.pointSize: 20
        onClicked:
        {
            count.running=true;
            if(textEdit_Name.text.length > 0)
            {
                //textEdit_Name.text.toString();
                saveLocalName.saveName(textEdit_Name.text);
                CodeImage_Face.openFaceRegister();
                close()
            }

//            myclassExposeByRegType.setRFaddr(textEdit_set_rfaddr1.text,textEdit_set_rfaddr2.text,textEdit_set_rfaddr3.text,textEdit_set_rfch.text);
//            myclassExposeByRegType.setsettingAddrFlag(true);
//            timer1.start();
        }


    }
    Button
    {
        id: button_set_rfaddr1
        x: 240
        y: 120
        text: qsTr("返回")
        font.bold: true
        font.pointSize: 20
        onClicked:
        {
            close()
        }
    }
}
