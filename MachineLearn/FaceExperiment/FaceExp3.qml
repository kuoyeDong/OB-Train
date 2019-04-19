import QtQuick 2.9
import QtQuick.Controls 2.2
import QtCharts 2.0
import QtQuick.Dialogs 1.2
import ".."

import RegisterFaceName 1.0

Page {
    id: page
    width: 1920
    height: 1080

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
            textField_image.text = ""
            textField_convert_image.text = ""
            textField_convert.text = ""
        }
    }

    Label
    {
        id: title
        x: 719
        width: 507
        height: 56
        text: qsTr("数字识别模型训练实验")
        anchors.horizontalCenterOffset: 8
        anchors.top: parent.top
        anchors.topMargin: 24
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 50
    }

    Button {
        id: data_path
        x: 38
        y: 560
        width: 284
        height: 100
        text: qsTr("选择训练数据")
        font.bold: false
        font.family: "SimSum"
        spacing: 4
        font.pointSize: 30
        onClicked:
        {
            training_datapath.open()
        }
    }
    FileDialog{
        id:training_datapath
        title:qsTr("选择训练数据")
        folder: shortcuts.desktop
//        selectExisting:true
//        selectFolder:true
//        selectMultiple:true
        nameFilters: ["Data Files (*.idx3-ubyte)", "*.*"]
        onAccepted: {
            textField0.text = training_datapath.fileUrl
        }
    }

    Button {
        id: data_label
        x: 38
        y: 715
        width: 284
        height: 100
        text: qsTr("选择数据标签")
        spacing: 4
        font.family: "SimSum"
        font.bold: false
        font.pointSize: 30
        onClicked:
        {
            training_labelpath.open()
        }
    }
    FileDialog{
        id:training_labelpath
        title:qsTr("选择数据标签")
        folder: shortcuts.desktop
//        selectExisting:true
//        selectFolder:true
//        selectMultiple:true
        nameFilters: ["Data Files (*.idx1-ubyte)", "*.*"]
        onAccepted: {
            textField1.text = training_labelpath.fileUrl
        }
    }


    Button {
        id: modelSavepath
        x: 38
        y: 870
        width: 284
        height: 100
        text: qsTr("选择保存路径")
        font.pointSize: 30
        onClicked:
        {
            save_modelpath.open()
        }
    }
    FileDialog{
        id:save_modelpath
        title:qsTr("选择保存路径")
        folder: shortcuts.desktop
        selectExisting:true
        selectFolder:true
        selectMultiple:true
        onAccepted: {
            textField2.text = save_modelpath.fileUrl
        }
    }

    ComboBox {
        id: kernelName
        x: 1595
        y: 110
        width: 246
        height: 80
        font.bold: true
        font.pointSize: 20
        model: ["RBF", "LINEAR", "SIGMOID"]
    }

    ComboBox {
        id: penaltyFactor
        x: 1595
        y: 510
        width: 246
        height: 80
        font.bold: true
        font.pointSize: 20
        model: ["7", "8", "9", "10", "11"]
    }

    ComboBox {
        id: gammaFactor
        x: 1595
        y: 310
        width: 246
        height: 80
        font.bold: true
        font.pointSize: 20
        model: ["0.005", "0.01", "0.15", "0.2"]
    }

    ComboBox {
        id: iterNum
        x: 1595
        y: 710
        width: 246
        height: 80
        font.bold: true
        font.pointSize: 20
        model: ["50", "100", "150", "200", "250"]
    }


    Button {
        id: start_training
        x: 1453
        y: 875
        width: 284
        height: 95
        text: qsTr("开始训练")
        font.pointSize: 40
        onClicked:
        {
            var len1 = String(training_datapath.fileUrl);
            var len2 = String(training_labelpath.fileUrl);
            var len3 = String(save_modelpath.fileUrl);
            if(len1.match("f") && len2.match("f") && len3.match("f"))
            {
                page.run = true
                ExpFaceImage.getParamsForFaceExp3(training_datapath.fileUrl,
                                                  training_labelpath.fileUrl,
                                                  save_modelpath.fileUrl,
                                                  kernelName.currentText,
                                                  penaltyFactor.currentText,
                                                  gammaFactor.currentText,
                                                  iterNum.currentText)
                ExpFaceImage.trainMNIST();
                page.run = false
                console.log("Run data successfully!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ")
            }
        }
    }

    Label {
        id: label
        x: 1319
        y: 110
        width: 270
        height: 80
        text: qsTr("选择核函数： ")
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 30
    }

    Label {
        id: label1
        x: 1319
        y: 310
        width: 270
        height: 80
        text: qsTr("惩罚系数： ")
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 30
    }

    Label {
        id: label2
        x: 1319
        y: 510
        width: 270
        height: 80
        text: qsTr("Gamma系数： ")
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 30
    }

    Label {
        id: label3
        x: 1319
        y: 710
        width: 270
        height: 80
        text: qsTr("迭代次数： ")
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 30
    }


    property bool run : false
    BusyIndicator {
        id: busyIndicator
        x: 1766
        y: 870
        width: 95
        height: 95
        running: page.run
    }

    TextField {
        id: textField0
        x: 362
        y: 560
        width: 906
        height: 100
        text: qsTr("")
        font.pointSize: 20
    }

    TextField {
        id: textField1
        x: 362
        y: 715
        width: 906
        height: 100
        text: qsTr("")
        font.pointSize: 20
    }

    TextField {
        id: textField2
        x: 362
        y: 870
        width: 906
        height: 100
        text: qsTr("")
        font.pointSize: 20
    }

    Button {
        id: image_path
        x: 38
        y: 110
        width: 284
        height: 100
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
            textField_image.text = image_choose_path.fileUrl
        }
    }

    Button {
        id: data_convert_path
        x: 38
        y: 265
        width: 284
        height: 100
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
            textField_convert_image.text = image_convert_path.fileUrl
        }
    }

    Button {
        id: start_make_train_data
        x: 38
        y: 414
        width: 284
        height: 95
        text: qsTr("开始制作")
        font.pointSize: 40
        onClicked: {
            var len1 = String(image_choose_path.fileUrl);
            var len2 = String(image_convert_path.fileUrl);
            if(len1.match("f") && len2.match("f"))
            {
                ExpFaceImage.getParamsForFaceExp4(image_choose_path.fileUrl,
                                                  image_convert_path.fileUrl);
                ExpFaceImage.runConvertTrainData();

                textField_convert.text = qsTr("转换结束")
            }
        }
    }

    TextField {
        id: textField_image
        x: 362
        y: 110
        width: 906
        height: 100
        text: qsTr("")
        font.pointSize: 20
    }

    TextField {
        id: textField_convert_image
        x: 362
        y: 265
        width: 906
        height: 100
        text: qsTr("")
        font.pointSize: 20
    }

    TextField {
        id: textField_convert
        x: 362
        y: 412
        width: 906
        height: 100
        text: qsTr("")
        font.pointSize: 20
    }


//    ProgressBar {
//        property color proColor: black //"#148014"
//        property color proBackgroundColor: "#AAAAAA"
//        property int proWidth: 2
//        property real progress: 0
//        property real proRadius: 3
//        property alias interval: timer.interval

//        function isRunning(){
//            return(timer.running)
//        }

//        function onStart(){
//            cProgress.progress = 0;
//            timer.running = true;
//        }

//        function onStop(){
//            timer.running = false;
//        }

//        id: cProgress
//        x: 362
//        y: 414
//        width: 906
//        height: 95
//        //anchors.centerIn: parent
//        value: (progress/100)
//        padding: 2

//        background: Rectangle {
//            implicitWidth: 200
//            implicitHeight: 16
//            color: cProgress.proBackgroundColor
//            radius: cProgress.proRadius
//        }

//        contentItem: Item {
//            implicitWidth: 200
//            implicitHeight: 10

//            Rectangle {
//                width: cProgress.visualPosition * parent.width
//                height: parent.height
//                radius: 2
//                color: cProgress.proColor
//            }
//        }

//        Timer{
//            id: timer
//            running: false
//            repeat: true
//            interval: 50
//            onTriggered:{
//                cProgress.progress++;
//                if (cProgress.progress > 100){
//                    cProgress.onStop();
//                    return;
//                }
//            }
//        }
//    }






}
