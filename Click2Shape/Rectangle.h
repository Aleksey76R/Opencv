#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"

class Rectangle : public Shape
{
private:
    int half_size;

public:
    Rectangle(cv::Point center, cv::Scalar color);

    void draw(cv::Mat &image) const override;
    bool containsPoint(int x, int y) const override;
};

#endif