#include <iostream>
#include "FaceExperiment.h"
#include "fastmtcnn.h"


void FaceExperiment::FaceDetbyMTCNN(cv::Mat frame, string savePath, int saveNum, int intervalNum)
{

	MTCNN detector("./Content_resource/Face/data/model");
	//MTCNN detector;

	float factor = 0.709f;
	float threshold[3] = { 0.7f, 0.6f, 0.6f };
	int minSize = 40;
	saveNum = saveNum * intervalNum;
	string saveFacePath = savePath + "/" + to_string(localnum / intervalNum + 1) + ".jpg";
	cv::Mat align_face;
	Mat frame_bgr;
	frame.copyTo(frame_bgr);
	cvtColor(frame, frame_bgr, COLOR_RGB2BGR);
	//double t = (double)cv::getTickCount();
	vector<FaceInfo> faceInfo = detector.Detect_mtcnn(frame_bgr, minSize, threshold, factor, 3);
	
	for (int i = 0; i < faceInfo.size(); i++)
	{
		//FaceInfo& box = faceInfo[i];
		int left = (int)faceInfo[i].bbox.xmin;
		int top = (int)faceInfo[i].bbox.ymin;
		int right = (int)faceInfo[i].bbox.xmax;
		int bottom = (int)faceInfo[i].bbox.ymax;
		int width = (int)(faceInfo[i].bbox.xmax - faceInfo[i].bbox.xmin + 1);
		int height = (int)(faceInfo[i].bbox.ymax - faceInfo[i].bbox.ymin + 1);

		if (faceInfo.size() == 1)
		{
			align_face = detector.getwarpAffineImg(frame_bgr, faceInfo[i].landmark);
			if (!align_face.empty())
			{
				resize(align_face, align_face, Size(112, 112));
				if (localnum < saveNum && (localnum % intervalNum == 0))
					imwrite(saveFacePath, align_face);	

				localnum++;

				if (localnum == saveNum)
					facedetFlag = false;
			}
		}

		cv::rectangle(frame, cv::Rect(left, top, width, height), cv::Scalar(0, 255, 0), 2);

		for (int j = 0; j < 5; j++)
		{
			cv::circle(frame, cv::Point((int)faceInfo[i].landmark[j * 2], (int)faceInfo[i].landmark[j * 2 + 1]), 1, cv::Scalar(0, 255, 0), 5);
		}
		
	}
	
	
}