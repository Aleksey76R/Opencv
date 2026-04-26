#ifndef SHAPE_H
#define SHAPE_H

#include <opencv2/opencv.hpp>

class Shape
{
protected:
    cv::Point center;
    cv::Scalar color;

public:
    Shape(cv::Point center, cv::Scalar color);

    virtual ~Shape() = default;

    virtual void draw(cv::Mat &image) const = 0;
    virtual bool containsPoint(int x, int y) const = 0;

    cv::Point getCenter() const;
};

#endif