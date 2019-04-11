#include "SaveFaceName.h"

#include<iostream>
#include <QDebug>
#include <stdio.h>

SaveFaceName::SaveFaceName()
{

}
void SaveFaceName::saveName(QString Qname)
{
	FILE *p_name;
	//string name = Qname.toStdString();
	//string name = Qname.toLocal8Bit();
	QByteArray name = Qname.toLocal8Bit();
	//保存对应人脸文件名
	//if ((p_name = fopen("./release/Content resource/Face/data/face_names.txt", "at")) != NULL)
	if ((p_name = fopen("./Content_resource/Face/data/face_names.txt", "at")) != NULL)
		fprintf(p_name, "%s\n", string(name));
	std::fclose(p_name);
}


void SaveFaceName::getChangePath(QString Qname)
{
	//string name = Qname.toStdString();
	QByteArray name = Qname.toLocal8Bit();
	string filename = "./Content_resource/Face/data/image/" + name + ".jpg";
	struct _finddata_t fileinfo;
	long handle;
	handle = _findfirst(filename.c_str(), &fileinfo);
	string filePath = ".\\Content_resource\\Face\\data\\image\\" + string(fileinfo.name);
	string movePath = ".\\Content_resource\\Face\\data\\" + string(fileinfo.name);
	MoveFileA(filePath.c_str(), movePath.c_str());
	string changePath = "move " + filePath + " " + movePath;
	//system(changePath.c_str());
}

void SaveFaceName::returnChangePath()
{

	//system("move .\\Content_resource\\Face\\data\\dog.jpg .\\Content_resource\\Face\\data\\image\\dog.jpg");
	const char* filename = "./Content_resource/Face/data/*.jpg";
	struct _finddata_t fileinfo;
	long handle;
	handle = _findfirst(filename, &fileinfo);
	string filePath = ".\\Content_resource\\Face\\data\\" + string(fileinfo.name);
	string movePath = ".\\Content_resource\\Face\\data\\image\\" + string(fileinfo.name);
	string changePath = "move " + filePath + " " + movePath;
	MoveFileA(filePath.c_str(), movePath.c_str());

	//system(changePath.c_str());
}