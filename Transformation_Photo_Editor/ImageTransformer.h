#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class ImageTransformer{
private:
    Mat image;
public:
    ImageTransformer(const Mat &img);

    Mat get_image() const;
    void set_image(const Mat& image);
    bool isEmpty() const;

    void flipHorizontal();
    void flipVertical();
    void transpone();
    void rotate90Clockwise();
    void rotate90CounterClockwise();
};

#endif