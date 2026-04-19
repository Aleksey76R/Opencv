#pragma once
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

class ImageProcesser
{
private:
	Mat m_originalImage;
	string m_imagePath;
	bool m_isLoaded;
public:
	ImageProcesser(string m_imagePath);
	~ImageProcesser();

	int get_weight() const;
	int get_height() const;
	string get_colorsSpaceInfo() const;
	double get_AverageBrightness() const;
	void applySaturationBoost();
	void saveResult(const string& suffix);
	Mat get_GrayscaleVersion() const;
};

