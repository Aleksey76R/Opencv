#include "ImageProcessor.h"

// Конструктор по умолчанию
ImageProcessor::ImageProcessor() {
    isLoaded = false;
    imagePath = "";
}

// Конструктор с параметром
ImageProcessor::ImageProcessor(const string& path) {
    imagePath = path;
    originalImage = imread(path);
    isLoaded = !originalImage.empty();
}

// Деструктор
ImageProcessor::~ImageProcessor() {
}

// Загрузка изображения
bool ImageProcessor::loadImage(const string& path) {
    imagePath = path;
    originalImage = imread(path);
    isLoaded = !originalImage.empty();
    return isLoaded;
}

// Отображение изображения
void ImageProcessor::displayImage() {
    if (isLoaded) {
        imshow("Project C++ OpenCV", originalImage);
        waitKey(0);
    }
    else {
        cout << "Ошибка: изображение не загружено!" << endl;
    }
}

// Вывод информации
void ImageProcessor::showInfo() {
    if (isLoaded) {
        cout << "Информация об изображении:" << endl;
        cout << "Путь: " << imagePath << endl;
        cout << "Ширина: " << originalImage.cols << " px" << endl;
        cout << "Высота: " << originalImage.rows << " px" << endl;
        cout << "Каналов: " << originalImage.channels() << endl;
    }
    else {
        cout << "Нет загруженного изображения!" << endl;
    }
}