#pragma once

//#include <cv.h>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <math.h>
#include "float.h"

using namespace cv;

#ifndef _OPENCV_EXTERNAL_LIB_HPP_
#define _OPENCV_EXTERNAL_LIB_HPP_

const int nClusters = 15;
float data[nClusters][3] = {
	{ 161.317504, 127.223401, 128.609333 },
	{ 142.922425, 128.666965, 127.532319 },
	{ 67.879757, 127.721830, 135.903311 },
	{ 92.705062, 129.965717, 137.399500 },
	{ 120.172257, 128.279647, 127.036493 },
	{ 195.470568, 127.857070, 129.345415 },
	{ 41.257102, 130.059468, 132.675336 },
	{ 12.014861, 129.480555, 127.064714 },
	{ 226.567086, 127.567831, 136.345727 },
	{ 154.664210, 131.676606, 156.481669 },
	{ 121.180447, 137.020793, 153.433743 },
	{ 87.042204, 137.211742, 98.614874 },
	{ 113.809537, 106.577104, 157.818094 },
	{ 81.083293, 170.051905, 148.904079 },
	{ 45.015485, 138.543124, 102.402528 } };


///////////////////////RectTools////////////////////
namespace RectTools
{

	template <typename t>
	inline cv::Vec<t, 2 > center(const cv::Rect_<t> &rect)
	{
		return cv::Vec<t, 2 >(rect.x + rect.width / (t)2, rect.y + rect.height / (t)2);
	}

	template <typename t>
	inline t x2(const cv::Rect_<t> &rect)
	{
		return rect.x + rect.width;
	}

	template <typename t>
	inline t y2(const cv::Rect_<t> &rect)
	{
		return rect.y + rect.height;
	}

	template <typename t>
	inline void resize(cv::Rect_<t> &rect, float scalex, float scaley = 0)
	{
		if (!scaley)scaley = scalex;
		rect.x -= rect.width * (scalex - 1.f) / 2.f;
		rect.width *= scalex;

		rect.y -= rect.height * (scaley - 1.f) / 2.f;
		rect.height *= scaley;

	}

	template <typename t>
	inline void limit(cv::Rect_<t> &rect, cv::Rect_<t> limit)
	{
		if (rect.x + rect.width > limit.x + limit.width)rect.width = (limit.x + limit.width - rect.x);
		if (rect.y + rect.height > limit.y + limit.height)rect.height = (limit.y + limit.height - rect.y);
		if (rect.x < limit.x)
		{
			rect.width -= (limit.x - rect.x);
			rect.x = limit.x;
		}
		if (rect.y < limit.y)
		{
			rect.height -= (limit.y - rect.y);
			rect.y = limit.y;
		}
		if (rect.width<0)rect.width = 0;
		if (rect.height<0)rect.height = 0;
	}

	template <typename t>
	inline void limit(cv::Rect_<t> &rect, t width, t height, t x = 0, t y = 0)
	{
		limit(rect, cv::Rect_<t >(x, y, width, height));
	}

	template <typename t>
	inline cv::Rect getBorder(const cv::Rect_<t > &original, cv::Rect_<t > & limited)
	{
		cv::Rect_<t > res;
		res.x = limited.x - original.x;
		res.y = limited.y - original.y;
		res.width = x2(original) - x2(limited);
		res.height = y2(original) - y2(limited);
		assert(res.x >= 0 && res.y >= 0 && res.width >= 0 && res.height >= 0);
		return res;
	}

	inline cv::Mat subwindow(const cv::Mat &in, const cv::Rect & window, int borderType = cv::BORDER_CONSTANT)
	{
		cv::Rect cutWindow = window;
		RectTools::limit(cutWindow, in.cols, in.rows);
		if (cutWindow.height <= 0 || cutWindow.width <= 0)assert(0); //return cv::Mat(window.height,window.width,in.type(),0) ;
		cv::Rect border = RectTools::getBorder(window, cutWindow);
		cv::Mat res = in(cutWindow);

		if (border != cv::Rect(0, 0, 0, 0))
		{
			cv::copyMakeBorder(res, res, border.y, border.height, border.x, border.width, borderType);
		}
		return res;
	}

	inline cv::Mat getGrayImage(cv::Mat img)
	{
		cv::cvtColor(img, img, CV_BGR2GRAY);
		img.convertTo(img, CV_32F, 1 / 255.f);
		return img;
	}

}


////////////////////////////FFTTools//////////////////////////////
namespace FFTTools
{
	// Previous declarations, to avoid warnings
	cv::Mat fftd(cv::Mat& img, bool backwards = false);
	cv::Mat real(cv::Mat& img);
	cv::Mat imag(cv::Mat& img);
	cv::Mat magnitude(cv::Mat& img);
	cv::Mat complexMultiplication(cv::Mat& a, cv::Mat& b);
	cv::Mat complexDivision(cv::Mat& a, cv::Mat& b);
	void rearrange(cv::Mat &img);
	void normalizedLogTransform(cv::Mat &img);


	cv::Mat fftd(cv::Mat& img, bool backwards)
	{
		/*
		#ifdef USE_FFTW

		fftw_complex * fm = (fftw_complex*) fftw_malloc(sizeof (fftw_complex) * img.cols * img.rows);

		fftw_plan p = fftw_plan_dft_2d(img.rows, img.cols, fm, fm, backwards ? 1 : -1, 0 * FFTW_ESTIMATE);


		if (img.channels() == 1)
		{
		for (int i = 0; i < img.rows; i++)
		for (int j = 0; j < img.cols; j++)
		{
		fm[i * img.cols + j][0] = img.at<float>(i, j);
		fm[i * img.cols + j][1] = 0;
		}
		}
		else
		{
		assert(img.channels() == 2);
		for (int i = 0; i < img.rows; i++)
		for (int j = 0; j < img.cols; j++)
		{
		fm[i * img.cols + j][0] = img.at<cv::Vec2d > (i, j)[0];
		fm[i * img.cols + j][1] = img.at<cv::Vec2d > (i, j)[1];
		}
		}
		fftw_execute(p);
		cv::Mat res(img.rows, img.cols, CV_64FC2);


		for (int i = 0; i < img.rows; i++)
		for (int j = 0; j < img.cols; j++)
		{
		res.at<cv::Vec2d > (i, j)[0] = fm[i * img.cols + j][0];
		res.at<cv::Vec2d > (i, j)[1] = fm[i * img.cols + j][1];

		//  _iout(fm[i * img.cols + j][0]);
		}

		if (backwards)res *= 1.d / (float) (res.cols * res.rows);

		fftw_free(p);
		fftw_free(fm);
		return res;

		#else
		*/
		if (img.channels() == 1)
		{
			cv::Mat planes[] = { cv::Mat_<float>(img), cv::Mat_<float>::zeros(img.size()) };
			//cv::Mat planes[] = {cv::Mat_<double> (img), cv::Mat_<double>::zeros(img.size())};
			cv::merge(planes, 2, img);
		}
		cv::dft(img, img, backwards ? (cv::DFT_INVERSE | cv::DFT_SCALE) : 0);

		return img;

		/*#endif*/

	}

	cv::Mat real(cv::Mat& img)
	{
		std::vector<cv::Mat> planes;
		cv::split(img, planes);
		return planes[0];
	}

	cv::Mat imag(cv::Mat& img)
	{
		std::vector<cv::Mat> planes;
		cv::split(img, planes);
		return planes[1];
	}

	cv::Mat magnitude(cv::Mat& img)
	{
		cv::Mat res;
		std::vector<cv::Mat> planes;
		cv::split(img, planes); // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
		if (planes.size() == 1) res = cv::abs(img);
		else if (planes.size() == 2) cv::magnitude(planes[0], planes[1], res); // planes[0] = magnitude
		else assert(0);
		return res;
	}

	cv::Mat complexMultiplication(cv::Mat& a, cv::Mat& b)
	{
		std::vector<cv::Mat> pa;
		std::vector<cv::Mat> pb;
		cv::split(a, pa);
		cv::split(b, pb);

		std::vector<cv::Mat> pres;
		pres.push_back(pa[0].mul(pb[0]) - pa[1].mul(pb[1]));
		pres.push_back(pa[0].mul(pb[1]) + pa[1].mul(pb[0]));

		cv::Mat res;
		cv::merge(pres, res);

		return res;
	}

	cv::Mat complexDivision(cv::Mat& a, cv::Mat& b)
	{
		std::vector<cv::Mat> pa;
		std::vector<cv::Mat> pb;
		cv::split(a, pa);
		cv::split(b, pb);

		cv::Mat divisor = 1. / (pb[0].mul(pb[0]) + pb[1].mul(pb[1]));

		std::vector<cv::Mat> pres;

		pres.push_back((pa[0].mul(pb[0]) + pa[1].mul(pb[1])).mul(divisor));
		pres.push_back((pa[1].mul(pb[0]) + pa[0].mul(pb[1])).mul(divisor));

		cv::Mat res;
		cv::merge(pres, res);
		return res;
	}

	void rearrange(cv::Mat &img)
	{
		// img = img(cv::Rect(0, 0, img.cols & -2, img.rows & -2));
		int cx = img.cols / 2;
		int cy = img.rows / 2;

		cv::Mat q0(img, cv::Rect(0, 0, cx, cy)); // Top-Left - Create a ROI per quadrant
		cv::Mat q1(img, cv::Rect(cx, 0, cx, cy)); // Top-Right
		cv::Mat q2(img, cv::Rect(0, cy, cx, cy)); // Bottom-Left
		cv::Mat q3(img, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

		cv::Mat tmp; // swap quadrants (Top-Left with Bottom-Right)
		q0.copyTo(tmp);
		q3.copyTo(q0);
		tmp.copyTo(q3);
		q1.copyTo(tmp); // swap quadrant (Top-Right with Bottom-Left)
		q2.copyTo(q1);
		tmp.copyTo(q2);
	}
	/*
	template < typename type>
	cv::Mat fouriertransFull(const cv::Mat & in)
	{
	return fftd(in);

	cv::Mat planes[] = {cv::Mat_<type > (in), cv::Mat_<type>::zeros(in.size())};
	cv::Mat t;
	assert(planes[0].depth() == planes[1].depth());
	assert(planes[0].size == planes[1].size);
	cv::merge(planes, 2, t);
	cv::dft(t, t);

	//cv::normalize(a, a, 0, 1, CV_MINMAX);
	//cv::normalize(t, t, 0, 1, CV_MINMAX);

	// cv::imshow("a",real(a));
	//  cv::imshow("b",real(t));
	// cv::waitKey(0);

	return t;
	}*/

	void normalizedLogTransform(cv::Mat &img)
	{
		img = cv::abs(img);
		img += cv::Scalar::all(1);
		cv::log(img, img);
		// cv::normalize(img, img, 0, 1, CV_MINMAX);
	}

}



typedef struct{
	int sizeX;
	int sizeY;
	int numFeatures;
	float *map;
} CvLSVMFeatureMapCaskade;

#define PI    CV_PI

#define EPS 0.000001

#define F_MAX FLT_MAX
#define F_MIN -FLT_MAX

// The number of elements in bin
// The number of sectors in gradient histogram building
#define NUM_SECTOR 9

// The number of levels in image resize procedure
// We need Lambda levels to resize image twice
#define LAMBDA 10

// Block size. Used in feature pyramid building procedure
#define SIDE_LENGTH 8

#define VAL_OF_TRUNCATE 0.2f 


//modified from "_lsvm_error.h"
#define LATENT_SVM_OK 0
#define LATENT_SVM_MEM_NULL 2
#define DISTANCE_TRANSFORM_OK 1
#define DISTANCE_TRANSFORM_GET_INTERSECTION_ERROR -1
#define DISTANCE_TRANSFORM_ERROR -2
#define DISTANCE_TRANSFORM_EQUAL_POINTS -3
#define LATENT_SVM_GET_FEATURE_PYRAMID_FAILED -4
#define LATENT_SVM_SEARCH_OBJECT_FAILED -5
#define LATENT_SVM_FAILED_SUPERPOSITION -6
#define FILTER_OUT_OF_BOUNDARIES -7
#define LATENT_SVM_TBB_SCHEDULE_CREATION_FAILED -8
#define LATENT_SVM_TBB_NUMTHREADS_NOT_CORRECT -9
#define FFT_OK 2
#define FFT_ERROR -10
#define LSVM_PARSER_FILE_NOT_FOUND -11

//modified from "lsvmc_routine.cpp"

int allocFeatureMapObject(CvLSVMFeatureMapCaskade **obj, const int sizeX,
	const int sizeY, const int numFeatures)
{
	int i;
	(*obj) = (CvLSVMFeatureMapCaskade *)malloc(sizeof(CvLSVMFeatureMapCaskade));
	(*obj)->sizeX = sizeX;
	(*obj)->sizeY = sizeY;
	(*obj)->numFeatures = numFeatures;
	(*obj)->map = (float *)malloc(sizeof (float)*
		(sizeX * sizeY  * numFeatures));
	for (i = 0; i < sizeX * sizeY * numFeatures; i++)
	{
		(*obj)->map[i] = 0.0f;
	}
	return LATENT_SVM_OK;
}

int freeFeatureMapObject(CvLSVMFeatureMapCaskade **obj)
{
	if (*obj == NULL) return LATENT_SVM_MEM_NULL;
	free((*obj)->map);
	free(*obj);
	(*obj) = NULL;
	return LATENT_SVM_OK;
}

int getFeatureMaps(const IplImage* image, const int k, CvLSVMFeatureMapCaskade **map)
{
	int sizeX, sizeY;
	int p, px, stringSize;
	int height, width, numChannels;
	int i, j, kk, c, ii, jj, d;
	float  * datadx, *datady;

	int   ch;
	float magnitude, x, y, tx, ty;

	IplImage * dx, *dy;
	int *nearest;
	float *w, a_x, b_x;

	float kernel[3] = { -1.f, 0.f, 1.f };
	CvMat kernel_dx = cvMat(1, 3, CV_32F, kernel);
	CvMat kernel_dy = cvMat(3, 1, CV_32F, kernel);

	float * r;
	int   * alfa;

	float boundary_x[NUM_SECTOR + 1];
	float boundary_y[NUM_SECTOR + 1];
	float max, dotProd;
	int   maxi;

	height = image->height;
	width = image->width;

	numChannels = image->nChannels;

	dx = cvCreateImage(cvSize(image->width, image->height),
		IPL_DEPTH_32F, 3);
	dy = cvCreateImage(cvSize(image->width, image->height),
		IPL_DEPTH_32F, 3);

	sizeX = width / k;
	sizeY = height / k;
	px = 3 * NUM_SECTOR;
	p = px;
	stringSize = sizeX * p;
	allocFeatureMapObject(map, sizeX, sizeY, p);

	cvFilter2D(image, dx, &kernel_dx, cvPoint(-1, 0));
	cvFilter2D(image, dy, &kernel_dy, cvPoint(0, -1));

	float arg_vector;
	for (i = 0; i <= NUM_SECTOR; i++)
	{
		arg_vector = ((float)i) * ((float)(PI) / (float)(NUM_SECTOR));
		boundary_x[i] = cosf(arg_vector);
		boundary_y[i] = sinf(arg_vector);
	}/*for(i = 0; i <= NUM_SECTOR; i++) */

	r = (float *)malloc(sizeof(float)* (width * height));
	alfa = (int   *)malloc(sizeof(int)* (width * height * 2));

	for (j = 1; j < height - 1; j++)
	{
		datadx = (float*)(dx->imageData + dx->widthStep * j);
		datady = (float*)(dy->imageData + dy->widthStep * j);
		for (i = 1; i < width - 1; i++)
		{
			c = 0;
			x = (datadx[i * numChannels + c]);
			y = (datady[i * numChannels + c]);

			r[j * width + i] = sqrtf(x * x + y * y);
			for (ch = 1; ch < numChannels; ch++)
			{
				tx = (datadx[i * numChannels + ch]);
				ty = (datady[i * numChannels + ch]);
				magnitude = sqrtf(tx * tx + ty * ty);
				if (magnitude > r[j * width + i])
				{
					r[j * width + i] = magnitude;
					c = ch;
					x = tx;
					y = ty;
				}
			}/*for(ch = 1; ch < numChannels; ch++)*/

			max = boundary_x[0] * x + boundary_y[0] * y;
			maxi = 0;
			for (kk = 0; kk < NUM_SECTOR; kk++)
			{
				dotProd = boundary_x[kk] * x + boundary_y[kk] * y;
				if (dotProd > max)
				{
					max = dotProd;
					maxi = kk;
				}
				else
				{
					if (-dotProd > max)
					{
						max = -dotProd;
						maxi = kk + NUM_SECTOR;
					}
				}
			}
			alfa[j * width * 2 + i * 2] = maxi % NUM_SECTOR;
			alfa[j * width * 2 + i * 2 + 1] = maxi;
		}/*for(i = 0; i < width; i++)*/
	}/*for(j = 0; j < height; j++)*/

	nearest = (int  *)malloc(sizeof(int)*  k);
	w = (float*)malloc(sizeof(float)* (k * 2));

	for (i = 0; i < k / 2; i++)
	{
		nearest[i] = -1;
	}/*for(i = 0; i < k / 2; i++)*/
	for (i = k / 2; i < k; i++)
	{
		nearest[i] = 1;
	}/*for(i = k / 2; i < k; i++)*/

	for (j = 0; j < k / 2; j++)
	{
		b_x = k / 2 + j + 0.5f;
		a_x = k / 2 - j - 0.5f;
		w[j * 2] = 1.0f / a_x * ((a_x * b_x) / (a_x + b_x));
		w[j * 2 + 1] = 1.0f / b_x * ((a_x * b_x) / (a_x + b_x));
	}/*for(j = 0; j < k / 2; j++)*/
	for (j = k / 2; j < k; j++)
	{
		a_x = j - k / 2 + 0.5f;
		b_x = -j + k / 2 - 0.5f + k;
		w[j * 2] = 1.0f / a_x * ((a_x * b_x) / (a_x + b_x));
		w[j * 2 + 1] = 1.0f / b_x * ((a_x * b_x) / (a_x + b_x));
	}/*for(j = k / 2; j < k; j++)*/

	for (i = 0; i < sizeY; i++)
	{
		for (j = 0; j < sizeX; j++)
		{
			for (ii = 0; ii < k; ii++)
			{
				for (jj = 0; jj < k; jj++)
				{
					if ((i * k + ii > 0) &&
						(i * k + ii < height - 1) &&
						(j * k + jj > 0) &&
						(j * k + jj < width - 1))
					{
						d = (k * i + ii) * width + (j * k + jj);
						(*map)->map[i * stringSize + j * (*map)->numFeatures + alfa[d * 2]] +=
							r[d] * w[ii * 2] * w[jj * 2];
						(*map)->map[i * stringSize + j * (*map)->numFeatures + alfa[d * 2 + 1] + NUM_SECTOR] +=
							r[d] * w[ii * 2] * w[jj * 2];
						if ((i + nearest[ii] >= 0) &&
							(i + nearest[ii] <= sizeY - 1))
						{
							(*map)->map[(i + nearest[ii]) * stringSize + j * (*map)->numFeatures + alfa[d * 2]] +=
								r[d] * w[ii * 2 + 1] * w[jj * 2];
							(*map)->map[(i + nearest[ii]) * stringSize + j * (*map)->numFeatures + alfa[d * 2 + 1] + NUM_SECTOR] +=
								r[d] * w[ii * 2 + 1] * w[jj * 2];
						}
						if ((j + nearest[jj] >= 0) &&
							(j + nearest[jj] <= sizeX - 1))
						{
							(*map)->map[i * stringSize + (j + nearest[jj]) * (*map)->numFeatures + alfa[d * 2]] +=
								r[d] * w[ii * 2] * w[jj * 2 + 1];
							(*map)->map[i * stringSize + (j + nearest[jj]) * (*map)->numFeatures + alfa[d * 2 + 1] + NUM_SECTOR] +=
								r[d] * w[ii * 2] * w[jj * 2 + 1];
						}
						if ((i + nearest[ii] >= 0) &&
							(i + nearest[ii] <= sizeY - 1) &&
							(j + nearest[jj] >= 0) &&
							(j + nearest[jj] <= sizeX - 1))
						{
							(*map)->map[(i + nearest[ii]) * stringSize + (j + nearest[jj]) * (*map)->numFeatures + alfa[d * 2]] +=
								r[d] * w[ii * 2 + 1] * w[jj * 2 + 1];
							(*map)->map[(i + nearest[ii]) * stringSize + (j + nearest[jj]) * (*map)->numFeatures + alfa[d * 2 + 1] + NUM_SECTOR] +=
								r[d] * w[ii * 2 + 1] * w[jj * 2 + 1];
						}
					}
				}/*for(jj = 0; jj < k; jj++)*/
			}/*for(ii = 0; ii < k; ii++)*/
		}/*for(j = 1; j < sizeX - 1; j++)*/
	}/*for(i = 1; i < sizeY - 1; i++)*/

	cvReleaseImage(&dx);
	cvReleaseImage(&dy);


	free(w);
	free(nearest);

	free(r);
	free(alfa);

	return LATENT_SVM_OK;
}

/*
// Feature map Normalization and Truncation
//
// API
// int normalizeAndTruncate(featureMap *map, const float alfa);
// INPUT
// map               - feature map
// alfa              - truncation threshold
// OUTPUT
// map               - truncated and normalized feature map
// RESULT
// Error status
*/
int normalizeAndTruncate(CvLSVMFeatureMapCaskade *map, const float alfa)
{
	int i, j, ii;
	int sizeX, sizeY, p, pos, pp, xp, pos1, pos2;
	float * partOfNorm; // norm of C(i, j)
	float * newData;
	float   valOfNorm;

	sizeX = map->sizeX;
	sizeY = map->sizeY;
	partOfNorm = (float *)malloc(sizeof(float)* (sizeX * sizeY));

	p = NUM_SECTOR;
	xp = NUM_SECTOR * 3;
	pp = NUM_SECTOR * 12;

	for (i = 0; i < sizeX * sizeY; i++)
	{
		valOfNorm = 0.0f;
		pos = i * map->numFeatures;
		for (j = 0; j < p; j++)
		{
			valOfNorm += map->map[pos + j] * map->map[pos + j];
		}/*for(j = 0; j < p; j++)*/
		partOfNorm[i] = valOfNorm;
	}/*for(i = 0; i < sizeX * sizeY; i++)*/

	sizeX -= 2;
	sizeY -= 2;

	newData = (float *)malloc(sizeof(float)* (sizeX * sizeY * pp));
	//normalization
	for (i = 1; i <= sizeY; i++)
	{
		for (j = 1; j <= sizeX; j++)
		{
			valOfNorm = sqrtf(
				partOfNorm[(i)*(sizeX + 2) + (j)] +
				partOfNorm[(i)*(sizeX + 2) + (j + 1)] +
				partOfNorm[(i + 1)*(sizeX + 2) + (j)] +
				partOfNorm[(i + 1)*(sizeX + 2) + (j + 1)]) + FLT_EPSILON;
			pos1 = (i)* (sizeX + 2) * xp + (j)* xp;
			pos2 = (i - 1) * (sizeX)* pp + (j - 1) * pp;
			for (ii = 0; ii < p; ii++)
			{
				newData[pos2 + ii] = map->map[pos1 + ii] / valOfNorm;
			}/*for(ii = 0; ii < p; ii++)*/
			for (ii = 0; ii < 2 * p; ii++)
			{
				newData[pos2 + ii + p * 4] = map->map[pos1 + ii + p] / valOfNorm;
			}/*for(ii = 0; ii < 2 * p; ii++)*/
			valOfNorm = sqrtf(
				partOfNorm[(i)*(sizeX + 2) + (j)] +
				partOfNorm[(i)*(sizeX + 2) + (j + 1)] +
				partOfNorm[(i - 1)*(sizeX + 2) + (j)] +
				partOfNorm[(i - 1)*(sizeX + 2) + (j + 1)]) + FLT_EPSILON;
			for (ii = 0; ii < p; ii++)
			{
				newData[pos2 + ii + p] = map->map[pos1 + ii] / valOfNorm;
			}/*for(ii = 0; ii < p; ii++)*/
			for (ii = 0; ii < 2 * p; ii++)
			{
				newData[pos2 + ii + p * 6] = map->map[pos1 + ii + p] / valOfNorm;
			}/*for(ii = 0; ii < 2 * p; ii++)*/
			valOfNorm = sqrtf(
				partOfNorm[(i)*(sizeX + 2) + (j)] +
				partOfNorm[(i)*(sizeX + 2) + (j - 1)] +
				partOfNorm[(i + 1)*(sizeX + 2) + (j)] +
				partOfNorm[(i + 1)*(sizeX + 2) + (j - 1)]) + FLT_EPSILON;
			for (ii = 0; ii < p; ii++)
			{
				newData[pos2 + ii + p * 2] = map->map[pos1 + ii] / valOfNorm;
			}/*for(ii = 0; ii < p; ii++)*/
			for (ii = 0; ii < 2 * p; ii++)
			{
				newData[pos2 + ii + p * 8] = map->map[pos1 + ii + p] / valOfNorm;
			}/*for(ii = 0; ii < 2 * p; ii++)*/
			valOfNorm = sqrtf(
				partOfNorm[(i)*(sizeX + 2) + (j)] +
				partOfNorm[(i)*(sizeX + 2) + (j - 1)] +
				partOfNorm[(i - 1)*(sizeX + 2) + (j)] +
				partOfNorm[(i - 1)*(sizeX + 2) + (j - 1)]) + FLT_EPSILON;
			for (ii = 0; ii < p; ii++)
			{
				newData[pos2 + ii + p * 3] = map->map[pos1 + ii] / valOfNorm;
			}/*for(ii = 0; ii < p; ii++)*/
			for (ii = 0; ii < 2 * p; ii++)
			{
				newData[pos2 + ii + p * 10] = map->map[pos1 + ii + p] / valOfNorm;
			}/*for(ii = 0; ii < 2 * p; ii++)*/
		}/*for(j = 1; j <= sizeX; j++)*/
	}/*for(i = 1; i <= sizeY; i++)*/
	//truncation
	for (i = 0; i < sizeX * sizeY * pp; i++)
	{
		if (newData[i] > alfa) newData[i] = alfa;
	}/*for(i = 0; i < sizeX * sizeY * pp; i++)*/
	//swop data

	map->numFeatures = pp;
	map->sizeX = sizeX;
	map->sizeY = sizeY;

	free(map->map);
	free(partOfNorm);

	map->map = newData;

	return LATENT_SVM_OK;
}
/*
// Feature map reduction
// In each cell we reduce dimension of the feature vector
// according to original paper special procedure
//
// API
// int PCAFeatureMaps(featureMap *map)
// INPUT
// map               - feature map
// OUTPUT
// map               - feature map
// RESULT
// Error status
*/
int PCAFeatureMaps(CvLSVMFeatureMapCaskade *map)
{
	int i, j, ii, jj, k;
	int sizeX, sizeY, p, pp, xp, yp, pos1, pos2;
	float * newData;
	float val;
	float nx, ny;

	sizeX = map->sizeX;
	sizeY = map->sizeY;
	p = map->numFeatures;
	pp = NUM_SECTOR * 3 + 4;
	yp = 4;
	xp = NUM_SECTOR;

	nx = 1.0f / sqrtf((float)(xp * 2));
	ny = 1.0f / sqrtf((float)(yp));

	newData = (float *)malloc(sizeof(float)* (sizeX * sizeY * pp));

	for (i = 0; i < sizeY; i++)
	{
		for (j = 0; j < sizeX; j++)
		{
			pos1 = ((i)*sizeX + j)*p;
			pos2 = ((i)*sizeX + j)*pp;
			k = 0;
			for (jj = 0; jj < xp * 2; jj++)
			{
				val = 0;
				for (ii = 0; ii < yp; ii++)
				{
					val += map->map[pos1 + yp * xp + ii * xp * 2 + jj];
				}/*for(ii = 0; ii < yp; ii++)*/
				newData[pos2 + k] = val * ny;
				k++;
			}/*for(jj = 0; jj < xp * 2; jj++)*/
			for (jj = 0; jj < xp; jj++)
			{
				val = 0;
				for (ii = 0; ii < yp; ii++)
				{
					val += map->map[pos1 + ii * xp + jj];
				}/*for(ii = 0; ii < yp; ii++)*/
				newData[pos2 + k] = val * ny;
				k++;
			}/*for(jj = 0; jj < xp; jj++)*/
			for (ii = 0; ii < yp; ii++)
			{
				val = 0;
				for (jj = 0; jj < 2 * xp; jj++)
				{
					val += map->map[pos1 + yp * xp + ii * xp * 2 + jj];
				}/*for(jj = 0; jj < xp; jj++)*/
				newData[pos2 + k] = val * nx;
				k++;
			} /*for(ii = 0; ii < yp; ii++)*/
		}/*for(j = 0; j < sizeX; j++)*/
	}/*for(i = 0; i < sizeY; i++)*/
	//swop data

	map->numFeatures = pp;

	free(map->map);

	map->map = newData;

	return LATENT_SVM_OK;
}

#endif