#include "PersonExperiment.hpp"
#include "../src/camera.hpp"
#include <Qdebug>

extern cv::Mat frame_read; //camera

PersonExperiment::PersonExperiment(QObject *parent) :
	QObject(parent)
{
	m_pImgProvider = new ImageProvider();
	cameraOpen = false;
	cameraStatus = 0;
	current_experiment = 0;
	current_app_id = -1;
	appStatus = false;
	config = false;
	imagePath = "";
	generalPath = "";
	tmp_pro_img_path = "";
	tmp_src_img_path = "";
}

PersonExperiment::~PersonExperiment()
{
}

QImage PersonExperiment::MattoQImage(cv::Mat cvImg)
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

void PersonExperiment::setImage()
{
	if (!frame_read.empty())
	{
		cvtColor(frame_read, frame, CV_BGRA2RGB);
	}

	// experiment with camera
	if (frame.empty())
	{
		cameraStatus = 0;
	}
	else
	{
		cameraStatus = 1;
		//正常显示
		if (!frame.empty() && current_experiment == 0)
		{
			m_pImgProvider->img = MattoQImage(frame);
		}

		//实验一
		if (!frame.empty() && current_experiment == 1)
		{
			experiment_one(generalPath, frame, current_app_id);
			m_pImgProvider->img = MattoQImage(frame);
		}
	}

	emit callQmlRefeshImg();
	qDebug() << "setImage";
}

void PersonExperiment::expStart()
{
	// experiment without camera
	if (current_experiment == 2)
	{
		experiment_two(imagePath, generalPath, frame, current_app_id);
	}
	//实验三
	if (current_experiment == 3)
	{
		experiment_three(imagePath, generalPath, frame, current_app_id);
	}

	//实验四
	if (current_experiment == 4)
	{
		experiment_four(imagePath, generalPath, frame, current_app_id);
	}

	//实验五
	if (!frame.empty() && current_experiment == 5)
	{
		experiment_five(imagePath, generalPath, frame, current_app_id);
	}
}

/******************   General Switch   *******************/
void PersonExperiment::setEx_id(int ex_id, int app_id)
{
	current_experiment = ex_id;
	current_app_id = app_id;
}

void PersonExperiment::camera_open()
{
	libvlc_camera_open();
	cameraOpen = true;
}

void PersonExperiment::camera_close()
{
	libvlc_camera_close();
	current_experiment = 0;
	current_app_id = -1;
	cameraOpen = false;
	cameraStatus = 0;
	appStatus = false;
	imagePath = "";
	generalPath = "";
}

bool PersonExperiment::isOpened()
{
	return cameraOpen;
}

int PersonExperiment::connectStatus()
{
	return cameraStatus;

	//if (overtime)
	//	return -1;
}

bool PersonExperiment::appRunStatus()
{
	return appStatus;
}

bool PersonExperiment::appConfigStatus()
{
	return config;
}

int PersonExperiment::get_app_id()
{
	return current_app_id;
}

int PersonExperiment::get_exp_id()
{
	return current_experiment;
}

void PersonExperiment::setPath(QString gpath, int choice)
{
	QByteArray path = gpath.toLocal8Bit();
	std::string Path_tmp = std::string(path);
	stringReplace(Path_tmp, "\\", "/");
	if (choice == 1)
	{
		generalPath = Path_tmp.substr(8, Path_tmp.size() - 1);
	}
	if (choice == 2)
	{
		imagePath = Path_tmp.substr(8, Path_tmp.size() - 1);
	}
}

QString PersonExperiment::get_tmp_path(int choice)
{
	if (choice == 0)return tmp_src_img_path;
	if (choice == 1)return tmp_pro_img_path;
}

void PersonExperiment::setAppRunStatusClose()
{
	appStatus = false;
}

void PersonExperiment::stringReplace(std::string &strsrc, const std::string &strtarget, const std::string &strdst)
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
/******************   General Switch   *******************/