#include "FaceExperiment.h"
#include "Image_to_BinaryData.h"
#include <iostream>


void FaceExperiment::runConvertTestData()
{
	Image2BinaryData IBD(28, 28);
	/*------------���ɲ��Լ��ļ�--------------------------*/
	printf("\n\n----------���ɲ��Լ��ļ�-------------\n");
	//string testfilefolder = "C:/Users/Administrator/Desktop/MNIST/test_images";		//����ͼƬ�ļ�·��
	vector<string> testfileLists = IBD.getFileLists(inputTrainDataPath);			//����ļ����б�

	const int test_size_list = testfileLists.size();
	cout << "Images Number: " << test_size_list << endl;									//����ļ�����
	string testimagebinfilepath = outputTrainDataPath + "/" + "test-images.idx3-ubyte";//"C:/Users/Administrator/Desktop/MNIST/t10k-images-idx3.ubyte";		//����ͼƬת������·��
	string testlabelbinfilepath = outputTrainDataPath + "/" + "test-labels.idx1-ubyte";//"C:/Users/Administrator/Desktop/MNIST/t10k-labels-idx1.ubyte";		//���Ա�ǩת������·��
	vector<cv::Mat> TestImagesMat;															//�����洢����ͼƬ����ֵ
	vector<int> test_image_labels(test_size_list);											//�����洢�������ǩ�б�
	IBD.ReadImage(inputTrainDataPath, testfileLists, test_image_labels, TestImagesMat);			//��ȡ����ͼƬ
	IBD.Image2BinaryFile(testimagebinfilepath, TestImagesMat, test_image_labels);			//����ͼƬת�������ļ�
	IBD.Label2BinaryFile(testlabelbinfilepath, test_image_labels);							//���Ա�ǩת�������ļ�
}


void FaceExperiment::testMNIST()
{
	//��ȡ����������
	ifstream if_testImags(inputTestDataPath, ios::binary);
	//ifstream if_testImags("C:/Users/Administrator/Desktop/MNIST/test_data.ubyte", ios::binary);
	//��ȡʧ��
	if (true == if_testImags.fail())
	{
		qDebug() << "read data unsuccessfully!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
	}
	qDebug() << "read data successfully!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
	int magic_num, testImgsNum, nrows, ncols;
	//��ȡmagic number
	if_testImags.read((char*)&magic_num, sizeof(magic_num));
	magic_num = reverseInt(magic_num);
	cout << "����ͼ�����ݿ�t10k-images-idx3-ubyte��magic numberΪ��" << magic_num << endl;
	//��ȡ����ͼ������
	if_testImags.read((char*)&testImgsNum, sizeof(testImgsNum));
	testImgsNum = reverseInt(testImgsNum);
	cout << "����ͼ�����ݿ�t10k-images-idx3-ubyte��ͼ������Ϊ��" << testImgsNum << endl;
	//��ȡͼ����д�С
	if_testImags.read((char*)&nrows, sizeof(nrows));
	nrows = reverseInt(nrows);
	cout << "����ͼ�����ݿ�t10k-images-idx3-ubyte��ͼ��ά��rowΪ��" << nrows << endl;
	//��ȡͼ����д�С
	if_testImags.read((char*)&ncols, sizeof(ncols));
	ncols = reverseInt(ncols);
	cout << "����ͼ�����ݿ�t10k-images-idx3-ubyte��ͼ��ά��colΪ��" << ncols << endl;

	//��ȡ����ͼ��
	int imgVectorLen = nrows * ncols;
	Mat testFeatures = Mat::zeros(testImgsNum, imgVectorLen, CV_32FC1);
	Mat temp = Mat::zeros(nrows, ncols, CV_8UC1);
	for (int i = 0; i < testImgsNum; i++)
	{
		if_testImags.read((char*)temp.data, imgVectorLen);
		Mat tempFloat;
		//����SVM��Ҫ�Ĳ������ݸ�ʽ��CV_32FC1�����������ת��
		temp.convertTo(tempFloat, CV_32FC1);
		memcpy(testFeatures.data + i * imgVectorLen * sizeof(float), tempFloat.data, imgVectorLen * sizeof(float));
	}
	//��һ��
	testFeatures = testFeatures / 255;
	//��ȡ����ͼ���Ӧ�ķ����ǩ
	ifstream if_testLabels(inputTestLabelPath, ios::binary);
	//ifstream if_testLabels("C:/Users/Administrator/Desktop/MNIST/test_label.ubyte", ios::binary);
	//��ȡʧ��
	if (true == if_testLabels.fail())
	{
		qDebug() << "read label unsuccessfully!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
	}
	qDebug() << "read label successfully!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
	int magic_num_2, testLblsNum;
	//��ȡmagic number
	if_testLabels.read((char*)&magic_num_2, sizeof(magic_num_2));
	magic_num_2 = reverseInt(magic_num_2);
	cout << "����ͼ���ǩ���ݿ�t10k-labels-idx1-ubyte��magic numberΪ��" << magic_num_2 << endl;
	//��ȡ����ͼ��ķ����ǩ������
	if_testLabels.read((char*)&testLblsNum, sizeof(testLblsNum));
	testLblsNum = reverseInt(testLblsNum);
	cout << "����ͼ���ǩ���ݿ�t10k-labels-idx1-ubyte�ı�ǩ����Ϊ��" << testLblsNum << endl;

	//����SVM��Ҫ����ı�ǩ������CV_32SC1�����������ת��
	Mat testLabels = Mat::zeros(testLblsNum, 1, CV_32SC1);
	Mat readLabels = Mat::zeros(testLblsNum, 1, CV_8UC1);
	if_testLabels.read((char*)readLabels.data, testLblsNum * sizeof(char));
	readLabels.convertTo(testLabels, CV_32SC1);

	//����ѵ���õ�SVMģ��
	Ptr<SVM> svm = SVM::load(inputModelPath);
	//Ptr<SVM> svm = SVM::load("C:/Users/Administrator/Desktop/MNIST/mnist.xml");
	qDebug() << "read model successfully!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
	int sum = 0;
	//��ÿһ������ͼ�����SVM����Ԥ��
	for (int i = 0; i < testLblsNum; i++)
	{
		Mat predict_mat = Mat::zeros(1, imgVectorLen, CV_32FC1);
		memcpy(predict_mat.data, testFeatures.data + i * imgVectorLen * sizeof(float), imgVectorLen * sizeof(float));
		//Ԥ��
		float predict_label = svm->predict(predict_mat);
		//��ʵ��������ǩ
		float truth_label = testLabels.at<int>(i);
		//�Ƚ��ж��Ƿ�Ԥ����ȷ
		if ((int)predict_label == (int)truth_label)
		{
			sum++;
		}
	}
	preAccuracy = (double)sum / (double)testLblsNum * 100;
	//cout << "Ԥ��׼ȷ��Ϊ��" << (double)sum / (double)testLblsNum << endl;

}

QString FaceExperiment::getContent()
{
	QString str_acc = QString::number(preAccuracy, 'f', 3);
	str_acc.append("%");
	return str_acc;
}

void FaceExperiment::testInputNum()
{
	int nrows = 28;
	int ncols = 28;
	int imgVectorLen = nrows * ncols;
	Mat imgRead = imread("./experiment_src/FaceExperiment/testNumImage.jpg", 0);
	if (!imgRead.empty())
		qDebug() << "Read Image Successfully !!!!!!!!";
	else
		qDebug() << "Read Image Wrong Path !!!!!!!!";
	Mat imgReadScal = Mat::zeros(nrows, ncols, CV_8UC1);
	Mat show_mat = Mat::zeros(nrows, ncols, CV_32FC1);

	resize(imgRead, imgReadScal, imgReadScal.size());

	imgReadScal.convertTo(show_mat, CV_32FC1);

	show_mat = show_mat / 255;

	//
	Mat predict_mat = Mat::zeros(1, imgVectorLen, CV_32FC1);
	//memcpy(show_mat.data, testFeatures.data + index*imgVectorLen * sizeof(float), imgVectorLen * sizeof(float));
	memcpy(predict_mat.data, show_mat.data, imgVectorLen * sizeof(float));

	Ptr<SVM> svm = SVM::load(inputModelPath);
	response = svm->predict(predict_mat);

	cout << "��ǩֵΪ" << response << endl;
}

QString FaceExperiment::getContentResponse()
{
	QString str_acc = QString::number(response);
	return str_acc;
}