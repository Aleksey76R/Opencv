#ifndef APPSTATE_H
#define APPSTATE_H

#include <vector>
#include <opencv2/opencv.hpp>
#include "Shape.h"

struct AppState
{
    std::vector<Shape *> shapes;
    bool drawCircleMode;
    cv::Mat canvas;
    int width;
    int height;

    AppState();

    ~AppState();

    AppState(const AppState &) = delete;
    AppState &operator=(const AppState &) = delete;

    void addShape(Shape *shape);
    bool removeShapeAt(int x, int y);
    void clearShapes();
    void redraw();
    void setMode(bool isCircleMode);

    static void onMouse(int event, int x, int y, int flags, void *userdata);
};

#endif