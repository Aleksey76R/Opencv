// Line.cpp
#include "Line.h"

Line::Line(Scalar col, int thick, Point s, Point e) : Shape(col, thick)
{
    this->start = s;
    this->end = e;
}

Line::~Line()
{
    cout << "Line destructor" << endl;
}

void Line::draw(Mat &image) const
{
    line(image, this->start, this->end, this->color, this->thickness);
}

void Line::printInfo(Mat &image, int x, int y) const
{
    string info = "Line: (" + to_string(this->start.x) + "," + to_string(this->start.y) +
                  ") -> (" + to_string(this->end.x) + "," + to_string(this->end.y) + ")";
    putText(image, info, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, this->color, 1);
}