#include "ImageProcessor.h"
#include <opencv2/opencv.hpp>

using namespace std;

int main() {
	ImageProcessor image1;
	ImageProcessor image2("C:/Users/user/Pictures/фото/IMG_20230701_154120.jpg");

	image2.invert();
	image2.extractChannel(2);

	Mat image3 = Mat::zeros(400, 400, CV_8UC3);
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < image3.cols; j++) {
			image3.at<Vec3b>(i, j) = Vec3b(0, 250, 0);
		}
	}
	return 0;
}