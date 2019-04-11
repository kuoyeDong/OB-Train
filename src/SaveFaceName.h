#ifndef SAVEFACENAME_H
#define SAVEFACENAME_H
#pragma once

#include "stdio.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <io.h>
#include <windows.h>

#include <opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QQmlListProperty>
#include <QQmlComponent>
#include <QQmlApplicationEngine>
#include <QPointF>

using namespace std;
//using namespace cv;

class SaveFaceName : public QObject
{
	Q_OBJECT
public:
	SaveFaceName();
	Q_INVOKABLE void saveName(QString Qname);
	Q_INVOKABLE void getChangePath(QString Qname);
	Q_INVOKABLE void returnChangePath();
};

#endif
