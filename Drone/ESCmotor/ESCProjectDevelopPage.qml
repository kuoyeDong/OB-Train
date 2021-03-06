﻿
import QtQuick 2.9
import QtQuick.Controls 2.2

import RegisterSystemOpen 1.0

ApplicationWindow {
    id: gyroprojectdeveloppage
    x: 0
    y: 0
    width: 480
    height: 100
    maximumHeight:100
    minimumHeight:100
    maximumWidth:480
    minimumWidth:480

    title: "旋翼电调-工程实例"

    background: Image {
        source: "../../../images/background.png"
    }

    Button {
        id: button_openproject
        x: 57
        y: 25
        text: qsTr("Demo工程")
        font.bold: true
        font.pointSize: 20

        onPressed:
        {
            mySystemOpenReg.openFile("/Content_resource/brussless_motor/Demo_Project/Drone_Templates/Projects/Project.uvprojx");
        }

    }


    MySystemOpen
    {
        id:mySystemOpenReg
    }

    Button {
        id: button_openvideo
        x: 241
        y: 25
        text: qsTr("开发操作视频")
        font.bold: true
        font.pointSize: 20

            onPressed: {
                mySystemOpenReg.openFile("/Content_resource/brussless_motor/material/video/download.mp4");
            }

    }

}
