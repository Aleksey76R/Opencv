#ifndef IMAGE_GRADIENT_H
#define IMAGE_GRADIENT_H

#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

class ImageGradient
{
private:
    string method_;
    int kernelSize_;

    Mat computeGradientInternal(const Mat &src) const;
    Mat normalizeToU8(const Mat &src) const;

public:
    explicit ImageGradient(const string &method = "sobel", int kernelSize = 3);

    void setMethod(const string &method);
    void setKernelSize(int size);
    string getMethod() const;
    int getKernelSize() const;

    Mat operator()(const Mat &src);
    Mat operator()(const Mat &src, int threshold) const;
};

Mat detectEdgesCanny(const Mat &grad, double low, double high);

#endif