// Line.h
#ifndef LINE_H
#define LINE_H

#include "Shape.h"

class Line : public Shape
{
private:
    Point start;
    Point end;

public:
    Line(Scalar col, int thick, Point s, Point e);
    ~Line();

    void draw(Mat &image) const override;
    void printInfo(Mat &image, int x, int y) const override;
};

#endif