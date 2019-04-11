#ifndef FACEEXPERIMENT_H
#define FACEEXPERIMENT_H

#pragma once

#include "stdio.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <QDebug>
#include <io.h>


#include "../src/camera.hpp"
#include "../src/ZhSupport.h"
#include"../src/imageprovider.h"

#include <QObject>
#include <opencv.hpp>

using namespace cv;
using namespace std;



class FaceExperiment : public QObject
{
    Q_OBJECT
public:
    explicit FaceExperiment(QObject *parent = 0);
	~FaceExperiment();
    ImageProvider* m_pImgProvider;
    QImage  MattoQImage(Mat cvImg);

public slots:
    void setImage();

signals:
    void callQmlRefeshImg();

/******************   General Switch   *******************/
//实验标志位开关函数
public:
	//图传标志位和状态
	Q_INVOKABLE void camera_open();
	Q_INVOKABLE void camera_close();
	Q_INVOKABLE bool isOpened();
	Q_INVOKABLE int connectStatus();

	//实验一标志位和状态
	Q_INVOKABLE bool faceDetRunStatus();
	Q_INVOKABLE void openFaceDetbyMTCNN();

	//实验二标志位和状态
	Q_INVOKABLE void openFaceExp2();
	Q_INVOKABLE void closeFaceExp2();

//实验标志位开关参数
private:
	bool cameraOpen;
	Mat frame_face;
	bool start_algorithm;
	int cameraStatus;
	bool overtime;
	bool facedetFlag = false;
	bool faceExp2 = false;
/******************   General Switch   *******************/



//实验相关功能代码
public:
	Q_INVOKABLE void getParamsForFaceExp1(QString video_path, QString num1, QString num2);
	Q_INVOKABLE void getParamsForFaceExp2(QString video_path, QString num1, QString num2, QString num3, QString kernName1, QString kernName2, QString kernName3);




public:
	//实验一功能代码
	void FaceDetbyMTCNN(cv::Mat frame, string savePath, int saveNum, int intervalNum);
	//实验二功能代码
	Mat kernelTest();//int n1, int n2, int n3, string kern1, string kern2, string kern3);


private:
	//实验一参数
	int saveFrameNum;
	int saveFrameIntervalNum;
	std::string saveFramePath;
	int localnum;

	//实验二参数
	int kernNum1, kernNum2, kernNum3;
	string srcImgPath;
	Mat dstImg;
	string kernel1, kernel2, kernel3;


private:
	void stringReplace(std::string &strsrc, const std::string &strtarget, const std::string &strdst);

};


#endif // FACEEXPPERIMENT_H