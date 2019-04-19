#include "FaceExperiment.h"
#include "Image_to_BinaryData.h"
#include <iostream>

//using namespace cv;
//using namespace std;


int FaceExperiment::reverseInt(int i)
{
	unsigned char c1, c2, c3, c4;

	c1 = i & 255;
	c2 = (i >> 8) & 255;
	c3 = (i >> 16) & 255;
	c4 = (i >> 24) & 255;

	return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}

void FaceExperiment::trainMNIST()
{
	//读取训练样本集
	ifstream if_trainImags(inputDataPath, ios::binary);
	//读取失败
	if (true == if_trainImags.fail())
	{
		cout << "Please check the path of file train-images-idx3-ubyte" << endl;
		//return;
	}
	int magic_num = 0;
	int trainImgsNum = 0;
	int nrows = 0;
	int ncols = 0;
	//读取magic number
	if_trainImags.read((char*)&magic_num, sizeof(magic_num));
	magic_num = reverseInt(magic_num);
	cout << "训练图像数据库train-images-idx3-ubyte的magic number为：" << magic_num << endl;
	//读取训练图像总数
	if_trainImags.read((char*)&trainImgsNum, sizeof(trainImgsNum));
	trainImgsNum = reverseInt(trainImgsNum);
	cout << "训练图像数据库train-images-idx3-ubyte的图像总数为：" << trainImgsNum << endl;
	//读取图像的行大小
	if_trainImags.read((char*)&nrows, sizeof(nrows));
	nrows = reverseInt(nrows);
	cout << "训练图像数据库train-images-idx3-ubyte的图像维度row为：" << nrows << endl;
	//读取图像的列大小
	if_trainImags.read((char*)&ncols, sizeof(ncols));
	ncols = reverseInt(ncols);
	cout << "训练图像数据库train-images-idx3-ubyte的图像维度col为：" << ncols << endl;

	//读取训练图像
	int imgVectorLen = nrows * ncols;
	Mat trainFeatures = Mat::zeros(trainImgsNum, imgVectorLen, CV_32FC1);
	Mat temp = Mat::zeros(nrows, ncols, CV_8UC1);
	for (int i = 0; i < trainImgsNum; i++)
	{
		if_trainImags.read((char*)temp.data, imgVectorLen);
		Mat tempFloat;
		//由于SVM需要的训练数据格式是CV_32FC1，在这里进行转换
		temp.convertTo(tempFloat, CV_32FC1);
		memcpy(trainFeatures.data + i * imgVectorLen * sizeof(float), tempFloat.data, imgVectorLen * sizeof(float));
	}
	//归一化
	trainFeatures = trainFeatures / 255;
	//读取训练图像对应的分类标签
	ifstream if_trainLabels(inputLabelPath, ios::binary);
	//读取失败
	if (true == if_trainLabels.fail())
	{
		cout << "Please check the path of file train-labels-idx1-ubyte" << endl;
		//return;
	}
	int magic_num_2, trainLblsNum;
	//读取magic number
	if_trainLabels.read((char*)&magic_num_2, sizeof(magic_num_2));
	magic_num_2 = reverseInt(magic_num_2);
	cout << "训练图像标签数据库train-labels-idx1-ubyte的magic number为：" << magic_num_2 << endl;
	//读取训练图像的分类标签的数量
	if_trainLabels.read((char*)&trainLblsNum, sizeof(trainLblsNum));
	trainLblsNum = reverseInt(trainLblsNum);
	cout << "训练图像标签数据库train-labels-idx1-ubyte的标签总数为：" << trainLblsNum << endl;

	//由于SVM需要输入的标签类型是CV_32SC1，在这里进行转换
	Mat trainLabels = Mat::zeros(trainLblsNum, 1, CV_32SC1);
	Mat readLabels = Mat::zeros(trainLblsNum, 1, CV_8UC1);
	if_trainLabels.read((char*)readLabels.data, trainLblsNum * sizeof(char));
	readLabels.convertTo(trainLabels, CV_32SC1);


	// 训练SVM分类器
	//初始化
	Ptr<SVM> svm = SVM::create();
	//多分类
	svm->setType(SVM::C_SVC);
	//kernal选用RBF
	if(kernelName == "RBF")
		svm->setKernel(SVM::RBF);
	if (kernelName == "LINEAR")
		svm->setKernel(SVM::LINEAR);
	if (kernelName == "SIGMOID")
		svm->setKernel(SVM::SIGMOID);
	//设置经验值 
	svm->setGamma(numGamma);
	svm->setC(numC);
	string strGamma = to_string(numGamma);
	string strC = to_string(numC);
	string striterNum = to_string(iterNum);
	//设置终止条件，在这里选择迭代200次
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, iterNum, FLT_EPSILON));
	//训练开始
	svm->train(trainFeatures, ROW_SAMPLE, trainLabels);

	cout << "训练结束，正写入xml:" << endl;
	//保存模型
	svm->save(outputPath + "/" + kernelName + "_" + strGamma + "_" + strC + "_" + striterNum + "_mnist.xml");

}

void FaceExperiment::runConvertTrainData()
{
	Image2BinaryData IBD(28, 28);
	/*------------生成训练集文件--------------------------*/
	printf("----------生成训练集文件-------------\n");
	//string trainfilefolder = "C:/Users/Administrator/Desktop/MNIST/train_images";		//训练图片文件路径
	vector<string> trainfileLists = IBD.getFileLists(inputTrainDataPath);				//获得文件名列表

	const int train_size_list = trainfileLists.size();
	cout << "Images Number: " << train_size_list << endl;							//输出文件个数

	string trainimagebinfilepath = outputTrainDataPath + "/" + "train-images.idx3-ubyte";//"C:/Users/Administrator/Desktop/MNIST/train-images-idx3.ubyte";		//训练图片转换保存路径
	string trainlabelbinfilepath = outputTrainDataPath + "/" + "train-labels.idx1-ubyte";//"C:/Users/Administrator/Desktop/MNIST/train-labels-idx1.ubyte";		//训练标签转换保存路径
	vector<cv::Mat> TrainImagesMat;															//用来存储训练图片像素值
	vector<int> train_image_labels(train_size_list);										//用来存储训练类标签列表
	IBD.ReadImage(inputTrainDataPath, trainfileLists, train_image_labels, TrainImagesMat);		//读取训练图片
	IBD.Image2BinaryFile(trainimagebinfilepath, TrainImagesMat, train_image_labels);		//训练图片转二进制文件
	IBD.Label2BinaryFile(trainlabelbinfilepath, train_image_labels);						//训练标签转二进制文件
}