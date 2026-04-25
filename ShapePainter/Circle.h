// Circle.h
#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"

class Circle : public Shape
{
private:
    Point center;
    int radius;

public:
    Circle(Scalar col, int thick, Point c, int r);
    ~Circle();

    void draw(Mat &image) const override;
    void printInfo(Mat &image, int x, int y) const override;
};

#endif