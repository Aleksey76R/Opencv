#include "imageProcessor.h"
#include <iostream>
#include <windows.h>

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    cv::Mat image = cv::imread("DSC00107.JPG");
    if (image.empty())
    {
        std::cerr << "Ошибка загрузки изображения!" << std::endl;
        return -1;
    }

    std::cout << "Исходное изображение: " << image.cols << "x" << image.rows << std::endl;

    cv::Mat resized = ImageUtils::resizeKeepAspect(image, 400, 300);
    cv::imwrite("resized.jpg", resized);
    std::cout << "Изменён размер. Обработано изображений: "
              << ImageUtils::getProcessedCount() << std::endl;

    cv::Rect roi(100, 50, 200, 150);
    cv::Mat cropped = ImageUtils::cropROI(image, roi);
    cv::imwrite("cropped.jpg", cropped);
    std::cout << "Выполнена обрезка. Обработано изображений: " << ImageUtils::getProcessedCount() << std::endl;

    if (!resized.empty())
    {
        cv::Mat another = ImageUtils::cropROI(resized, cv::Rect(10, 10, 100, 100));
        cv::imwrite("another_crop.jpg", another);
        std::cout << "Ещё одна обрезка. Всего обработано: "
                  << ImageUtils::getProcessedCount() << std::endl;
    }

    return 0;
}