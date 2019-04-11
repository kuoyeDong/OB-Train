#include "PersonExperiment.hpp"
#include "../src/tracking/kcftracker.hpp"
#include "../Tools/Tools.hpp"

using namespace std;
using namespace cv;

Mat image_read_exp2_src;
Mat image_read_exp2_tmp;
Mat image_crop_exp2;

Rect roi_exp2 = Rect(270, 26, 175, 350);

float viz_factor = 2.0f;
int scaleFactor = 3;

string image_fullpath_exp2 = "";
string save_fullpath_exp2 = "";

void PersonExperiment::exp2Param(int para)
{
	intParams.clear();
	intParams.push_back(para);
}

string getHist(int currentaction)
{
	if (!image_read_exp2_src.empty())
	{
		vector<Mat> rgb_planes;
		split(image_read_exp2_src, rgb_planes);

		int histSize = 10;
		float range[] = { 0, 255 };
		const float* histRange = { range };
		bool uniform = true;
		bool accumulate = false;

		Mat r_hist, g_hist, b_hist;

		// 计算直方图:  
		calcHist(&rgb_planes[0], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);
		calcHist(&rgb_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
		calcHist(&rgb_planes[2], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);

		// 创建直方图画布  
		int hist_w = 650;
		int hist_h = 650;
		int bin_w = cvRound((double)hist_w / histSize);

		Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(144, 144, 144));

		// 将直方图归一化到范围 [ 0, histImage.rows ]  
		normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
		normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
		normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

		// 在直方图画布上画出直方图  
		for (int i = 1; i < histSize; i++)
		{
			line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
				Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
				Scalar(0, 0, 255), 2, 8, 0);
			line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
				Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
				Scalar(0, 255, 0), 2, 8, 0);
			line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
				Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
				Scalar(255, 0, 0), 2, 8, 0);
		}
		string savepath = GetProgramDir() + "/" + "modify_pro_img.jpg";
		imwrite(savepath, histImage);
		string path = "file:///" + savepath;

		//save data to txt
		if (!save_fullpath_exp2.empty())
		{
			string action;
			if (currentaction == 0)action = "dark";
			if (currentaction == 1)action = "light";
			string image_name = filename(image_fullpath_exp2);
			string data_save_file = save_fullpath_exp2 + "/" + image_name + "_RGB_hist.txt";
			writeMatToFile(r_hist, data_save_file.c_str(), action, true);
			writeMatToFile(g_hist, data_save_file.c_str(), action, false);
			writeMatToFile(b_hist, data_save_file.c_str(), action, false);
		}
		return path;
	}
}

string hogVisualize()
{
	if (!image_read_exp2_src.empty())
	{
		image_read_exp2_tmp = image_read_exp2_src.clone();

		Rect roi_tmp = roi_exp2;
		if (roi_exp2.x + roi_exp2.width > image_read_exp2_tmp.cols - 1)
		{
			roi_tmp.x = roi_exp2.x;
			roi_tmp.width = image_read_exp2_tmp.cols - 1 - roi_exp2.x;
		}
		if (roi_exp2.y + roi_exp2.height > image_read_exp2_tmp.rows - 1)
		{
			roi_tmp.y = roi_exp2.y;
			roi_tmp.height = image_read_exp2_tmp.rows - 1 - roi_exp2.y;
		}
		if (roi_exp2.x < 0)
		{
			roi_tmp.x = 0;
		}
		if (roi_exp2.y < 0)
		{
			roi_tmp.y = 0;
		}
		
		if (roi_tmp.width > 0 && roi_tmp.height > 0)
		{
			image_crop_exp2 = image_read_exp2_tmp(roi_tmp);
			HOGDescriptor hog;
			hog.winSize = Size(64, 128);
			vector<float> des;
			Mat crop_tmp;
			resize(image_crop_exp2, crop_tmp, Size(64, 128));
			hog.compute(crop_tmp, des);

			Mat frame(crop_tmp.rows*scaleFactor, crop_tmp.cols*scaleFactor, CV_8UC3, Scalar(0, 0, 0));

			int gradientBinSize = 9;
			// dividing 180° into 9 bins, how large (in rad) is one bin?
			float radRangeForOneBin = 3.1415926 / (float)gradientBinSize;

			// prepare data structure: 9 orientation / gradient strenghts for each cell
			int cells_in_x_dir = hog.winSize.width / hog.cellSize.width;
			int cells_in_y_dir = hog.winSize.height / hog.cellSize.height;
			int totalnrofcells = cells_in_x_dir * cells_in_y_dir;
			float*** gradientStrengths = new float**[cells_in_y_dir];
			int** cellUpdateCounter = new int*[cells_in_y_dir];
			for (int y = 0; y < cells_in_y_dir; y++)
			{
				gradientStrengths[y] = new float*[cells_in_x_dir];
				cellUpdateCounter[y] = new int[cells_in_x_dir];
				for (int x = 0; x < cells_in_x_dir; x++)
				{
					gradientStrengths[y][x] = new float[gradientBinSize];
					cellUpdateCounter[y][x] = 0;

					for (int bin = 0; bin < gradientBinSize; bin++)
						gradientStrengths[y][x][bin] = 0.0;
				}
			}

			// nr of blocks = nr of cells - 1
			// since there is a new block on each cell (overlapping blocks!) but the last one
			int blocks_in_x_dir = cells_in_x_dir - 1;
			int blocks_in_y_dir = cells_in_y_dir - 1;

			// compute gradient strengths per cell
			int descriptorDataIdx = 0;
			int cellx = 0;
			int celly = 0;

			for (int blockx = 0; blockx < blocks_in_x_dir; blockx++)
			{
				for (int blocky = 0; blocky < blocks_in_y_dir; blocky++)
				{
					// 4 cells per block ...
					for (int cellNr = 0; cellNr < 4; cellNr++)
					{
						// compute corresponding cell nr
						int cellx = blockx;
						int celly = blocky;
						if (cellNr == 1) celly++;
						if (cellNr == 2) cellx++;
						if (cellNr == 3)
						{
							cellx++;
							celly++;
						}

						for (int bin = 0; bin < gradientBinSize; bin++)
						{
							float gradientStrength = des[descriptorDataIdx];
							descriptorDataIdx++;

							gradientStrengths[celly][cellx][bin] += gradientStrength;

						} // for (all bins)


					// note: overlapping blocks lead to multiple updates of this sum!
					// we therefore keep track how often a cell was updated,
					// to compute average gradient strengths
						cellUpdateCounter[celly][cellx]++;

					} // for (all cells)
				} // for (all block x pos)
			} // for (all block y pos)


		// compute average gradient strengths
			for (int celly = 0; celly < cells_in_y_dir; celly++)
			{
				for (int cellx = 0; cellx < cells_in_x_dir; cellx++)
				{

					float NrUpdatesForThisCell = (float)cellUpdateCounter[celly][cellx];

					// compute average gradient strenghts for each gradient bin direction
					for (int bin = 0; bin < gradientBinSize; bin++)
					{
						gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;
					}
				}
			}


			//cout << "descriptorDataIdx = " << descriptorDataIdx << endl;

			// draw cells
			for (int celly = 0; celly < cells_in_y_dir; celly++)
			{
				for (int cellx = 0; cellx < cells_in_x_dir; cellx++)
				{
					int drawX = cellx * hog.cellSize.width;
					int drawY = celly * hog.cellSize.height;

					int mx = drawX + hog.cellSize.width / 2;
					int my = drawY + hog.cellSize.height / 2;

					//rectangle(visual_image,
					//	Point(drawX*scaleFactor, drawY*scaleFactor),
					//	Point((drawX + cellSize.width)*scaleFactor,
					//	(drawY + cellSize.height)*scaleFactor),
					//	CV_RGB(100, 100, 100),
					//	1);

					// draw in each cell all 9 gradient strengths
					for (int bin = 0; bin < gradientBinSize; bin++)
					{
						float currentGradStrength = gradientStrengths[celly][cellx][bin];

						// no line to draw?
						if (currentGradStrength == 0)
							continue;

						float currRad = bin * radRangeForOneBin + radRangeForOneBin / 2;

						float dirVecX = cos(currRad);
						float dirVecY = sin(currRad);
						float maxVecLen = hog.cellSize.width / 2;
						float scale = viz_factor; // just a visual_imagealization scale,
						// to see the lines better

						// compute line coordinates
						float x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
						float y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
						float x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
						float y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;

						// draw gradient visual_imagealization
						line(frame,
							Point(x1*scaleFactor, y1*scaleFactor),
							Point(x2*scaleFactor, y2*scaleFactor),
							CV_RGB(255, 255, 255), 1);

					} // for (all bins)

				} // for (cellx)
			} // for (celly)


		// don't forget to free memory allocated by helper data structures!
			for (int y = 0; y < cells_in_y_dir; y++)
			{
				for (int x = 0; x < cells_in_x_dir; x++)
				{
					delete[] gradientStrengths[y][x];
				}
				delete[] gradientStrengths[y];
				delete[] cellUpdateCounter[y];
			}
			delete[] gradientStrengths;
			delete[] cellUpdateCounter;

			//save data to txt
			if (!save_fullpath_exp2.empty())
			{
				string image_name = filename(image_fullpath_exp2);
				string data_save_file = save_fullpath_exp2 + "/" + image_name + "_hog_feature.txt";

				std::ofstream fout;
				fout.open(data_save_file.c_str(), ios::out | ios::app);
				if (fout)
				{
					fout << "hog :" << endl;
					for (int i = 0; i < des.size(); i++)
					{
						fout << des[i] << " ";
					}
					fout << endl;
					fout.close();
				}
			}

			string savepath = GetProgramDir() + "/" + "modify_pro_img.jpg";
			resize(frame, frame, Size(650, 650));
			imwrite(savepath, frame);
			string path = "file:///" + savepath;
			return path;
		}
		else
		{
			string path = "";
			return path;
		}
	}
}

string getLinearModel()
{
	if (!image_read_exp2_src.empty())
	{
		image_read_exp2_tmp = image_read_exp2_src.clone();

		Rect roi_tmp = roi_exp2;
		if (roi_exp2.x + roi_exp2.width > image_read_exp2_tmp.cols - 1)
		{
			roi_tmp.x = roi_exp2.x;
			roi_tmp.width = image_read_exp2_tmp.cols - 1 - roi_exp2.x;
		}
		if (roi_exp2.y + roi_exp2.height > image_read_exp2_tmp.rows - 1)
		{
			roi_tmp.y = roi_exp2.y;
			roi_tmp.height = image_read_exp2_tmp.rows - 1 - roi_exp2.y;
		}
		if (roi_exp2.x < 0)
		{
			roi_tmp.x = 0;
		}
		if (roi_exp2.y < 0)
		{
			roi_tmp.y = 0;
		}
		KCFTracker tracker(true, false, true, true);
		tracker.init(roi_tmp, image_read_exp2_tmp);

		string savepath = GetProgramDir() + "/" + "modify_pro_img.jpg";
		Mat model = tracker._tmpl;
		model.convertTo(model, CV_8U, 255);
		imwrite(savepath, model);
		string path = "file:///" + savepath;

		//save data to txt
		if (!save_fullpath_exp2.empty())
		{
			string action = "model";
			string image_name = filename(image_fullpath_exp2);
			string data_save_file = save_fullpath_exp2 + "/" + image_name + "_model.txt";
			writeMatToFile(model, data_save_file.c_str(), action, true);
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
// get hist from RGB frame : 4
// get hog feature : 5
// train the feature : 6
void PersonExperiment::experiment_two(string image_path, string save_path, cv::Mat &frame, int app_Id)
{
	if (!save_path.empty()) {
		save_fullpath_exp2 = save_path;
	}

	if (!image_path.empty() && image_read_exp2_src.empty())
	{
		image_fullpath_exp2 = image_path;
		image_read_exp2_src = imread(image_path);
		image_read_exp2_tmp = image_read_exp2_src.clone();
		appStatus = true;
	}

	if(!image_path.empty() && app_Id == 0 && !image_read_exp2_src.empty())
	{
		image_fullpath_exp2 = image_path;
		image_read_exp2_src = imread(image_path);
		image_read_exp2_tmp = image_read_exp2_src.clone();
		appStatus = true;
	}

	if (app_Id == 1)
	{
		if (intParams[0] == 1)
		{
			string path = lightDark(image_read_exp2_src, 20);
		    stringReplace(path, "\\", "/");
			tmp_src_img_path = QString::fromStdString(path);
		}
		if (intParams[0] == 0)
		{
			string path = lightDark(image_read_exp2_src , -20);
			stringReplace(path, "\\", "/");
			tmp_src_img_path = QString::fromStdString(path);
		}
		appStatus = true;
	}

	if (app_Id == 2)
	{
		string path = imgFlip(image_read_exp2_src);
		stringReplace(path, "\\", "/");
		tmp_src_img_path = QString::fromStdString(path);
		appStatus = true;
	}

	if (app_Id == 3)
	{
		if (intParams[0] == 0)moveRect(roi_exp2, 0);
		if (intParams[0] == 1)moveRect(roi_exp2, 1);
		if (intParams[0] == 2)moveRect(roi_exp2, 2);
		if (intParams[0] == 3)moveRect(roi_exp2, 3);
		if (intParams[0] == 4)moveRect(roi_exp2, 4);

		string path = setRect(image_read_exp2_src, image_read_exp2_tmp, roi_exp2);
		stringReplace(path, "\\", "/");
		tmp_src_img_path = QString::fromStdString(path);
		appStatus = true;
	}

	if (app_Id == 4)
	{
		string path;
		if (intParams.empty())
		{
			path = getHist(-1);
		}
		else
		{
			path = getHist(intParams[0]);
		}
		stringReplace(path, "\\", "/");
		tmp_pro_img_path = QString::fromStdString(path);
		appStatus = true;
	}

	if (app_Id == 5)
	{
		string path = hogVisualize();
		stringReplace(path, "\\", "/");
		tmp_pro_img_path = QString::fromStdString(path);
		appStatus = true;
	}

	if (app_Id == 6)
	{
		string path = getLinearModel();
		stringReplace(path, "\\", "/");
		tmp_pro_img_path = QString::fromStdString(path);
		appStatus = true;
	}
}