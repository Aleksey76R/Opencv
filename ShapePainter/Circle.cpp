// Circle.cpp
#include "Circle.h"

Circle::Circle(Scalar col, int thick, Point c, int r) : Shape(col, thick)
{
    this->center = c;
    this->radius = r;
}

Circle::~Circle()
{
    cout << "Circle destructor" << endl;
}

void Circle::draw(Mat &image) const
{
    circle(image, this->center, this->radius, this->color, this->thickness);
}

void Circle::printInfo(Mat &image, int x, int y) const
{
    string info = "Circle: center(" + to_string(this->center.x) + "," + to_string(this->center.y) +
                  ") radius=" + to_string(this->radius);
    putText(image, info, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, this->color, 1);
}