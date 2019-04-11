#include "systemopen.h"
#include<iostream>
#include <qdir.h>
#include <qprocess.h>
#include <QDesktopServices>
#include <qurl.h>
#include <QCoreApplication>
systemOpen::systemOpen()
{

}

void systemOpen::openProject()
{
    system("C:/Users/admin/Desktop/Magnetometer/Drone_Templates/Projects/Project.uvprojx");
}

void systemOpen::openMagPresent()
{
    system("C:/Users/admin/Desktop/magPresent.ppt");
}


void systemOpen::openMagFlow()
{
    system("C:/Users/admin/Desktop/magFlow.ppt");
}

void systemOpen::openFaceDet()
{
    system("C:/Users/Administrator/Desktop/dlibResNet_DBW/FaceDet.py");
}

void systemOpen::openFaceRec()
{
    system("C:/Users/Administrator/Desktop/dlibResNet_DBW/FaceRec.py");
}

void systemOpen::openFile(QString relative_path)
{
     QString path = QCoreApplication::applicationDirPath();
     path += relative_path;
     path.replace("/","\\");
     QProcess::startDetached("explorer "+path);
}


