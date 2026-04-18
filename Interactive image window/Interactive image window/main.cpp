#include "ImageProcessor.h"
#include <iostream>
#include <locale.h>

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    string path = "C:/Users/user/Pictures/фото/IMG_20230917_221255.jpg";

    ImageProcessor image1;

    if (image1.loadImage(path)) {
        cout << "Изображение успешно загружено!" << endl;
        image1.showInfo();
        image1.displayImage();
    }
    else {
        cout << "Не удалось загрузить изображение!" << endl;
        cout << "Проверьте путь: " << path << endl;
    }

    return 0;
}