#pragma once
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class ImageProcessor {
private:
	Mat image;
	string name;
public:
	ImageProcessor();
	ImageProcessor(string imageWay);
	ImageProcessor(Mat image, string name);

	void invert();
	void extractChannel(int channel);
	bool save(const string& path);
	void showInfo();
};