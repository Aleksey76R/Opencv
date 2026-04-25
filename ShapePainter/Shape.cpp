// Shape.cpp
#include "Shape.h"

Shape::Shape(Scalar col, int thick)
{
    this->color = col;
    this->thickness = thick;
}

Shape::~Shape()
{
    cout << "Shape destructor" << endl;
}

void Shape::printInfo(Mat &image, int x, int y) const
{
    putText(image, "Shape", Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, this->color, 1);
}