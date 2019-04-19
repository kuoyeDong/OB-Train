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
	//��ȡѵ��������
	ifstream if_trainImags(inputDataPath, ios::binary);
	//��ȡʧ��
	if (true == if_trainImags.fail())
	{
		cout << "Please check the path of file train-images-idx3-ubyte" << endl;
		//return;
	}
	int magic_num = 0;
	int trainImgsNum = 0;
	int nrows = 0;
	int ncols = 0;
	//��ȡmagic number
	if_trainImags.read((char*)&magic_num, sizeof(magic_num));
	magic_num = reverseInt(magic_num);
	cout << "ѵ��ͼ�����ݿ�train-images-idx3-ubyte��magic numberΪ��" << magic_num << endl;
	//��ȡѵ��ͼ������
	if_trainImags.read((char*)&trainImgsNum, sizeof(trainImgsNum));
	trainImgsNum = reverseInt(trainImgsNum);
	cout << "ѵ��ͼ�����ݿ�train-images-idx3-ubyte��ͼ������Ϊ��" << trainImgsNum << endl;
	//��ȡͼ����д�С
	if_trainImags.read((char*)&nrows, sizeof(nrows));
	nrows = reverseInt(nrows);
	cout << "ѵ��ͼ�����ݿ�train-images-idx3-ubyte��ͼ��ά��rowΪ��" << nrows << endl;
	//��ȡͼ����д�С
	if_trainImags.read((char*)&ncols, sizeof(ncols));
	ncols = reverseInt(ncols);
	cout << "ѵ��ͼ�����ݿ�train-images-idx3-ubyte��ͼ��ά��colΪ��" << ncols << endl;

	//��ȡѵ��ͼ��
	int imgVectorLen = nrows * ncols;
	Mat trainFeatures = Mat::zeros(trainImgsNum, imgVectorLen, CV_32FC1);
	Mat temp = Mat::zeros(nrows, ncols, CV_8UC1);
	for (int i = 0; i < trainImgsNum; i++)
	{
		if_trainImags.read((char*)temp.data, imgVectorLen);
		Mat tempFloat;
		//����SVM��Ҫ��ѵ�����ݸ�ʽ��CV_32FC1�����������ת��
		temp.convertTo(tempFloat, CV_32FC1);
		memcpy(trainFeatures.data + i * imgVectorLen * sizeof(float), tempFloat.data, imgVectorLen * sizeof(float));
	}
	//��һ��
	trainFeatures = trainFeatures / 255;
	//��ȡѵ��ͼ���Ӧ�ķ����ǩ
	ifstream if_trainLabels(inputLabelPath, ios::binary);
	//��ȡʧ��
	if (true == if_trainLabels.fail())
	{
		cout << "Please check the path of file train-labels-idx1-ubyte" << endl;
		//return;
	}
	int magic_num_2, trainLblsNum;
	//��ȡmagic number
	if_trainLabels.read((char*)&magic_num_2, sizeof(magic_num_2));
	magic_num_2 = reverseInt(magic_num_2);
	cout << "ѵ��ͼ���ǩ���ݿ�train-labels-idx1-ubyte��magic numberΪ��" << magic_num_2 << endl;
	//��ȡѵ��ͼ��ķ����ǩ������
	if_trainLabels.read((char*)&trainLblsNum, sizeof(trainLblsNum));
	trainLblsNum = reverseInt(trainLblsNum);
	cout << "ѵ��ͼ���ǩ���ݿ�train-labels-idx1-ubyte�ı�ǩ����Ϊ��" << trainLblsNum << endl;

	//����SVM��Ҫ����ı�ǩ������CV_32SC1�����������ת��
	Mat trainLabels = Mat::zeros(trainLblsNum, 1, CV_32SC1);
	Mat readLabels = Mat::zeros(trainLblsNum, 1, CV_8UC1);
	if_trainLabels.read((char*)readLabels.data, trainLblsNum * sizeof(char));
	readLabels.convertTo(trainLabels, CV_32SC1);


	// ѵ��SVM������
	//��ʼ��
	Ptr<SVM> svm = SVM::create();
	//�����
	svm->setType(SVM::C_SVC);
	//kernalѡ��RBF
	if(kernelName == "RBF")
		svm->setKernel(SVM::RBF);
	if (kernelName == "LINEAR")
		svm->setKernel(SVM::LINEAR);
	if (kernelName == "SIGMOID")
		svm->setKernel(SVM::SIGMOID);
	//���þ���ֵ 
	svm->setGamma(numGamma);
	svm->setC(numC);
	string strGamma = to_string(numGamma);
	string strC = to_string(numC);
	string striterNum = to_string(iterNum);
	//������ֹ������������ѡ�����200��
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, iterNum, FLT_EPSILON));
	//ѵ����ʼ
	svm->train(trainFeatures, ROW_SAMPLE, trainLabels);

	cout << "ѵ����������д��xml:" << endl;
	//����ģ��
	svm->save(outputPath + "/" + kernelName + "_" + strGamma + "_" + strC + "_" + striterNum + "_mnist.xml");

}

void FaceExperiment::runConvertTrainData()
{
	Image2BinaryData IBD(28, 28);
	/*------------����ѵ�����ļ�--------------------------*/
	printf("----------����ѵ�����ļ�-------------\n");
	//string trainfilefolder = "C:/Users/Administrator/Desktop/MNIST/train_images";		//ѵ��ͼƬ�ļ�·��
	vector<string> trainfileLists = IBD.getFileLists(inputTrainDataPath);				//����ļ����б�

	const int train_size_list = trainfileLists.size();
	cout << "Images Number: " << train_size_list << endl;							//����ļ�����

	string trainimagebinfilepath = outputTrainDataPath + "/" + "train-images.idx3-ubyte";//"C:/Users/Administrator/Desktop/MNIST/train-images-idx3.ubyte";		//ѵ��ͼƬת������·��
	string trainlabelbinfilepath = outputTrainDataPath + "/" + "train-labels.idx1-ubyte";//"C:/Users/Administrator/Desktop/MNIST/train-labels-idx1.ubyte";		//ѵ����ǩת������·��
	vector<cv::Mat> TrainImagesMat;															//�����洢ѵ��ͼƬ����ֵ
	vector<int> train_image_labels(train_size_list);										//�����洢ѵ�����ǩ�б�
	IBD.ReadImage(inputTrainDataPath, trainfileLists, train_image_labels, TrainImagesMat);		//��ȡѵ��ͼƬ
	IBD.Image2BinaryFile(trainimagebinfilepath, TrainImagesMat, train_image_labels);		//ѵ��ͼƬת�������ļ�
	IBD.Label2BinaryFile(trainlabelbinfilepath, train_image_labels);						//ѵ����ǩת�������ļ�
}