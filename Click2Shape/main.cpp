#include <iostream>
#include <cstdlib>
#include <ctime>
#include "AppState.h"

using namespace std;
using namespace cv;

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));

    AppState state;

    namedWindow("Annotation Tool");
    setMouseCallback("Annotation Tool", AppState::onMouse, &state);

    cout << "=== Interactive Shape Annotator ===\n";
    cout << "Left click  -> add shape (C/R mode)\n";
    cout << "Right click -> remove shape under cursor\n";
    cout << "Key C       -> Circle mode\n";
    cout << "Key R       -> Rectangle mode\n";
    cout << "ESC         -> exit\n\n";

    state.redraw();

    while (true)
    {
        char key = static_cast<char>(waitKey(20));

        if (key == 27)
        { 
            cout << "\nExiting application...\n";
            break;
        }
        else if (key == 'c' || key == 'C')
        {
            state.setMode(true);
            cout << "Switched to CIRCLE mode\n";
        }
        else if (key == 'r' || key == 'R')
        {
            state.setMode(false);
            cout << "Switched to RECTANGLE mode\n";
        }
    }

    destroyAllWindows();

    return 0;
}