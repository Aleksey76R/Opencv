#include "imageGradient.h"
#include <cmath>

ImageGradient::ImageGradient(const string &method, int kernelSize)
    : method_(method), kernelSize_(kernelSize) {}

void ImageGradient::setMethod(const string &method)
{
    method_ = method;
}

void ImageGradient::setKernelSize(int size)
{
    kernelSize_ = size;
}

string ImageGradient::getMethod() const
{
    return method_;
}

int ImageGradient::getKernelSize() const
{
    return kernelSize_;
}

Mat ImageGradient::normalizeToU8(const Mat &src) const
{
    Mat normalized;
    double minVal, maxVal;
    minMaxLoc(src, &minVal, &maxVal);

    if (maxVal - minVal > 0)
    {
        src.convertTo(normalized, CV_8UC1, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));
    }
    else
    {
        normalized = Mat::zeros(src.size(), CV_8UC1);
    }

    return normalized;
}

Mat ImageGradient::computeGradientInternal(const Mat &src) const
{
    Mat srcFloat;

    if (src.type() == CV_8UC1)
    {
        src.convertTo(srcFloat, CV_32F);
    }
    else
    {
        srcFloat = src;
    }

    Mat gradX, gradY;

    if (method_ == "sobel")
    {
        Sobel(srcFloat, gradX, CV_32F, 1, 0, kernelSize_);
        Sobel(srcFloat, gradY, CV_32F, 0, 1, kernelSize_);
    }
    else
    {
        Scharr(srcFloat, gradX, CV_32F, 1, 0);
        Scharr(srcFloat, gradY, CV_32F, 0, 1);
    }

    Mat magnitude;
    magnitude.create(src.size(), CV_32F);

    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            float gx = gradX.at<float>(i, j);
            float gy = gradY.at<float>(i, j);
            magnitude.at<float>(i, j) = sqrt(gx * gx + gy * gy);
        }
    }

    return magnitude;
}

Mat ImageGradient::operator()(const Mat &src)
{
    Mat gradient = computeGradientInternal(src);
    return normalizeToU8(gradient);
}

Mat ImageGradient::operator()(const Mat &src, int threshold) const
{
    Mat gradient = computeGradientInternal(src);

    Mat thresholded;
    thresholded.create(gradient.size(), CV_8UC1);

    for (int i = 0; i < gradient.rows; i++)
    {
        for (int j = 0; j < gradient.cols; j++)
        {
            float val = gradient.at<float>(i, j);
            thresholded.at<uchar>(i, j) = (val >= static_cast<float>(threshold)) ? 255 : 0;
        }
    }

    return thresholded;
}

Mat detectEdgesCanny(const Mat &grad, double low, double high)
{
    Mat gradU8;

    if (grad.type() == CV_32F)
    {
        double minVal, maxVal;
        minMaxLoc(grad, &minVal, &maxVal);
        grad.convertTo(gradU8, CV_8UC1, 255.0 / (maxVal - minVal));
    }
    else
    {
        gradU8 = grad;
    }

    int lowInt = static_cast<int>(low);
    int highInt = static_cast<int>(high);

    lowInt = max(0, min(255, lowInt));
    highInt = max(0, min(255, highInt));

    Mat edges;
    Canny(gradU8, edges, lowInt, highInt);

    return edges;
}