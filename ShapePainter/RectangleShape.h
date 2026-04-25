// RectangleShape.h
#ifndef RECTANGLESHAPE_H
#define RECTANGLESHAPE_H

#include "Shape.h"

class RectangleShape : public Shape
{
private:
    Point topLeft;
    Point bottomRight;

public:
    RectangleShape(Scalar col, int thick, Point tl, Point br);
    ~RectangleShape();

    void draw(Mat &image) const override;
    void printInfo(Mat &image, int x, int y) const override;
};

#endif