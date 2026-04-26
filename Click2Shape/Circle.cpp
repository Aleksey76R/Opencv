#include "Circle.h"

Circle::Circle(cv::Point center, cv::Scalar color)
    : Shape(center, color), radius(50) {}

void Circle::draw(cv::Mat &image) const
{
    cv::circle(image, center, radius, color, cv::FILLED);
}

bool Circle::containsPoint(int x, int y) const
{
    int dx = x - center.x;
    int dy = y - center.y;
    return (dx * dx + dy * dy) <= (radius * radius);
}