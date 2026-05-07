#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

class HistogramAnalyzer;

class ImageContainer
{
private:
    Mat image;
    string path;
    bool loaded;

public:
    ImageContainer(string imgPath) : path(imgPath), loaded(false) {}

    ImageContainer(Mat img) : image(img.clone()), loaded(true) {}

    ~ImageContainer() {}

    bool load()
    {
        image = imread(path);
        if (image.empty())
        {
            loaded = false;
            return false;
        }
        loaded = true;
        return true;
    }

    bool isLoaded() const
    {
        return loaded && !image.empty();
    }

    Mat getImage() const
    {
        return image;
    }

    void show() const
    {
        if (isLoaded())
        {
            imshow("Image", image);
            waitKey(0);
            destroyWindow("Image");
        }
    }

    friend void analyzeBrightness(const ImageContainer &img);
    friend class HistogramAnalyzer;
};

void analyzeBrightness(const ImageContainer &img)
{
    if (!img.isLoaded())
    {
        cout << "Image not loaded!" << endl;
        return;
    }

    Mat gray;
    if (img.image.channels() == 3)
    {
        cvtColor(img.image, gray, COLOR_BGR2GRAY);
    }
    else
    {
        gray = img.image.clone();
    }

    int histSize = 256;
    float range[] = {0, 256};
    const float *histRange = {range};
    Mat hist;
    calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, &histRange);

    double minVal, maxVal;
    minMaxLoc(gray, &minVal, &maxVal);
    Scalar meanVal = mean(gray);

    int peaks = 0;
    for (int i = 1; i < histSize - 1; i++)
    {
        float v = hist.at<float>(i);
        float p = hist.at<float>(i - 1);
        float n = hist.at<float>(i + 1);
        if (v > p && v > n)
        {
            peaks++;
        }
    }

    cout << "\n=== Friend Function ===" << endl;
    cout << "Direct access to private image!" << endl;
    cout << "Mean brightness: " << meanVal[0] << endl;
    cout << "Range: [" << minVal << ", " << maxVal << "]" << endl;
    cout << "Peaks: " << peaks << endl;
}

class HistogramAnalyzer
{
public:
    void computeHistogram(const ImageContainer &img, int bins = 256)
    {
        if (!img.isLoaded())
        {
            cout << "Image not loaded!" << endl;
            return;
        }

        cout << "\n=== Friend Class ===" << endl;
        cout << "Direct access to private image!" << endl;

        if (img.image.channels() == 1)
        {
            Mat hist;
            int histSize = bins;
            float range[] = {0, 256};
            const float *histRange = {range};
            calcHist(&img.image, 1, 0, Mat(), hist, 1, &histSize, &histRange);

            cout << "Histogram computed" << endl;
            showHistogram(hist, "Grayscale Histogram");
        }
        else if (img.image.channels() == 3)
        {
            vector<Mat> planes;
            split(img.image, planes);

            int histSize = bins;
            float range[] = {0, 256};
            const float *histRange = {range};

            Mat b_hist, g_hist, r_hist;
            calcHist(&planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange);
            calcHist(&planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange);
            calcHist(&planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange);

            cout << "BGR histograms computed" << endl;
            showHistogram(b_hist, "Blue Channel", Scalar(255, 0, 0));
            showHistogram(g_hist, "Green Channel", Scalar(0, 255, 0));
            showHistogram(r_hist, "Red Channel", Scalar(0, 0, 255));
        }
    }

    void saveHistogramToCSV(const ImageContainer &img, const string &filename)
    {
        if (!img.isLoaded())
        {
            cout << "Image not loaded!" << endl;
            return;
        }

        Mat gray;
        if (img.image.channels() == 3)
        {
            cvtColor(img.image, gray, COLOR_BGR2GRAY);
        }
        else
        {
            gray = img.image.clone();
        }

        int histSize = 256;
        float range[] = {0, 256};
        const float *histRange = {range};
        Mat hist;
        calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, &histRange);

        FILE *f = fopen(filename.c_str(), "w");
        if (f)
        {
            fprintf(f, "Bin,Value\n");
            for (int i = 0; i < histSize; i++)
            {
                fprintf(f, "%d,%f\n", i, hist.at<float>(i));
            }
            fclose(f);
            cout << "Saved to: " << filename << endl;
        }
    }

    double computeEntropy(const ImageContainer &img)
    {
        if (!img.isLoaded())
        {
            cout << "Image not loaded!" << endl;
            return -1.0;
        }

        Mat gray;
        if (img.image.channels() == 3)
        {
            cvtColor(img.image, gray, COLOR_BGR2GRAY);
        }
        else
        {
            gray = img.image.clone();
        }

        int histSize = 256;
        float range[] = {0, 256};
        const float *histRange = {range};
        Mat hist;
        calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, &histRange);

        float total = gray.total();
        double entropy = 0.0;

        for (int i = 0; i < histSize; i++)
        {
            float p = hist.at<float>(i) / total;
            if (p > 0)
            {
                entropy += -p * log2(p);
            }
        }

        cout << "Entropy: " << entropy << " bits" << endl;
        return entropy;
    }

private:
    void showHistogram(const Mat &hist, const string &title, Scalar color = Scalar(255, 255, 255))
    {
        int hist_w = 512;
        int hist_h = 400;
        int bin_w = cvRound((double)hist_w / hist.rows);

        Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
        normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

        for (int i = 1; i < hist.rows; i++)
        {
            line(histImage,
                 Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
                 Point(bin_w * i, hist_h - cvRound(hist.at<float>(i))),
                 color, 2, 8, 0);
        }

        imshow(title, histImage);
        waitKey(0);
        destroyWindow(title);
    }
};

int main()
{
    cout << "=== Image Histogram Analyzer ===" << endl;
    cout << "Demonstration of friend functions and classes" << endl;

    string imagePath;
    cout << "Enter image path: ";
    cin >> imagePath;

    ImageContainer img(imagePath);

    if (!img.load())
    {
        cout << "Failed to load image!" << endl;
        return -1;
    }

    int choice;
    HistogramAnalyzer analyzer;

    do
    {
        cout << "\n=== Menu ===" << endl;
        cout << "1. Show image" << endl;
        cout << "2. Standard access (getter)" << endl;
        cout << "3. Friend function analysis" << endl;
        cout << "4. Friend class histogram" << endl;
        cout << "5. Save histogram to CSV" << endl;
        cout << "6. Compute entropy" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            img.show();
            break;
        case 2:
        {
            Mat current = img.getImage();
            cout << "Image accessed via getter: " << current.cols << "x" << current.rows << endl;
            break;
        }
        case 3:
            analyzeBrightness(img);
            break;
        case 4:
            analyzer.computeHistogram(img);
            break;
        case 5:
        {
            string filename;
            cout << "Enter CSV filename: ";
            cin >> filename;
            analyzer.saveHistogramToCSV(img, filename);
            break;
        }
        case 6:
            analyzer.computeEntropy(img);
            break;
        case 0:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice!" << endl;
        }
    } while (choice != 0);

    return 0;
}