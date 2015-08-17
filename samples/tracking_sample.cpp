#include <tracker.hpp>
#include "gui.hpp"

#include <iostream>

void help(const char *argv0)
{
    std::cout <<
        "Usage:\n"
        << argv0 <<
        " <tracker_algorithm> <video_name> <bounding box>\n"
        "\nVideo examples can be found in opencv_extra/testdata/cv/tracking/\n"
        "\nBounding box should be given in format \"x1,y1,x2,y2\",\n"
        "where x's and y's are integer cordinates of opposed corners of bounding box"
              << std::endl;
}

static const char* keys =
{
    "{1 | |                 | Tracker algorithm }"
    "{2 | |                 | video name        }"
    "{3 | | 0,0,0,0         | Initial bounding frame    }"
};

int main( int argc, char** argv )
{
    cv::CommandLineParser parser( argc, argv, keys );

    // Parse and validate input parameters
    std::string tracker_algorithm = parser.get<std::string>("1");
    std::string video_name = parser.get<std::string>("2");

    std::cout << "Tracker algorithm: " << tracker_algorithm << std::endl;
    std::cout << "Video name: " << video_name << std::endl;

    if( tracker_algorithm.empty() || video_name.empty() )
    {
        std::cout << "Error: can't recognize tracking algorithm or open video" << std::endl;
        help(argv[0]);
        return 1;
    }

    // Open the video file
    cv::VideoCapture cap;
    cap.open( video_name );

    if( !cap.isOpened() )
    {
        std::cout << "Error: could not initialize video capturing...\n";
        help(argv[0]);
        return 1;
    }

    // Get the first frame
    cv::Mat frame;
    cap >> frame;

    // Initialize GUI
    GUI gui;
    cv::Rect bounding_box = gui.initBoundingBox(parser.get<std::string>("3"), frame);

    // Create Tracker
    cv::Ptr<Tracker> tracker = createTracker(tracker_algorithm);

    if (tracker == NULL)
    {
        std::cout << "Error: can't create tracker \"" << tracker_algorithm << "\"" << std::endl;
        return 1;
    }

    if (!tracker->init(frame, bounding_box))
    {
        std::cout << "Error: can't initialize tracker..." << std::endl;
        return 1;
    }

    // Run tracking
    while (true)
    {
        cap >> frame;
        if(frame.empty())
            break;

        cv::Rect position;
        bool found = tracker->track(frame, position);
        if (!gui.displayImage(frame, found ? position : cv::Rect()))
            break;
    }

    return 0;
}
