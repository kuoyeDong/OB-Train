#include "PersonExperiment.hpp"
#include "../src/tracking/kcftracker.hpp"
#include "../Tools/Tools.hpp"
#include <fstream>

using namespace std;
using namespace cv;

Mat image_read_exp4_src;
Mat image_read_exp4_tmp;
Mat image_crop_exp4;
Mat model;
Mat alpha;
Mat alpha_c1;
Mat alpha_c2;
Mat prob;
Mat hann;

float scale = 0.0f;
int cell_size = 0;
int twidth = 0;
int theight = 0;
bool modelread = false;

Rect roi_exp4 = Rect(270, 26, 175, 350);

string image_fullpath_exp4 = "";
string save_fullpath_exp4 = "";
string model_fullpath_exp4 = "";

KCFTracker tracker_exp4;
bool isReInit_exp4 = false;
bool trackerCreat = false;

template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

void PersonExperiment::configRelease()
{
	if (modelread)
	{
		modelread = false;
		model.release();
		alpha.release();
		prob.release();

		scale = 0.0f;
		cell_size = 0;
		twidth = 0;
		theight = 0;
	}
}

void PersonExperiment::exp4Param(int para, int hog, int rgb)
{
	intParams.clear();
	intParams.push_back(para);
	intParams.push_back(hog);
	intParams.push_back(rgb);
}

void reInit_exp4()
{
	isReInit_exp4 = true;
}

bool readModels(string dirpath, bool hog, bool rgb)
{
	string tmplFile;
	string alphaFile_c1;
	string alphaFile_c2;
	string probFile;
	string paramsFile;
	string hannFile;
	if (hog || rgb)
	{
		tmplFile = dirpath + '/' + "hog_rgb_latest_model.txt";
		alphaFile_c1 = dirpath + '/' + "hog_rgb_latest_alpha_c1.txt";
		alphaFile_c2 = dirpath + '/' + "hog_rgb_latest_alpha_c2.txt";
		probFile = dirpath + '/' + "hog_rgb_prob.txt";
		paramsFile = dirpath + '/' + "hog_rgb_tmpl_params.txt";
		hannFile = dirpath + '/' + "hog_rgb_hann.txt";
	}
	else
	{
		tmplFile = dirpath + '/' + "gray_latest_model.txt";
		alphaFile_c1 = dirpath + '/' + "gray_latest_alpha_c1.txt";
		alphaFile_c2 = dirpath + '/' + "gray_latest_alpha_c2.txt";
		probFile = dirpath + '/' + "gray_prob.txt";
		paramsFile = dirpath + '/' + "gray_tmpl_params.txt";
		hannFile = dirpath + '/' + "gray_hann.txt";
	}

	if (!txt2Mat(tmplFile, model))return false;
	if (!txt2Mat(probFile, prob))return false;
	if (!txt2Mat(alphaFile_c1, alpha_c1))return false;
	if (!txt2Mat(alphaFile_c2, alpha_c2))return false;
	if (!txt2Mat(hannFile, hann))return false;

	vector<Mat> channels = { alpha_c1 , alpha_c2 };
	merge(channels, alpha);

	ifstream in(paramsFile);
	vector<float>tmp;
	if (in)
	{
		string line, result;
		getline(in, line);
		stringstream input(line);
		while (input >> result)
			tmp.push_back(stringToNum<float>(result));
		twidth = int(tmp[0]);
		theight = int(tmp[1]);
		scale = tmp[2];
		cell_size = int(tmp[3]);
	}
	else
		return false;

	modelread = true;
	return true;
}

void creatTracker(bool hog, bool rgb)
{
	if (modelread && !trackerCreat)
	{
		trackerCreat = true;
		tracker_exp4 = KCFTracker(hog, false, true, rgb);
		tracker_exp4._alphaf = alpha;
		tracker_exp4._tmpl = model;
		tracker_exp4.cell_size = cell_size;
		tracker_exp4._tmpl_sz.width = twidth;
		tracker_exp4._tmpl_sz.height = theight;
		tracker_exp4._scale = scale;
		tracker_exp4.hann = hann;
		tracker_exp4._prob = prob;
	}
}

void releaseTracker()
{
	if (isReInit_exp4 && trackerCreat)
	{
		trackerCreat = false;
		isReInit_exp4 = false;
		tracker_exp4.~KCFTracker();
	}

	if (trackerCreat)
	{
		trackerCreat = false;
		tracker_exp4.~KCFTracker();
	}
}

string TestModel(bool hog, bool rgb)
{
	if (!image_read_exp4_src.empty())
	{
		image_read_exp4_tmp = image_read_exp4_src.clone();

		Rect roi_tmp = roi_exp4;
		if (roi_exp4.x + roi_exp4.width > image_read_exp4_tmp.cols - 1)
		{
			roi_tmp.x = roi_exp4.x;
			roi_tmp.width = image_read_exp4_tmp.cols - 2 - roi_exp4.x;
		}
		if (roi_exp4.y + roi_exp4.height > image_read_exp4_tmp.rows - 1)
		{
			roi_tmp.y = roi_exp4.y;
			roi_tmp.height = image_read_exp4_tmp.rows - 2 - roi_exp4.y;
		}
		if (roi_exp4.x < 0)
		{
			roi_tmp.x = 0;
		}
		if (roi_exp4.y < 0)
		{
			roi_tmp.y = 0;

		}

		tracker_exp4._roi = roi_tmp;

		Mat res = tracker_exp4.test(tracker_exp4._tmpl, tracker_exp4.getFeatures(image_read_exp4_tmp, 0, 1.0f));

		Mat show;
		cv::resize(res, show, cv::Size(tracker_exp4._tmpl_sz.width * 5, tracker_exp4._tmpl_sz.height * 5));
		show.convertTo(show, CV_8U, 255);

		if (!save_fullpath_exp4.empty())
		{
			string action = "";
			string image_name = filename(image_fullpath_exp4);
			string data_save_file;
			if (hog || rgb)
			{
				data_save_file = save_fullpath_exp4 + "/" + image_name + "_hog_rgb_res_map.txt";
			}
			else
			{
				data_save_file = save_fullpath_exp4 + "/" + image_name + "_gray_res_map.txt";
			}

			writeMatToFile(show, data_save_file.c_str(), action, false);
		}

		string savepath = GetProgramDir() + "/" + "modify_pro_img.jpg";
		imwrite(savepath, show);
		string path = "file:///" + savepath;
		return path;
	}
	else
	{
		string path = "";
		return path;
	}
}

// app ID:
// change image : 0
// read model and other files: 1
// light or dark : 2
// image flip : 3
// set Rectangle : 4
// reset the params : 5
// test the model : 6
void PersonExperiment::experiment_four(string image_path, string save_path, cv::Mat &frame, int app_Id)
{
	if (!save_path.empty() && app_Id != 1) {
		save_fullpath_exp4 = save_path;
	}

	if (!save_path.empty() && app_Id == 1){
		model_fullpath_exp4 = save_path;
	}

	if (!image_path.empty() && app_Id == 0)
	{
		image_fullpath_exp4 = image_path;
		image_read_exp4_src = imread(image_path);
		image_read_exp4_tmp = image_read_exp4_src.clone();
		appStatus = true;
	}

	if (!model_fullpath_exp4.empty() && app_Id == 1)
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
		config = readModels(model_fullpath_exp4, hog, rgb);
		if (modelread)
		{
			if (creatTracker)releaseTracker();
			creatTracker(hog, rgb);
		}
		appStatus = true;
	}

	//if(!image_path.empty() && app_Id == 0 && !image_read_exp4_src.empty())
	//{
	//	image_fullpath_exp4 = image_path;
	//	image_read_exp4_src = imread(image_path);
	//	image_read_exp4_tmp = image_read_exp4_src.clone();
	//	appStatus = true;
	//}

	if (!image_read_exp4_src.empty() && app_Id == 2)
	{
		if (intParams[0] == 1)
		{
			string path = lightDark(image_read_exp4_src, 20);
		    stringReplace(path, "\\", "/");
			tmp_src_img_path = QString::fromStdString(path);
		}
		if (intParams[0] == 0)
		{
			string path = lightDark(image_read_exp4_src , -20);
			stringReplace(path, "\\", "/");
			tmp_src_img_path = QString::fromStdString(path);
		}
		appStatus = true;
	}

	if (!image_read_exp4_src.empty() && app_Id == 3)
	{
		string path = imgFlip(image_read_exp4_src);
		stringReplace(path, "\\", "/");
		tmp_src_img_path = QString::fromStdString(path);
		appStatus = true;
	}

	if (!image_read_exp4_tmp.empty() && app_Id == 4)
	{
		if (intParams[0] == 0)moveRect(roi_exp4, 0);
		if (intParams[0] == 1)moveRect(roi_exp4, 1);
		if (intParams[0] == 2)moveRect(roi_exp4, 2);
		if (intParams[0] == 3)moveRect(roi_exp4, 3);
		if (intParams[0] == 4)moveRect(roi_exp4, 4);

		string path = setRect(image_read_exp4_src, image_read_exp4_tmp, roi_exp4);
		stringReplace(path, "\\", "/");
		tmp_src_img_path = QString::fromStdString(path);
		appStatus = true;
	}

	if (app_Id == 5)
	{
		reInit_exp4();
		configRelease();
		releaseTracker();
		appStatus = true;
	}

	if (modelread && trackerCreat && !image_read_exp4_src.empty() && app_Id == 6)
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
	}
}