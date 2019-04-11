#include <iostream>
#include "FaceExperiment.h"

Mat srcImage;

Mat FaceExperiment::kernelTest()//int n1, int n2, int n3, string kern1, string kern2, string kern3)
{
	srcImage = imread(srcImgPath);

	Mat Kernel_Sobel_H = (Mat_<int>(3, 3) << -1, -1, -1,
		0, 0, 0,
		1, 1, 1);

	Mat Kernel_Sobel_V = (Mat_<int>(3, 3) << -1, 0, 1,
		-1, 0, 1,
		-1, 0, 1);

	Mat Kernel_Laplace = (Mat_<char>(3, 3) << 0, -1, 0,
		-1, 5, -1,
		0, -1, 0);
	cout << kernel1 << endl;
	cout << kernel2 << endl;
	cout << kernel3 << endl;

	if (kernel1 == "Sobel_H" && kernNum1 > 0)
	{
		for (int i = 0; i < kernNum1; i++)
		{
			filter2D(srcImage, srcImage, srcImage.depth(), Kernel_Sobel_H);
		}
	}
	if (kernel1 == "Sobel_V" && kernNum1 > 0)
	{
		for (int i = 0; i < kernNum1; i++)
		{
			filter2D(srcImage, srcImage, srcImage.depth(), Kernel_Sobel_V);
		}
	}
	if (kernel1 == "Laplace" && kernNum1 > 0)
	{
		for (int i = 0; i < kernNum1; i++)
		{
			filter2D(srcImage, srcImage, srcImage.depth(), Kernel_Laplace);
		}
	}


	if (kernel2 == "Sobel_H" && kernNum2 > 0)
	{
		for (int i = 0; i < kernNum2; i++)
		{
			filter2D(srcImage, srcImage, srcImage.depth(), Kernel_Sobel_H);
		}
	}
	if (kernel2 == "Sobel_V" && kernNum2 > 0)
	{
		for (int i = 0; i < kernNum2; i++)
		{
			filter2D(srcImage, srcImage, srcImage.depth(), Kernel_Sobel_V);
		}
	}
	if (kernel2 == "Laplace" && kernNum2 > 0)
	{
		for (int i = 0; i < kernNum2; i++)
		{
			filter2D(srcImage, srcImage, srcImage.depth(), Kernel_Laplace);
		}
	}


	if (kernel3 == "Sobel_H" && kernNum3 > 0)
	{
		for (int i = 0; i < kernNum3; i++)
		{
			filter2D(srcImage, srcImage, srcImage.depth(), Kernel_Sobel_H);
		}
	}
	if (kernel3 == "Sobel_V" && kernNum3 > 0)
	{
		for (int i = 0; i < kernNum3; i++)
		{
			filter2D(srcImage, srcImage, srcImage.depth(), Kernel_Sobel_V);
		}
	}
	if (kernel3 == "Laplace" && kernNum3 > 0)
	{
		for (int i = 0; i < kernNum3; i++)
		{
			filter2D(srcImage, srcImage, srcImage.depth(), Kernel_Laplace);
		}
	}
	cvtColor(srcImage, srcImage, CV_BGR2RGB);

	return srcImage;
}