// main.cpp
#include "Shape.h"
#include "Line.h"
#include "RectangleShape.h"
#include "Circle.h"

int main()
{
    Mat image(600, 800, CV_8UC3, Scalar(0, 0, 0));

    Line line(Scalar(0, 255, 0), 3, Point(100, 100), Point(400, 200));
    RectangleShape rect(Scalar(0, 0, 255), 2, Point(500, 50), Point(750, 250));
    Circle circle(Scalar(255, 0, 0), 4, Point(400, 450), 80);

    vector<Shape *> shapes;
    shapes.push_back(&line);
    shapes.push_back(&rect);
    shapes.push_back(&circle);

    int textY = 30;
    for (auto *sh : shapes)
    {
        sh->draw(image);
        sh->printInfo(image, 20, textY);
        textY += 25;
    }

    imshow("ShapePainter", image);
    waitKey(0);

    return 0;
}