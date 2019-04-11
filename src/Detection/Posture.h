#pragma once
#ifndef POSTURE_H
#define POSTURE_H

#include <opencv2/opencv.hpp>

bool detector_init();

void posture_recognize(cv::Mat& frame);

void take_photo(cv::Mat frame);

void video_record();

void detector_clear();

#endif // POSTURE_H