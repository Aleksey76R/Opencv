#include "HistImage.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    try
    {
        cout << "=== Histogram and Operators Demo ===" << endl;

        cout << "Creating test image..." << endl;

        Mat testImage(400, 400, CV_8U);
        for (int i = 0; i < testImage.rows; i++)
        {
            for (int j = 0; j < testImage.cols; j++)
            {
                testImage.at<uchar>(i, j) = (i + j) * 255 / (testImage.rows + testImage.cols);
            }
        }

        imwrite("test_gradient.jpg", testImage);
        cout << "Saved test_gradient.jpg" << endl;

        HistImage img1(testImage, "Gradient");

        cout << "\n=== Testing operator << ===" << endl;
        cout << img1 << endl;

        cout << "\n=== Testing operator = ===" << endl;
        HistImage img2;
        img2 = img1;
        cout << img2 << endl;

        Mat black = Mat::zeros(100, 100, CV_8U);
        Mat white = Mat::ones(100, 100, CV_8U) * 150;

        HistImage a(black, "Black");
        HistImage b(white, "White");
        HistImage sum = a + b;

        cout << "\n=== Testing operator + ===" << endl;
        cout << sum << endl;

        cout << "\n=== Testing equalizeHist ===" << endl;
        cout << "Before equalization mean: " << img1.getMeanBrightness() << endl;
        img1.applyEqualizeHist();
        cout << "After equalization mean: " << img1.getMeanBrightness() << endl;

        cout << "\n=== Displaying images ===" << endl;

        namedWindow("Original Gradient", WINDOW_NORMAL);
        imshow("Original Gradient", testImage);

        namedWindow("After Equalization", WINDOW_NORMAL);
        img1.show("After Equalization");

        namedWindow("Black + White", WINDOW_NORMAL);
        sum.show("Black + White");

        cout << "Press any key to exit..." << endl;
        waitKey(0);

        destroyAllWindows();
    }
    catch (const exception &e)
    {
        cerr << "\n!!! ERROR: " << e.what() << endl;
        return 1;
    }

    cout << "\nProgram completed successfully!" << endl;
    return 0;
}