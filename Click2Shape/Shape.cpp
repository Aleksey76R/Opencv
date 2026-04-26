#include "Shape.h"

Shape::Shape(cv::Point center, cv::Scalar color)
    : center(center), color(color) {}

cv::Point Shape::getCenter() const
{
    return center;
}