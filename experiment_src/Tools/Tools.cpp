#include "Tools.hpp"

using namespace std;

template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

wchar_t* CharToWchar(const char* c)
{
	wchar_t *m_wchar;
	int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}

string create_dir(string savePath)
{
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y%m%d_%H%M%S", localtime(&timep));
	string dirName = tmp;
	string fullDir = savePath + "/" + dirName;
	LPCTSTR FullDir = CharToWchar(fullDir.c_str());
	CreateDirectory(FullDir, NULL);
	return fullDir;
}

string GetProgramDir(){
	wchar_t exeFullPath[MAX_PATH]; // Full path
	string strPath = "";
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	char CharString[MAX_PATH];
	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, CharString, MAX_PATH, exeFullPath, _TRUNCATE);
	strPath = (string)CharString;    // Get full path of the file   
	int pos = strPath.find_last_of('\\', strPath.length());
	return strPath.substr(0, pos);  // Return the directory without the file name   
}

string filename(string & str)
{
	int found0 = str.find_last_of("/");
	int found1 = str.find_last_of(".");
	string s1 = str.substr(found0 + 1);
	return s1.substr(0, found1 - found0 - 1);
}

void writeMatToFile(cv::Mat& m, const char* filename, string feature, bool isFeature)
{
	std::ofstream fout;
	fout.open(filename, ios::out | ios::app);

	if (!fout)
	{
		std::cout << "File Not Opened" << std::endl;
		return;
	}

	if (isFeature)
	{
		if (!feature.empty())
		{
			fout << feature << " :" << endl;
		}
		else
		{
			fout << "origin :" << endl;
		}
	}

	for (int i = 0; i < m.rows; i++)
	{
		for (int j = 0; j < m.cols; j++)
		{
			if(isnan(m.at<float>(i, j)))
				fout << 0 << " ";
			else
				fout << m.at<float>(i, j) << " ";
		}
		//fout << std::endl;
	}
	fout << std::endl;

	fout.close();
}

std::string lightDark(cv::Mat& src, int beta)
{
	if (!src.empty())
	{
		src.convertTo(src, src.type(), 1.0, beta);
		string savepath = GetProgramDir() + "/" + "modify_src_img.jpg";
		cv::imwrite(savepath, src);
		string path = "file:///" + savepath;
		return path;
	}
}

std::string imgFlip(cv::Mat& src)
{
	if (!src.empty())
	{
		cv::flip(src, src, 0);
		string savepath = GetProgramDir() + "/" + "modify_src_img.jpg";
		cv::imwrite(savepath, src);
		string path = "file:///" + savepath;
		return path;
	}
}

void moveRect(cv::Rect& rect, int direction)
{
	if (direction == 0)rect = cv::Rect(270, 26, 175, 350);//reset
	if (direction == 1)rect.y -= 5;//up
	if (direction == 2)rect.y += 5;//down
	if (direction == 3)rect.x += 10;//right
	if (direction == 4)rect.x -= 10;//left
}

std::string setRect(cv::Mat& src, cv::Mat& dst, cv::Rect& rect)
{
	if (!src.empty())
	{
		dst = src.clone();
		cv::rectangle(dst, rect, cv::Scalar(0, 255, 0), 2);
		string savepath = GetProgramDir() + "/" + "modify_src_img.jpg";
		cv::imwrite(savepath, dst);
		string path = "file:///" + savepath;
		return path;
	}
}

bool txt2Mat(std::string filename, cv::Mat& image)
{
	string line;
	string result;
	vector<float> pixels;
	int count = 0;
	ifstream in(filename);

	if (in) // 有该文件  
	{
		while (getline(in, line)) // line中不包括每行的换行符  
		{
			stringstream input(line);
			if (count == 0)
			{
				vector<int> size;
				while (input >> result)
					size.push_back(stringToNum<int>(result));
				image = cv::Mat(size[1], size[0], CV_32F);
				count++;
			}
			else
			{
				while (input >> result)
					pixels.push_back(stringToNum<float>(result));
			}
		}

		float* p;
		for (int i = 0; i < image.rows; ++i)
		{
			p = image.ptr<float>(i);
			for (int j = 0; j < image.cols; ++j)
			{
				p[j] = pixels[i * image.cols + j];
			}
		}
		return true;
	}
	else // 没有该文件  
	{
		return false;
	}
}

void GetAllFilesNotIncludeSubfolder(string path, vector<string>& files)
{
	// 文件句柄
	intptr_t hFile = 0;
	// 文件信息
	struct _finddata_t fileinfo;

	string p;

	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1) {
		do {
			// 第一种：保存文件的全路径
			files.push_back(p.assign(path).append("/").append(fileinfo.name));
			//第二种：不保存文件的全路径
			files.push_back(fileinfo.name);
		} while (_findnext(hFile, &fileinfo) == 0); //寻找下一个，成功返回0，否则-1

		_findclose(hFile);
	}
}

void GetAllFilesIncludeSubfolder(string path, vector<string>& files) {
	//文件句柄
	intptr_t hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1) {
		do {
			if ((fileinfo.attrib & _A_SUBDIR)) { //比较文件类型是否是文件夹
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					// 第一种：保存文件的全路径
					files.push_back(p.assign(path).append("/").append(fileinfo.name));
					//第二种：不保存文件的全路径
					files.push_back(fileinfo.name);
					//递归搜索
					GetAllFilesIncludeSubfolder(p.assign(path).append("/").append(fileinfo.name), files);
				}
			}
			else {
				// 第一种：保存文件的全路径
				files.push_back(p.assign(path).append("/").append(fileinfo.name));
				//第二种：不保存文件的全路径
				files.push_back(fileinfo.name);
			}
		} while (_findnext(hFile, &fileinfo) == 0); //寻找下一个，成功返回0，否则-1
		_findclose(hFile);
	}
}

/*
path: 指定目录
files: 保存结果
fileType: 指定的文件格式，如 .jpg
*/
void GetAllFilesIncludeSubfolder(string path, vector<string>& files, string fileType)
{
	// 文件句柄
	intptr_t hFile = 0;
	// 文件信息
	struct _finddata_t fileinfo;

	string p;

	if ((hFile = _findfirst(p.assign(path).append("/*" + fileType).c_str(), &fileinfo)) != -1) {
		do {
			// 第一种：保存文件的全路径
			files.push_back(p.assign(path).append("/").append(fileinfo.name));
			//第二种：不保存文件的全路径
			files.push_back(fileinfo.name);

		} while (_findnext(hFile, &fileinfo) == 0); //寻找下一个，成功返回0，否则-1

		_findclose(hFile);
	}
}

void split(const string& str, const string& sp, vector<string>& vec)
{
	size_t size = sp.size();

	vec.clear();
	size_t end = 0, start = 0;
	while (start != string::npos && start < str.size())
	{
		end = str.find(sp, start);
		vec.push_back(str.substr(start, end - start)); // end == 0 时压入空字符串
		start = end == string::npos ? end : end + size;
	}

	if (vec.empty())
		vec.push_back(str);
}


void GetSpecialFilesFromDirectory(string path, string fileType, vector<string>& files)
{
	vector<string> tempFileTypes;
	split(fileType, " ", tempFileTypes);
	if (tempFileTypes.size() == 0)
		return;

	vector<int> file_num;

	for (int i = 0; i < tempFileTypes.size(); ++i)
	{
		// 文件句柄
		intptr_t hFile = 0;
		// 文件信息
		struct _finddata_t fileinfo;

		string p;

		if ((hFile = _findfirst(p.assign(path).append("/*" + tempFileTypes[i]).c_str(), &fileinfo)) != -1) {
			do
			{
				// 第一种：保存文件的全路径
				//files.push_back(p.assign(path).append("/").append(fileinfo.name));
				//第二种：不保存文件的全路径
				string filename = fileinfo.name;
				file_num.push_back(stringToNum<int>(filename.substr(0, filename.rfind("."))));
				//files.push_back(fileinfo.name);
			} while (_findnext(hFile, &fileinfo) == 0); //寻找下一个，成功返回0，否则-1
			_findclose(hFile);
		}
		sort(file_num.begin(), file_num.end());
		for (i = 0; i < file_num.size(); i++)
		{
			string filename = to_string(file_num[i]) + tempFileTypes[0];
			files.push_back(filename);
		}
	}
}

void GetAllFilesIncludeSubfolder(string path, string fileType, vector<string>& files)
{
	vector<string> tempFileTypes;
	split(fileType, " ", tempFileTypes);
	if (tempFileTypes.size() == 0)
		return;
	//文件句柄
	intptr_t hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{ //比较文件类型是否是文件夹
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					string subForld = p.assign(path).append("/").append(fileinfo.name);
					//递归搜索
					GetSpecialFilesFromDirectory(subForld, fileType, files);
					GetAllFilesIncludeSubfolder(p.assign(path).append("/").append(fileinfo.name), fileType, files);
				}
			}
			else
			{
				for (int i = 0; i < tempFileTypes.size(); ++i)
				{
					string fileName = fileinfo.name;
					if (fileName.find(tempFileTypes[i]) != std::string::npos)
					{
						// 第一种：保存文件的全路径
						files.push_back(p.assign(path).append("/").append(fileinfo.name));
						//第二种：不保存文件的全路径
						//files.push_back(fileinfo.name);
					}
					else
					{
						continue;
					}
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0); //寻找下一个，成功返回0，否则-1
		_findclose(hFile);
	}
}