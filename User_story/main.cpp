#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <string>

using namespace std;
using namespace cv;

class ROIProcessor
{
public:
    virtual ~ROIProcessor() = default;
    virtual void process(Mat &roi) const = 0;
    virtual string name() const { return "BaseProcessor"; }
};

class BlurProcessor : public ROIProcessor
{
public:
    void process(Mat &roi) const override
    {
        GaussianBlur(roi, roi, Size(15, 15), 0);
    }

    string name() const override
    {
        return "Blur (Gaussian 15x15)";
    }
};

class GrayscaleProcessor : public ROIProcessor
{
public:
    void process(Mat &roi) const override
    {
        if (roi.channels() == 3)
        {
            Mat gray;
            cvtColor(roi, gray, COLOR_BGR2GRAY);
            cvtColor(gray, roi, COLOR_GRAY2BGR);
        }
    }

    std::string name() const override
    {
        return "Grayscale";
    }
};

class BrightnessProcessor : public ROIProcessor
{
private:
    int delta_;

public:
    explicit BrightnessProcessor(int delta = 50) : delta_(delta) {}

    void process(Mat &roi) const override
    {
        roi.convertTo(roi, -1, 1, delta_);
    }

    string name() const override
    {
        return "Brightness (+" + to_string(delta_) + ")";
    }
};


class ROIHandler
{
private:
    Mat originalImage_;
    Mat workingImage_;
    Rect currentRoi_;
    Point startPoint_;
    Point endPoint_;
    bool isSelecting_;
    bool hasRoi_;

public:
    ROIHandler() : isSelecting_(false), hasRoi_(false) {}

    void setImage(const Mat &img)
    {
        img.copyTo(originalImage_);
        img.copyTo(workingImage_);
        resetROI();
    }

    void handleMouse(int event, int x, int y)
    {
        switch (event)
        {
        case EVENT_LBUTTONDOWN:
            startPoint_ = Point(x, y);
            endPoint_ = startPoint_;
            isSelecting_ = true;
            break;

        case EVENT_MOUSEMOVE:
            if (isSelecting_)
            {
                endPoint_ = Point(x, y);
                updateDisplay();
            }
            break;

        case EVENT_LBUTTONUP:
            if (isSelecting_)
            {
                endPoint_ = Point(x, y);
                isSelecting_ = false;

                currentRoi_.x = min(startPoint_.x, endPoint_.x);
                currentRoi_.y = min(startPoint_.y, endPoint_.y);
                currentRoi_.width = abs(startPoint_.x - endPoint_.x);
                currentRoi_.height = abs(startPoint_.y - endPoint_.y);

                if (currentRoi_.width > 5 && currentRoi_.height > 5)
                {
                    hasRoi_ = true;
                }
                else
                {
                    hasRoi_ = false;
                    currentRoi_ = Rect();
                }

                updateDisplay();
            }
            break;

        case EVENT_RBUTTONDOWN:
            resetROI();
            break;
        }
    }

    void updateDisplay()
    {
        originalImage_.copyTo(workingImage_);

        if (hasRoi_ && currentRoi_.width > 0 && currentRoi_.height > 0)
        {
            rectangle(workingImage_, currentRoi_, Scalar(0, 255, 0), 2);
        }

        if (isSelecting_)
        {
            Rect tempRoi(
                min(startPoint_.x, endPoint_.x),
                min(startPoint_.y, endPoint_.y),
                abs(startPoint_.x - endPoint_.x),
                abs(startPoint_.y - endPoint_.y));
            rectangle(workingImage_, tempRoi, Scalar(255, 0, 0), 2);
        }

        if (hasRoi_)
        {
            string info = "ROI: " + to_string(currentRoi_.width) + "x" + to_string(currentRoi_.height);
            putText(workingImage_, info, Point(10, 30),
                        FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 255, 0), 2);
        }

        putText(workingImage_, "Left click: select ROI | Right click / R: reset | C: apply | ESC: exit",
                    Point(10, workingImage_.rows - 10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(200, 200, 200), 1);
    }

    void show() const
    {
        imshow("ROI Selector - Polymorphism Demo", workingImage_);
    }

    void resetROI()
    {
        hasRoi_ = false;
        isSelecting_ = false;
        currentRoi_ = Rect();
        originalImage_.copyTo(workingImage_);
        updateDisplay();
    }

    bool hasROI() const
    {
        return hasRoi_;
    }

    Rect getROI() const
    {
        return currentRoi_;
    }

    void applyProcessor(const ROIProcessor &processor)
    {
        if (!hasRoi_)
        {
            cout << "No ROI selected! Press R to reset and select area first." << endl;
            return;
        }

        if (currentRoi_.x < 0)
            currentRoi_.x = 0;
        if (currentRoi_.y < 0)
            currentRoi_.y = 0;
        if (currentRoi_.x + currentRoi_.width > originalImage_.cols)
            currentRoi_.width = originalImage_.cols - currentRoi_.x;
        if (currentRoi_.y + currentRoi_.height > originalImage_.rows)
            currentRoi_.height = originalImage_.rows - currentRoi_.y;

        if (currentRoi_.width <= 0 || currentRoi_.height <= 0)
        {
            cout << "Invalid ROI dimensions!" << endl;
            return;
        }

        cout << "Applying: " << processor.name() << " to ROI at ("
                  << currentRoi_.x << ", " << currentRoi_.y << ") size "
                  << currentRoi_.width << "x" << currentRoi_.height << endl;

        Mat roi = originalImage_(currentRoi_);

        processor.process(roi);

        updateDisplay();
    }
    Mat &getOriginalImage()
    {
        return originalImage_;
    }
};

ROIHandler *g_roiHandler = nullptr;

void onMouse(int event, int x, int y, int flags, void *userdata)
{
    if (g_roiHandler)
    {
        g_roiHandler->handleMouse(event, x, y);
    }
}


int main(int argc, char **argv)
{
    cout << "========================================" << endl;
    cout << "  ROI Selector with Polymorphism Demo  " << endl;
    cout << "========================================" << endl;
    cout << endl;

    string imagePath = "test.jpg";

    if (argc > 1)
    {
        imagePath = argv[1];
    }

    Mat image = imread(imagePath);
    if (image.empty())
    {
        cout << "File not found: " << imagePath << endl;
        cout << "Creating test image..." << endl;
        image = Mat(600, 800, CV_8UC3, Scalar(100, 150, 200));
        putText(image, "Test Image - Draw ROI with mouse",
                    Point(50, 300), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
        rectangle(image, Point(100, 100), Point(300, 200), Scalar(0, 0, 255), 3);
        circle(image, Point(500, 400), 50, Scalar(0, 255, 0), -1);
    }

    ROIHandler handler;
    handler.setImage(image);
    g_roiHandler = &handler;

    vector<unique_ptr<ROIProcessor>> processors;
    processors.push_back(make_unique<BlurProcessor>());
    processors.push_back(make_unique<GrayscaleProcessor>());
    processors.push_back(make_unique<BrightnessProcessor>(50));
    processors.push_back(make_unique<BrightnessProcessor>(-40));

    cout << "Available processors (polymorphism demonstration):" << endl;
    for (size_t i = 0; i < processors.size(); ++i)
    {
        cout << "  " << i + 1 << ". " << processors[i]->name() << endl;
    }
    cout << endl;
    cout << "Instructions:" << endl;
    cout << "  • Left click and drag - select ROI" << endl;
    cout << "  • Right click or R - reset ROI" << endl;
    cout << "  • C - select processor to apply" << endl;
    cout << "  • ESC/Q - exit" << endl;
    cout << endl;

    namedWindow("ROI Selector - Polymorphism Demo", WINDOW_NORMAL);
    setMouseCallback("ROI Selector - Polymorphism Demo", onMouse, nullptr);

    while (true)
    {
        handler.updateDisplay();
        handler.show();

        char key = static_cast<char>(waitKey(30));

        switch (key)
        {
        case 27:
        case 'q':
        case 'Q':
            cout << "Exiting..." << endl;
            goto exit_loop;

        case 'r':
        case 'R':
            cout << "Resetting ROI..." << endl;
            handler.resetROI();
            break;

        case 'c':
        case 'C':
            if (!handler.hasROI())
            {
                cout << "No ROI selected! Please select an area first." << endl;
                break;
            }

            cout << "\nSelect processor to apply (polymorphic call demonstration):" << endl;
            for (size_t i = 0; i < processors.size(); ++i)
            {
                cout << "  " << i + 1 << ". " << processors[i]->name() << endl;
            }
            cout << "Enter number (1-" << processors.size() << "): ";

            int choice;
            cin >> choice;
            cin.ignore();

            if (choice >= 1 && choice <= static_cast<int>(processors.size()))
            {
                ROIProcessor *processor = processors[choice - 1].get();
                handler.applyProcessor(*processor);
                cout << "✓ Applied: " << processor->name() << endl;
            }
            else
            {
                cout << "Invalid choice!" << endl;
            }
            cout << endl;
            break;

        default:
            break;
        }
    }

exit_loop:
    destroyAllWindows();
    return 0;
}