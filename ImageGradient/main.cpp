#include "imageGradient.h"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    Mat img = imread("input.png", IMREAD_GRAYSCALE);

    if (img.empty())
    {
        cout << "Создаю синтетическое изображение..." << endl;
        img = Mat::zeros(512, 512, CV_8UC1);
        rectangle(img, Point(100, 100), Point(412, 412), Scalar(255), -1);
        circle(img, Point(256, 256), 150, Scalar(128), -1);
    }

    ImageGradient grad("sobel", 3);

    grad.setMethod("sobel");
    cout << "Метод: " << grad.getMethod() << ", ядро: " << grad.getKernelSize() << endl;

    Mat gradMag = grad(img);
    Mat gradThresh = grad(img, 50);

    Mat edges = detectEdgesCanny(gradMag, 50.0, 150.0);

    imshow("Исходное изображение(1)", img);
    imshow("Градиент (Sobel)(2)", gradMag);
    imshow("Градиент с порогом 50(3)", gradThresh);
    imshow("Детектор Канни(4)", edges);

    imwrite("gradient_result.jpg", gradMag);
    imwrite("canny_result.jpg", edges);

    cout << "Нажмите любую клавишу для выхода..." << endl;
    waitKey(0);

    cout << "\nДемонстрация const-перегрузки:" << endl;
    const ImageGradient constGrad("scharr", 3);
    Mat result = constGrad(img, 100);
    cout << "const-объект вызвал operator() с порогом" << endl;

    return 0;
}