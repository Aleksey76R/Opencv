#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace cv;

class HistImage
{
private:
    Mat image;
    string name;

public:
    HistImage();
    HistImage(const Mat &image, const string &name);
    HistImage(const Mat &image);
    HistImage(const HistImage &other);
    ~HistImage() = default;

    Mat getImage() const;
    string getName() const;
    void setImage(const Mat &img);
    void setName(const string &name);

    HistImage &operator=(const HistImage &other);
    HistImage operator+(const HistImage &other) const;

    friend ostream &operator<<(ostream &os, const HistImage &img);
    friend istream &operator>>(istream &is, HistImage &img);

    void applyEqualizeHist();
    Mat getNormalizedHistogram() const;
    void show(const string &windowName) const;
    double getMeanBrightness() const;
};