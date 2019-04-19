#include "PersonExperiment.hpp"
#include "../src/tracking/kcftracker.hpp"
#include "../Tools/Tools.hpp"
#include "../src/Detection/detection.h"
#include "../src/ZhSupport.h"
#include <fstream>

using namespace std;
using namespace cv;

Mat image_read_exp5_src;
Mat image_read_exp5_tmp;
Mat image_crop_exp5;

Rect roi_exp5 = Rect(270, 26, 175, 350);

string seq_fullpath_exp5 = "";
string save_fullpath_exp5 = "";
vector<string> filenames;
vector<float> pv;

KCFTracker tracker_exp5;
bool firstFrame = true;
bool isReInit_exp5 = false;
bool isTrackerInit = false;
long frame_count = 0;

Detector detect_label;
int numThre = 1;
double scoreThre = 0.2;
double iouThre = 0.9;
vector<float> boxes;
vector<float> scores;
vector<size_t> labels;
string inLayer = "image_tensor:0";
vector<string> outLayer = { "detection_boxes:0", "detection_scores:0", "detection_classes:0" };
bool isDetectorInit = false;

bool humanDetectInit()
{
	int status_detect_model = detect_label.load_model_detection("./Content_resource/Posture/data/INRIA-80000/frozen_inference_graph.pb");
	int status_recognize_model = detect_label.load_model_recognition("./Content_resource/Posture/data/PostureV3/frozen_graph.pb");
	if (status_detect_model == 0 && status_recognize_model == 0)
		return true;
	else
		return false;
}

void PersonExperiment::exp5Param(int param, int hog, int rgb)
{
	intParams.clear();
	intParams.push_back(param);
	intParams.push_back(hog);
	intParams.push_back(rgb);
}

void reInit_exp5()
{
	isReInit_exp5 = true;
}

void trackerInit(bool hog, bool rgb)
{
	if (!isTrackerInit)
	{
		isTrackerInit = true;
		tracker_exp5 = KCFTracker(hog, false, true, rgb);
	}
}

void trackerRelease()
{
	if (isReInit_exp5 && isTrackerInit)
	{
		isTrackerInit = false;
		isReInit_exp5 = false;
		tracker_exp5.~KCFTracker();
	}

	if (isTrackerInit)
	{
		isTrackerInit = false;
		tracker_exp5.~KCFTracker();
	}
}

bool getTrackingResult(string image_path, string & src_path, string & pro_path)
{
	bool isEnd = false;
	Mat show;
	if (firstFrame)
	{
		tracker_exp5.init(roi_exp5, image_read_exp5_src);
		firstFrame = false;
		image_read_exp5_src.copyTo(image_read_exp5_tmp);
		rectangle(image_read_exp5_tmp, roi_exp5, Scalar(0, 255, 0), 4);
		frame_count++;
		if (pv.size() > 0)pv.clear();
	}
	else
	{
		if (frame_count < filenames.size())
		{
			string filepath = image_path + "/" + filenames[frame_count];
			image_read_exp5_src = imread(filepath);
			image_read_exp5_tmp = image_read_exp5_src.clone();
			roi_exp5 = tracker_exp5.update(image_read_exp5_src);
			rectangle(image_read_exp5_tmp, roi_exp5, Scalar(0, 255, 0), 4);

			cv::resize(tracker_exp5.Res, show, cv::Size(tracker_exp5._tmpl_sz.width * 5, tracker_exp5._tmpl_sz.height * 5));
			show.convertTo(show, CV_8U, 255);

			frame_count++;
			isEnd = false;

			pv.push_back(tracker_exp5.peak_value);
		}
		else
		{
			firstFrame = true;
			frame_count = 0;
			isEnd = true;

			if (!save_fullpath_exp5.empty())
			{
				time_t timep;
				time(&timep);
				char tmp[64];
				strftime(tmp, sizeof(tmp), "%Y%m%d_%H%M%S", localtime(&timep));
				string fileName = tmp;

				ofstream fout;
				string savefile = save_fullpath_exp5 + '/' + fileName + "_exp5_test.txt";
				fout.open(savefile);
				for (int i = 0; i < pv.size(); i++)
				{
					fout << pv[i] << ' ';
				}
				fout << endl;
				fout.close();
			}
		}
	}
	string savepath;
	savepath = GetProgramDir() + "/" + "modify_src_img.jpg";
	cv::imwrite(savepath, image_read_exp5_tmp);
	src_path = "file:///" + savepath;

	if (!show.empty())
	{
		savepath = GetProgramDir() + "/" + "modify_pro_img.jpg";
		imwrite(savepath, show);
		pro_path = "file:///" + savepath;
	}

	return isEnd;
}

// app ID:
// change image : 0
// label the first image: 1
// set Rectangle : 2
// reset the params : 3
// tracking : 4
void PersonExperiment::experiment_five(string image_path, string save_path, cv::Mat &frame, int app_Id)
{
	if (!save_path.empty()) {
		save_fullpath_exp5 = save_path;
	}

	if (!image_path.empty() && app_Id == 0)
	{
		seq_fullpath_exp5 = image_path;
		if (filenames.size() > 0)filenames.clear();
		GetSpecialFilesFromDirectory(seq_fullpath_exp5, ".jpg", filenames);
		if (filenames.size() == 0)
		{
			config = false;
		}
		else
		{
			config = true;
			string filepath = image_path + "/" + filenames[0];
			image_read_exp5_src = imread(filepath);
			image_read_exp5_tmp = image_read_exp5_src.clone();
			filepath = "file:///" + filepath;
			tmp_src_img_path = QString::fromStdString(filepath);

			bool hog = false;
			bool rgb = false;
			if (intParams.size() > 1)
			{
				if (intParams.size() == 2)
				{
					hog = intParams[1] == 1 ? true : false;
				}
				if (intParams.size() == 3)
				{
					hog = intParams[1] == 1 ? true : false;
					rgb = intParams[2] == 1 ? true : false;
				}
			}
			if (isTrackerInit)trackerRelease();
			trackerInit(hog, rgb);
		}
		appStatus = true;
	}

	if (!image_read_exp5_tmp.empty() && app_Id == 1)
	{
		if (!isDetectorInit)
		{
			if(humanDetectInit())
				isDetectorInit = true;
			else
			{
				config = false;
			}
		}

		if (isDetectorInit)
		{
			int detectStatus = detect_label.detect(image_read_exp5_src, scoreThre, iouThre, inLayer, outLayer, boxes, scores, labels);
			if (detectStatus >= 0)
			{
				if (scores.size() == 0)
				{
					config = false;
				}
				if (scores.size() == 1)
				{
					roi_exp5.y = boxes[0];
					roi_exp5.x = boxes[1];
					roi_exp5.height = boxes[2] - boxes[0];
					roi_exp5.width = boxes[3] - boxes[1]; 
					image_read_exp5_src.copyTo(image_read_exp5_tmp);
					rectangle(image_read_exp5_tmp, roi_exp5, Scalar(0, 255, 0), 4);
					config = true;
				}
			}
		}
		string savepath = GetProgramDir() + "/" + "modify_src_img.jpg";
		cv::imwrite(savepath, image_read_exp5_tmp);
		string filepath = "file:///" + savepath;
		tmp_src_img_path = QString::fromStdString(filepath);
		appStatus = true;
	}

	if (!image_read_exp5_tmp.empty() && app_Id == 2)
	{
		if (intParams[0] == 0)moveRect(roi_exp5, 0);
		if (intParams[0] == 1)moveRect(roi_exp5, 1);
		if (intParams[0] == 2)moveRect(roi_exp5, 2);
		if (intParams[0] == 3)moveRect(roi_exp5, 3);
		if (intParams[0] == 4)moveRect(roi_exp5, 4);

		string path = setRect(image_read_exp5_src, image_read_exp5_tmp, roi_exp5);
		stringReplace(path, "\\", "/");
		tmp_src_img_path = QString::fromStdString(path);
		appStatus = true;
	}

	if (app_Id == 3)
	{
		reInit_exp5();
		trackerRelease();
		appStatus = true;
	}

	if (isTrackerInit && !image_path.empty() && app_Id == 4)
	{
		string src_path, pro_path;
		appStatus = getTrackingResult(image_path, src_path, pro_path);
		stringReplace(src_path, "\\", "/");
		stringReplace(pro_path, "\\", "/");
		tmp_src_img_path = QString::fromStdString(src_path);
		tmp_pro_img_path = QString::fromStdString(pro_path);
	}
}