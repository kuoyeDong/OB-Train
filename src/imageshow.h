#ifndef IMAGESHOW_H
#define IMAGESHOW_H

#pragma once

#include "stdio.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#include"imageprovider.h"
#include "tracking/kcftracker.hpp"
#include "Detection/Posture.h"
//#include "face/face.hpp"
#include <QObject>
#include <opencv.hpp>

#include <dlib/dnn.h>
#include <dlib/gui_widgets.h>
#include <dlib/clustering.h>
#include <dlib/string.h>
#include <dlib/image_io.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/opencv.h>

using namespace cv;
using namespace dlib;
using namespace std;

template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N, BN, 1, tag1<SUBNET>>>;

template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2, 2, 2, 2, skip1<tag2<block<N, BN, 2, tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block = BN<con<N, 3, 3, 1, 1, relu<BN<con<N, 3, 3, stride, stride, SUBNET>>>>>;

template <int N, typename SUBNET> using ares = relu<residual<block, N, affine, SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block, N, affine, SUBNET>>;

template <typename SUBNET> using alevel0 = ares_down<256, SUBNET>;
template <typename SUBNET> using alevel1 = ares<256, ares<256, ares_down<256, SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128, ares<128, ares_down<128, SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64, ares<64, ares<64, ares_down<64, SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32, ares<32, ares<32, SUBNET>>>;

using anet_type = loss_metric<fc_no_bias<128, avg_pool_everything<
	alevel0<
	alevel1<
	alevel2<
	alevel3<
	alevel4<
	max_pool<3, 3, 2, 2, relu<affine<con<32, 7, 7, 2, 2,
	input_rgb_image_sized<150>
	>>>>>>>>>>>>;


class ShowImage : public QObject
{
    Q_OBJECT
public:
    explicit ShowImage(QObject *parent = 0);
    ImageProvider* m_pImgProvider;
    QImage  MattoQImage(Mat cvImg);

public slots:
    void setImage();

	/*******************************add by dbw*******************************/
	Q_INVOKABLE void FaceDet(cv::Mat frame);
	Q_INVOKABLE void FaceRec(cv::Mat frame);
	Q_INVOKABLE void FaceChange(cv::Mat frame);
	Q_INVOKABLE void FaceRegister(cv::Mat frame);
	/*******************************add by dbw*******************************/

signals:
    void callQmlRefeshImg();

/******************   General Switch   *******************/
public:
	Q_INVOKABLE	void setAlgorithmStart();
	Q_INVOKABLE void setAlgorithmStop();
	Q_INVOKABLE void camera_open();
	Q_INVOKABLE void camera_close();
	Q_INVOKABLE bool isOpened();
	Q_INVOKABLE int connectStatus();

private:
	bool cameraOpen;
	Mat frame_all;
	bool start_algorithm;
	int cameraStatus;
	bool overtime;
/******************   General Switch   *******************/


/******************        Tracking        *******************/
public:
	Q_INVOKABLE void openTracker();
	void runTracker();

private:
	KCFTracker tracker;
	Rect roi;
	bool tracking;
	bool first_frame;
/******************        Tracking        *******************/


/******************        Posture        *******************/
public:
	Q_INVOKABLE void openPosture();
	Q_INVOKABLE void closePosture();

private:
	bool posture;
/******************        Posture        *******************/


/*******************         Face          ********************/
public:

	//导入人脸检测和识别模型
	void LoadModel();//string detModel, string recModel);

	//导入本地人脸库数据
	void GetLocalFaceData();// string feature, string name);
	Q_INVOKABLE void openFaceDet();
	Q_INVOKABLE void openFaceRec();
	Q_INVOKABLE void openFaceChange();
	Q_INVOKABLE void openFaceRegister();

	Q_INVOKABLE	void setFaceDetStart();
	Q_INVOKABLE void setFaceDetStop();
	Q_INVOKABLE	void setFaceRecStart();
	Q_INVOKABLE void setFaceRecStop();
	Q_INVOKABLE	void setCameraOpen();
	Q_INVOKABLE void setCameraClose();

private:
	/*******************************add by dbw*******************************/
	dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
	dlib::matrix<dlib::rgb_pixel> cimg;
	std::vector<matrix<rgb_pixel>> faces;
	shape_predictor sp;
	anet_type net;
	cv::Rect rct;

	//导入本地人脸库数据
	std::vector<float> face_feature;
	std::vector<string> face_name;
	ifstream infile_feature;
	ifstream infile_name;
	string s;
	float temp;
	bool facedetFlag = false;
	bool facerecFlag = false;
	bool facechangeFlag = false;
	bool faceregisterFlag = false;
	/*******************************add by dbw*******************************/

};


#endif // IMAGESHOW_H
