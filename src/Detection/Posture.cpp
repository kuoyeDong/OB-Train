#include "Posture.h"
#include "detection.h"
#include "../ZhSupport.h"

#include <ctime>

using namespace std;
using namespace cv;

Detector detector;

int posture_count = 0;
int human_count = 0;
string label = "";
int pre_label_id = -1;
int current_label_id = -1;
float mean_score = 0.0f;
float max_score = 0.0f;
bool is_human = false;
bool posture_done = false;
Rect human_result = Rect(-1, -1, 0, 0);
int mis_recognition = 0;
float timer = 0.0f;
float time_redetection = 1;

float recog_num = 10.0f;
double recognition_thres = 0.95;
int imgWidth = 224;
int imgHeight = 224;
string input_layer_recoginition = "input:0";
vector<string> output_layer_recoginition = { "MobilenetV1/Predictions/Reshape_1:0" };
vector<string> labels = { "Hor", "I", "Photo", "TurnY", "Y" };
vector<string> labels_zh = { "ÀÆ∆Ω", "¥π÷±", "’’œ‡", "µπY–Õ", "Y–Õ" };
vector<float> all_scores;

int detect_num = 5;
double thresholdScore = 0.2;
double thresholdIOU = 0.9;
vector<float> outBoxes;
vector<float> outScores;
vector<size_t> outLabels;
string input_layer_detection = "image_tensor:0";
vector<string> output_layer_detection = { "detection_boxes:0", "detection_scores:0", "detection_classes:0" };

clock_t t1, t2;


bool detector_init()
{
	//int status_detect_model = detector.load_model_detection("./release/Content resource/Posture/data/INRIA-80000/frozen_inference_graph.pb");
	//int status_recognize_model = detector.load_model_recognition("./release/Content resource/Posture/data/PostureV3/frozen_graph.pb");
	int status_detect_model = detector.load_model_detection("./Content_resource/Posture/data/INRIA-80000/frozen_inference_graph.pb");
	int status_recognize_model = detector.load_model_recognition("./Content_resource/Posture/data/PostureV3/frozen_graph.pb");
	if (status_detect_model == 0 && status_recognize_model == 0)
		return true;
	else
		return false;
}

void posture_recognize(cv::Mat& frame)
{
	if (frame.empty())
		return;

	cv::Mat frame_copy = frame.clone();
	float possibility;
	string pos;

	if (posture_done)
	{
		t1 = clock();
		Sleep(10);
		t2 = clock();
		timer += float(t2 - t1) / CLOCKS_PER_SEC;
		if (timer >= 2)
		{
			posture_done = false;
		}
	}
	else
	{
		if (!is_human)
		{
			//putTextZH(frame, "Œ¥ºÏ≤‚µΩ––»À", Point(30, 20), Scalar(255, 0, 0), 25, "Œ¢»Ì—≈∫⁄");
			int detectStatus = detector.detect(frame_copy, thresholdScore, thresholdIOU,
				input_layer_detection, output_layer_detection,
				outBoxes, outScores, outLabels);
			if (detectStatus >= 0 && outScores.size() > 0)
			{
				human_count++;
				human_result.y = outBoxes[0];
				human_result.x = outBoxes[1];
				human_result.height = outBoxes[2] - outBoxes[0];
				human_result.width = outBoxes[3] - outBoxes[1];

				//if (human_result.width != 0 && human_result.height != 0)
				//{
				//	rectangle(frame, human_result, Scalar(0, 255, 0), 4);
				//}

				if (human_count == detect_num)
				{
					human_count = 0;
					is_human = true;
					t1 = clock();
				}
			}
		}

		if (is_human)
		{
			if (human_result.width != 0 && human_result.height != 0)
			{
				rectangle(frame, human_result, Scalar(0, 255, 0), 4);
			}

			int recogStatus = detector.recognize(frame, recognition_thres,
				input_layer_recoginition, output_layer_recoginition, all_scores,
				current_label_id, max_score, imgWidth, imgHeight);
			if (recogStatus >= 0)
			{
				if (current_label_id != -1)
				{
					if (current_label_id == 3)
					{
						if (max_score >= 0.995) 
						{
							possibility = max_score * 100;
							pos = "ø…ƒ‹–‘:  " + to_string(possibility) + "%";
							putTextZH(frame, labels_zh[current_label_id].c_str(), Point(30, 20), Scalar(255, 0, 0), 25, "Œ¢»Ì—≈∫⁄");
							putTextZH(frame, pos.c_str(), Point(30, 60), Scalar(255, 0, 0), 25, "Œ¢»Ì—≈∫⁄");
						}
					}
					else if(current_label_id == 4)
					{
						if (max_score >= 0.99)
						{
							possibility = max_score * 100;
							pos = "ø…ƒ‹–‘:  " + to_string(possibility) + "%";
							putTextZH(frame, labels_zh[current_label_id].c_str(), Point(30, 20), Scalar(255, 0, 0), 25, "Œ¢»Ì—≈∫⁄");
							putTextZH(frame, pos.c_str(), Point(30, 60), Scalar(255, 0, 0), 25, "Œ¢»Ì—≈∫⁄");
						}
					}
					else
					{
						possibility = max_score * 100;
						pos = "ø…ƒ‹–‘:  " + to_string(possibility) + "%";
						putTextZH(frame, labels_zh[current_label_id].c_str(), Point(30, 20), Scalar(255, 0, 0), 25, "Œ¢»Ì—≈∫⁄");
						putTextZH(frame, pos.c_str(), Point(30, 60), Scalar(255, 0, 0), 25, "Œ¢»Ì—≈∫⁄");
					}
					//putText(frame, labels[current_label_id], Point(30, 30), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0), 2);
					//putText(frame, to_string(max_score), Point(30, 70), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0), 2);
					//putText(frame, to_string(all_scores[2]), Point(30, 110), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0), 2);
					//putText(frame, to_string(all_scores[3]), Point(30, 150), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0), 2);
					//putText(frame, to_string(all_scores[4]), Point(30, 190), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0), 2);
				}
				else
				{
					putTextZH(frame, "√ª”–ºÏ≤‚µΩ◊ÀÃ¨", Point(30, 20), Scalar(255, 0, 0), 25, "Œ¢»Ì—≈∫⁄");
				}
			}
			t2 = clock();
			timer = double(t2 - t1) / CLOCKS_PER_SEC;
			if (timer >= time_redetection)
			{
				is_human = false;
				human_result = Rect(-1, -1, 0, 0);
			}
		}

		//if (!is_human)
		//{
		//	int detectStatus = detector.detect(frame_copy, thresholdScore, thresholdIOU,
		//		input_layer_detection, output_layer_detection,
		//		outBoxes, outScores, outLabels);
		//	if (detectStatus >= 0 && outScores.size() > 0)
		//	{
		//		human_count++;
		//		human_result.y = outBoxes[0];
		//		human_result.x = outBoxes[1];
		//		human_result.height = outBoxes[2] - outBoxes[0];
		//		human_result.width = outBoxes[3] - outBoxes[1];

		//		if (human_result.width != 0 && human_result.height != 0)
		//		{
		//			rectangle(frame, human_result, Scalar(0, 255, 0), 4);
		//		}

		//		putText(frame, "One Person", Point(30, 30), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0), 2);
		//		if (human_count == detect_num)
		//		{
		//			is_human = true;
		//			//putText(frame, "Person Over", Point(30, 30), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0), 2);
		//			//Sleep(2000);
		//			human_count = 0;
		//			//human_result = Rect(-1, -1, 0, 0);
		//		}
		//	}
		//	else if (outScores.size() > 1)
		//	{
		//		putText(frame, "More Than One Person", Point(30, 30), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0), 2);
		//	}
		//}
		//else
		//{
		//	putText(frame, "Start Reco", Point(30, 30), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0), 2);
		//	//Sleep(500);
		//	int recogStatus = detector.recognize(frame, recognition_thres,
		//		input_layer_recoginition, output_layer_recoginition,
		//		current_label_id, max_score, imgWidth, imgHeight);
		//	if (recogStatus >= 0)
		//		putText(frame, to_string(current_label_id), Point(30, 70), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0), 2);
			//Sleep(500);
			//if (recogStatus >= 0 && current_label_id != -1)
			//{
			//	putText(frame, to_string(current_label_id), Point(30, 70), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0), 2);
			//	Sleep(1000);
			//	if (pre_label_id == -1)
			//	{
			//		mean_score += max_score;
			//		pre_label_id = current_label_id;
			//		posture_count++;
			//	}
			//	else if (pre_label_id != -1 && pre_label_id == current_label_id)
			//	{
			//		mis_recognition = 0;
			//		posture_count++;
			//		mean_score += max_score;
			//		if (posture_count == recog_num)
			//		{
			//			detector_clear();
			//			mean_score /= recog_num;
			//			posture_done = true;
			//			if (mean_score >= recognition_thres)
			//			{
			//				putText(frame, labels[pre_label_id], Point(30, 30), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255), 2);
			//				Sleep(2000);
			//				switch (pre_label_id) {
			//				case 0:
			//					break;
			//				case 1:
			//					break;
			//				case 2:
			//					take_photo(frame);
			//					break;
			//				case 3:
			//					break;
			//				case 4:
			//					break;
			//				}
			//			}
			//		}
			//	}
			//	else
			//	{
			//		putText(frame, "miss", Point(30, 30), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255), 2);
			//		Sleep(2000);
			//		mis_recognition++;
			//		if (mis_recognition > 3)
			//			detector_clear();
			//	}
			//}
		//}
	}
}

void take_photo(Mat frame)
{

}

void video_record()
{

}

void detector_clear()
{
	posture_count = 0;
	human_count = 0;
	label = "";
	pre_label_id = -1;
	current_label_id = -1;
	mean_score = 0.0f;
	max_score = 0.0f;
	is_human = false;
	human_result = Rect(-1, -1, 0, 0);
	mis_recognition = 0;
}