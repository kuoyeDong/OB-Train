#ifndef FACEEXPERIMENT_H
#define FACEEXPERIMENT_H

#pragma once

#include "stdio.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <io.h>

#include <QDebug>
#include <QObject>

//#include <Python.h>
//#include <numpy/arrayobject.h>

#include "../src/camera.hpp"
#include "../src/ZhSupport.h"
#include"../src/imageprovider.h"


#include <opencv.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>


using namespace cv;
using namespace std;
using namespace cv::ml;


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
	Q_INVOKABLE void getParamsForFaceExp3(QString input_datapath, QString input_labelpath, QString output_path, QString kernName, QString num_c, QString num_gamma, QString iter_num);
	Q_INVOKABLE void getParamsForFaceExp4(QString input_datapath, QString output_path);
	Q_INVOKABLE void getParamsForFaceExp4_1(QString input_datapath, QString input_labelpath, QString model_path);
	Q_INVOKABLE void getParamsForFaceExp4_2(QString model_path);
	Q_INVOKABLE void trainMNIST();
	Q_INVOKABLE void testMNIST();
	Q_INVOKABLE void testInputNum();
	Q_INVOKABLE void runConvertTrainData();
	Q_INVOKABLE void runConvertTestData();
	Q_INVOKABLE QString getContent();
	Q_INVOKABLE QString getContentResponse();




public:
	//ʵ��һ���ܴ���
	void FaceDetbyMTCNN(cv::Mat frame, string savePath, int saveNum, int intervalNum);
	void saveNumFrame(cv::Mat frame);
	//ʵ������ܴ���
	Mat kernelTest();//int n1, int n2, int n3, string kern1, string kern2, string kern3);
	//ʵ�������ܴ���
	int reverseInt(int i);
	


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

	//ʵ��������
	int iterNum;
	float numC, numGamma;
	string kernelName;
	string inputDataPath, inputLabelPath, outputPath;
	string inputTrainDataPath, outputTrainDataPath;

	//ʵ���Ĳ���
	string inputTestDataPath, inputTestLabelPath, inputModelPath;
	double preAccuracy;
	int response;



private:
	void stringReplace(std::string &strsrc, const std::string &strtarget, const std::string &strdst);

};


#endif // FACEEXPPERIMENT_H