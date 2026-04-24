#include "ImageTransformer.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{
    string path;
    cout << "Enter path to image: ";
    cin >> path;

    cv::Mat img = cv::imread(path);
    if (img.empty())
    {
        cerr << "Error: Cannot load image!" << endl;
        return -1;
    }

    ImageTransformer transformer(img);
    cout << "Image loaded: " << img.cols << "x" << img.rows << endl;

    int choice;
    do
    {
        cout << "\n--- Menu ---" << endl;
        cout << "1. Show image" << endl;
        cout << "2. Horizontal flip" << endl;
        cout << "3. Vertical flip" << endl;
        cout << "4. Transpose" << endl;
        cout << "5. Rotate 90° clockwise" << endl;
        cout << "6. Rotate 90° counter-clockwise" << endl;
        cout << "7. Save image" << endl;
        cout << "8. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cv::imshow("Current Image", transformer.get_image());
            cv::waitKey(0);
            break;
        case 2:
            transformer.flipHorizontal();
            break;
        case 3:
            transformer.flipVertical();
            break;
        case 4:
            transformer.transpone();
            break;
        case 5:
            transformer.rotate90Clockwise();
            break;
        case 6:
            transformer.rotate90CounterClockwise();
            break;
        case 7:
        {
            string filename;
            cout << "Enter filename to save: ";
            cin >> filename;
            cv::imwrite(filename, transformer.get_image());
            cout << "Saved as " << filename << endl;
            break;
        }
        case 8:
            cout << "Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice!" << endl;
        }
    } while (choice != 8);

    return 0;
}