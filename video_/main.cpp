#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <string>

class Sensor
{
protected:
    std::string sensorId;
    bool isInitialized;

public:
    Sensor() : sensorId("UNKNOWN"), isInitialized(false)
    {
        std::cout << "[Sensor] Конструктор базового класса Sensor" << std::endl;
    }

    virtual ~Sensor()
    {
        std::cout << "[Sensor] Деструктор Sensor: " << sensorId << std::endl;
    }

    virtual void initialize() = 0;
    virtual std::string getType() const = 0;
    virtual void printInfo() const = 0;

    bool getInitializedStatus() const
    {
        return isInitialized;
    }

    void setSensorId(const std::string &id)
    {
        sensorId = id;
    }

    std::string getSensorId() const
    {
        return sensorId;
    }
};

class PhysicalCamera : virtual public Sensor
{
protected:
    int cameraFPS;

public:
    PhysicalCamera() : cameraFPS(30)
    {
        std::cout << "[PhysicalCamera] Конструктор физической камеры" << std::endl;
    }

    virtual ~PhysicalCamera()
    {
        std::cout << "[PhysicalCamera] Деструктор" << std::endl;
    }

    virtual cv::Mat getFrame() = 0;
    virtual void setFPS(int fps)
    {
        cameraFPS = fps;
        std::cout << "[PhysicalCamera] Установлен FPS: " << cameraFPS << std::endl;
    }

    virtual int getFPS() const
    {
        return cameraFPS;
    }
};

class AnalyticalProcessor : virtual public Sensor
{
protected:
    std::string processingMethod;

public:
    AnalyticalProcessor() : processingMethod("NONE")
    {
        std::cout << "[AnalyticalProcessor] Конструктор аналитического процессора" << std::endl;
    }

    virtual ~AnalyticalProcessor()
    {
        std::cout << "[AnalyticalProcessor] Деструктор" << std::endl;
    }

    virtual void analyze(const cv::Mat &frame) = 0;
    virtual void setProcessingMethod(const std::string &method)
    {
        processingMethod = method;
        std::cout << "[AnalyticalProcessor] Установлен метод обработки: " << processingMethod << std::endl;
    }

    virtual std::string getProcessingMethod() const
    {
        return processingMethod;
    }
};

class CombinedSensor : public PhysicalCamera, public AnalyticalProcessor
{
private:
    cv::VideoCapture videoCapture;
    std::string videoPath;
    bool useWebcam;
    int frameCounter;
    double totalBrightness;

    struct ProcessingStats
    {
        int totalFramesProcessed;
        double averageProcessingTimeMs;
        int emptyFrames;

        ProcessingStats() : totalFramesProcessed(0), averageProcessingTimeMs(0.0), emptyFrames(0) {}
    } stats;

public:
    CombinedSensor(const std::string &path) : videoPath(path),
                                              useWebcam(false),
                                              frameCounter(0),
                                              totalBrightness(0.0)
    {

        std::cout << "\n=== Конструктор CombinedSensor (видеофайл) ===" << std::endl;

        setSensorId("COMBINED_SENSOR_001");

        setFPS(30);
        setProcessingMethod("GRAYSCALE_BRIGHTNESS_ANALYSIS");
    }

    CombinedSensor(int cameraId) : videoPath(std::to_string(cameraId)),
                                   useWebcam(true),
                                   frameCounter(0),
                                   totalBrightness(0.0)
    {

        std::cout << "\n=== Конструктор CombinedSensor (веб-камера) ===" << std::endl;
        setSensorId("COMBINED_SENSOR_WEBCAM");
        setFPS(30);
        setProcessingMethod("EDGE_DETECTION");
    }

    ~CombinedSensor()
    {
        std::cout << "\n=== Деструктор CombinedSensor ===" << std::endl;
        if (videoCapture.isOpened())
        {
            videoCapture.release();
        }

        std::cout << "\n--- Статистика работы сенсора ---" << std::endl;
        std::cout << "Всего обработано кадров: " << stats.totalFramesProcessed << std::endl;
        std::cout << "Пустых кадров: " << stats.emptyFrames << std::endl;
        std::cout << "Среднее время обработки: " << stats.averageProcessingTimeMs << " мс" << std::endl;
        if (stats.totalFramesProcessed > 0)
        {
            std::cout << "Средняя яркость всех кадров: "
                      << (totalBrightness / stats.totalFramesProcessed) << std::endl;
        }
    }

    void initialize() override
    {
        std::cout << "\n[CombinedSensor::initialize] Инициализация сенсора..." << std::endl;

        if (!useWebcam)
        {
            videoCapture.open(videoPath);
            if (!videoCapture.isOpened())
            {
                throw std::runtime_error("Не удалось открыть видеофайл: " + videoPath);
            }
            std::cout << "[CombinedSensor] Видеофайл успешно открыт: " << videoPath << std::endl;
        }
        else
        {
            int cameraId = std::stoi(videoPath);
            videoCapture.open(cameraId);
            if (!videoCapture.isOpened())
            {
                throw std::runtime_error("Не удалось открыть веб-камеру с ID: " + std::to_string(cameraId));
            }
            std::cout << "[CombinedSensor] Веб-камера успешно открыта (ID: " << cameraId << ")" << std::endl;
        }

        double fps = videoCapture.get(cv::CAP_PROP_FPS);
        double width = videoCapture.get(cv::CAP_PROP_FRAME_WIDTH);
        double height = videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT);
        double totalFrames = videoCapture.get(cv::CAP_PROP_FRAME_COUNT);

        std::cout << "[CombinedSensor] Параметры видео:" << std::endl;
        std::cout << "  - FPS: " << fps << std::endl;
        std::cout << "  - Размер: " << width << "x" << height << std::endl;
        if (!useWebcam)
        {
            std::cout << "  - Всего кадров: " << totalFrames << std::endl;
        }

        isInitialized = true;
        std::cout << "[CombinedSensor] Инициализация завершена успешно!" << std::endl;
    }

    std::string getType() const override
    {
        return "CombinedSensor (Физическая камера + Аналитический процессор)";
    }

    void printInfo() const override
    {
        std::cout << "\n=== Информация о сенсоре ===" << std::endl;
        std::cout << "ID сенсора: " << getSensorId() << std::endl;
        std::cout << "Тип: " << getType() << std::endl;
        std::cout << "Состояние: " << (isInitialized ? "Инициализирован" : "Не инициализирован") << std::endl;
        std::cout << "FPS: " << getFPS() << std::endl;
        std::cout << "Метод обработки: " << getProcessingMethod() << std::endl;
        std::cout << "Режим работы: " << (useWebcam ? "Веб-камера" : "Видеофайл") << std::endl;
        if (!useWebcam && videoCapture.isOpened())
        {
            std::cout << "Источник: " << videoPath << std::endl;
            std::cout << "Всего кадров в источнике: " << videoCapture.get(cv::CAP_PROP_FRAME_COUNT) << std::endl;
        }
        std::cout << "============================" << std::endl;
    }

    cv::Mat getFrame() override
    {
        cv::Mat frame;

        if (!videoCapture.isOpened())
        {
            std::cerr << "[CombinedSensor::getFrame] Ошибка: видео не открыто!" << std::endl;
            return frame;
        }

        videoCapture >> frame;

        if (frame.empty())
        {
            stats.emptyFrames++;
        }
        else
        {
            frameCounter++;
        }

        return frame;
    }

    void analyze(const cv::Mat &frame) override
    {
        if (frame.empty())
        {
            return;
        }

        auto startTime = std::chrono::high_resolution_clock::now();

        cv::Mat processedFrame;
        std::string analysisResult;

        if (processingMethod == "GRAYSCALE_BRIGHTNESS_ANALYSIS")
        {
            cv::cvtColor(frame, processedFrame, cv::COLOR_BGR2GRAY);

            cv::Scalar mean = cv::mean(processedFrame);
            double brightness = mean[0];
            totalBrightness += brightness;

            analysisResult = "Средняя яркость: " + std::to_string(brightness);

            cv::putText(processedFrame, analysisResult, cv::Point(10, 50),
                        cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255), 2);

            std::string frameInfo = "Frame #" + std::to_string(frameCounter);
            cv::putText(processedFrame, frameInfo, cv::Point(10, 30),
                        cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255), 2);
        }
        else if (processingMethod == "EDGE_DETECTION")
        {
            cv::Mat gray;
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            cv::Canny(gray, processedFrame, 100, 200);

            analysisResult = "Edge detection (Canny)";

            cv::putText(processedFrame, analysisResult, cv::Point(10, 50),
                        cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255), 2);
            cv::putText(processedFrame, "Frame #" + std::to_string(frameCounter),
                        cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255), 2);
        }
        else if (processingMethod == "BLUR_ANALYSIS")
        {
            cv::GaussianBlur(frame, processedFrame, cv::Size(15, 15), 0);
            cv::Mat gray;
            cv::cvtColor(processedFrame, gray, cv::COLOR_BGR2GRAY);
            cv::Scalar mean = cv::mean(gray);

            analysisResult = "Размыто, средняя яркость: " + std::to_string(mean[0]);

            cv::putText(processedFrame, analysisResult, cv::Point(10, 50),
                        cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
            cv::putText(processedFrame, "Frame #" + std::to_string(frameCounter),
                        cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
        }
        else
        {
            processedFrame = frame.clone();
            analysisResult = "Обработка: " + processingMethod;
            cv::putText(processedFrame, analysisResult, cv::Point(10, 50),
                        cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 255), 2);
        }

        if (frameCounter % 30 == 0)
        {
            std::cout << "[Анализ] Кадр #" << frameCounter
                      << " | " << analysisResult
                      << " | Размер: " << frame.cols << "x" << frame.rows
                      << std::endl;
        }

        cv::imshow("Аналитическая система наблюдения", processedFrame);

        stats.totalFramesProcessed++;
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        stats.averageProcessingTimeMs = (stats.averageProcessingTimeMs * (stats.totalFramesProcessed - 1) + duration.count()) / stats.totalFramesProcessed;
    }

    bool isOpened() const
    {
        return videoCapture.isOpened();
    }

    int getCurrentFrame() const
    {
        return frameCounter;
    }

    void resetVideo()
    {
        if (!useWebcam && videoCapture.isOpened())
        {
            videoCapture.set(cv::CAP_PROP_POS_FRAMES, 0);
            frameCounter = 0;
            std::cout << "[CombinedSensor] Видео сброшено на начало" << std::endl;
        }
    }
};

void demonstratePolymorphism(Sensor *sensor)
{
    std::cout << "\n=== Демонстрация полиморфизма ===" << std::endl;
    std::cout << "Вызов виртуальных методов через указатель на Sensor:" << std::endl;
    std::cout << "Тип объекта: " << sensor->getType() << std::endl;
    std::cout << "Статус инициализации: " << (sensor->getInitializedStatus() ? "Да" : "Нет") << std::endl;
    sensor->printInfo();
}

void demonstratePhysicalCamera(PhysicalCamera *camera)
{
    std::cout << "\n=== Демонстрация работы с PhysicalCamera ===" << std::endl;
    std::cout << "FPS камеры: " << camera->getFPS() << std::endl;
}

void demonstrateAnalyticalProcessor(AnalyticalProcessor *processor)
{
    std::cout << "\n=== Демонстрация работы с AnalyticalProcessor ===" << std::endl;
    std::cout << "Метод обработки: " << processor->getProcessingMethod() << std::endl;
}

int main(int argc, char **argv)
{
    std::cout << "========================================" << std::endl;
    std::cout << "  АНАЛИТИЧЕСКАЯ СИСТЕМА НАБЛЮДЕНИЯ" << std::endl;
    std::cout << "  Демонстрация множественного наследования" << std::endl;
    std::cout << "  и работы с видео OpenCV" << std::endl;
    std::cout << "========================================\n"
              << std::endl;

    std::cout << "\n*** ПРОБЛЕМА РОМБА И ЕЁ РЕШЕНИЕ ***" << std::endl;
    std::cout << "Иерархия классов:" << std::endl;
    std::cout << "        Sensor (абстрактный класс)" << std::endl;
    std::cout << "        /                    \\" << std::endl;
    std::cout << "    (virtual)              (virtual)" << std::endl;
    std::cout << "      /                        \\" << std::endl;
    std::cout << "PhysicalCamera        AnalyticalProcessor" << std::endl;
    std::cout << "      \\                        /" << std::endl;
    std::cout << "       \\                      /" << std::endl;
    std::cout << "         CombinedSensor (один экземпляр Sensor!)" << std::endl;
    std::cout << "\nБез virtual наследования в CombinedSensor было бы ДВЕ копии Sensor," << std::endl;
    std::cout << "что вызвало бы неоднозначность при вызове методов.\n"
              << std::endl;

    CombinedSensor *sensor = nullptr;

    std::cout << "Выберите источник видео:" << std::endl;
    std::cout << "1 - Видеофайл (test_video.mp4)" << std::endl;
    std::cout << "2 - Веб-камера" << std::endl;
    std::cout << "3 - Веб-камера (альтернативный ID)" << std::endl;
    std::cout << "Ваш выбор (1-3): ";

    int choice;
    std::cin >> choice;

    try
    {
        switch (choice)
        {
        case 1:
            sensor = new CombinedSensor("C:/Users/user/Desktop/Progect_Opencv/video_/build/Debug/test_video.mp4");
            break;
        case 2:
            sensor = new CombinedSensor(0);
            break;
        case 3:
            sensor = new CombinedSensor(1);
            break;
        default:
            std::cout << "Неверный выбор, использую видеофайл" << std::endl;
            sensor = new CombinedSensor("C:/Users/user/Desktop/Progect_Opencv/video_/build/Debug/test_video.mp4");
        }

        demonstratePolymorphism(sensor);
        demonstratePhysicalCamera(sensor);
        demonstrateAnalyticalProcessor(sensor);

        sensor->initialize();

        std::cout << "\nВыберите метод обработки кадров:" << std::endl;
        std::cout << "1 - Оттенки серого + анализ яркости" << std::endl;
        std::cout << "2 - Детекция границ (Canny)" << std::endl;
        std::cout << "3 - Размытие + анализ" << std::endl;
        std::cout << "Ваш выбор: ";

        int methodChoice;
        std::cin >> methodChoice;

        switch (methodChoice)
        {
        case 1:
            sensor->setProcessingMethod("GRAYSCALE_BRIGHTNESS_ANALYSIS");
            break;
        case 2:
            sensor->setProcessingMethod("EDGE_DETECTION");
            break;
        case 3:
            sensor->setProcessingMethod("BLUR_ANALYSIS");
            break;
        default:
            std::cout << "Использую метод по умолчанию" << std::endl;
        }

        std::cout << "\n=== НАЧАЛО ОБРАБОТКИ ВИДЕО ===" << std::endl;
        std::cout << "Управление:" << std::endl;
        std::cout << "  ESC или q - выход" << std::endl;
        std::cout << "  r - сбросить видео на начало" << std::endl;
        std::cout << "  s - показать статистику" << std::endl;
        std::cout << "  1,2,3 - смена метода обработки" << std::endl;
        std::cout << "==============================\n"
                  << std::endl;

        cv::namedWindow("Аналитическая система наблюдения", cv::WINDOW_NORMAL);

        bool running = true;
        while (running)
        {
            cv::Mat frame = sensor->getFrame();

            if (frame.empty())
            {
                std::cout << "\nДостигнут конец видео или кадр не получен" << std::endl;

                if (choice == 1)
                {
                    std::cout << "Нажмите 'r' для повтора или ESC для выхода" << std::endl;
                    int key = cv::waitKey(0);
                    if (key == 'r' || key == 'R')
                    {
                        sensor->resetVideo();
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }

            sensor->analyze(frame);

            int key = cv::waitKey(30);
            switch (key)
            {
            case 27:
            case 'q':
            case 'Q':
                running = false;
                std::cout << "\nЗавершение работы по запросу пользователя" << std::endl;
                break;
            case 'r':
            case 'R':
                if (choice == 1)
                {
                    sensor->resetVideo();
                    std::cout << "Видео сброшено" << std::endl;
                }
                break;
            case 's':
            case 'S':
                sensor->printInfo();
                break;
            case '1':
                sensor->setProcessingMethod("GRAYSCALE_BRIGHTNESS_ANALYSIS");
                std::cout << "Метод обработки изменен на: Анализ яркости" << std::endl;
                break;
            case '2':
                sensor->setProcessingMethod("EDGE_DETECTION");
                std::cout << "Метод обработки изменен на: Детекция границ" << std::endl;
                break;
            case '3':
                sensor->setProcessingMethod("BLUR_ANALYSIS");
                std::cout << "Метод обработки изменен на: Размытие" << std::endl;
                break;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "\nОШИБКА: " << e.what() << std::endl;
        std::cerr << "Убедитесь, что:" << std::endl;
        std::cerr << "  - OpenCV установлен и настроен" << std::endl;
        std::cerr << "  - Видеофайл test_video.mp4 существует (если выбран файл)" << std::endl;
        std::cerr << "  - Веб-камера доступна (если выбран режим камеры)" << std::endl;
        return -1;
    }

    cv::destroyAllWindows();
    delete sensor;

    std::cout << "\n=== ПРОГРАММА ЗАВЕРШЕНА ===" << std::endl;

    return 0;
}