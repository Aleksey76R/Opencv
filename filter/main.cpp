#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vector>
#include <stdexcept>

using namespace std;
using namespace cv;

class PixelWalker
{
private:
    const Mat *image;
    int row;
    int col;
    int totalRows;
    int totalCols;

public:
    PixelWalker(const Mat &img)
        : image(&img), row(0), col(0),
          totalRows(img.rows), totalCols(img.cols) {}

    PixelWalker(const Mat &img, int startRow, int startCol)
        : image(&img), row(startRow), col(startCol),
          totalRows(img.rows), totalCols(img.cols) {}

    PixelWalker(const PixelWalker &other)
        : image(other.image), row(other.row), col(other.col),
          totalRows(other.totalRows), totalCols(other.totalCols) {}

    PixelWalker &operator++()
    {
        col++;
        if (col >= totalCols)
        {
            col = 0;
            row++;
        }
        return *this;
    }

    PixelWalker operator++(int)
    {
        PixelWalker temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const PixelWalker &other) const
    {
        return (image == other.image && row == other.row && col == other.col);
    }

    bool operator!=(const PixelWalker &other) const
    {
        return !(*this == other);
    }

    bool operator<(const PixelWalker &other) const
    {
        if (row != other.row)
            return row < other.row;
        return col < other.col;
    }

    bool operator>(const PixelWalker &other) const
    {
        return other < *this;
    }

    uchar getPixel() const
    {
        return image->at<uchar>(row, col);
    }

    bool isEnd() const
    {
        return row >= totalRows;
    }

    void getPosition(int &r, int &c) const
    {
        r = row;
        c = col;
    }

    void reset()
    {
        row = 0;
        col = 0;
    }
};

void invertRegionByReference(Mat &image, int x, int y, int width, int height)
{
    cout << "Инвертируем область через ССЫЛКУ..." << endl;

    int startX = max(0, x);
    int startY = max(0, y);
    int endX = min(image.cols, x + width);
    int endY = min(image.rows, y + height);

    for (int i = startY; i < endY; i++)
    {
        for (int j = startX; j < endX; j++)
        {
            uchar &pixel = image.at<uchar>(i, j);
            pixel = 255 - pixel;
        }
    }

    cout << "Область инвертирована. Изменения применены к ОРИГИНАЛУ." << endl;
}

Mat invertRegionByCopy(const Mat &image, int x, int y, int width, int height)
{
    cout << "Инвертируем область через КОПИЮ..." << endl;

    Mat result = image.clone();

    int startX = max(0, x);
    int startY = max(0, y);
    int endX = min(result.cols, x + width);
    int endY = min(result.rows, y + height);

    for (int i = startY; i < endY; i++)
    {
        for (int j = startX; j < endX; j++)
        {
            uchar &pixel = result.at<uchar>(i, j);
            pixel = 255 - pixel;
        }
    }

    cout << "Область инвертирована. Изменения применены только к КОПИИ." << endl;
    return result;
}

void applyBlur(const Mat &src, Mat &dst, int blurType, int kernelSize)
{
    if (kernelSize % 2 == 0)
    {
        kernelSize++;
        cout << "Размер ядра был изменён на " << kernelSize << " (должен быть нечётным)" << endl;
    }

    switch (blurType)
    {
    case 0:
        cout << "Применяем GaussianBlur с ядром " << kernelSize << "x" << kernelSize << endl;
        GaussianBlur(src, dst, Size(kernelSize, kernelSize), 0);
        break;
    case 1:
        cout << "Применяем medianBlur с ядром " << kernelSize << endl;
        medianBlur(src, dst, kernelSize);
        break;
    default:
        throw invalid_argument("Неверный тип размытия");
    }
}

void applyThreshold(const Mat &src, Mat &dst, int method, int param)
{
    switch (method)
    {
    case 0:
        cout << "Применяем обычный threshold с порогом " << param << endl;
        threshold(src, dst, param, 255, THRESH_BINARY);
        break;
    case 1:
    {
        if (param % 2 == 0)
        {
            param++;
            cout << "Размер блока был изменён на " << param << " (должен быть нечётным)" << endl;
        }
        int blockSize = max(3, param);
        cout << "Применяем adaptiveThreshold с блоком " << blockSize << endl;
        adaptiveThreshold(src, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C,
                          THRESH_BINARY, blockSize, 2);
    }
    break;
    default:
        throw invalid_argument("Неверный тип пороговой обработки");
    }
}

int countBoundaries(PixelWalker &walker)
{
    int transitions = 0;
    uchar previousPixel = walker.getPixel();

    ++walker;

    while (!walker.isEnd())
    {
        uchar currentPixel = walker.getPixel();

        if (previousPixel != currentPixel)
        {
            transitions++;
        }

        previousPixel = currentPixel;
        ++walker;
    }

    return transitions;
}

Mat createSyntheticImage(int width, int height)
{
    Mat image(height, width, CV_8UC1);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int value = (i * 255 / height) + (j * 255 / width);
            value = value / 2;
            image.at<uchar>(i, j) = static_cast<uchar>(value);
        }
    }

    return image;
}

void demonstratePixelWalker()
{
    cout << "\n========== Демонстрация PixelWalker ==========" << endl;

    Mat binary(5, 5, CV_8UC1, Scalar(0));

    for (int i = 1; i < 4; i++)
    {
        binary.at<uchar>(i, 2) = 255;
    }

    cout << "Тестовое бинарное изображение 5x5:" << endl;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            cout << (binary.at<uchar>(i, j) == 255 ? "1 " : "0 ");
        }
        cout << endl;
    }

    PixelWalker walker(binary);

    cout << "\nДемонстрация префиксного ++ (возвращает ссылку):" << endl;
    PixelWalker &prefixed = ++walker;
    int row, col;
    prefixed.getPosition(row, col);
    cout << "После префиксного ++, позиция: (" << row << ", " << col << ")" << endl;

    cout << "\nДемонстрация постфиксного ++ (возвращает копию):" << endl;
    PixelWalker copy = walker++;
    int copyRow, copyCol;
    copy.getPosition(copyRow, copyCol);
    cout << "Копия (состояние до инкремента): (" << copyRow << ", " << copyCol << ")" << endl;

    walker.getPosition(row, col);
    cout << "Оригинал (после инкремента): (" << row << ", " << col << ")" << endl;

    PixelWalker walker1(binary, 0, 0);
    PixelWalker walker2(binary, 0, 1);
    PixelWalker walker3(binary, 0, 0);

    cout << "\nДемонстрация операторов сравнения:" << endl;
    cout << "walker1 == walker3: " << (walker1 == walker3 ? "true" : "false") << endl;
    cout << "walker1 != walker2: " << (walker1 != walker2 ? "true" : "false") << endl;
    cout << "walker1 < walker2: " << (walker1 < walker2 ? "true" : "false") << endl;
    cout << "walker2 > walker1: " << (walker2 > walker1 ? "true" : "false") << endl;

    walker.reset();
    int boundaries = countBoundaries(walker);
    cout << "\nКоличество переходов (границ) в тестовом изображении: " << boundaries << endl;
}

void displayImages(const vector<pair<string, Mat>> &images, int delay = 0)
{
    for (const auto &img : images)
    {
        if (!img.second.empty())
        {
            imshow(img.first, img.second);
        }
    }
    waitKey(delay);
}

int main(int argc, char **argv)
{
    cout << "========== Фильтр контуров с настраиваемой пороговой обработкой ==========" << endl;

    Mat originalImage;

    if (argc > 1)
    {
        originalImage = imread(argv[1], IMREAD_GRAYSCALE);
        if (originalImage.empty())
        {
            cerr << "Ошибка: не удалось загрузить изображение " << argv[1] << endl;
            cout << "Создаём синтетическое изображение..." << endl;
            originalImage = createSyntheticImage(640, 480);
        }
        else
        {
            cout << "Загружено изображение: " << argv[1]
                 << ", размер " << originalImage.cols << "x" << originalImage.rows << endl;
        }
    }
    else
    {
        cout << "Файл не указан. Создаём синтетическое изображение 640x480..." << endl;
        originalImage = createSyntheticImage(640, 480);
    }

    Mat displayOriginal = originalImage.clone();

    int blurType;
    cout << "\nВыберите размытие:" << endl;
    cout << "0 - GaussianBlur" << endl;
    cout << "1 - medianBlur" << endl;
    cout << "> ";
    cin >> blurType;

    int kernelSize;
    cout << "Размер ядра (нечётное, 1-21): ";
    cin >> kernelSize;
    kernelSize = max(1, min(21, kernelSize));
    if (kernelSize % 2 == 0)
        kernelSize++;

    Mat blurredImage;
    applyBlur(originalImage, blurredImage, blurType, kernelSize);

    int thresholdMethod;
    cout << "\nВыберите пороговую обработку:" << endl;
    cout << "0 - threshold" << endl;
    cout << "1 - adaptiveThreshold" << endl;
    cout << "> ";
    cin >> thresholdMethod;

    int param;
    Mat thresholdImage;

    if (thresholdMethod == 0)
    {
        cout << "Порог (0-255): ";
        cin >> param;
        param = max(0, min(255, param));
        applyThreshold(blurredImage, thresholdImage, thresholdMethod, param);
    }
    else
    {
        cout << "Размер блока (нечётное, 3-99): ";
        cin >> param;
        param = max(3, min(99, param));
        if (param % 2 == 0)
            param++;
        applyThreshold(blurredImage, thresholdImage, thresholdMethod, param);
    }

    cout << "\n========== Демонстрация оператора [] и копирования vs ссылки ==========" << endl;
    cout << "Исходное изображение (оригинал) будет изменяться только при работе со ссылкой" << endl;

    Mat originalCopy = originalImage.clone();

    invertRegionByReference(originalImage, 100, 100, 80, 60);

    Mat invertedCopy = invertRegionByCopy(originalCopy, 200, 150, 100, 80);

    cout << "\nПоказываем результаты (нажмите любую клавишу для продолжения)..." << endl;
    displayImages({{"Исходное изображение", originalCopy},
                   {"После размытия", blurredImage},
                   {"После пороговой обработки", thresholdImage},
                   {"После инверсии по ссылке (оригинал изменён)", originalImage},
                   {"Инверсия через копию", invertedCopy}},
                  0);

    demonstratePixelWalker();

    if (!thresholdImage.empty())
    {
        cout << "\n========== Подсчёт границ на обработанном изображении ==========" << endl;
        PixelWalker boundaryWalker(thresholdImage);
        int totalBoundaries = countBoundaries(boundaryWalker);
        cout << "Количество переходов (границ) в обработанном изображении: " << totalBoundaries << endl;
    }

    cout << "\nПрограмма завершена. Нажмите любую клавишу для выхода..." << endl;
    waitKey(0);
    destroyAllWindows();

    return 0;
}