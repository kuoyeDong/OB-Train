#include "Image_to_BinaryData.h"
#include <atlfile.h>
#include <cstdlib>
#include <fstream>
//using namespace cv;

void Image2BinaryData::ReverseInt(ofstream &file, int i)
{
	uchar ch1 = (uchar)(i >> 24);
	uchar ch2 = (uchar)((i << 8) >> 24);
	uchar ch3 = (uchar)((i << 16) >> 24);
	uchar ch4 = (uchar)((i << 24) >> 24);
	file.write((char*)&ch1, sizeof(ch4));
	file.write((char*)&ch2, sizeof(ch3));
	file.write((char*)&ch3, sizeof(ch2));
	file.write((char*)&ch4, sizeof(ch1));
}

vector<string> Image2BinaryData::getFileLists(string file_folder)
{
	/*
	param
	file_folder:		�ļ���·��
	*/
	file_folder += "/*.*";
	const char *mystr = file_folder.c_str();
	vector<string> flist;
	string lineStr;
	vector<string> extendName;			//�����ļ���չ��
	extendName.push_back("JPG");
	extendName.push_back("jpg");
	extendName.push_back("bmp");
	extendName.push_back("png");
	extendName.push_back("tiff");

	HANDLE file;
	WIN32_FIND_DATA fileData;
	char line[1024];
	wchar_t fn[1000];
	mbstowcs(fn, mystr, 999);
	file = FindFirstFile(fn, &fileData);
	FindNextFile(file, &fileData);
	while (FindNextFile(file, &fileData))
	{
		wcstombs(line, (const wchar_t*)fileData.cFileName, 259);
		lineStr = line;
		// remove the files which are not images
		for (int i = 0; i < 4; i++)
		{
			if (lineStr.find(extendName[i]) < 999)
			{
				flist.push_back(lineStr);
				break;
			}
		}
	}
	return flist;
}

/*
���ļ��ľ���·�����еõ��ļ���
���磺"C:\\Users\\lyf\\Desktop\\test.txt"������"test.txt"
*/
string Image2BinaryData::getFileName(string & filename)
{
	/*
	param
	filename:		�ļ��ľ���·��
	*/
	int iBeginIndex = filename.find_last_of("\\") + 1;
	int iEndIndex = filename.length();

	return filename.substr(iBeginIndex, iEndIndex - iBeginIndex);

	cout << "Done!" << endl;
}

void Image2BinaryData::ReadImage(string filefolder, vector<string>& img_Lists, vector<int>& img_Labels, vector<cv::Mat> &ImagesMat)
{
	const int size_list = img_Lists.size();
	for (int i = 0; i < size_list; ++i) {
		string curPath = filefolder + "\\" + img_Lists[i];
		cv::Mat image = cv::imread(curPath, cv::IMREAD_UNCHANGED);
		ImagesMat.push_back(image);
		char label = img_Lists[i][0];
		img_Labels[i] = label - '0';
		printf("���ڶ�ȡͼƬ�����Ե�: %.2lf%%\r", i*100.0 / (size_list - 1));
	}
	printf("\nͼƬ��ȡ���!\n\n");
}

void Image2BinaryData::Image2BinaryFile(string filefolder, vector<cv::Mat>& ImagesMat, vector<int>& img_Labels)
{
	ofstream file(filefolder, ios::binary);
	int idx = filefolder.find_last_of("\\") + 1;
	string subName = filefolder.substr(idx);
	if (file.is_open()) {
		cout << subName << "�ļ������ɹ�." << endl;

		int magic_number = 2051;
		int number_of_images = img_Labels.size();
		int n_rows = Height;
		int n_cols = Width;
		ReverseInt(file, magic_number);
		ReverseInt(file, number_of_images);
		ReverseInt(file, n_rows);
		ReverseInt(file, n_cols);

		cout << "��Ҫת����ͼƬ��Ϊ:  " << ImagesMat.size() << endl;
		for (int i = 0; i < ImagesMat.size(); ++i) {
			for (int r = 0; r < n_rows; ++r) {
				for (int c = 0; c < n_cols; ++c) {
					uchar tmp = ImagesMat[i].at<uchar>(r, c);
					file.write((char*)&tmp, sizeof(tmp));
				}
			}
			printf("ͼƬ����ת�������Ե�......%.2lf%%\r", i*100.0 / (ImagesMat.size() - 1));
		}
		printf("\n******ת�����!******\n\n");
	}
	else
		cout << subName << "�ļ�����ʧ��." << endl << endl;
	if (file.is_open())
		file.close();
	return;
}

void Image2BinaryData::Label2BinaryFile(string filefolder, vector<int>& img_Labels)
{
	ofstream file(filefolder, ios::binary);
	int idx = filefolder.find_last_of("\\") + 1;
	string subName = filefolder.substr(idx);
	if (file.is_open()) {
		cout << subName << "�ļ������ɹ�." << endl;

		int magic_number = 2049;
		int number_of_images = img_Labels.size();
		ReverseInt(file, magic_number);
		ReverseInt(file, number_of_images);
		cout << "��Ҫת���ı�ǩ��Ϊ: " << img_Labels.size() << endl;
		for (int i = 0; i < img_Labels.size(); ++i) {
			uchar tmp = (uchar)img_Labels[i];
			file.write((char*)&tmp, sizeof(tmp));
			printf("��ǩ����ת�������Ե�......%.2lf%%\r", i*100.0 / (img_Labels.size() - 1));
		}
		printf("\n******ת�����!******\n");
	}
	else
		cout << subName << "�ļ�����ʧ��." << endl;
	if (file.is_open())
		file.close();
	return;
}

void Image2BinaryData::runConvert()
{
	//Image2BinaryData IBD(28, 28);											//����ͼƬ��С(Height,Width)

	/*------------����ѵ�����ļ�--------------------------*/
	printf("----------����ѵ�����ļ�-------------\n");
	string trainfilefolder = "C:/Users/Administrator/Desktop/MNIST/train_images";		//ѵ��ͼƬ�ļ�·��
	vector<string> trainfileLists = getFileLists(trainfilefolder);				//����ļ����б�

	const int train_size_list = trainfileLists.size();
	cout << "Images Number: " << train_size_list << endl;							//����ļ�����

	string trainimagebinfilepath = "C:/Users/Administrator/Desktop/MNIST/train-images-idx3.ubyte";		//ѵ��ͼƬת������·��
	string trainlabelbinfilepath = "C:/Users/Administrator/Desktop/MNIST/train-labels-idx1.ubyte";		//ѵ����ǩת������·��
	vector<cv::Mat> TrainImagesMat;															//�����洢ѵ��ͼƬ����ֵ
	vector<int> train_image_labels(train_size_list);										//�����洢ѵ�����ǩ�б�
	ReadImage(trainfilefolder, trainfileLists, train_image_labels, TrainImagesMat);		//��ȡѵ��ͼƬ
	Image2BinaryFile(trainimagebinfilepath, TrainImagesMat, train_image_labels);		//ѵ��ͼƬת�������ļ�
	Label2BinaryFile(trainlabelbinfilepath, train_image_labels);						//ѵ����ǩת�������ļ�

	/*------------���ɲ��Լ��ļ�--------------------------*/
	printf("\n\n----------���ɲ��Լ��ļ�-------------\n");
	string testfilefolder = "C:/Users/Administrator/Desktop/MNIST/test_images";		//����ͼƬ�ļ�·��
	vector<string> testfileLists = getFileLists(testfilefolder);			//����ļ����б�

	const int test_size_list = testfileLists.size();
	cout << "Images Number: " << test_size_list << endl;									//����ļ�����
	string testimagebinfilepath = "C:/Users/Administrator/Desktop/MNIST/t10k-images-idx3.ubyte";		//����ͼƬת������·��
	string testlabelbinfilepath = "C:/Users/Administrator/Desktop/MNIST/t10k-labels-idx1.ubyte";		//���Ա�ǩת������·��
	vector<cv::Mat> TestImagesMat;															//�����洢����ͼƬ����ֵ
	vector<int> test_image_labels(test_size_list);											//�����洢�������ǩ�б�
	ReadImage(testfilefolder, testfileLists, test_image_labels, TestImagesMat);			//��ȡ����ͼƬ
	Image2BinaryFile(testimagebinfilepath, TestImagesMat, test_image_labels);			//����ͼƬת�������ļ�
	Label2BinaryFile(testlabelbinfilepath, test_image_labels);							//���Ա�ǩת�������ļ�
}

