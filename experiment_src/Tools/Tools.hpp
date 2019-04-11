#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <direct.h>
#include <time.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <float.h>
#include<io.h>
#include <sstream>
#include <algorithm>

//文件夹相关操作
wchar_t* CharToWchar(const char* c);
std::string create_dir(std::string savePath);
std::string GetProgramDir();
std::string filename(std::string & str);
void GetAllFilesNotIncludeSubfolder(std::string path, std::vector<std::string>& files);
void GetAllFilesIncludeSubfolder(std::string path, std::vector<std::string>& files);
void GetAllFilesIncludeSubfolder(std::string path, std::vector<std::string>& files, std::string fileType);
void GetSpecialFilesFromDirectory(std::string path, std::string fileType, std::vector<std::string>& files);
void GetAllFilesIncludeSubfolder(std::string path, std::string fileType, std::vector<std::string>& files);

//基本图像处理操作
void writeMatToFile(cv::Mat& m, const char* filename, std::string feature, bool isFeature);
std::string lightDark(cv::Mat& src, int beta);
std::string imgFlip(cv::Mat& src);
void moveRect(cv::Rect& rect, int direction);
std::string setRect(cv::Mat& src, cv::Mat& dsr, cv::Rect& rect);
bool txt2Mat(std::string filename, cv::Mat& image);