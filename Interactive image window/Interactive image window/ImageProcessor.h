#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class ImageProcessor {
private:
    Mat originalImage;
    string imagePath;
    bool isLoaded;

public:
    // Конструкторы
    ImageProcessor();
    ImageProcessor(const string& path);

    // Деструктор
    ~ImageProcessor();

    // Методы
    bool loadImage(const string& path);
    void displayImage();
    void showInfo();
};

#endif