﻿import QtQuick 2.9
import QtQuick.Controls 2.2
import ".."
import "./Detection"

Page {
    id: page


    property StackView stack: null

    width: 1920
    height: 1080

    background: Image {
        source: "./images/PersonGuidePage.jpg"
    }

    Button {
        height: 32
        width: 120
        text: "<-"
        anchors.left: parent.left
        anchors.top: parent.top
        onClicked: stack.pop()
    }

    Button {
        id: button
        x: 1491
        y: 879
        width: 181
        height: 52
        text: qsTr("继续>>>>")
        font.family: "华文楷体"
        display: AbstractButton.TextBesideIcon
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: -712
        anchors.bottom: parent.verticalCenter
        anchors.bottomMargin: -391
        spacing: 7
        font.pixelSize: 35
        focusPolicy: Qt.NoFocus
        onClicked: {
            //stack.pop();
            page_Tracking.visible = true;
            page_Tracking.stack = stack;
            stack.push(page_Tracking);
        }
    }

    Vision_Examples_Tracking{
        id: page_Tracking
        visible: false
        stack: stack
    }


}

