#include "HistImage.h"
#include <stdexcept>
#include <iostream>

using namespace std;
using namespace cv;


HistImage::HistImage() : image(Mat()), name("") {}

HistImage::HistImage(const Mat &image, const string &name)
{
    this->image = image.clone();
    this->name = name;
}

HistImage::HistImage(const Mat &image)
{
    this->image = image.clone();
    this->name = "unknown";
}

HistImage::HistImage(const HistImage &other)
{
    this->image = other.image.clone();
    this->name = other.name;
}


Mat HistImage::getImage() const
{
    return image;
}

string HistImage::getName() const
{
    return name;
}

void HistImage::setImage(const Mat &img)
{
    image = img.clone();
}

void HistImage::setName(const string &name)
{
    this->name = name;
}

HistImage &HistImage::operator=(const HistImage &other)
{
    if (this != &other)
    {
        image = other.image.clone();
        name = other.name;
    }
    return *this;
}

HistImage HistImage::operator+(const HistImage &other) const
{
    if (image.size() != other.image.size())
    {
        throw invalid_argument("Images have different sizes!");
    }

    Mat result;
    add(image, other.image, result, noArray(), CV_8U);
    return HistImage(result, name + "+" + other.name);
}

ostream &operator<<(ostream &os, const HistImage &img)
{
    if (img.image.empty())
    {
        os << "Image[" << img.name << "]: EMPTY";
    }
    else
    {
        os << "Image[" << img.name << "]: ("
           << img.image.cols << "x" << img.image.rows
           << "), mean brightness = " << img.getMeanBrightness();
    }
    return os;
}

istream &operator>>(istream &is, HistImage &img)
{
    string path, name;
    cout << "Enter image path: ";
    is >> path;
    cout << "Enter image name: ";
    is >> name;

    Mat loaded = imread(path, IMREAD_GRAYSCALE);
    if (loaded.empty())
    {
        throw runtime_error("Failed to load image: " + path);
    }

    img.setImage(loaded);
    img.setName(name);
    return is;
}

void HistImage::applyEqualizeHist()
{
    if (image.empty())
    {
        cerr << "Warning: Cannot equalize empty image!" << endl;
        return;
    }

    Mat result;
    equalizeHist(image, result);
    image = result;
}

Mat HistImage::getNormalizedHistogram() const
{
    if (image.empty())
    {
        return Mat();
    }

    const int channels[] = {0};
    const int histSize = 256;
    float range[] = {0, 256};
    const float *ranges[] = {range};

    Mat hist;
    calcHist(&image, 1, channels, Mat(), hist, 1, &histSize, ranges);

    Mat normalized;
    normalize(hist, normalized, 0, 255, NORM_MINMAX, CV_8U);

    return normalized;
}

void HistImage::show(const string &windowName) const
{
    if (image.empty())
    {
        cerr << "Warning: Cannot show empty image!" << endl;
        return;
    }
    imshow(windowName, image);
}

double HistImage::getMeanBrightness() const
{
    if (image.empty())
        return 0.0;

    Scalar meanVal = mean(image);
    return meanVal[0];
}