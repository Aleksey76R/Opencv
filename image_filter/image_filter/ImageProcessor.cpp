#include "ImageProcessor.h"

ImageProcessor::ImageProcessor()
{
	Mat canvas = Mat::zeros(100, 100, CV_8UC3);
}

ImageProcessor::ImageProcessor(string imageWay)
{
	Mat image = imread(imageWay);
	if (image.empty())
		return;
	else {
		imshow("windows", image);
		waitKey(0);
	}
}

ImageProcessor::ImageProcessor(Mat image, string name)
{
	if (image.empty())
		return;
	else {
		this->image = image.clone();
		this->name = name;
	}
}

void ImageProcessor::invert()
{
	if (this->image.empty())
		return;
	bitwise_not(this->image, this->image);
}

void ImageProcessor::extractChannel(int channel)
{
	if (0 > channel || channel > 2 || image.empty())
		return;
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			Vec3b& pixel = image.at<Vec3b>(i, j);
			for (int c = 0; c < 3; c++) {
				if (c != channel)
					pixel[c] = 0;
			}
		}
	}
}

bool ImageProcessor::save(const string& path)
{
	if (this->image.empty())
		return false;
	imwrite(path, this->image);
	return true;
}

void ImageProcessor::showInfo()
{
	if (this->image.empty())
		return;
	cout << this->image.rows;
	cout << this->image.cols;
	cout << this->image.type();
}


