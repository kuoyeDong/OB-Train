#include "PersonExperiment.hpp"
#include "../src/tracking/kcftracker.hpp"
#include "../Tools/Tools.hpp"
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

KCFTracker tracker_exp5;
bool isReInit_exp5 = false;
bool isTrackerInit = false;

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

void readSeqs()
{
	filenames.clear();
}

// app ID:
// change image : 0
// label image: 1
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
			string filepath = image_path + "/" + filenames[0];
			image_read_exp5_src = imread(filepath);
			image_read_exp5_tmp = image_read_exp5_src.clone();
			filepath = "file:///" + filepath;
			tmp_src_img_path = QString::fromStdString(filepath);
		}
		appStatus = true;
	}

	if (!image_read_exp5_tmp.empty() && app_Id == 1)
	{
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

	/*if (modelread && trackerCreat && !image_read_exp4_src.empty() && app_Id == 6)
	{
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
		string path = TestModel(hog, rgb);
		stringReplace(path, "\\", "/");
		tmp_pro_img_path = QString::fromStdString(path);
		appStatus = true;
	}*/
}