#include "FaceExperiment.h"




extern cv::Mat frame_read;


FaceExperiment::FaceExperiment(QObject *parent) :
	QObject(parent)
{
	m_pImgProvider = new ImageProvider();
	cameraOpen = false;
	cameraStatus = 0;
}

FaceExperiment::~FaceExperiment()
{
}

QImage FaceExperiment::MattoQImage(cv::Mat cvImg)
{
	QImage qImg;
	if (cvImg.channels() == 3)
	{
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_RGB888);
	}
	else if (cvImg.channels() == 1) {
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_Indexed8);
	}
	else
	{
		qImg = QImage((const unsigned char*)(cvImg.data),

			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_RGB888);
	}
	return qImg;
}

void FaceExperiment::camera_open()
{
	libvlc_camera_open();
	
	cameraOpen = true;

	facedetFlag = false;
}

void FaceExperiment::camera_close()
{
	libvlc_camera_close();
	cameraOpen = false;
	cameraStatus = 0;

	facedetFlag = false;
	frame_face.release();
}

bool FaceExperiment::isOpened()
{
	return cameraOpen;
}

int FaceExperiment::connectStatus()
{
	return cameraStatus;
}

void FaceExperiment::openFaceDetbyMTCNN()
{
	facedetFlag = true;
}

bool FaceExperiment::faceDetRunStatus()
{
	return facedetFlag;
}

void FaceExperiment::openFaceExp2()
{
	faceExp2 = true;
}
void FaceExperiment::closeFaceExp2()
{
	faceExp2 = false;
}


void FaceExperiment::setImage()
{
	if (!frame_read.empty())
	{
		cvtColor(frame_read, frame_face, CV_BGRA2RGB);
	}


	if (frame_face.empty())
	{
		cameraStatus = 0;
		if (faceExp2)
		{
			//dstImg = kernelTest(srcImgPath, kernNum1, kernNum2, kernNum3, kernel1, kernel2, kernel3);
			dstImg = kernelTest();//srcImgPath, kernNum1, kernNum2, kernNum3, kernel1, kernel2, kernel3);
			m_pImgProvider->img = MattoQImage(dstImg);
		}
	}
	else
	{
		cameraStatus = 1;
		//正常显示
		if (!frame_face.empty() && !facedetFlag)
		{
			m_pImgProvider->img = MattoQImage(frame_face);
		}

		//实验一
		if (!frame_face.empty() && facedetFlag)
		{
			//FaceDetbyMTCNN(frame_face, saveFramePath, saveFrameNum, saveFrameIntervalNum);
			saveNumFrame(frame_face);
			m_pImgProvider->img = MattoQImage(frame_face);
		}

		////实验一
		//if (!frame_face.empty() && current_experiment == 1)
		//{
		//	m_pImgProvider->img = MattoQImage(frame_face);
		//}

	}

	emit callQmlRefeshImg();
	qDebug() << "setImage";
}


void FaceExperiment::getParamsForFaceExp1(QString video_path, QString num1, QString num2)
{
	QByteArray path = video_path.toLocal8Bit();
	std::string videoPath_tmp = std::string(path);
	stringReplace(videoPath_tmp, "\\", "/");
	saveFramePath = videoPath_tmp.substr(8, videoPath_tmp.size() - 1);
	saveFrameNum = num1.toInt();
	saveFrameIntervalNum = num2.toInt();
	localnum = 0;
	//imagePath = images_path;
}

void FaceExperiment::getParamsForFaceExp2(QString video_path, QString num1, QString num2, QString num3, QString kernName1, QString kernName2, QString kernName3)
{
	QByteArray path = video_path.toLocal8Bit();
	std::string videoPath_tmp = std::string(path);
	stringReplace(videoPath_tmp, "\\", "/");
	srcImgPath = videoPath_tmp.substr(8, videoPath_tmp.size() - 1);
	kernNum1 = num1.toInt();
	kernNum2 = num2.toInt();
	kernNum3 = num3.toInt();
	kernel1 = kernName1.toStdString();
	kernel2 = kernName2.toStdString();
	kernel3 = kernName3.toStdString();

}

void FaceExperiment::getParamsForFaceExp3(QString input_datapath, QString input_labelpath, QString output_path, QString kernName, QString num_c, QString num_gamma, QString iter_num)
{
	QByteArray path1 = input_datapath.toLocal8Bit();
	std::string input_datapath_tmp = std::string(path1);
	stringReplace(input_datapath_tmp, "\\", "/");
	inputDataPath = input_datapath_tmp.substr(8, input_datapath_tmp.size() - 1);

	QByteArray path2 = input_labelpath.toLocal8Bit();
	std::string input_labelpath_tmp = std::string(path2);
	stringReplace(input_labelpath_tmp, "\\", "/");
	inputLabelPath = input_labelpath_tmp.substr(8, input_labelpath_tmp.size() - 1);

	QByteArray path3 = output_path.toLocal8Bit();
	std::string output_path_tmp = std::string(path3);
	stringReplace(output_path_tmp, "\\", "/");
	outputPath = output_path_tmp.substr(8, output_path_tmp.size() - 1);

	kernelName = kernName.toStdString();
	numC = num_c.toDouble();
	numGamma = num_gamma.toDouble();
	iterNum = iter_num.toInt();
}
void FaceExperiment::getParamsForFaceExp4(QString input_datapath, QString output_path)
{
	QByteArray path1 = input_datapath.toLocal8Bit();
	std::string input_traindatapath_tmp = std::string(path1);
	stringReplace(input_traindatapath_tmp, "\\", "/");
	inputTrainDataPath = input_traindatapath_tmp.substr(8, input_traindatapath_tmp.size() - 1);

	QByteArray path2 = output_path.toLocal8Bit();
	std::string output_traindatapath_tmp = std::string(path2);
	stringReplace(output_traindatapath_tmp, "\\", "/");
	outputTrainDataPath = output_traindatapath_tmp.substr(8, output_traindatapath_tmp.size() - 1);
}

void FaceExperiment::getParamsForFaceExp4_1(QString input_datapath, QString input_labelpath, QString model_path)
{
	QByteArray path1 = input_datapath.toLocal8Bit();
	std::string input_traindatapath_tmp = std::string(path1);
	stringReplace(input_traindatapath_tmp, "\\", "/");
	inputTestDataPath = input_traindatapath_tmp.substr(8, input_traindatapath_tmp.size() - 1);

	QByteArray path2 = input_labelpath.toLocal8Bit();
	std::string input_labelpath_tmp = std::string(path2);
	stringReplace(input_labelpath_tmp, "\\", "/");
	inputTestLabelPath = input_labelpath_tmp.substr(8, input_labelpath_tmp.size() - 1);

	QByteArray path3 = model_path.toLocal8Bit();
	std::string model_path_tmp = std::string(path3);
	stringReplace(model_path_tmp, "\\", "/");
	inputModelPath = model_path_tmp.substr(8, model_path_tmp.size() - 1);
}

void FaceExperiment::getParamsForFaceExp4_2(QString model_path)
{
	QByteArray path = model_path.toLocal8Bit();
	std::string model_path_tmp = std::string(path);
	stringReplace(model_path_tmp, "\\", "/");
	inputModelPath = model_path_tmp.substr(8, model_path_tmp.size() - 1);
}

void FaceExperiment::stringReplace(std::string &strsrc, const std::string &strtarget, const std::string &strdst)
{
	std::string::size_type pos = 0;
	std::string::size_type srclen = strtarget.size();
	std::string::size_type dstlen = strdst.size();

	while ((pos = strsrc.find(strtarget, pos)) != std::string::npos)
	{
		strsrc.replace(pos, srclen, strdst);
		pos += dstlen;
	}
}

