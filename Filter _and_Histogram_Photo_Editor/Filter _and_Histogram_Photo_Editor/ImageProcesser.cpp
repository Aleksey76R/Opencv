#include "ImageProcesser.h"

ImageProcesser::ImageProcesser(string imagePath)
{
	this->m_originalImage = imread(imagePath);
	if (this->m_originalImage.empty())
		return;
	imshow("windows", this->m_originalImage);
	waitKey(0);
}

ImageProcesser::~ImageProcesser()
{
	cout << "ImageProcessor destroyed. Released resources for: " << this->m_imagePath;
}

int ImageProcesser::get_weight() const
{
	return this->m_originalImage.cols;
}

int ImageProcesser::get_height() const
{
	return this->m_originalImage.rows;
}

string ImageProcesser::get_colorsSpaceInfo() const
{
	if (m_originalImage.empty())
		return "";
	if (m_originalImage.channels() == 1)
		return "Grayscale (1 канал)";
	if (m_originalImage.channels() == 3)
		return "BGR";
	else
		return "other";
}

double ImageProcesser::get_AverageBrightness() const
{
	if (m_originalImage.empty())
		return 0.0;
	Mat gray;
	cvtColor(m_originalImage, gray, COLOR_BGR2GRAY);
	Scalar mean1 = mean(gray);
	return mean1[0];

}

void ImageProcesser::applySaturationBoost()
{
	if (m_originalImage.empty())
		return;

	Mat hsvimage;
	cvtColor(m_originalImage, hsvimage, COLOR_BGR2HSV);

	vector<Mat> channels;
	split(hsvimage, channels);

	//увеличиваем насыщеность
	Mat saturationBoosted;
	add(channels[1], 50, saturationBoosted);
	channels[1] = saturationBoosted;

	Mat resultBgr;
	cvtColor(hsvimage, resultBgr, COLOR_HSV2BGR);

	m_originalImage = resultBgr;
}

void ImageProcesser::saveResult(const string& suffix)
{
	if (m_originalImage.empty()) {
		cout << "Нет изображения для сохранения" << endl;
		return;
	}

	// Формируем имя файла
	size_t lastDot = m_imagePath.find_last_of('.');
	string baseName = (lastDot != string::npos) ? m_imagePath.substr(0, lastDot) : m_imagePath;
	string outputPath = baseName + suffix + ".png";

	// Сохраняем
	imwrite(outputPath, m_originalImage);
	cout << "Результат сохранён: " << outputPath << endl;
}

Mat ImageProcesser::get_GrayscaleVersion() const
{
	if (m_originalImage.empty()) {
		return Mat();
	}

	Mat gray;
	cvtColor(m_originalImage, gray, COLOR_BGR2GRAY);
	return gray;
}