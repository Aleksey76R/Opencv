#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

const Size BOARD_SIZE(9, 6);
const int RADIUS = 6;

Scalar getRainbowColor(int index, int total)
{
    double hue = (double)index / total * 180.0;
    cv::Mat hsv(1, 1, CV_8UC3, cv::Scalar(hue, 255, 255));
    cv::Mat bgr;
    cv::cvtColor(hsv, bgr, cv::COLOR_HSV2BGR);
    return cv::Scalar(
        bgr.at<cv::Vec3b>(0, 0)[0],
        bgr.at<cv::Vec3b>(0, 0)[1],
        bgr.at<cv::Vec3b>(0, 0)[2]);
}

int main(int argc, char **argv)
{
    cv::VideoCapture cap;

    if (argc > 1)
    {
        cap.open(argv[1]);
        if (!cap.isOpened())
        {
            std::cerr << "Error: Cannot open video file " << argv[1] << std::endl;
            return -1;
        }
    }
    else
    {
        cap.open(0);
        if (!cap.isOpened())
        {
            std::cerr << "Error: Cannot open webcam" << std::endl;
            return -1;
        }
    }

    cv::Mat frame, gray;
    int frameCount = 0;

    std::cout << "Chessboard Detector started" << std::endl;
    std::cout << "Board size: " << BOARD_SIZE.width << "x" << BOARD_SIZE.height << " corners" << std::endl;
    std::cout << "Press ESC to exit" << std::endl;

    while (true)
    {
        cap >> frame;
        if (frame.empty())
        {
            std::cout << "End of video stream" << std::endl;
            break;
        }

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        std::vector<cv::Point2f> corners;

        bool found = cv::findChessboardCorners(
            gray,
            BOARD_SIZE,
            corners,
            cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

        if (found)
        {
            cv::cornerSubPix(
                gray,
                corners,
                cv::Size(5, 5),
                cv::Size(-1, -1),
                cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.001));

            for (size_t i = 0; i < corners.size(); i++)
            {
                cv::Scalar color = getRainbowColor(i, corners.size());
                cv::circle(frame, corners[i], RADIUS, color, -1);
                cv::circle(frame, corners[i], RADIUS, cv::Scalar(255, 255, 255), 1);
            }

            cv::drawChessboardCorners(frame, BOARD_SIZE, corners, found);

            cv::putText(frame, "Chessboard found! (" + std::to_string(corners.size()) + " corners)",
                        cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
        }
        else
        {
            cv::putText(frame, "No chessboard found",
                        cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 2);
        }

        cv::imshow("Chessboard Detector", frame);

        if (cv::waitKey(1) == 27)
        {
            break;
        }

        frameCount++;
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}