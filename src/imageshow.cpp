#include"imageshow.h"
#include "camera.hpp"
#include "ZhSupport.h"
#include <QDebug>
#include <io.h>

extern Mat frame_read;

ShowImage::ShowImage(QObject *parent):
    QObject(parent)
{
	//camera_setting();
	cameraOpen = false;
	overtime = false;
	cameraStatus = 0;

	tracking = false;

	facedetFlag = false;
	facerecFlag = false;

	posture = false;
	detector_init();

    m_pImgProvider = new ImageProvider();
}

void ShowImage::setImage()
{
	if (!frame_read.empty())
	{
		cvtColor(frame_read, frame_all, CV_BGRA2RGB);
	}

	if (frame_all.empty())
	{
		cameraStatus = 0;
	}
	else
	{
		cameraStatus = 1;
		//正常显示
		if (!tracking && !frame_all.empty() && !facedetFlag && !facerecFlag && !posture)
		{
			m_pImgProvider->img = MattoQImage(frame_all);
		}

		//跟踪
		if (tracking && !frame_all.empty())
		{
			runTracker();
			m_pImgProvider->img = MattoQImage(frame_all);
		}

		//手势姿态识别
		if (posture && !frame_all.empty())
		{
			posture_recognize(frame_all);
			m_pImgProvider->img = MattoQImage(frame_all);
		}

		//人脸检测
		if (facedetFlag && !facerecFlag && !facechangeFlag && !frame_all.empty())
		{
			FaceDet(frame_all);
			m_pImgProvider->img = MattoQImage(frame_all);
		}

		//人脸识别
		if (facerecFlag && !facedetFlag && !facechangeFlag && !frame_all.empty())
		{
			FaceRec(frame_all);
			m_pImgProvider->img = MattoQImage(frame_all);
		}

		//人脸变换
		if (facechangeFlag && !facedetFlag && !facerecFlag && !frame_all.empty())
		{
			FaceChange(frame_all);
			m_pImgProvider->img = MattoQImage(frame_all);
		}
		//人脸注册
		if (faceregisterFlag && !facechangeFlag && !facedetFlag && !facerecFlag && !frame_all.empty())
		{
			FaceRegister(frame_all);
			m_pImgProvider->img = MattoQImage(frame_all);
		}
	}

	emit callQmlRefeshImg();
	qDebug() << "setImage";
}
QImage ShowImage::MattoQImage(Mat cvImg)
{
    QImage qImg;
    if(cvImg.channels()==3)
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }
    else if(cvImg.channels()==1) {
         qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(cvImg.data),

                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }
    return qImg;
}


/******************   General Switch   *******************/
void ShowImage::setAlgorithmStart()
{
	start_algorithm = true;
}

void ShowImage::setAlgorithmStop()
{
	start_algorithm = false;

	tracking = false;
	first_frame = true;

	posture = false;

	//facerecFlag = false;
	//facedetFlag = false;
}

void ShowImage::camera_open()
{
	libvlc_camera_open();
	cameraOpen = true;
}

void ShowImage::camera_close()
{
	libvlc_camera_close();
	cameraOpen = false;
	cameraStatus = 0;
}

bool ShowImage::isOpened()
{
	return cameraOpen;
}

int ShowImage::connectStatus()
{
	return cameraStatus;

	//if (overtime)
	//	return -1;
}
/******************   General Switch   *******************/


/******************        Tracking        *******************/
void ShowImage::openTracker()
{
	tracking = true;
	start_algorithm = false;
	first_frame = true;
	tracker = KCFTracker(true, false, true, true);
}

void ShowImage::runTracker()
{
	// 未按下“开始”按键
	if (tracking && !start_algorithm && !frame_all.empty())
	{
		roi.x = frame_all.cols / 2 - 40;
		roi.y = frame_all.rows / 2 - 40;
		roi.width = 80;
		roi.height = 80;
		cv::rectangle(frame_all, roi, cv::Scalar(0, 255, 0), 2);
	}

	// 按下“开始”按键
	if (start_algorithm && first_frame && !frame_all.empty())
	{
		roi.x = frame_all.cols / 2 - 40;
		roi.y = frame_all.rows / 2 - 40;
		roi.width = 80;
		roi.height = 80;
		tracker.init(roi, frame_all);
		first_frame = false;
		cv::rectangle(frame_all, roi, cv::Scalar(0, 255, 0), 2);
	}

	// 持续跟踪
	if (start_algorithm && !frame_all.empty())
	{
		roi = tracker.update(frame_all);
		cv::rectangle(frame_all, roi, cv::Scalar(0, 255, 0), 2);
	}
}
/******************        Tracking        *******************/


/******************        Posture        *******************/

void ShowImage::openPosture()
{
	posture = true;
}

void ShowImage::closePosture()
{
	posture = false;
	detector_clear();
}

/******************        Posture        *******************/


/*******************         Face          ********************/

void ShowImage::FaceDet(cv::Mat frame)
{
	/*FaceDetDBW DetDBW;
	DetDBW.FaceDet(frame);*/
	cv::Mat smallFrame;
	int resizeScale = 2;
	cv::resize(frame, smallFrame, cv::Size(frame.cols / resizeScale, frame.rows / resizeScale), cv::INTER_LINEAR);
	assign_image(cimg, dlib::cv_image<dlib::rgb_pixel>(smallFrame));

	for (auto face : detector(cimg))
	{
		rct.x = face.left() * resizeScale;
		rct.y = face.top() * resizeScale;
		rct.width = (face.right() - face.left()) * resizeScale;
		rct.height = (face.bottom() - face.top()) * resizeScale;
		if (face.left() >= 0 && face.top() >= 0 && face.right() <= smallFrame.cols && face.bottom() <= smallFrame.rows)
		{
			cv::rectangle(frame, rct, cv::Scalar(0, 255, 0), 3);
		}

	}

}

void ShowImage::LoadModel()//string detModel, string recModel)
{
	//deserialize("./release/Content resource/Face/data/shape_predictor_5_face_landmarks.dat") >> sp;
	//deserialize("./release/Content resource/Face/data/dlib_face_recognition_resnet_model_v1.dat") >> net;
	deserialize("./Content_resource/Face/data/shape_predictor_5_face_landmarks.dat") >> sp;
	deserialize("./Content_resource/Face/data/dlib_face_recognition_resnet_model_v1.dat") >> net;
}

void ShowImage::GetLocalFaceData()//string feature, string name)
{
	//infile_feature.open("./release/Content resource/Face/data/face_encodings.txt");
	//infile_name.open("./release/Content resource/Face/data/face_names.txt");
	infile_feature.open("./Content_resource/Face/data/face_encodings.txt");
	infile_name.open("./Content_resource/Face/data/face_names.txt");
	while (getline(infile_feature, s))
	{
		temp = atof(s.c_str());
		face_feature.push_back(temp);
	}
	infile_feature.close();
	while (getline(infile_name, s))
	{
		face_name.push_back(s);
	}
	infile_name.close();
}


void ShowImage::FaceRec(cv::Mat frame)
{
	LoadModel();
	GetLocalFaceData();
	cv::Mat smallFrame;
	int resizeScale = 2;
	cv::resize(frame, smallFrame, cv::Size(frame.cols / resizeScale, frame.rows / resizeScale), cv::INTER_LINEAR);
	assign_image(cimg, cv_image<rgb_pixel>(smallFrame));
	for (auto face : detector(cimg))
	{
		auto shape = sp(cimg, face);
		matrix<rgb_pixel> face_chip;

		extract_image_chip(cimg, get_face_chip_details(shape, 150, 0.25), face_chip);
		faces.push_back(move(face_chip));

		rct.x = face.left() * resizeScale;
		rct.y = face.top() * resizeScale;
		rct.width = (face.right() - face.left()) * resizeScale;
		rct.height = (face.bottom() - face.top()) * resizeScale;
		if (face.left() >= 0 && face.top() >= 0 && face.right() <= smallFrame.cols && face.bottom() <= smallFrame.rows)
		{
			cv::Mat face_img = frame(rct);
			cv::rectangle(frame, rct, cv::Scalar(0, 255, 0), 3);
			cv::rectangle(frame, cv::Point(face.left() * resizeScale - 2, face.bottom() * resizeScale), cv::Point(face.right() * resizeScale + 1, face.bottom() * resizeScale + 35), cv::Scalar(0, 255, 0), -1);
		}

	}
	if (faces.size() != 0)
	{
		std::vector<matrix<float, 0, 1>> face_descriptors = net(faces);
		int count_num = 0;
		for (int i = 0; i < face_feature.size() / 128; i++)
		{
			float cp = 0;
			float tmp = 0;
			for (int j = 0; j < 128; j++)
			{
				tmp = (face_feature[i * 128 + j] - face_descriptors[0](j)) * (face_feature[i * 128 + j] - face_descriptors[0](j));
				cp = cp + tmp;
			}
			cp = sqrt(cp);
			if (cp < 0.4)
			{
				putTextZH(frame, face_name[i].c_str(), cv::Point(rct.x, rct.y + rct.height), cv::Scalar(0, 0, 255), 25, "微软雅黑");
				//cv::putText(frame, face_name[i], cv::Point(rct.x, rct.y + rct.height + 25), 1, 2, cv::Scalar(0, 0, 255), 2, 8, false);
				//cout << cp << endl << face_name[i] << endl;
			}
			if (cp >= 0.4)
				count_num++;
			if (count_num == face_feature.size() / 128)
			{
				putTextZH(frame, "不认识", cv::Point(rct.x, rct.y + rct.height), cv::Scalar(0, 0, 255), 25, "微软雅黑");
				//cv::putText(frame, "Unkown", cv::Point(rct.x, rct.y + rct.height + 25), 1, 2, cv::Scalar(0, 0, 255), 2, 8, false);
				//cout << cp << endl << "Unkown" << endl;
			}
		}
	}
	faces.clear();
}

void ShowImage::FaceChange(cv::Mat frame)
{

	cv::Mat smallFrame;
	cv::Mat logo_dst;
	//cv::Mat logo = cv::imread("C:/Users/Administrator/Desktop/OBTrain/images/dog.jpg");
	const char* filename = "./Content_resource/Face/data/*.jpg";
	struct _finddata_t fileinfo;
	long handle;
	handle = _findfirst(filename, &fileinfo);
	string filePath = ".\\Content_resource\\Face\\data\\" + string(fileinfo.name);
	cv::Mat logo = cv::imread(filePath);
	//cv::Mat logo = cv::imread("./Content resource/Face/data/dog.jpg");
	cv::cvtColor(logo, logo, CV_RGB2BGR);
	int resizeScale = 2;
	cv::resize(frame, smallFrame, cv::Size(frame.cols / resizeScale, frame.rows / resizeScale), cv::INTER_LINEAR);
	assign_image(cimg, dlib::cv_image<dlib::rgb_pixel>(smallFrame));

	for (auto face : detector(cimg))
	{
		rct.x = face.left() * resizeScale;
		rct.y = face.top() * resizeScale;
		rct.width = (face.right() - face.left()) * resizeScale;
		rct.height = (face.bottom() - face.top()) * resizeScale;
		if (face.left() >= 0 && face.top() >= 0 && face.right() <= smallFrame.cols && face.bottom() <= smallFrame.rows)
		{
			//cv::rectangle(frame, rct, cv::Scalar(0, 255, 0), 3);
			cv::Mat face_img = frame(rct);
			cv::resize(logo, logo_dst, cv::Size(rct.width, rct.height), (0, 0), (0, 0), cv::INTER_LINEAR);
			logo_dst.copyTo(face_img);
		}

	}

}

void ShowImage::FaceRegister(cv::Mat frame)
{
	FILE *p_encoding;
	//FILE *p_name;
	LoadModel();
	cv::Mat smallFrame;
	int resizeScale = 2;
	cv::resize(frame, smallFrame, cv::Size(frame.cols / resizeScale, frame.rows / resizeScale), cv::INTER_LINEAR);
	assign_image(cimg, cv_image<rgb_pixel>(smallFrame));
	for (auto face : detector(cimg))
	{
		auto shape = sp(cimg, face);
		matrix<rgb_pixel> face_chip;
		extract_image_chip(cimg, get_face_chip_details(shape, 150, 0.25), face_chip);
		faces.push_back(move(face_chip));

	}
	if (faces.size() == 1)
	{
		std::vector<matrix<float, 0, 1>> face_descriptors = net(faces);
		//if ((p_encoding = fopen("./release/Content resource/Face/data/face_encodings.txt", "at")) != NULL)//  && (p_name = fopen("face_names.txt", "wt")) != NULL)
		if ((p_encoding = fopen("./Content_resource/Face/data/face_encodings.txt", "at")) != NULL)
			for (int j = 0; j < face_descriptors.size(); j++)
			{
				//fprintf(p_name, "%s", files[i]);
				for (int k = 0; k < face_descriptors[j].size(); k++)
					fprintf(p_encoding, "%f\n", face_descriptors[j](k));
			}
		//保存对应人脸文件名
		/*if ((p_name = fopen("./data_model/face_names.txt", "at")) != NULL)
			fprintf(p_name, "%s\n", "ggg");
		std::fclose(p_name);*/
		std::fclose(p_encoding);
	}
	faces.clear();
	faceregisterFlag = false;
}

void ShowImage::openFaceDet()
{
	facedetFlag = true;
	facerecFlag = false;
	facechangeFlag = false;
	faceregisterFlag = false;
}

void ShowImage::openFaceRec()
{
	facerecFlag = true;
	facedetFlag = false;
	facechangeFlag = false;
	faceregisterFlag = false;
}

void ShowImage::openFaceChange()
{
	facechangeFlag = true;
	facedetFlag = false;
	facerecFlag = false;
	faceregisterFlag = false;
}

void ShowImage::openFaceRegister()
{
	faceregisterFlag = true;
	facechangeFlag = false;
	facedetFlag = false;
	facerecFlag = false;
}

void ShowImage::setCameraOpen()
{
	facedetFlag = false;
	facerecFlag = false;
	facechangeFlag = false;
}

void ShowImage::setCameraClose()
{
	facedetFlag = false;
	facerecFlag = false;
	facechangeFlag = false;
}

void ShowImage::setFaceDetStart()
{
	facedetFlag = true;
	facerecFlag = false;
}

void ShowImage::setFaceDetStop()
{
	facedetFlag = false;
	facerecFlag = false;
	facechangeFlag = false;
}
void ShowImage::setFaceRecStart()
{
	facerecFlag = true;
	facedetFlag = false;
}

void ShowImage::setFaceRecStop()
{
	facerecFlag = false;
	facedetFlag = false;
}
/*******************         Face          ********************/