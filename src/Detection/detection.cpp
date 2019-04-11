#include "detection.h"

#include <fstream>
#include <utility>
#include <iostream>
#include <numeric> 
#include <algorithm>

using tensorflow::Flag;
using tensorflow::Tensor;
using tensorflow::Status;
using tensorflow::string;
using tensorflow::int32;

using namespace std;
using namespace cv;

Detector::Detector()
{
}

Detector::~Detector()
{
}

int Detector::load_model_detection(std::string modelPath)
{
	Status loadGraphStatus = loadGraph(modelPath, &(this->session_detection));
	if (!loadGraphStatus.ok()) {
		LOG(ERROR) << "loadGraph(): ERROR" << loadGraphStatus;
		return -1;
	}
	else {
		LOG(INFO) << "loadGraph(): Model Loaded" << endl;
		return 0;
	}
}

int Detector::load_model_recognition(std::string modelPath)
{
	Status loadGraphStatus = loadGraph(modelPath, &(this->session_recognition));
	if (!loadGraphStatus.ok()) {
		LOG(ERROR) << "loadGraph(): ERROR" << loadGraphStatus;
		return -1;
	}
	else {
		LOG(INFO) << "loadGraph(): Model Loaded" << endl;
		return 0;
	}
}

int Detector::detect(Mat& frame, double thresholdScore, double thresholdIOU,
	                           std::string& input_layer, vector<string>& output_layer,
	                           vector<float> &outBoxes, vector<float> &outScores, vector<size_t> &outLabels)
{
	int width = frame.cols;
	int height = frame.rows;
	tensorflow::TensorShape shape = tensorflow::TensorShape();
	shape.AddDim(1);
	shape.AddDim(height);
	shape.AddDim(width);
	shape.AddDim(3);
	Tensor inputTensor;
	inputTensor = readTensorFromMat_Detect(frame);
	// run graph on tensor
	//string inputLayer = "image_tensor:0";
	//vector<string> outputLayer = { "detection_boxes:0", "detection_scores:0", "detection_classes:0" };
	outBoxes.clear();
	outScores.clear();
	outLabels.clear();
	vector<Tensor> outputs;
	Status runStatus = this->session_detection->Run({ {input_layer, inputTensor} }, output_layer, {}, &outputs);
	if (!runStatus.ok()) {
		LOG(ERROR) << "Running model failed: " << runStatus;
		return -1;
	}
	else {
		LOG(INFO) << "Running graph done!";
	}

	// extract results
	tensorflow::TTypes<float, 3>::Tensor boxes = outputs[0].flat_outer_dims<float, 3>();
	tensorflow::TTypes<float>::Flat scores = outputs[1].flat<float>();
	tensorflow::TTypes<float>::Flat labels = outputs[2].flat<float>();
	vector<size_t> goodIdxs = filterBoxes(scores, boxes, thresholdScore, thresholdIOU);
	for (size_t i = 0; i != goodIdxs.size(); i++)
	{
		outScores.push_back(scores(goodIdxs.at(i)));
		outLabels.push_back(labels(goodIdxs.at(i)));
		outBoxes.push_back(boxes(0, goodIdxs.at(i), 0) * height);
		outBoxes.push_back(boxes(0, goodIdxs.at(i), 1) * width);
		outBoxes.push_back(boxes(0, goodIdxs.at(i), 2) * height);
		outBoxes.push_back(boxes(0, goodIdxs.at(i), 3) * width);
	}
	//LOG(INFO) << "outBoxes info: " << outBoxes.size();
	return 0;
}

int Detector::recognize(cv::Mat& frame, double thresholdScore, 
	                                 std::string& input_layer, vector<string>& output_layer, std::vector<float>& all_scores,
	                                 int& label_id, float& max_score, int imgWidth, int imgHeight)
{
	tensorflow::TensorShape shape = tensorflow::TensorShape();
	shape.AddDim(1);
	shape.AddDim(imgHeight);
	shape.AddDim(imgWidth);
	shape.AddDim(3);
	Tensor inputTensor;
	inputTensor = readTensorFromMat_recognize(frame, imgWidth, imgHeight);
	// run graph on tensor
	//string inputLayer = "input:0";
	//vector<string> outputLayer = { "MobilenetV1/Predictions/Reshape_1:0" };
	vector<Tensor> outputs;
	Status runStatus = this->session_recognition->Run({ {input_layer, inputTensor} }, output_layer, {}, &outputs);
	if (!runStatus.ok()) {
		LOG(ERROR) << "Running model failed: " << runStatus;
		return -1;
	}
	else {
		LOG(INFO) << "Running graph done!";
	}

	// extract results
	all_scores.clear();
	tensorflow::TTypes<float>::Flat scores = outputs[0].flat<float>();
	for (int i = 0; i < 5; i++)
	{
		all_scores.push_back(scores(i));
	}

	label_id = max_element(all_scores.begin(), all_scores.end()) - all_scores.begin();
	max_score = *max_element(all_scores.begin(), all_scores.end());

	label_id = max_score > thresholdScore ? label_id : -1;
	max_score = max_score > thresholdScore ? max_score : 0;

	return 0;
}


/*************************************************     private     *************************************************/
// load the tensorflow graph
Status Detector::loadGraph(const std::string &graph_file_name, std::unique_ptr<tensorflow::Session> *session)
{
	tensorflow::GraphDef graph_def;
	Status load_graph_status =
		ReadBinaryProto(tensorflow::Env::Default(), graph_file_name, &graph_def);
	if (!load_graph_status.ok()) {
		return tensorflow::errors::NotFound("Failed to load compute graph at '",
			graph_file_name, "'");
	}
	session->reset(tensorflow::NewSession(tensorflow::SessionOptions()));
	Status session_create_status = (*session)->Create(graph_def);
	if (!session_create_status.ok()) {
		return session_create_status;
	}
	return Status::OK();
}

// convet the Mat to Tensor (for detection)
Tensor Detector::readTensorFromMat_Detect(const cv::Mat &mat)
{
	int height = mat.rows;
	int width = mat.cols;
	int depth = mat.channels();
	Tensor inputTensor(tensorflow::DT_UINT8, tensorflow::TensorShape({ 1, height, width, depth }));
	auto inputTensorMapped = inputTensor.tensor<tensorflow::uint8, 4>();

	cv::Mat frame;
	mat.convertTo(frame, CV_8UC3);
	const tensorflow::uint8* source_data = (tensorflow::uint8*)frame.data;
	for (int y = 0; y < height; y++) {
		const tensorflow::uint8* source_row = source_data + (y*width*depth);
		for (int x = 0; x < width; x++) {
			const tensorflow::uint8* source_pixel = source_row + (x*depth);
			for (int c = 0; c < depth; c++) {
				const tensorflow::uint8* source_value = source_pixel + c;
				inputTensorMapped(0, y, x, c) = *source_value;
			}
		}
	}
	return inputTensor;
}

// convet the Mat to Tensor (for recognition)
Tensor Detector::readTensorFromMat_recognize(const Mat &mat, int imgWidth, int imgHeight)
{
	cv::Mat frame;
	cv::resize(mat, frame, cv::Size(imgWidth, imgWidth));
	frame.convertTo(frame, CV_32F, 1 / 255.0, -1);

	int width = frame.cols;
	int height = frame.rows;
	int channel = frame.channels();
	tensorflow::Tensor inputTensor(tensorflow::DT_FLOAT, tensorflow::TensorShape({ 1 ,height, width, channel }));
	auto input_tensor_map = inputTensor.tensor<float, 4>();

	const float* source_row;
	const float* source_pixel;
	const float* source_value;

	for (int y = 0; y < height; y++) {
		source_row = (float*)frame.data + (y * width * channel);
		for (int x = 0; x < width; x++) {
			source_pixel = source_row + (x * channel);
			for (int c = 0; c < channel; c++) {
				source_value = source_pixel + c;
				input_tensor_map(0, y, x, c) = *source_value;
			}
		}
	}
	return inputTensor;
}

// for detection
double Detector::IOU(cv::Rect2f box1, cv::Rect2f box2)
{
	float xA = max(box1.tl().x, box2.tl().x);
	float yA = max(box1.tl().y, box2.tl().y);
	float xB = min(box1.br().x, box2.br().x);
	float yB = min(box1.br().y, box2.br().y);

	float intersectArea = abs((xB - xA) * (yB - yA));
	float unionArea = abs(box1.area()) + abs(box2.area()) - intersectArea;

	return 1. * intersectArea / unionArea;
}

// for detection
vector<size_t> Detector::filterBoxes(tensorflow::TTypes<float>::Flat &scores,
	tensorflow::TTypes<float, 3>::Tensor &boxes, double thresholdScore, double thresholdIOU)
{
	vector<size_t> sortIdxs(scores.size());
	iota(sortIdxs.begin(), sortIdxs.end(), 0);

	// Create set of "bad" idxs
	set<size_t> badIdxs = set<size_t>();
	size_t i = 0;
	while (i < sortIdxs.size()) {
		// get bad idx with low score
		if (scores(sortIdxs.at(i)) < thresholdScore) {
			badIdxs.insert(sortIdxs.at(i));
		}
		if (badIdxs.find(sortIdxs.at(i)) != badIdxs.end()) {
			i++;
			continue;
		}
		// get bad idx with high iou
		Rect2f box1 = Rect2f(Point2f(boxes(0, sortIdxs.at(i), 1), boxes(0, sortIdxs.at(i), 0)),
			Point2f(boxes(0, sortIdxs.at(i), 3), boxes(0, sortIdxs.at(i), 2)));
		for (size_t j = i + 1; j < sortIdxs.size(); j++) {
			if (scores(sortIdxs.at(j)) < thresholdScore) {
				badIdxs.insert(sortIdxs.at(j));
				continue;
			}
			Rect2f box2 = Rect2f(Point2f(boxes(0, sortIdxs.at(j), 1), boxes(0, sortIdxs.at(j), 0)),
				Point2f(boxes(0, sortIdxs.at(j), 3), boxes(0, sortIdxs.at(j), 2)));
			if (IOU(box1, box2) > thresholdIOU)
				badIdxs.insert(sortIdxs.at(j));
		}
		i++;
	}

	vector<size_t> goodIdxs = vector<size_t>();
	for (auto it = sortIdxs.begin(); it != sortIdxs.end(); ++it) {
		if (badIdxs.find(sortIdxs.at(*it)) == badIdxs.end())
			goodIdxs.push_back(sortIdxs.at(*it));
	}
	return goodIdxs;
}
