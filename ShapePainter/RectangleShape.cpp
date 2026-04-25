// RectangleShape.cpp
#include "RectangleShape.h"

RectangleShape::RectangleShape(Scalar col, int thick, Point tl, Point br) : Shape(col, thick)
{
    this->topLeft = tl;
    this->bottomRight = br;
}

RectangleShape::~RectangleShape()
{
    cout << "Rectangle destructor" << endl;
}

void RectangleShape::draw(Mat &image) const
{
    rectangle(image, this->topLeft, this->bottomRight, this->color, this->thickness);
}

void RectangleShape::printInfo(Mat &image, int x, int y) const
{
    string info = "Rectangle: [" + to_string(this->topLeft.x) + "," + to_string(this->topLeft.y) +
                  "] - [" + to_string(this->bottomRight.x) + "," + to_string(this->bottomRight.y) + "]";
    putText(image, info, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, this->color, 1);
}