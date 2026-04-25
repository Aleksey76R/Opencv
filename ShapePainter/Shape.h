// Shape.h
#ifndef SHAPE_H
#define SHAPE_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

class Shape
{
protected:
    Scalar color;
    int thickness;

public:
    Shape(Scalar col, int thick);
    virtual ~Shape();

    virtual void draw(Mat &image) const = 0;
    virtual void printInfo(Mat &image, int x, int y) const;
};

#endif