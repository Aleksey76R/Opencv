#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"

class Circle : public Shape
{
private:
    int radius;

public:
    Circle(cv::Point center, cv::Scalar color);

    void draw(cv::Mat &image) const override;
    bool containsPoint(int x, int y) const override;
};

#endif