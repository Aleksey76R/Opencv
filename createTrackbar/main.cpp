#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class ThresholdAlgorithm{
public:
    virtual void apply(const Mat &src, Mat &dst, int thresholdValue) = 0;
    virtual ~ThresholdAlgorithm() {};
};

class BinaryThreshold : public ThresholdAlgorithm
{
    void apply(const Mat &src, Mat &dst, int thresholdVAlue) override{
        threshold(src, dst, thresholdVAlue, 255, THRESH_BINARY);
    }
};

class InverseBinaryThreshold : public ThresholdAlgorithm{
    void apply(const Mat &src, Mat &dst, int thresholdVAlue) override {
        threshold(src, dst, thresholdVAlue, 255, THRESH_BINARY_INV);
    }
};

Mat g_srcImage;
Mat g_resultImage;
ThresholdAlgorithm *g_algorithm = nullptr;

void onThresholdChange(int thresholdValue, void *userdata)
{
    if (g_srcImage.empty() || !g_algorithm)
    {
        return;
    }

    g_algorithm->apply(g_srcImage, g_resultImage, thresholdValue);

    imshow("Thresholded", g_resultImage);

    static int lastValue = -1;
    if (lastValue != thresholdValue)
    {
        cout << "Threshold value changed to: " << thresholdValue << endl;
        lastValue = thresholdValue;
    }
}

int main(int argc, char **argv)
{
    string imagePath = "test.jpg";
    if (argc > 1)
    {
        imagePath = argv[1];
    }

    g_srcImage = imread(imagePath, IMREAD_GRAYSCALE);
    if (g_srcImage.empty())
    {
        cerr << "Ошибка: не удалось загрузить изображение: " << imagePath << endl;
        cerr << "Пожалуйста, укажите корректный путь к изображению (например, .\\image.jpg)" << endl;
        return -1;
    }

    BinaryThreshold binaryAlg;
    g_algorithm = &binaryAlg;

    namedWindow("Original", WINDOW_AUTOSIZE);
    namedWindow("Thresholded", WINDOW_AUTOSIZE);

    int initialThreshold = 128;
    createTrackbar("Threshold value", "Thresholded", &initialThreshold, 255, onThresholdChange);

    onThresholdChange(initialThreshold, nullptr);

    imshow("Original", g_srcImage);

    cout << "=== Программа запущена ===" << endl;
    cout << "Используемый алгоритм: BinaryThreshold" << endl;
    cout << "Двигайте слайдер в окне 'Thresholded'" << endl;
    cout << "Нажмите ESC для выхода" << endl;

    while (true)
    {
        int key = waitKey(30) & 0xFF;
        if (key == 27)
        {
            break;
        }
    }

    destroyAllWindows();
    return 0;
}