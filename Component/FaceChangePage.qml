import QtQuick 2.9
import QtQuick.Controls 2.2

//import RegisterMyType 1.0
import RegisterFaceName 1.0

ApplicationWindow {
    id: facechangepage

    x: 350
    y: 200

    width: 400
    height: 200

    title: "人脸变换"

    background: Image {
        source: "../images/background.png"
    }

    Label {
        id: label0
        x: 0
        y: 60
        width: 200
        text: qsTr("请输入生肖名：")
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
        id: button_set_usrborn
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
                saveLocalName.returnChangePath();
                saveLocalName.getChangePath(textEdit_Name.text)
                CodeImage_Face.openFaceChange();
                close()
            }
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
