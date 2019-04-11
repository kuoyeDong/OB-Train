#include "PersonExperiment.hpp"
#include "../Tools/Tools.hpp"
#include "../src/Detection/detection.h"
#include "../src/ZhSupport.h"
#include <time.h>

using namespace std;
using namespace cv;

string dstPath = "";
float sleep_time = 0.0f;
int durationTime = 5;
int fps = 10;
int frameCount = 0;
int pic_index = 0;
time_t t1, t2;
bool writeImg = true;

Detector detect_video;
int detectNum = 5;
double detectScore = 0.2;
double detectIOU = 0.9;
vector<float> Boxes;
vector<float> Scores;
vector<size_t> Labels;
Rect result;
string input_layer = "image_tensor:0";
vector<string> output_layer = { "detection_boxes:0", "detection_scores:0", "detection_classes:0" };
int missdetect = 0;
bool isInit = false;

bool detectorInit()
{
	int status_detect_model = detect_video.load_model_detection("./Content_resource/Posture/data/INRIA-80000/frozen_inference_graph.pb");
	int status_recognize_model = detect_video.load_model_recognition("./Content_resource/Posture/data/PostureV3/frozen_graph.pb");
	if (status_detect_model == 0 && status_recognize_model == 0)
		return true;
	else
		return false;
}

bool video_recode(string& savePath, Mat frame)
{
	Mat frame_write;
	cvtColor(frame, frame_write, CV_BGRA2RGB);

	if (frameCount == 0)
	{
		dstPath = create_dir(savePath);
		sleep_time = 1000.0f / fps;
		frameCount++;
	}
	else
	{
		frameCount++;
	}

	if (writeImg)
	{
		t1 = clock();
		string imgPath = dstPath + "/" + to_string(pic_index + 1) + ".jpg";
		imwrite(imgPath, frame_write);
		writeImg = false;
		pic_index++;
	}
	else
	{
		int detectStatus = detect_video.detect(frame_write, detectScore, detectIOU, input_layer, output_layer, Boxes, Scores, Labels);
		if (detectStatus >= 0)
		{
			if (Scores.size() > 1)
			{
				putTextZH(frame, "注意：检测到两个以上行人！", Point(30, 60), Scalar(255, 0, 0), 25, "微软雅黑");
			}
			if (Scores.size() == 0)
			{
				//missdetect++;
				//if (missdetect == 3)
				//{
				//	putTextZH(frame, "注意：未检测到行人！", Point(30, 60), Scalar(255, 0, 0), 25, "微软雅黑");
				//}
				putTextZH(frame, "注意：未检测到行人！", Point(30, 60), Scalar(255, 0, 0), 25, "微软雅黑");
			}
			if (Scores.size() == 1)
			{
				result.y = Boxes[0];
				result.x = Boxes[1];
				result.height = Boxes[2] - Boxes[0];
				result.width = Boxes[3] - Boxes[1];
				rectangle(frame, result, Scalar(0, 255, 0), 4);
				missdetect = 0;
			}
		}
	}

	t2 = clock();

	float current_time = float(t2 - t1) / CLOCKS_PER_SEC * 1000;

	if (current_time >= sleep_time)
	{
		writeImg = true;
	}

	if(pic_index >= durationTime * fps)
	{
		frameCount = 0;
		pic_index = 0;
		writeImg = true;
		missdetect = 0;
		Boxes.clear();
		Scores.clear();
		Labels.clear();
		result = Rect(-1, -1, 0, 0);
		return false;
	}
	return true;
}

void PersonExperiment::exp1Param(QString time, QString fps)
{
	intParams.clear();
	intParams.push_back(time.toInt());
	intParams.push_back(fps.toInt());
}

// app ID:
// recore video : 1
void PersonExperiment::experiment_one(string video_path, Mat &frame, int app_Id)
{
	if (!isInit)
	{
		detectorInit();
		isInit = true;
	}

	if (!intParams.empty() && intParams.size() <= 2)
	{
		durationTime = intParams[0];
		fps = intParams[1];
		intParams.clear();
	}
	if (app_Id == 1)
	{
		if (!video_path.empty())
		{
			bool status = video_recode(video_path, frame);
			if (!status)
			{
				appStatus = false;
				setEx_id(0, 0);
				intParams.clear();
			}
			else
			{
				appStatus = true;
			}
		}
		else
		{
			setEx_id(0, -1);
			intParams.clear();
		}
	}
}