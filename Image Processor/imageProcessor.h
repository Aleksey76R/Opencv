#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <algorithm>

class ImageUtils
{
private:
    static int processedCount; // статическое поле
    ImageUtils() = delete;     // запрет создания объектов

public:
    // статический метод для получения счётчика
    static int getProcessedCount();

    // встраиваемый метод: resize с сохранением пропорций
    static inline cv::Mat resizeKeepAspect(const cv::Mat &src, int maxWidth, int maxHeight)
    {
        if (src.empty())
            return cv::Mat();

        double ratio = std::min(static_cast<double>(maxWidth) / src.cols,
                                static_cast<double>(maxHeight) / src.rows);
        int newWidth = static_cast<int>(src.cols * ratio);
        int newHeight = static_cast<int>(src.rows * ratio);

        cv::Mat dst;
        cv::resize(src, dst, cv::Size(newWidth, newHeight));

        processedCount++; // увеличиваем счётчик при обработке
        return dst;
    }

    // встраиваемый метод: обрезка по ROI
    static inline cv::Mat cropROI(const cv::Mat &src, const cv::Rect &roi)
    {
        if (src.empty() || roi.x < 0 || roi.y < 0 ||
            roi.x + roi.width > src.cols || roi.y + roi.height > src.rows)
        {
            return cv::Mat(); // невалидный ROI
        }

        cv::Mat dst = src(roi).clone(); // создаём копию области

        processedCount++;
        return dst;
    }
};

#endif