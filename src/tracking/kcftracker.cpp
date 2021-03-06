/*

Tracker based on Kernelized Correlation Filter (KCF) [1] and Circulant Structure with Kernels (CSK) [2].
CSK is implemented by using raw gray level features, since it is a single-channel filter.
KCF is implemented by using HOG features (the default), since it extends CSK to multiple channels.

[1] J. F. Henriques, R. Caseiro, P. Martins, J. Batista,
"High-Speed Tracking with Kernelized Correlation Filters", TPAMI 2015.

[2] J. F. Henriques, R. Caseiro, P. Martins, J. Batista,
"Exploiting the Circulant Structure of Tracking-by-detection with Kernels", ECCV 2012.

Authors: Joao Faro, Christian Bailer, Joao F. Henriques
Contacts: joaopfaro@gmail.com, Christian.Bailer@dfki.de, henriques@isr.uc.pt
Institute of Systems and Robotics - University of Coimbra / Department Augmented Vision DFKI


Constructor parameters, all boolean:
    hog: use HOG features (default), otherwise use raw pixels
    fixed_window: fix window size (default), otherwise use ROI size (slower but more accurate)
    multiscale: use multi-scale tracking (default; cannot be used with fixed_window = true)

Default values are set for all properties of the tracker depending on the above choices.
Their values can be customized further before calling init():
    interp_factor: linear interpolation factor for adaptation
    sigma: gaussian kernel bandwidth
    lambda: regularization
    cell_size: HOG cell size
    padding: area surrounding the target, relative to its size
    output_sigma_factor: bandwidth of gaussian target
    template_size: template size in pixels, 0 to use ROI size
    scale_step: scale step for multi-scale estimation, 1 to disable it
    scale_weight: to downweight detection scores of other scales for added stability

For speed, the value (template_size/cell_size) should be a power of 2 or a product of small prime numbers.

Inputs to init():
   image is the initial frame.
   roi is a cv::Rect with the target positions in the initial frame

Inputs to update():
   image is the current frame.

Outputs of update():
   cv::Rect with target positions for the current frame


By downloading, copying, installing or using the software you agree to this license.
If you do not agree to this license, do not download, install,
copy or use the software.


                          License Agreement
               For Open Source Computer Vision Library
                       (3-clause BSD License)

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  * Neither the names of the copyright holders nor the names of the contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

This software is provided by the copyright holders and contributors "as is" and
any express or implied warranties, including, but not limited to, the implied
warranties of merchantability and fitness for a particular purpose are disclaimed.
In no event shall copyright holders or contributors be liable for any direct,
indirect, incidental, special, exemplary, or consequential damages
(including, but not limited to, procurement of substitute goods or services;
loss of use, data, or profits; or business interruption) however caused
and on any theory of liability, whether in contract, strict liability,
or tort (including negligence or otherwise) arising in any way out of
the use of this software, even if advised of the possibility of such damage.
 */

#ifndef _KCFTRACKER_HEADERS
#include "kcftracker.hpp"
#include "external_lib.hpp"
#include <algorithm>
//#include "fhog.hpp"
#endif

// Constructor
KCFTracker::KCFTracker(bool hog, bool fixed_window, bool multiscale, bool lab)
{
	meanAPCE = 0.0f;
    // Parameters equal in all cases
    lambda = 0.0001;
    padding = 2.5; 
    //output_sigma_factor = 0.1;
    output_sigma_factor = 0.125;


    if (hog) {    // HOG
        // VOT
        interp_factor = 0.012;
        sigma = 0.6; 
        // TPAMI
        //interp_factor = 0.02;
        //sigma = 0.5; 
        cell_size = 4;
        _hogfeatures = true;

        if (lab) {
            interp_factor = 0.005;
            sigma = 0.4; 
            //output_sigma_factor = 0.025;
            output_sigma_factor = 0.1;

            _labfeatures = true;
            _labCentroids = cv::Mat(nClusters, 3, CV_32FC1, &data);
            cell_sizeQ = cell_size*cell_size;
        }
        else{
            _labfeatures = false;
        }
    }
    else {   // RAW
        interp_factor = 0.075;
        sigma = 0.2; 
        cell_size = 1;
        _hogfeatures = false;

        if (lab) {
            printf("Lab features are only used with HOG features.\n");
            _labfeatures = false;
        }
    }


    if (multiscale) { // multiscale
        template_size = 96;
        //template_size = 100;
        scale_step = 1.05;
        scale_weight = 0.95;
        if (!fixed_window) {
            //printf("Multiscale does not support non-fixed window.\n");
            fixed_window = true;
        }
    }
    else if (fixed_window) {  // fit correction without multiscale
        template_size = 96;
        //template_size = 100;
        scale_step = 1;
    }
    else {
        template_size = 1;
        scale_step = 1;
    }
	scale = 0;
}

// Initialize tracker 
void KCFTracker::init(cv::Rect &roi, cv::Mat image)
{
	cv::Mat imgTmp = cv::Mat(image.rows, image.cols, CV_8UC3);
	cv::cvtColor(image, imgTmp, CV_BGRA2BGR);
	//resize(imgTmp, imgTmp, cv::Size(640, 368));
	//cv::Rect newroi;
	//newroi.x = roi.x * 640 / 720;
	//newroi.width = roi.width * 640 / 720;
	//newroi.y = roi.y * 364 / 404;
	//newroi.height = roi.height * 368 / 404;

	//_roi = newroi;

	//_roi = newroi;
	//_roi.width = 60;
	//_roi.height = 90;
	//_roi.x = (newroi.x + newroi.width / 2 - 30) < 0 ? 0 : (newroi.x + newroi.width / 2 - 30);
	//_roi.y = (newroi.y + newroi.height / 2 - 45) < 0 ? 0 : (newroi.y + newroi.height / 2 - 45);

	//roi.x = _roi.x * 720 / 640;
	//roi.width = _roi.width * 720 / 640;
	//roi.y = _roi.y * 404 / 368;
	//roi.height = _roi.height * 404 / 368;

	_roi = roi;
    assert(roi.width >= 0 && roi.height >= 0);
	_tmpl = getFeatures(imgTmp, 1);
    _prob = createGaussianPeak(size_patch[0], size_patch[1]);
    _alphaf = cv::Mat(size_patch[0], size_patch[1], CV_32FC2, float(0));
    //_num = cv::Mat(size_patch[0], size_patch[1], CV_32FC2, float(0));
    //_den = cv::Mat(size_patch[0], size_patch[1], CV_32FC2, float(0));
    train(_tmpl, 1.0); // train with initial frame
	imgTmp.release();
 }
// Update position based on the new frame
cv::Rect KCFTracker::update(cv::Mat image)
{
	cv::Mat imgTmp = cv::Mat(image.rows, image.cols, CV_8UC3);
	cv::cvtColor(image, imgTmp, CV_BGRA2BGR);
	//resize(imgTmp, imgTmp, Size(640, 368));

    if (_roi.x + _roi.width <= 0) _roi.x = -_roi.width + 1;
    if (_roi.y + _roi.height <= 0) _roi.y = -_roi.height + 1;
	if (_roi.x >= imgTmp.cols - 1) _roi.x = imgTmp.cols - 2;
	if (_roi.y >= imgTmp.rows - 1) _roi.y = imgTmp.rows - 2;

    float cx = _roi.x + _roi.width / 2.0f;
    float cy = _roi.y + _roi.height / 2.0f;

	//if (scale == 0 || scale == 1)
	if (true)
	{
		//float peak_value;
		cv::Point2f res = detect(_tmpl, getFeatures(imgTmp, 0, 1.0f), peak_value);

		if (scale_step != 1) {
			// Test at a bigger _scale
			float new_peak_value;
			cv::Point2f new_res = detect(_tmpl, getFeatures(imgTmp, 0, scale_step), new_peak_value);

			if (scale_weight * new_peak_value > peak_value) {
				res = new_res;
				peak_value = new_peak_value;
				_scale *= scale_step;
				_roi.width *= scale_step;
				_roi.height *= scale_step;
				scale = 1;
			}
			else
				scale = 0;

			// Test at a smaller _scale
			new_res = detect(_tmpl, getFeatures(imgTmp, 0, 1.0f / 1.02), new_peak_value);

			if (0.98 * new_peak_value > peak_value) {
			//if (scale_weight * new_peak_value > peak_value) {
				res = new_res;
				peak_value = new_peak_value;
				_scale /= 1.02;
				_roi.width /= 1.02;
				_roi.height /= 1.02;
				scale = 2;
			}
			else
				scale = 0;
		}

		// Adjust by cell size and _scale
		_roi.x = cx - _roi.width / 2.0f + ((float)res.x * cell_size * _scale);
		_roi.y = cy - _roi.height / 2.0f + ((float)res.y * cell_size * _scale);

		if (_roi.x >= imgTmp.cols - 1) _roi.x = imgTmp.cols - 1;
		if (_roi.y >= imgTmp.rows - 1) _roi.y = imgTmp.rows - 1;
		if (_roi.x + _roi.width <= 0) _roi.x = -_roi.width + 2;
		if (_roi.y + _roi.height <= 0) _roi.y = -_roi.height + 2;

		assert(_roi.width >= 0 && _roi.height >= 0);
		cv::Mat x = getFeatures(imgTmp, 0);
		train(x, interp_factor);
	}
	//else
	//{
	//	float peak_value;
	//	cv::Point2f res;

	//	cv::Rect curroi = _roi;
	//	GetdetectWindow(imgTmp, curroi, detectroi, 1.5);
	//	cv::rectangle(imgTmp, detectroi, cv::Scalar(0, 0, 255), 2);
	//	HD.fastscan(imgTmp(detectroi), results, score);
	//	if (results.size() ==1)
	//	{
	//		results[0].x += detectroi.x;
	//		results[0].y += detectroi.y;
	//		_roi = results[0];
	//		//cv::rectangle(imgTmp, results[0], cv::Scalar(0, 255, 0), 2);
	//		//res = detect(_tmpl, getFeatures(imgTmp, 0, 1.0f), peak_value);
	//		//scale = 0;
	//	}
	//	else if (results.size() > 1)
	//	{
	//		int minx = 0, miny = 0, maxx = 0, maxy = 0;
	//		for (int i = 0; i < results.size(); i++)
	//		{
	//			minx = min(minx, results[i].x);
	//			miny = min(miny, results[i].y);
	//			maxx = max(maxx, results[i].x + results[i].width);
	//			maxy = max(maxy, results[i].y + results[i].height);
	//			//cv::rectangle(imgTmp, results[i], cv::Scalar(0, 255, 0), 2);
	//		}
	//		_roi.x = minx;
	//		_roi.y = miny;
	//		_roi.width = maxx - minx;
	//		_roi.height = maxy - miny;
	//	}
	//	// Test at a smaller _scale
	//	//res = detect(_tmpl, getFeatures(imgTmp, 0, 1.0f), peak_value);
	//	res = detect(_tmpl, getFeatures(imgTmp, 0, 1.0f / scale_step), peak_value);
	//	scale = 0;

	//	// Adjust by cell size and _scale
	//	_roi.x = cx - _roi.width / 2.0f + ((float)res.x * cell_size * _scale);
	//	_roi.y = cy - _roi.height / 2.0f + ((float)res.y * cell_size * _scale);

	//	if (_roi.x >= imgTmp.cols - 1) _roi.x = imgTmp.cols - 1;
	//	if (_roi.y >= imgTmp.rows - 1) _roi.y = imgTmp.rows - 1;
	//	if (_roi.x + _roi.width <= 0) _roi.x = -_roi.width + 2;
	//	if (_roi.y + _roi.height <= 0) _roi.y = -_roi.height + 2;

	//	assert(_roi.width >= 0 && _roi.height >= 0);
	//	cv::Mat x = getFeatures(imgTmp, 0);
	//	train(x, interp_factor);
	//}


	imgTmp.release();

	//cv::Rect newroi;
	//newroi.x = _roi.x * 720 / 640;
	//newroi.width = _roi.width * 720 / 640;
	//newroi.y = _roi.y * 404 / 368;
	//newroi.height = _roi.height * 404 / 368;

	return _roi;
}

cv::Mat KCFTracker::test(cv::Mat z, cv::Mat x)
{
	using namespace FFTTools;

	cv::Mat k = gaussianCorrelation(x, z);
	cv::Mat fftdTmp = fftd(k);
	cv::Mat calRes = complexMultiplication(_alphaf, fftdTmp);
	fftdTmp = fftd(calRes, true);
	cv::Mat res = real(fftdTmp);

	return res;
}

// Detect object in the current frame.
cv::Point2f KCFTracker::detect(cv::Mat z, cv::Mat x, float &peak_value)
{
    using namespace FFTTools;

    cv::Mat k = gaussianCorrelation(x, z);
	cv::Mat fftdTmp = fftd(k);
	cv::Mat calRes = complexMultiplication(_alphaf, fftdTmp);
	fftdTmp = fftd(calRes, true);
	Res = real(fftdTmp);

	//cv::imshow("res", res);
	//cv::Mat res_show;
	//cv::resize(res, res_show, cv::Size(_tmpl_sz.width * 5, _tmpl_sz.height * 5));
	//cv::imshow("Map", res_show);
	//cv::waitKey(1);

    //minMaxLoc only accepts doubles for the peak, and integer points for the coordinates
    cv::Point2i pi, vi;
    double pv, vv;
    cv::minMaxLoc(Res, &vv, &pv, &vi, &pi);
    peak_value = (float) pv;
	float valley_value = (float) vv;
	/****************************Modify*****************************/
	cv::Mat minValMap(Res.rows, Res.cols, CV_32FC1, valley_value);
	cv::Mat tmp;
	pow((Res - minValMap), 2, tmp);
	cv::Scalar sum_value = cv::sum(tmp);

	float APCE = ((pv - vv) * (pv - vv)) / ((double)sum_value[0] / (Res.rows * Res.cols));
	if (meanAPCE == 0)
		meanAPCE = APCE;
	/****************************Modify*****************************/

    //subpixel peak estimation, coordinates will be non-integer
    cv::Point2f p((float)pi.x, (float)pi.y);

    if (pi.x > 0 && pi.x < Res.cols-1) {
        p.x += subPixelPeak(Res.at<float>(pi.y, pi.x-1), peak_value, Res.at<float>(pi.y, pi.x+1));
    }

    if (pi.y > 0 && pi.y < Res.rows-1) {
        p.y += subPixelPeak(Res.at<float>(pi.y-1, pi.x), peak_value, Res.at<float>(pi.y+1, pi.x));
    }

    p.x -= (Res.cols) / 2;
    p.y -= (Res.rows) / 2;

    return p;
}

// train tracker with a single image
void KCFTracker::train(cv::Mat x, float train_interp_factor)
{
    using namespace FFTTools;

    cv::Mat k = gaussianCorrelation(x, x);
	cv::Mat tmp = fftd(k) + lambda;
    cv::Mat alphaf = complexDivision(_prob, tmp);
    
    _tmpl = (1 - train_interp_factor) * _tmpl + (train_interp_factor) * x;
    _alphaf = (1 - train_interp_factor) * _alphaf + (train_interp_factor) * alphaf;


    /*cv::Mat kf = fftd(gaussianCorrelation(x, x));
    cv::Mat num = complexMultiplication(kf, _prob);
    cv::Mat den = complexMultiplication(kf, kf + lambda);
    
    _tmpl = (1 - train_interp_factor) * _tmpl + (train_interp_factor) * x;
    _num = (1 - train_interp_factor) * _num + (train_interp_factor) * num;
    _den = (1 - train_interp_factor) * _den + (train_interp_factor) * den;

    _alphaf = complexDivision(_num, _den);*/

}

// Evaluates a Gaussian kernel with bandwidth SIGMA for all relative shifts between input images X and Y, which must both be MxN. They must    also be periodic (ie., pre-processed with a cosine window).
cv::Mat KCFTracker::gaussianCorrelation(cv::Mat x1, cv::Mat x2)
{
    using namespace FFTTools;
    cv::Mat c = cv::Mat( cv::Size(size_patch[1], size_patch[0]), CV_32F, cv::Scalar(0) );
    // HOG features
    if (_hogfeatures) {
        cv::Mat caux;
        cv::Mat x1aux;
        cv::Mat x2aux;
        for (int i = 0; i < size_patch[2]; i++) {
            x1aux = x1.row(i);   // Procedure do deal with cv::Mat multichannel bug
            x1aux = x1aux.reshape(1, size_patch[0]);
            x2aux = x2.row(i).reshape(1, size_patch[0]);
            cv::mulSpectrums(fftd(x1aux), fftd(x2aux), caux, 0, true); 
            caux = fftd(caux, true);
            rearrange(caux);
            caux.convertTo(caux,CV_32F);
            c = c + real(caux);
        }
    }
    // Gray features
    else {
        cv::mulSpectrums(fftd(x1), fftd(x2), c, 0, true);
        c = fftd(c, true);
        rearrange(c);
        c = real(c);
    }
    cv::Mat d; 
    cv::max(( (cv::sum(x1.mul(x1))[0] + cv::sum(x2.mul(x2))[0])- 2. * c) / (size_patch[0]*size_patch[1]*size_patch[2]) , 0, d);

    cv::Mat k;
    cv::exp((-d / (sigma * sigma)), k);
    return k;
}

// Create Gaussian Peak. Function called only in the first frame.
cv::Mat KCFTracker::createGaussianPeak(int sizey, int sizex)
{
    cv::Mat_<float> res(sizey, sizex);

    int syh = (sizey) / 2;
    int sxh = (sizex) / 2;

    float output_sigma = std::sqrt((float) sizex * sizey) / padding * output_sigma_factor;
    float mult = -0.5 / (output_sigma * output_sigma);

    for (int i = 0; i < sizey; i++)
        for (int j = 0; j < sizex; j++)
        {
            int ih = i - syh;
            int jh = j - sxh;
            res(i, j) = std::exp(mult * (float) (ih * ih + jh * jh));
        }
    return FFTTools::fftd(res);
}

// Obtain sub-window from image, with replication-padding and extract features
cv::Mat KCFTracker::getFeatures(const cv::Mat & image, bool inithann, float scale_adjust)
{
    cv::Rect extracted_roi;

    float cx = _roi.x + _roi.width / 2;
    float cy = _roi.y + _roi.height / 2;

    if (inithann) {
        int padded_w = _roi.width * padding;
        int padded_h = _roi.height * padding;
        
        if (template_size > 1) {  // Fit largest dimension to the given template size
            if (padded_w >= padded_h)  //fit to width
                _scale = padded_w / (float) template_size;
            else
                _scale = padded_h / (float) template_size;

            _tmpl_sz.width = padded_w / _scale;
            _tmpl_sz.height = padded_h / _scale;
        }
        else {  //No template size given, use ROI size
            _tmpl_sz.width = padded_w;
            _tmpl_sz.height = padded_h;
            _scale = 1;
            // original code from paper:
            /*if (sqrt(padded_w * padded_h) >= 100) {   //Normal size
                _tmpl_sz.width = padded_w;
                _tmpl_sz.height = padded_h;
                _scale = 1;
            }
            else {   //ROI is too big, track at half size
                _tmpl_sz.width = padded_w / 2;
                _tmpl_sz.height = padded_h / 2;
                _scale = 2;
            }*/
        }

        if (_hogfeatures) {
            // Round to cell size and also make it even
            _tmpl_sz.width = ( ( (int)(_tmpl_sz.width / (2 * cell_size)) ) * 2 * cell_size ) + cell_size*2;
            _tmpl_sz.height = ( ( (int)(_tmpl_sz.height / (2 * cell_size)) ) * 2 * cell_size ) + cell_size*2;
        }
        else {  //Make number of pixels even (helps with some logic involving half-dimensions)
            _tmpl_sz.width = (_tmpl_sz.width / 2) * 2;
            _tmpl_sz.height = (_tmpl_sz.height / 2) * 2;
        }
    }

    extracted_roi.width = scale_adjust * _scale * _tmpl_sz.width;
    extracted_roi.height = scale_adjust * _scale * _tmpl_sz.height;

    // center roi with new size
    extracted_roi.x = cx - extracted_roi.width / 2;
    extracted_roi.y = cy - extracted_roi.height / 2;

    cv::Mat FeaturesMap;  
    cv::Mat z = RectTools::subwindow(image, extracted_roi, cv::BORDER_REPLICATE);
    
    if (z.cols != _tmpl_sz.width || z.rows != _tmpl_sz.height) {
        cv::resize(z, z, _tmpl_sz);
    }   

    // HOG features
    if (_hogfeatures) {
        IplImage z_ipl = z;
        CvLSVMFeatureMapCaskade *map;
        getFeatureMaps(&z_ipl, cell_size, &map);
        normalizeAndTruncate(map,0.2f);
        PCAFeatureMaps(map);
        size_patch[0] = map->sizeY;
        size_patch[1] = map->sizeX;
        size_patch[2] = map->numFeatures;

        FeaturesMap = cv::Mat(cv::Size(map->numFeatures,map->sizeX*map->sizeY), CV_32F, map->map);  // Procedure do deal with cv::Mat multichannel bug
        FeaturesMap = FeaturesMap.t();
        freeFeatureMapObject(&map);

        // Lab features
        if (_labfeatures) {
            cv::Mat imgLab;
            cvtColor(z, imgLab, CV_BGR2Lab);
            unsigned char *input = (unsigned char*)(imgLab.data);

            // Sparse output vector
            cv::Mat outputLab = cv::Mat(_labCentroids.rows, size_patch[0]*size_patch[1], CV_32F, float(0));

            int cntCell = 0;
            // Iterate through each cell
            for (int cY = cell_size; cY < z.rows-cell_size; cY+=cell_size){
                for (int cX = cell_size; cX < z.cols-cell_size; cX+=cell_size){
                    // Iterate through each pixel of cell (cX,cY)
                    for(int y = cY; y < cY+cell_size; ++y){
                        for(int x = cX; x < cX+cell_size; ++x){
                            // Lab components for each pixel
                            float l = (float)input[(z.cols * y + x) * 3];
                            float a = (float)input[(z.cols * y + x) * 3 + 1];
                            float b = (float)input[(z.cols * y + x) * 3 + 2];

                            // Iterate trough each centroid
                            float minDist = FLT_MAX;
                            int minIdx = 0;
                            float *inputCentroid = (float*)(_labCentroids.data);
                            for(int k = 0; k < _labCentroids.rows; ++k){
                                float dist = ( (l - inputCentroid[3*k]) * (l - inputCentroid[3*k]) )
                                           + ( (a - inputCentroid[3*k+1]) * (a - inputCentroid[3*k+1]) ) 
                                           + ( (b - inputCentroid[3*k+2]) * (b - inputCentroid[3*k+2]) );
                                if(dist < minDist){
                                    minDist = dist;
                                    minIdx = k;
                                }
                            }
                            // Store result at output
                            outputLab.at<float>(minIdx, cntCell) += 1.0 / cell_sizeQ; 
                            //((float*) outputLab.data)[minIdx * (size_patch[0]*size_patch[1]) + cntCell] += 1.0 / cell_sizeQ; 
                        }
                    }
                    cntCell++;
                }
            }
            // Update size_patch[2] and add features to FeaturesMap
            size_patch[2] += _labCentroids.rows;
            FeaturesMap.push_back(outputLab);
        }
    }
    else {
        FeaturesMap = RectTools::getGrayImage(z);
        FeaturesMap -= (float) 0.5; // In Paper;
        size_patch[0] = z.rows;
        size_patch[1] = z.cols;
        size_patch[2] = 1;  
    }
    
    if (inithann) {
        createHanningMats();
    }
    FeaturesMap = hann.mul(FeaturesMap);
    return FeaturesMap;
}
    
// Initialize Hanning window. Function called only in the first frame.
void KCFTracker::createHanningMats()
{   
    cv::Mat hann1t = cv::Mat(cv::Size(size_patch[1],1), CV_32F, cv::Scalar(0));
    cv::Mat hann2t = cv::Mat(cv::Size(1,size_patch[0]), CV_32F, cv::Scalar(0)); 

    for (int i = 0; i < hann1t.cols; i++)
        hann1t.at<float > (0, i) = 0.5 * (1 - std::cos(2 * 3.14159265358979323846 * i / (hann1t.cols - 1)));
    for (int i = 0; i < hann2t.rows; i++)
        hann2t.at<float > (i, 0) = 0.5 * (1 - std::cos(2 * 3.14159265358979323846 * i / (hann2t.rows - 1)));

    cv::Mat hann2d = hann2t * hann1t;
    // HOG features
    if (_hogfeatures) {
        cv::Mat hann1d = hann2d.reshape(1,1); // Procedure do deal with cv::Mat multichannel bug
        
        hann = cv::Mat(cv::Size(size_patch[0]*size_patch[1], size_patch[2]), CV_32F, cv::Scalar(0));
        for (int i = 0; i < size_patch[2]; i++) {
            for (int j = 0; j<size_patch[0]*size_patch[1]; j++) {
                hann.at<float>(i,j) = hann1d.at<float>(0,j);
            }
        }
    }
    // Gray features
    else {
        hann = hann2d;
    }
}

// Calculate sub-pixel peak for one dimension
float KCFTracker::subPixelPeak(float left, float center, float right)
{   
    float divisor = 2 * center - right - left;

    if (divisor == 0)
        return 0;
    
    return 0.5 * (right - left) / divisor;
}

/////////////////////////UAVdata///////////////////////////////
void DataInit(UAVData& data)
{
	data.BBox.x = -1;
	data.BBox.y = -1;
	data.BBox.width = 0;
	data.BBox.height = 0;

	data.distance = 0;
	data.ratio = 0;

	data.ahead = false;
	data.back = false;
	data.right = false;
	data.left = false;
	data.up = false;
	data.down = false;

	data.aheadBack = 128;
	data.leftRight = 128;
	data.upDown = 128;
}

void DataAnalyze(UAVData& dataold, UAVData& datanew, cv::Rect& result, int& framewidth, int& frameheight, bool GPS)
{
	datanew.BBox = result;
	datanew.distance = result.area();
	datanew.ratio = float(result.width) / float(result.height);
	float fw = float(framewidth);
	float fh = float(frameheight);

	cv::Point2f center, boxcenter;
	center.x = fw / 2;
	center.y = fh / 2;
	boxcenter.x = result.x + float(result.width) / 2;
	boxcenter.y = result.y + float(result.height) / 2;

	float dis = 0.0;

	//back(priority 1)
	if (GPS)
	{
		if (boxcenter.y < 180 || boxcenter.y > 253)
		{
			if (boxcenter.y < 180){
				datanew.ahead = true;
				datanew.back = false;
				datanew.aheadBack = -0.55 * fabs(boxcenter.y - 180) + 102;
			}
			if (boxcenter.y > 253){
				datanew.back = true;
				datanew.ahead = false;
				datanew.aheadBack = 0.36 * fabs(boxcenter.y - 220) + 154;
			}
			//datanew.aheadBack = 128;
		}
	}
	else
	{
		if (dataold.BBox.x != -1 && dataold.BBox.y != -1)
		{
			dis = (datanew.distance - dataold.distance) / dataold.distance;
			std::cout << "dis: " << dis << std::endl;
			if (fabs(dis) > 0.2 && fabs(dis) < 0.7){
				dis = fabs(dis) > 1 ? 1 : dis;
				datanew.back = true;
				datanew.ahead = false;
				if (fabs(dis) > 0.2 && fabs(dis) < 0.7)
					datanew.aheadBack = 160 * fabs(dis) + 122;
				else
					datanew.aheadBack = 234;
			}
			else
				datanew.aheadBack = 128;
		}
	}

	//right and left (priority 2)
	if (boxcenter.x < 280 || boxcenter.x > 440)
	{
		if (boxcenter.x < 280){
			datanew.right = true;
			datanew.left = false;
			datanew.leftRight = 0.31 * fabs(boxcenter.x - 280) + 169;
		}
		if (boxcenter.x > 440){
			datanew.right = false;
			datanew.left = true;
			datanew.leftRight = -0.31 * fabs(boxcenter.x - 440) + 87;
		}
		return;
	}

	if (!GPS)
	{
		//up and down for indoor
		if (boxcenter.y < 124 || boxcenter.y > 243)
		{
			if (boxcenter.y < 150){
				datanew.down = false;
				datanew.up = true;
				datanew.upDown = 0.53 * fabs(boxcenter.y - 150) + 154;
			}
			if (boxcenter.y > 253){
				datanew.down = true;
				datanew.up = false;
				datanew.upDown = -0.53 * fabs(boxcenter.y - 253) + 102;
			}
			//datanew.aheadBack = 128;
			return;
		}

		//ahead (priority 4) for indoor
		if (dataold.BBox.x != -1 && dataold.BBox.y != -1){
			dis = (datanew.distance - dataold.distance) / dataold.distance;
			std::cout << "dis: " << dis << std::endl;
			if (dis < 0 && fabs(dis) > 0.1){
				dis = fabs(dis) > 1 ? -1 : dis;
				datanew.back = false;
				datanew.ahead = true;
				if (fabs(dis) > 0.2 && fabs(dis) < 0.6)
					datanew.aheadBack = -175 * fabs(dis) + 137;
				else
					datanew.aheadBack = 32;
			}
			else{
				datanew.back = false;
				datanew.ahead = false;
				datanew.upDown = false;
				datanew.upDown = 128;
				datanew.leftRight = 128;
			}
			return;
		}
	}
}

//human detect
void GetdetectWindow(cv::Mat &src, cv::Rect &tracking_window, cv::Rect& final_window, float xscale, float yscale)
{
	Point2i center;
	center.x = tracking_window.x + float(tracking_window.width) / 2;
	center.y = tracking_window.y + float(tracking_window.height) / 2;

	final_window.width = tracking_window.width * xscale;
	final_window.height = tracking_window.height * yscale;
	//if (tracking_window.height > tracking_window.width * 2)
	//	final_window.width = tracking_window.width * xscale * 2;
	//else
	//	final_window.width = tracking_window.width * xscale;

	final_window.x = (center.x - final_window.width / 2 - 1) < 0 ? 0 : (center.x - final_window.width / 2 - 1);
	final_window.y = (center.y - final_window.height / 2 - 1) < 0 ? 0 : (center.y - final_window.height / 2 - 1);

	final_window.width = final_window.x + final_window.width > (src.cols - 1) ? (src.cols - final_window.x - 1) : final_window.width;
	final_window.height = final_window.y + final_window.height > (src.rows - 1) ? (src.rows - final_window.y - 1) : final_window.height;
}

void boxMerge(cv::Rect& KCFresult, cv::Rect& SSDresult, cv::Rect& finalresult)
{
	if (SSDresult.width == 0 && SSDresult.height == 0)
	{
		finalresult = KCFresult;
	}
	else
	{
		float minSize = KCFresult.area() > SSDresult.area() ? SSDresult.area() : KCFresult.area();
		if (fabs((SSDresult.area() - KCFresult.area()) / minSize) >= 0.6)
			finalresult = KCFresult;
		else
		{
			float ratio = KCFresult.width / KCFresult.height;
			Point KCFcenter, SSDcenter, Mergecenter;
			KCFcenter.x = KCFresult.x + KCFresult.width / 2;
			KCFcenter.y = KCFresult.y + KCFresult.height / 2;
			SSDcenter.x = SSDresult.x + SSDresult.width / 2;
			SSDcenter.y = SSDresult.y + SSDresult.height / 2;

			Mergecenter.x = (KCFcenter.x + SSDcenter.x) / 2;
			Mergecenter.y = (KCFcenter.y + SSDcenter.y) / 2;

			finalresult.width = SSDresult.width;
			finalresult.height = finalresult.width / ratio;
			finalresult.x = Mergecenter.x - finalresult.width / 2;
			finalresult.y = Mergecenter.y - finalresult.height / 2;
		}
	}
}