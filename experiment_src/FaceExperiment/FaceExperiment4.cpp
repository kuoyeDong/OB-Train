#include "FaceExperiment.h"
#include "Image_to_BinaryData.h"
#include <iostream>


void FaceExperiment::runConvertTestData()
{
	Image2BinaryData IBD(28, 28);
	/*------------生成测试集文件--------------------------*/
	printf("\n\n----------生成测试集文件-------------\n");
	//string testfilefolder = "C:/Users/Administrator/Desktop/MNIST/test_images";		//测试图片文件路径
	vector<string> testfileLists = IBD.getFileLists(inputTrainDataPath);			//获得文件名列表

	const int test_size_list = testfileLists.size();
	cout << "Images Number: " << test_size_list << endl;									//输出文件个数
	string testimagebinfilepath = outputTrainDataPath + "/" + "test-images.idx3-ubyte";//"C:/Users/Administrator/Desktop/MNIST/t10k-images-idx3.ubyte";		//测试图片转换保存路径
	string testlabelbinfilepath = outputTrainDataPath + "/" + "test-labels.idx1-ubyte";//"C:/Users/Administrator/Desktop/MNIST/t10k-labels-idx1.ubyte";		//测试标签转换保存路径
	vector<cv::Mat> TestImagesMat;															//用来存储测试图片像素值
	vector<int> test_image_labels(test_size_list);											//用来存储测试类标签列表
	IBD.ReadImage(inputTrainDataPath, testfileLists, test_image_labels, TestImagesMat);			//读取测试图片
	IBD.Image2BinaryFile(testimagebinfilepath, TestImagesMat, test_image_labels);			//测试图片转二进制文件
	IBD.Label2BinaryFile(testlabelbinfilepath, test_image_labels);							//测试标签转二进制文件
}


void FaceExperiment::testMNIST()
{
	//读取测试样本集
	ifstream if_testImags(inputTestDataPath, ios::binary);
	//ifstream if_testImags("C:/Users/Administrator/Desktop/MNIST/test_data.ubyte", ios::binary);
	//读取失败
	if (true == if_testImags.fail())
	{
		qDebug() << "read data unsuccessfully!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
	}
	qDebug() << "read data successfully!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
	int magic_num, testImgsNum, nrows, ncols;
	//读取magic number
	if_testImags.read((char*)&magic_num, sizeof(magic_num));
	magic_num = reverseInt(magic_num);
	cout << "测试图像数据库t10k-images-idx3-ubyte的magic number为：" << magic_num << endl;
	//读取测试图像总数
	if_testImags.read((char*)&testImgsNum, sizeof(testImgsNum));
	testImgsNum = reverseInt(testImgsNum);
	cout << "测试图像数据库t10k-images-idx3-ubyte的图像总数为：" << testImgsNum << endl;
	//读取图像的行大小
	if_testImags.read((char*)&nrows, sizeof(nrows));
	nrows = reverseInt(nrows);
	cout << "测试图像数据库t10k-images-idx3-ubyte的图像维度row为：" << nrows << endl;
	//读取图像的列大小
	if_testImags.read((char*)&ncols, sizeof(ncols));
	ncols = reverseInt(ncols);
	cout << "测试图像数据库t10k-images-idx3-ubyte的图像维度col为：" << ncols << endl;

	//读取测试图像
	int imgVectorLen = nrows * ncols;
	Mat testFeatures = Mat::zeros(testImgsNum, imgVectorLen, CV_32FC1);
	Mat temp = Mat::zeros(nrows, ncols, CV_8UC1);
	for (int i = 0; i < testImgsNum; i++)
	{
		if_testImags.read((char*)temp.data, imgVectorLen);
		Mat tempFloat;
		//由于SVM需要的测试数据格式是CV_32FC1，在这里进行转换
		temp.convertTo(tempFloat, CV_32FC1);
		memcpy(testFeatures.data + i * imgVectorLen * sizeof(float), tempFloat.data, imgVectorLen * sizeof(float));
	}
	//归一化
	testFeatures = testFeatures / 255;
	//读取测试图像对应的分类标签
	ifstream if_testLabels(inputTestLabelPath, ios::binary);
	//ifstream if_testLabels("C:/Users/Administrator/Desktop/MNIST/test_label.ubyte", ios::binary);
	//读取失败
	if (true == if_testLabels.fail())
	{
		qDebug() << "read label unsuccessfully!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
	}
	qDebug() << "read label successfully!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
	int magic_num_2, testLblsNum;
	//读取magic number
	if_testLabels.read((char*)&magic_num_2, sizeof(magic_num_2));
	magic_num_2 = reverseInt(magic_num_2);
	cout << "测试图像标签数据库t10k-labels-idx1-ubyte的magic number为：" << magic_num_2 << endl;
	//读取测试图像的分类标签的数量
	if_testLabels.read((char*)&testLblsNum, sizeof(testLblsNum));
	testLblsNum = reverseInt(testLblsNum);
	cout << "测试图像标签数据库t10k-labels-idx1-ubyte的标签总数为：" << testLblsNum << endl;

	//由于SVM需要输入的标签类型是CV_32SC1，在这里进行转换
	Mat testLabels = Mat::zeros(testLblsNum, 1, CV_32SC1);
	Mat readLabels = Mat::zeros(testLblsNum, 1, CV_8UC1);
	if_testLabels.read((char*)readLabels.data, testLblsNum * sizeof(char));
	readLabels.convertTo(testLabels, CV_32SC1);

	//载入训练好的SVM模型
	Ptr<SVM> svm = SVM::load(inputModelPath);
	//Ptr<SVM> svm = SVM::load("C:/Users/Administrator/Desktop/MNIST/mnist.xml");
	qDebug() << "read model successfully!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
	int sum = 0;
	//对每一个测试图像进行SVM分类预测
	for (int i = 0; i < testLblsNum; i++)
	{
		Mat predict_mat = Mat::zeros(1, imgVectorLen, CV_32FC1);
		memcpy(predict_mat.data, testFeatures.data + i * imgVectorLen * sizeof(float), imgVectorLen * sizeof(float));
		//预测
		float predict_label = svm->predict(predict_mat);
		//真实的样本标签
		float truth_label = testLabels.at<int>(i);
		//比较判定是否预测正确
		if ((int)predict_label == (int)truth_label)
		{
			sum++;
		}
	}
	preAccuracy = (double)sum / (double)testLblsNum * 100;
	//cout << "预测准确率为：" << (double)sum / (double)testLblsNum << endl;

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

	cout << "标签值为" << response << endl;
}

QString FaceExperiment::getContentResponse()
{
	QString str_acc = QString::number(response);
	return str_acc;
}