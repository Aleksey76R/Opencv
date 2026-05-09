#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

int main()
{
    std::cout << "=== WINDOWS CAMERA FIX (MSMF MODE) ===" << std::endl;

    cv::VideoCapture cap;
    bool camera_found = false;
    int working_index = -1;

    // Список индексов для проверки (часто камера может быть на 0, 1 или даже 700+)
    std::vector<int> indices = {0, 1, 2, 3, 4, 700, 701};

    // Пытаемся открыть через MSMF (самый стабильный для новых камер в Windows 10/11) [citation:1]
    for (int idx : indices)
    {
        std::cout << "Trying MSMF with index " << idx << "... ";
        cap.open(idx, cv::CAP_MSMF); // Явно указываем MSMF

        if (cap.isOpened())
        {
            std::cout << "SUCCESS! (Opened)" << std::endl;

            // Проверяем, читается ли картинка (иногда открывается, но кадр не берет)
            cv::Mat test_frame;
            cap >> test_frame;
            if (!test_frame.empty())
            {
                std::cout << "Frame check: PASSED. Camera is working!" << std::endl;
                camera_found = true;
                working_index = idx;
                break;
            }
            else
            {
                std::cout << "Frame check: FAILED (black screen). Trying next index..." << std::endl;
                cap.release();
            }
        }
        else
        {
            std::cout << "FAILED" << std::endl;
        }
    }

    // Резервный вариант: DSHOW, если MSMF не сработал
    if (!camera_found)
    {
        std::cout << "\nTrying fallback DSHOW..." << std::endl;
        for (int idx : {0, 1, 2})
        {
            cap.open(idx, cv::CAP_DSHOW);
            if (cap.isOpened())
            {
                camera_found = true;
                working_index = idx;
                std::cout << "DSHOW opened index " << idx << std::endl;
                break;
            }
        }
    }

    if (!camera_found)
    {
        std::cerr << "\n!!! CAMERA NOT FOUND !!!" << std::endl;
        std::cerr << "Next steps:" << std::endl;
        std::cerr << "1. Go to Settings -> Privacy & security -> Camera" << std::endl;
        std::cerr << "2. ENABLE 'Camera access' and 'Let apps access your camera'" << std::endl;
        std::cerr << "3. Also check your antivirus (Avast, Kaspersky) - they have 'Webcam Protection' features that block OpenCV [citation:4]" << std::endl;
        std::cerr << "4. Restart PC and try again" << std::endl;
        std::cout << "\nPress Enter...";
        std::cin.get();
        return -1;
    }

    std::cout << "\n======================================" << std::endl;
    std::cout << "CAMERA FOUND! (Index: " << working_index << ")" << std::endl;
    std::cout << "Press ESC to quit, SPACE for pause" << std::endl;
    std::cout << "======================================\n"
              << std::endl;

    // Настройка параметров для улучшения совместимости
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    // Принудительно запрашиваем MJPEG, если камера его поддерживает [citation:2]
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));

    cv::Mat frame;
    bool paused = false;

    while (true)
    {
        if (!paused)
        {
            cap >> frame;
            if (frame.empty())
            {
                std::cerr << "Warning: Empty frame! Trying to reconnect..." << std::endl;
                cv::waitKey(500);
                continue;
            }

            // Визуальный индикатор работы
            cv::putText(frame, "LIVE: WORKING", cv::Point(10, 30),
                        cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
            cv::imshow("Camera Test", frame);
        }

        int key = cv::waitKey(30);
        if (key == 27)
            break; // ESC
        if (key == 32)
            paused = !paused; // SPACE
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}