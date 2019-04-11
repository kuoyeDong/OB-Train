#pragma once
#ifndef DETECTION_H
#define DETECTION_H

#include "TensorflowConfig.h"
#include <opencv2/opencv.hpp>
#include <iostream>

class Detector
{
public:
	std::unique_ptr<tensorflow::Session> session_detection;
	std::unique_ptr<tensorflow::Session> session_recognition;

public:
	Detector();
	~Detector();

	int load_model_detection(std::string modelPath);
	int load_model_recognition(std::string modelPath);

	int detect(cv::Mat& frame, double thresholdScore, double thresholdIOU, 
		            std::string& input_layer, std::vector<std::string>& outputLayer,
		            std::vector<float> &boxes, std::vector<float> &scores, std::vector<size_t> &labels);

	int recognize(cv::Mat& frame, double thresholdScore,
		                 std::string& input_layer, std::vector<std::string>& outputLayer, std::vector<float>& all_scores, 
		                 int& label_id, float& max_score, int imgWidth, int imgHeight);

private:

	// load the tensorflow graph
	tensorflow::Status loadGraph(const std::string &graph_file_name, std::unique_ptr<tensorflow::Session> *session);

	// convet the Mat to Tensor
	tensorflow::Tensor readTensorFromMat_Detect(const cv::Mat &mat);
	tensorflow::Tensor readTensorFromMat_recognize(const cv::Mat &mat, int imgWidth, int imgHeight);

	// for detection
	double IOU(cv::Rect2f box1, cv::Rect2f box2);
	// for detection
	std::vector<size_t> filterBoxes(tensorflow::TTypes<float>::Flat &scores,
		tensorflow::TTypes<float, 3>::Tensor &boxes, double thresholdScore, double thresholdIOU);

};


#endif // DETECTION_H