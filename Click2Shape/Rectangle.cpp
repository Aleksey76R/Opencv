#include "Rectangle.h"

Rectangle::Rectangle(cv::Point center, cv::Scalar color)
    : Shape(center, color), half_size(40) {}

void Rectangle::draw(cv::Mat &image) const
{
    cv::Rect rect(center.x - half_size, center.y - half_size,
                  2 * half_size, 2 * half_size);
    cv::rectangle(image, rect, color, cv::FILLED);
}

bool Rectangle::containsPoint(int x, int y) const
{
    return (x >= center.x - half_size && x <= center.x + half_size &&
            y >= center.y - half_size && y <= center.y + half_size);
}