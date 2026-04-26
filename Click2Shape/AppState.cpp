#include "AppState.h"
#include "Circle.h"
#include "Rectangle.h"
#include <iostream>

AppState::AppState()
    : drawCircleMode(true), width(800), height(600)
{
    canvas = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
}

AppState::~AppState()
{
    clearShapes();
}

void AppState::addShape(Shape *shape)
{
    shapes.push_back(shape);
    redraw();
}

bool AppState::removeShapeAt(int x, int y)
{
    for (auto it = shapes.begin(); it != shapes.end(); ++it)
    {
        if ((*it)->containsPoint(x, y))
        {
            delete *it;
            shapes.erase(it);
            redraw();
            return true;
        }
    }
    return false;
}

void AppState::clearShapes()
{
    for (auto *shape : shapes)
    {
        delete shape;
    }
    shapes.clear();
}

void AppState::redraw()
{
    canvas = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

    for (const auto &shape : shapes)
    {
        shape->draw(canvas);
    }

    std::string modeText = drawCircleMode ? "Mode: CIRCLE (C)" : "Mode: RECTANGLE (R)";
    cv::putText(canvas, modeText, cv::Point(10, 30),
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 2);

    cv::imshow("Annotation Tool", canvas);
}

void AppState::setMode(bool isCircleMode)
{
    drawCircleMode = isCircleMode;
    redraw();
}

void AppState::onMouse(int event, int x, int y, int flags, void *userdata)
{
    AppState *state = static_cast<AppState *>(userdata);

    if (event == cv::EVENT_MOUSEMOVE)
    {
        bool overShape = false;
        for (const auto &shape : state->shapes)
        {
            if (shape->containsPoint(x, y))
            {
                overShape = true;
                break;
            }
        }
        std::cout << "Mouse at (" << x << ", " << y << ") - "
                  << (overShape ? "OVER a shape" : "empty area") << std::endl;
    }
    else if (event == cv::EVENT_LBUTTONDOWN)
    {
        cv::Scalar randomColor(rand() % 255, rand() % 255, rand() % 255);

        Shape *newShape = nullptr;
        if (state->drawCircleMode)
        {
            newShape = new Circle(cv::Point(x, y), randomColor);
            std::cout << "Added Circle at (" << x << ", " << y << ")\n";
        }
        else
        {
            newShape = new Rectangle(cv::Point(x, y), randomColor);
            std::cout << "Added Rectangle at (" << x << ", " << y << ")\n";
        }

        if (newShape)
        {
            state->addShape(newShape);
        }
    }
    else if (event == cv::EVENT_RBUTTONDOWN)
    {
        if (state->removeShapeAt(x, y))
        {
            std::cout << "Deleted shape at (" << x << ", " << y << ")\n";
        }
    }
}