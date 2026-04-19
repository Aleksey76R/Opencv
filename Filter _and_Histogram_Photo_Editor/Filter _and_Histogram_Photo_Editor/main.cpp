#include "ImageProcesser.h"


int main()
{
    ImageProcesser processor("C:/Users/user/Pictures/фото/IMG_20230917_221255.jpg");

    cout << "\n=== Информация об изображении ===" << endl;
    cout << "Ширина: " << processor.get_weight() << " px" << endl;
    cout << "Высота: " << processor.get_height() << " px" << endl;
    cout << "Цветовое пространство: " << processor.get_colorsSpaceInfo() << endl;
    cout << "Средняя яркость: " << processor.get_AverageBrightness() << endl;

    Mat grayImage = processor.get_GrayscaleVersion();
    imshow("Оттенки серого", grayImage);
    waitKey(0);
    destroyWindow("Оттенки серого");

    processor.applySaturationBoost();

    Mat result = processor.get_GrayscaleVersion();
    imshow("После увеличения насыщенности", grayImage);

    processor.saveResult("_saturated");

    cout << "\n=== Программа завершена ===" << endl;

    return 0;
}