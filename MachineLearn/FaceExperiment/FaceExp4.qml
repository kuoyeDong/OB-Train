import QtQuick 2.9
import QtQuick.Controls 2.5
import QtCharts 2.0
import QtQuick.Dialogs 1.2
import ".."

import RegisterFaceName 1.0

Page {
    id: page
    width: 1920
    height: 1080
    font.pointSize: 20
    enabled: true
    visible: true

    property StackView stack: null

    background: Image {
        source: "../../images/background.png"
    }

    MySaveFaceName
    {
        id:saveLocalName
    }

    Button
    {
        height: 32
        width: 120
        text: "<-"
        anchors.left: parent.left
        anchors.top: parent.top
        onClicked:
        {

            stack.pop()
            textField0.text = ""
            textField1.text = ""
            textField2.text = ""
            textField3.text = ""
            textField4.text = ""
            textField_testimage.text = ""
            textField_convert_testimage.text = ""
            textField_convert_test.text = ""
        }
    }

    Label
    {
        id: title
        x: 719
        width: 507
        height: 56
        text: qsTr("数字识别模型验证实验")
        anchors.horizontalCenterOffset: 8
        anchors.top: parent.top
        anchors.topMargin: 24
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 50
    }

    Button {
        id: data_path
        x: 38
        y: 510
        width: 284
        height: 80
        text: qsTr("选择验证数据")
        font.bold: false
        font.family: "SimSum"
        spacing: 4
        font.pointSize: 30
        onClicked:
        {
            test_datapath.open()
        }
    }
    FileDialog{
        id:test_datapath
        title:qsTr("选择验证数据")
        folder: shortcuts.desktop
        nameFilters: ["Data Files (*.idx3-ubyte)", "*.*"]
        onAccepted: {
            textField0.text = test_datapath.fileUrl
        }
    }

    Button {
        id: data_label
        x: 38
        y: 630
        width: 284
        height: 80
        text: qsTr("选择数据标签")
        spacing: 4
        font.family: "SimSum"
        font.bold: false
        font.pointSize: 30
        onClicked:
        {
            test_labelpath.open()
        }
    }
    FileDialog{
        id:test_labelpath
        title:qsTr("选择数据标签")
        folder: shortcuts.desktop
        nameFilters: ["Data Files (*.idx1-ubyte)", "*.*"]
        onAccepted: {
            textField1.text = test_labelpath.fileUrl
        }
    }

    Button {
        id: data_label1
        x: 38
        y: 750
        width: 284
        height: 80
        text: qsTr("选择模型")
        font.family: "SimSum"
        spacing: 4
        font.bold: false
        font.pointSize: 30
        onClicked:
        {
            model_path.open()
        }
    }
    FileDialog{
        id:model_path
        title:qsTr("选择模型")
        folder: shortcuts.desktop
        nameFilters: ["Model Files (*.xml)", "*.*"]
        onAccepted: {
            textField2.text = model_path.fileUrl
        }
    }

    TextField {
        id: textField2
        x: 362
        y: 750
        width: 906
        height: 80
        text: qsTr("")
        font.pointSize: 20
    }


    Button {
        id: modelSavepath
        x: 38
        y: 870
        width: 284
        height: 80
        text: qsTr("开始验证")
        font.pointSize: 30
        onClicked:
        {
//            ExpFaceImage.testMNIST()
//            textField3.text = ExpFaceImage.getContent()
            var len1 = String(test_datapath.fileUrl);
            var len2 = String(test_labelpath.fileUrl);
            var len3 = String(model_path.fileUrl);
            if(len1.match("f") && len2.match("f") && len3.match("f"))
            {
                ExpFaceImage.getParamsForFaceExp4_1(test_datapath.fileUrl,
                                                    test_labelpath.fileUrl,
                                                    model_path.fileUrl)
                ExpFaceImage.testMNIST()
                textField3.text = "Accuracy Rate: " + ExpFaceImage.getContent()
            }

        }
    }



    Button {
        id: start_training
        x: 1453
        y: 763
        width: 284
        height: 95
        text: qsTr("识别当前数字")
        font.pointSize: 30
        onClicked:
        {
            var len = String(model_path.fileUrl);
            if(len.match("f"))
            {
                canvas.save("./experiment_src/FaceExperiment/testNumImage.jpg")
                ExpFaceImage.getParamsForFaceExp4_2(model_path.fileUrl)
                ExpFaceImage.testInputNum()
                textField4.text = ExpFaceImage.getContentResponse()
            }
        }
    }

    TextField {
        id: textField0
        x: 362
        y: 510
        width: 906
        height: 80
        text: qsTr("")
        font.pointSize: 20
    }

    TextField {
        id: textField1
        x: 362
        y: 630
        width: 906
        height: 80
        text: qsTr("")
        font.pointSize: 20
    }

    TextField {
        id: textField3
        x: 362
        y: 870
        width: 906
        height: 80
        text: qsTr("")
        font.pointSize: 20
    }

    Button {
        id: image_path
        x: 38
        y: 110
        width: 284
        height: 80
        text: qsTr("选择图像文件")
        spacing: 4
        font.family: "SimSum"
        font.bold: false
        font.pointSize: 30
        onClicked:
        {
            image_choose_path.open()
        }
    }
    FileDialog{
        id:image_choose_path
        title:qsTr("选择图像文件夹")
        folder: shortcuts.desktop
        selectExisting:true
        selectFolder:true
        selectMultiple:true
        onAccepted: {
            textField_testimage.text = image_choose_path.fileUrl
        }
    }

    Button {
        id: data_convert_path
        x: 38
        y: 230
        width: 284
        height: 80
        text: qsTr("数据保存路径")
        spacing: 4
        font.family: "SimSum"
        font.bold: false
        font.pointSize: 30
        onClicked:
        {
            image_convert_path.open()
        }
    }
    FileDialog{
        id:image_convert_path
        title:qsTr("选择数据保存路径")
        folder: shortcuts.desktop
        selectExisting:true
        selectFolder:true
        selectMultiple:true
        onAccepted: {
            textField_convert_testimage.text = image_convert_path.fileUrl
        }
    }

    Button {
        id: start_make_test_data
        x: 38
        y: 350
        width: 284
        height: 80
        text: qsTr("开始制作")
        font.pointSize: 30
        onClicked: {
            var len1 = String(image_choose_path.fileUrl);
            var len2 = String(image_convert_path.fileUrl);
            if(len1.match("f") && len2.match("f"))
            {
                ExpFaceImage.getParamsForFaceExp4(image_choose_path.fileUrl,
                                                  image_convert_path.fileUrl);
                ExpFaceImage.runConvertTestData();

                textField_convert_test.text = qsTr("转换结束")
            }
        }
    }

    TextField {
        id: textField_testimage
        x: 362
        y: 110
        width: 906
        height: 80
        text: qsTr("")
        font.pointSize: 20
    }

    TextField {
        id: textField_convert_testimage
        x: 362
        y: 230
        width: 906
        height: 80
        text: qsTr("")
        font.pointSize: 20
    }

    TextField {
        id: textField_convert_test
        x: 362
        y: 350
        width: 906
        height: 80
        text: qsTr("")
        font.pointSize: 20
    }

    Rectangle {
        id: canvasrectangle
        anchors.fill: canvas
        color: "#000000"
    }

    Canvas{
        id:canvas
        x: 1308
        y: 110
        width: 574
        height: 574

        //鼠标点击坐标位置
        property real lastX
        property real lastY
        property color color: "white"
        property bool validMouse

        onPaint: {
            var ctx = getContext("2d")

            if(validMouse)
            {
                ctx.lineWidth = slider.value
                ctx.lineCap="round";
                ctx.strokeStyle = canvas.color;
                ctx.beginPath()
                ctx.moveTo(lastX,lastY)
                lastX = drawArea.mouseX;
                lastY = drawArea.mouseY;
                ctx.lineTo(lastX,lastY)
                ctx.stroke()
            }
            if(!validMouse)
            {
                ctx.reset();
                validMouse = true
            }
        }
        MouseArea{
            id:drawArea;
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            anchors.fill: parent;
            onPressed: {
                if (mouse.button === Qt.LeftButton)
                {
                    canvas.lastX = mouseX;
                    canvas.lastY = mouseY;
                    console.log("Press Leftbutton!!!!!!!!!!!!!!!")
                }
            }
//            onDoubleClicked: {
//                canvas.save("abc.jpg")
//                canvas.validMouse = false
//            }
            onClicked: {
                if (mouse.button === Qt.RightButton)
                {
                    canvas.validMouse = false
                    canvas.clear()
                    console.log("Press Rightbutton!!!!!!!!!!!!!!!")
                }
            }
            onPositionChanged: {
                canvas.requestPaint()//当鼠标press位置改变  完成当前绘制
            }
        }

        function clear()
        {
            var ctx = getContext("2d");
            ctx.reset();
            canvas.requestPaint();
        }


    }

    TextField {
        id: textField4
        x: 1638
        y: 870
        width: 244
        height: 100
        text: qsTr("")
        font.pointSize: 30
    }

    Label {
        id: label
        x: 1308
        y: 870
        width: 314
        height: 100
        font.pointSize: 40
        text: qsTr("当前数字为： ")
        verticalAlignment: Text.AlignVCenter
    }

    Slider {
        id: slider
        x: 1484
        y: 706
        width: 398
        height: 40
        from: 1
        stepSize: 1
        to: 100
        value: 20
    }

    Label {
        id: label1
        x: 1308
        y: 693
        width: 170
        height: 66
        text: qsTr("画笔大小：") + slider.value
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 20
    }

    Label {
        id: label2
        x: 1308
        y: 64
        width: 574
        height: 45
        text: qsTr("提示：点击鼠标左键书写，点击鼠标右键清空 ")
        font.pointSize: 20
        verticalAlignment: Text.AlignVCenter
    }

}
