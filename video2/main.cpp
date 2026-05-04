#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include <locale.h>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

class FrameProcessor {
public:
    virtual ~FrameProcessor() {
        std::cout << "[~FrameProcessor] Виртуальный деструктор базового класса" << std::endl;
    }
    
    virtual cv::Mat process(const cv::Mat& frame) = 0;
    
    virtual void showInfo() const {
        std::cout << "Тип объекта (typeid): " << typeid(*this).name() << std::endl;
    }
    
    virtual void specificMethod() const {
        std::cout << "Базовый метод (не переопределен)" << std::endl;
    }
};

class GrayProcessor : public FrameProcessor {
public:
    ~GrayProcessor() override {
        std::cout << "[~GrayProcessor] Деструктор потомка" << std::endl;
    }
    
    cv::Mat process(const cv::Mat& frame) override {
        cv::Mat result;
        cv::cvtColor(frame, result, cv::COLOR_BGR2GRAY);
        cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
        return result;
    }
    
    void specificMethod() const override {
        std::cout << "GrayProcessor::specificMethod() - специфичный метод для градаций серого" << std::endl;
    }
    
    void uniqueGrayMethod() const {
        std::cout << "GrayProcessor::uniqueGrayMethod() - метод, доступный только через dynamic_cast" << std::endl;
    }
};

class BlurProcessor : public FrameProcessor {
private:
    int _kernel_size;
    
public:
    BlurProcessor(int ksize = 15) : _kernel_size(ksize) {}
    
    ~BlurProcessor() override {
        std::cout << "[~BlurProcessor] Деструктор потомка" << std::endl;
    }
    
    cv::Mat process(const cv::Mat& frame) override {
        cv::Mat result;
        cv::GaussianBlur(frame, result, cv::Size(_kernel_size, _kernel_size), 0);
        return result;
    }
    
    void specificMethod() const override {
        std::cout << "BlurProcessor::specificMethod() - специфичный метод для размытия, kernel size = " 
                  << _kernel_size << std::endl;
    }
    
    void setKernelSize(int size) {
        _kernel_size = size;
        std::cout << "BlurProcessor::setKernelSize() - изменен размер ядра через dynamic_cast" << std::endl;
    }
};

class CannyProcessor : public FrameProcessor {
private:
    double _threshold1, _threshold2;
    
public:
    CannyProcessor(double thresh1 = 100, double thresh2 = 200) 
        : _threshold1(thresh1), _threshold2(thresh2) {}
    
    ~CannyProcessor() override {
        std::cout << "[~CannyProcessor] Деструктор потомка" << std::endl;
    }
    
    cv::Mat process(const cv::Mat& frame) override {
        cv::Mat gray, edges, result;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::Canny(gray, edges, _threshold1, _threshold2);
        cv::cvtColor(edges, result, cv::COLOR_GRAY2BGR);
        return result;
    }
    
    void specificMethod() const override {
        std::cout << "CannyProcessor::specificMethod() - специфичный метод для Canny, пороги: "
                  << _threshold1 << ", " << _threshold2 << std::endl;
    }
    
    void adjustThresholds(double t1, double t2) {
        _threshold1 = t1;
        _threshold2 = t2;
        std::cout << "CannyProcessor::adjustThresholds() - пороги изменены через dynamic_cast" << std::endl;
    }
};

int selectCodec() {
    std::cout << "\n=== ВЫБОР КОДЕКА ===" << std::endl;
    std::cout << "1. MJPG (Motion JPEG) - хорошая совместимость" << std::endl;
    std::cout << "2. XVID (MPEG-4) - хорошее сжатие" << std::endl;
    std::cout << "3. H264 (H.264/AVC) - лучшее сжатие" << std::endl;
    std::cout << "4. VP90 (VP90) - современный кодек" << std::endl;
    std::cout << "Выберите кодек (1-4): ";
    
    int choice;
    std::cin >> choice;
    
    switch(choice) {
        case 1: return cv::VideoWriter::fourcc('M','J','P','G');
        case 2: return cv::VideoWriter::fourcc('X','V','I','D');
        case 3: return cv::VideoWriter::fourcc('H','2','6','4');
        case 4: return cv::VideoWriter::fourcc('V','P','9','0');
        default:
            std::cout << "Неверный выбор, используем MJPG по умолчанию" << std::endl;
            return cv::VideoWriter::fourcc('M','J','P','G');
    }
}

std::unique_ptr<FrameProcessor> selectProcessor() {
    std::cout << "\n=== ВЫБОР ТИПА ОБРАБОТКИ ===" << std::endl;
    std::cout << "1. Оттенки серого (GrayProcessor)" << std::endl;
    std::cout << "2. Размытие (BlurProcessor)" << std::endl;
    std::cout << "3. Детекция границ (CannyProcessor)" << std::endl;
    std::cout << "Выберите режим (1-3): ";
    
    int choice;
    std::cin >> choice;
    
    std::unique_ptr<FrameProcessor> processor;
    
    switch(choice) {
        case 1:
            processor = std::make_unique<GrayProcessor>();
            break;
        case 2:
            processor = std::make_unique<BlurProcessor>(15);
            break;
        case 3:
            processor = std::make_unique<CannyProcessor>(100, 200);
            break;
        default:
            std::cout << "Неверный выбор, используем GrayProcessor" << std::endl;
            processor = std::make_unique<GrayProcessor>();
    }
    
    std::cout << "\n[ДЕМОНСТРАЦИЯ RTTI - typeid]" << std::endl;
    std::cout << "Создан объект типа: " << typeid(*processor).name() << std::endl;
    std::cout << "Человекочитаемое имя: ";
    if (typeid(*processor) == typeid(GrayProcessor))
        std::cout << "GrayProcessor";
    else if (typeid(*processor) == typeid(BlurProcessor))
        std::cout << "BlurProcessor";
    else if (typeid(*processor) == typeid(CannyProcessor))
        std::cout << "CannyProcessor";
    std::cout << std::endl;
    
    return processor;
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "=== ПРОГРАММА ДЛЯ ИЗУЧЕНИЯ RTTI, ВИРТУАЛЬНЫХ ДЕСТРУКТОРОВ И OpenCV VideoWriter ===" << std::endl;
    
    std::string inputPath;
    std::cout << "\nВведите путь к видеофайлу (или '0' для веб-камеры): ";
    std::cin >> inputPath;
    
    cv::VideoCapture cap;
    if (inputPath == "0") {
        cap.open(0);
    } else {
        cap.open(inputPath);
    }
    
    if (!cap.isOpened()) {
        std::cerr << "Ошибка: не удалось открыть видео!" << std::endl;
        return -1;
    }
    
    double fps = cap.get(cv::CAP_PROP_FPS);
    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    int totalFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
    
    std::cout << "\nПараметры видео:" << std::endl;
    std::cout << "  Размер: " << width << "x" << height << std::endl;
    std::cout << "  FPS: " << fps << std::endl;
    std::cout << "  Всего кадров: " << totalFrames << std::endl;
    
    int codec = selectCodec();
    std::string outputPath = "output_video.avi";
    
    cv::VideoWriter writer;
    bool writerOpened = writer.open(outputPath, codec, fps, cv::Size(width, height));
    
    if (!writerOpened) {
        std::cerr << "Ошибка: не удалось открыть VideoWriter с выбранным кодеком!" << std::endl;
        
        std::cout << "Пробуем альтернативные кодеки..." << std::endl;
        int fallbackCodecs[] = {
            cv::VideoWriter::fourcc('M','J','P','G'),
            cv::VideoWriter::fourcc('X','V','I','D'),
            cv::VideoWriter::fourcc('H','2','6','4')
        };
        
        for (int fallback : fallbackCodecs) {
            if (writer.open(outputPath, fallback, fps, cv::Size(width, height))) {
                std::cout << "Успешно! Используется кодек: " << fallback << std::endl;
                codec = fallback;
                break;
            }
        }
        
        if (!writer.isOpened()) {
            std::cerr << "Не удалось открыть VideoWriter ни с одним кодеком!" << std::endl;
            return -1;
        }
    }
    
    auto processor = selectProcessor();
    
    std::cout << "\n[ДЕМОНСТРАЦИЯ RTTI - dynamic_cast]" << std::endl;
    
    if (auto* grayProc = dynamic_cast<GrayProcessor*>(processor.get())) {
        std::cout << "dynamic_cast успешен: объект является GrayProcessor" << std::endl;
        grayProc->uniqueGrayMethod();
        grayProc->specificMethod();
    } 
    else if (auto* blurProc = dynamic_cast<BlurProcessor*>(processor.get())) {
        std::cout << "dynamic_cast успешен: объект является BlurProcessor" << std::endl;
        blurProc->setKernelSize(21);
        blurProc->specificMethod();
    }
    else if (auto* cannyProc = dynamic_cast<CannyProcessor*>(processor.get())) {
        std::cout << "dynamic_cast успешен: объект является CannyProcessor" << std::endl;
        cannyProc->adjustThresholds(50, 150);
        cannyProc->specificMethod();
    }
    else {
        std::cout << "dynamic_cast не удался (неизвестный тип)" << std::endl;
    }
    
    std::cout << "\nВызов specificMethod() через указатель на FrameProcessor: ";
    processor->specificMethod();
    
    std::cout << "\n=== НАЧАЛО ОБРАБОТКИ ВИДЕО ===" << std::endl;
    cv::Mat frame;
    int frameCount = 0;
    auto startTime = std::chrono::steady_clock::now();
    
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            std::cout << "Достигнут конец видео" << std::endl;
            break;
        }
        
        cv::Mat processedFrame = processor->process(frame);
        
        writer.write(processedFrame);
        
        cv::imshow("Обработанное видео (нажмите ESC для выхода)", processedFrame);
        
        frameCount++;
        if (frameCount % 30 == 0) {
            std::cout << "Обработано кадров: " << frameCount << std::endl;
        }
        
        char key = cv::waitKey(1);
        if (key == 27) {
            std::cout << "Прерывание пользователем" << std::endl;
            break;
        }
    }
    
    auto endTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
    
    std::cout << "\n=== РЕЗУЛЬТАТЫ ===" << std::endl;
    std::cout << "Обработано кадров: " << frameCount << std::endl;
    std::cout << "Время выполнения: " << duration.count() << " секунд" << std::endl;
    std::cout << "Видео сохранено в: " << outputPath << std::endl;
    
    std::cout << "\n=== ДЕМОНСТРАЦИЯ ВИРТУАЛЬНОГО ДЕСТРУКТОРА ===" << std::endl;
    std::cout << "При выходе из области видимости умного указателя автоматически вызовутся:" << std::endl;
    std::cout << "1. Деструктор потомка" << std::endl;
    std::cout << "2. Виртуальный деструктор базового класса" << std::endl;
    std::cout << "Благодаря виртуальному деструктору все ресурсы будут корректно освобождены\n" << std::endl;
    
    cv::destroyAllWindows();
    
    return 0;
}