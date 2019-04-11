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
//ʵ���־λ���غ���
public:
	//ͼ����־λ��״̬
	Q_INVOKABLE void camera_open();
	Q_INVOKABLE void camera_close();
	Q_INVOKABLE bool isOpened();
	Q_INVOKABLE int connectStatus();

	//ʵ��һ��־λ��״̬
	Q_INVOKABLE bool faceDetRunStatus();
	Q_INVOKABLE void openFaceDetbyMTCNN();

	//ʵ�����־λ��״̬
	Q_INVOKABLE void openFaceExp2();
	Q_INVOKABLE void closeFaceExp2();

//ʵ���־λ���ز���
private:
	bool cameraOpen;
	Mat frame_face;
	bool start_algorithm;
	int cameraStatus;
	bool overtime;
	bool facedetFlag = false;
	bool faceExp2 = false;
/******************   General Switch   *******************/



//ʵ����ع��ܴ���
public:
	Q_INVOKABLE void getParamsForFaceExp1(QString video_path, QString num1, QString num2);
	Q_INVOKABLE void getParamsForFaceExp2(QString video_path, QString num1, QString num2, QString num3, QString kernName1, QString kernName2, QString kernName3);




public:
	//ʵ��һ���ܴ���
	void FaceDetbyMTCNN(cv::Mat frame, string savePath, int saveNum, int intervalNum);
	//ʵ������ܴ���
	Mat kernelTest();//int n1, int n2, int n3, string kern1, string kern2, string kern3);


private:
	//ʵ��һ����
	int saveFrameNum;
	int saveFrameIntervalNum;
	std::string saveFramePath;
	int localnum;

	//ʵ�������
	int kernNum1, kernNum2, kernNum3;
	string srcImgPath;
	Mat dstImg;
	string kernel1, kernel2, kernel3;


private:
	void stringReplace(std::string &strsrc, const std::string &strtarget, const std::string &strdst);

};


#endif // FACEEXPPERIMENT_H