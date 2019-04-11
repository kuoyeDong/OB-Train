#include "PersonExperiment.hpp"
#include "../src/tracking/kcftracker.hpp"
#include "../Tools/Tools.hpp"
#include <fstream>

using namespace std;
using namespace cv;

Mat image_read_exp3_src;
Mat image_read_exp3_tmp;
Mat image_crop_exp3;

Rect roi_exp3 = Rect(270, 26, 175, 350);

string image_fullpath_exp3 = "";
string save_fullpath_exp3 = "";
string fea1 = "hog";
string fea2 = "rgb";
string fea3 = "gray";

KCFTracker tracker_exp3;
bool isFirstFrame = true;
bool isReInit = false;

void PersonExperiment::exp3Param(int para, int hog, int rgb, QString leaning_rate)
{
	floatParams.clear();
	intParams.clear();
	floatParams.push_back(leaning_rate.toFloat());
	intParams.push_back(para);
	intParams.push_back(hog);
	intParams.push_back(rgb);
}

void PersonExperiment::reInit(int choice)
{
	if (choice == 1)
	{
		isReInit = true;
	}
}

string TrainModel(bool hog, bool rgb, float learning_rate)
{
	if (isReInit)
	{
		isReInit = false;
		isFirstFrame = true;
		tracker_exp3.~KCFTracker();
	}

	if (!image_read_exp3_src.empty())
	{
		image_read_exp3_tmp = image_read_exp3_src.clone();

		Rect roi_tmp = roi_exp3;
		if (roi_exp3.x + roi_exp3.width > image_read_exp3_tmp.cols - 1)
		{
			roi_tmp.x = roi_exp3.x;
			roi_tmp.width = image_read_exp3_tmp.cols - 2 - roi_exp3.x;
		}
		if (roi_exp3.y + roi_exp3.height > image_read_exp3_tmp.rows - 1)
		{
			roi_tmp.y = roi_exp3.y;
			roi_tmp.height = image_read_exp3_tmp.rows - 2 - roi_exp3.y;
		}
		if (roi_exp3.x < 0)
		{
			roi_tmp.x = 0;
		}
		if (roi_exp3.y < 0)
		{
			roi_tmp.y = 0;
		}

		cv::Mat src_tmpl;

		if (isFirstFrame)
		{
			tracker_exp3 = KCFTracker(hog, false, true, rgb);
			if (learning_rate > 1)
			{
				tracker_exp3.interp_factor = 1.0f;
			}
			else if (learning_rate < 0)
			{
				tracker_exp3.interp_factor = 0.0f;
			}
			else
			{
				tracker_exp3.interp_factor = learning_rate;
			}
			tracker_exp3.init(roi_tmp, image_read_exp3_tmp);
			isFirstFrame = false;

			src_tmpl = tracker_exp3._tmpl;

			// save prob data, hann data, model size and scale
			string tmpl_params;
			string src_hann_file;
			if ((hog || rgb) && !save_fullpath_exp3.empty())
			{
				src_hann_file = save_fullpath_exp3 + "/" + "hog_rgb_hann.txt";
				tmpl_params = save_fullpath_exp3 + "/" + "hog_rgb_tmpl_params.txt";
			}
			else
			{
				src_hann_file = save_fullpath_exp3 + "/" + "gray_hann.txt";
				tmpl_params = save_fullpath_exp3 + "/" + "gray_tmpl_params.txt";
			}

			if (!tracker_exp3.hann.empty())
			{
				ofstream fout;
				fout.open(src_hann_file);
				fout << tracker_exp3.hann.cols << ' ' << tracker_exp3.hann.rows << endl;
				for (int i = 0; i < tracker_exp3.hann.rows; i++)
				{
					for (int j = 0; j < tracker_exp3.hann.cols; j++)
					{
						if (isnan(tracker_exp3.hann.at<float>(i, j)))
							fout << 0 << " ";
						else
							fout << tracker_exp3.hann.at<float>(i, j) << " ";
					}
					fout << std::endl;
				}

				fout.close();
			}

			ofstream fout;
			fout.open(tmpl_params);
			fout << tracker_exp3._tmpl_sz.width << ' ' << tracker_exp3._tmpl_sz.height << ' ' << tracker_exp3._scale << ' ' << tracker_exp3.cell_size << endl;

		}
		else
		{
			tracker_exp3._roi = roi_tmp;
			cv::Mat x = tracker_exp3.getFeatures(image_read_exp3_tmp, 0);
			tracker_exp3.train(x, tracker_exp3.interp_factor);

			src_tmpl = tracker_exp3.getFeatures(image_read_exp3_tmp, 1);
		}

		string savepath = GetProgramDir() + "/" + "modify_pro_img.jpg";
		Mat model = tracker_exp3._tmpl;
		model.convertTo(model, CV_8U, 255);
		imwrite(savepath, model);
		string path = "file:///" + savepath;

		Mat new_tmpl;
		src_tmpl.convertTo(new_tmpl, CV_8U, 255);

		//save data to txt and image
		if (!save_fullpath_exp3.empty())
		{
			string action1 = "trained_model";
			string action2 = "original_model";
			string image_name = filename(image_fullpath_exp3);
			string data_save_file;
			if (hog || rgb)
			{
				data_save_file = save_fullpath_exp3 + "/" + image_name + "_hog_rgb_model.txt";
			}
			else
			{
				data_save_file = save_fullpath_exp3 + "/" + image_name + "_gray_model.txt";
			}

			// write model data to txt
			writeMatToFile(model, data_save_file.c_str(), action1, true);
			writeMatToFile(new_tmpl, data_save_file.c_str(), action2, true);

			// save model and alpha data
			string src_model_file;
			string src_alpah_file_c1;
			string src_alpah_file_c2;
			if (hog || rgb)
			{
				src_model_file = save_fullpath_exp3 + "/" + "hog_rgb_latest_model.txt";
				src_alpah_file_c1 = save_fullpath_exp3 + "/" + "hog_rgb_latest_alpha_c1.txt";
				src_alpah_file_c2 = save_fullpath_exp3 + "/" + "hog_rgb_latest_alpha_c2.txt";
			}
			else
			{
				src_model_file = save_fullpath_exp3 + "/" + "gray_latest_model.txt";
				src_alpah_file_c1 = save_fullpath_exp3 + "/" + "gray_latest_alpha_c1.txt";
				src_alpah_file_c2 = save_fullpath_exp3 + "/" + "gray_latest_alpha_c2.txt";
			}

			if (!tracker_exp3._tmpl.empty())
			{
				ofstream fout;
				fout.open(src_model_file);
				fout << tracker_exp3._tmpl.cols << ' ' << tracker_exp3._tmpl.rows << endl;
				for (int i = 0; i < tracker_exp3._tmpl.rows; i++)
				{
					for (int j = 0; j < tracker_exp3._tmpl.cols; j++)
					{
						if (isnan(tracker_exp3._tmpl.at<float>(i, j)))
							fout << 0 << " ";
						else
							fout << tracker_exp3._tmpl.at<float>(i, j) << " ";
					}
					fout << std::endl;
				}
				fout.close();
			}

			if (!tracker_exp3._alphaf.empty())
			{
				vector<Mat> pa;
				split(tracker_exp3._alphaf, pa);

				ofstream fout;
				vector<string>files;
				files.push_back(src_alpah_file_c1);
				files.push_back(src_alpah_file_c2);

				for (int i = 0; i < files.size(); i++)
				{
					fout.open(files[i]);
					fout << pa[i].cols << ' ' << pa[i].rows << endl;
					for (int j = 0; j < pa[i].rows; j++)
					{
						for (int k = 0; k < pa[i].cols; k++)
						{
							if (isnan(pa[i].at<float>(j, k)))
								fout << 0 << " ";
							else
								fout << pa[i].at<float>(j, k) << " ";
						}
						fout << std::endl;
					}
					fout.close();
				}
			}
		}

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
// light or dark : 1
// image flip : 2
// set Rectangle : 3
// train the model : 4
void PersonExperiment::experiment_three(string image_path, string save_path, cv::Mat &frame, int app_Id)
{
	if (!save_path.empty()) {
		save_fullpath_exp3 = save_path;
	}

	if (!image_path.empty() && image_read_exp3_src.empty())
	{
		image_fullpath_exp3 = image_path;
		image_read_exp3_src = imread(image_path);
		image_read_exp3_tmp = image_read_exp3_src.clone();
		appStatus = true;
	}

	if(!image_path.empty() && app_Id == 0 && !image_read_exp3_src.empty())
	{
		image_fullpath_exp3 = image_path;
		image_read_exp3_src = imread(image_path);
		image_read_exp3_tmp = image_read_exp3_src.clone();
		appStatus = true;
	}

	if (app_Id == 1)
	{
		if (intParams[0] == 1)
		{
			string path = lightDark(image_read_exp3_src, 20);
		    stringReplace(path, "\\", "/");
			tmp_src_img_path = QString::fromStdString(path);
		}
		if (intParams[0] == 0)
		{
			string path = lightDark(image_read_exp3_src , -20);
			stringReplace(path, "\\", "/");
			tmp_src_img_path = QString::fromStdString(path);
		}
		appStatus = true;
	}

	if (app_Id == 2)
	{
		string path = imgFlip(image_read_exp3_src);
		stringReplace(path, "\\", "/");
		tmp_src_img_path = QString::fromStdString(path);
		appStatus = true;
	}

	if (app_Id == 3)
	{
		if (intParams[0] == 0)moveRect(roi_exp3, 0);
		if (intParams[0] == 1)moveRect(roi_exp3, 1);
		if (intParams[0] == 2)moveRect(roi_exp3, 2);
		if (intParams[0] == 3)moveRect(roi_exp3, 3);
		if (intParams[0] == 4)moveRect(roi_exp3, 4);

		string path = setRect(image_read_exp3_src, image_read_exp3_tmp, roi_exp3);
		stringReplace(path, "\\", "/");
		tmp_src_img_path = QString::fromStdString(path);
		appStatus = true;
	}

	if (app_Id == 4)
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
				rgb =  intParams[2] == 1 ? true : false;
			}
		}
		string path = TrainModel(hog, rgb, floatParams[0]);
		stringReplace(path, "\\", "/");
		tmp_pro_img_path = QString::fromStdString(path);
		appStatus = true;
	}
}