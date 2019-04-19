#pragma once
#include <opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <direct.h>

#include "../src/imageprovider.h"
#include <QObject>

class PersonExperiment : public QObject
{
	Q_OBJECT
public:
	cv::Mat frame;

public:
	explicit PersonExperiment(QObject *parent = 0);
	~PersonExperiment();

	ImageProvider* m_pImgProvider;
	QImage  MattoQImage(cv::Mat cvImg);

/******************   QT   *******************/
public slots:
	void setImage();

signals:
	void callQmlRefeshImg();
/******************   QT   *******************/

/******************   General Switch   *******************/
public:
	// experiment
	Q_INVOKABLE void setEx_id(int ex_id, int app_id);
	Q_INVOKABLE int get_exp_id();
	Q_INVOKABLE int get_app_id();
	Q_INVOKABLE void exp1Param(QString time, QString fps);
	Q_INVOKABLE void exp2Param(int para);
	Q_INVOKABLE void exp3Param(int para, int hog, int rgb, QString leaning_rate);
	Q_INVOKABLE void exp4Param(int param, int hog, int rgb);
	Q_INVOKABLE void exp5Param(int param, int hog, int rgb);
	Q_INVOKABLE void reInit(int choice);

	 // with camera
	Q_INVOKABLE void camera_open();
	Q_INVOKABLE void camera_close();
	Q_INVOKABLE bool isOpened();
	Q_INVOKABLE int connectStatus();

	// without camera
	Q_INVOKABLE void expStart();

	// other
	Q_INVOKABLE bool appRunStatus();
	Q_INVOKABLE bool appConfigStatus();
	Q_INVOKABLE void configRelease();
	Q_INVOKABLE void setAppRunStatusClose();
	Q_INVOKABLE void setPath(QString generalPath, int choice);
	Q_INVOKABLE QString get_tmp_path(int choice);

private:
	int current_experiment;
	int current_app_id;
	int cameraStatus;
	bool cameraOpen;
	bool appStatus;
	bool config;
	std::vector<int> intParams;
	std::vector<float> floatParams;

	std::string generalPath;
	std::string imagePath;

	QString tmp_src_img_path;
	QString tmp_pro_img_path;

/******************   General Switch   *******************/

/*******************   Experiments   ********************/
public:
	void experiment_one(std::string video_path, cv::Mat &frame, int app_Id);
	void experiment_two(std::string image_path, std::string save_path, cv::Mat &frame, int app_Id);
	void experiment_three(std::string image_path, std::string save_path, cv::Mat &frame, int app_Id);
	void experiment_four(std::string image_path, std::string save_path, cv::Mat &frame, int app_Id);
	void experiment_five(std::string image_path, std::string save_path, cv::Mat &frame, int app_Id);

private:
	//tools
	void stringReplace(std::string &strsrc, const std::string &strtarget, const std::string &strdst);
	//Experiment One


/*******************   Experiments   ********************/

};