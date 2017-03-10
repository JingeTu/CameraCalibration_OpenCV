const char *windowName = "Take Images";

bool m_saveCurrentFrame = false;

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <sstream>

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

#if !defined VK_ESCAPE
    #define VK_ESCAPE 0x1B      // Escape character (27)
#endif

// Get access to the webcam.
void initWebcam(VideoCapture &videoCapture, int cameraNumber)
{
    // Get access to the default camera.
    try {   // Surround the OpenCV call by a try/catch block so we can give a useful error message!
        videoCapture.open(cameraNumber);
    } catch (cv::Exception &e) {}
    if ( !videoCapture.isOpened() ) {
        cerr << "ERROR: Could not access the camera!" << endl;
        exit(1);
    }
    cout << "Loaded camera " << cameraNumber << "." << endl;
}


// Keypress event handler. Note that it should be a 'char' and not an 'int' to better support Linux.
void onKeypress(char key)
{
    switch (key) {
    case 'w':
        m_saveCurrentFrame = true;
        cout << "Save Current Frame." << endl;
        break;
    }
}


int main(int argc, char *argv[])
{
    cout << "Take WebCamera Images, by Jinge Tu. Base on the work of Shervin Emami (https://github.com/MasteringOpenCV/code/blob/master/Chapter1_AndroidCartoonifier/Cartoonifier_Desktop/main_desktop.cpp)."<< endl;
    cout << "Compiled with OpenCV version " << CV_VERSION << endl;
    cout << endl;

    cout << "Keyboard commands (press in the GUI window):" << endl;
    cout << "    Esc:  Quit the program." << endl;
    cout << "    w:    save current frame to disk." << endl;
    cout << endl;

    // Allow the user to specify a camera number, since not all computers will be the same camera number.
    int cameraNumber = 0;   // Change this if you want to use a different camera device.
    if (argc > 1) {
        cameraNumber = atoi(argv[1]);
    }

    int DESIRED_CAMERA_WIDTH = 640;
    int DESIRED_CAMERA_HEIGHT = 480;
    if (argc > 2) {
        DESIRED_CAMERA_WIDTH = atoi(argv[2]);
        DESIRED_CAMERA_HEIGHT = atoi(argv[3]);
    }

    int IMAGE_NUM = 15;
    if (argc > 3) {
        IMAGE_NUM = atoi(argv[4]);
    }

    cout << "Set Camera Width to " << DESIRED_CAMERA_WIDTH << ", Height to " << DESIRED_CAMERA_HEIGHT << "." << endl;

    cout << "Take " << IMAGE_NUM << "images." << endl;

    // Get access to the camera.
    VideoCapture camera;
    initWebcam(camera, cameraNumber);

    // Try to set the camera resolution. Note that this only works for some cameras on
    // some computers and only for some drivers, so don't rely on it to work!
    camera.set(CV_CAP_PROP_FRAME_WIDTH, DESIRED_CAMERA_WIDTH);
    camera.set(CV_CAP_PROP_FRAME_HEIGHT, DESIRED_CAMERA_HEIGHT);

    // Create a GUI window for display on the screen.
    namedWindow(windowName); // Resizable window, might not work on Windows.

    int count = 0;

    // Run forever, until the user hits Escape to "break" out of this loop.
    while (true) {

        // Grab the next camera frame. Note that you can't modify camera frames.
        Mat cameraFrame;
        camera >> cameraFrame;
        if( cameraFrame.empty() ) {
            cerr << "ERROR: Couldn't grab the next camera frame." << endl;
            exit(1);
        }

        imshow(windowName, cameraFrame);

        // IMPORTANT: Wait for atleast 20 milliseconds, so that the image can be displayed on the screen!
        // Also checks if a key was pressed in the GUI window. Note that it should be a "char" to support Linux.
        char keypress = waitKey(20);  // This is needed if you want to see anything!
        if (keypress == VK_ESCAPE) {   // Escape Key
            // Quit the program!
            break;
        }
        // Process any other keypresses.
        if (keypress > 0) {
            onKeypress(keypress);
        }

        if (m_saveCurrentFrame) {
            stringstream ss;
            ss << "data/" << ++count << ".png";

            // string filename = itoa(count++) + ".png";
            if (imwrite(ss.str(), cameraFrame)) {
                cout << ss.str() << " saved." << std::endl;
            }
            else {
                count --;
            }
            m_saveCurrentFrame = false;

            if (count == IMAGE_NUM) {
                break;
            }
        }

    }//end while

    return EXIT_SUCCESS;
}
