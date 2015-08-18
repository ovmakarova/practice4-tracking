#include <tracker.hpp>
#include "gui.hpp"

#include <iostream>
#include <fstream>

bool exists(const std::string &filename)
{
    std::ifstream infile(filename.c_str());
    bool ret = infile.good();
    infile.close();
    return ret;
}

cv::Rect parseRect(std::string rep)
{
    std::replace(rep.begin(), rep.end(), ',', ' ');
    std::istringstream init_stream(rep);
    std::vector<float> coords(4, 0.0f);
    for (size_t i = 0; i < coords.size(); i++)
        init_stream >> coords[i];

    return cv::Rect(cv::Point(coords[0] + 0.5, coords[1] + 0.5),
                    cv::Point(coords[2] + 0.5, coords[3] + 0.5));
}

float overlap(const cv::Rect &guess, const cv::Rect &gt)
{
    if (guess == cv::Rect() && gt == cv::Rect())
        return 1.0f;

    cv::Rect intersection = guess & gt;
    if (intersection == cv::Rect())
        return 0.0f;

    float div = guess.area() + gt.area() - intersection.area();
    if (div > 0.0f)
        return intersection.area() / div;
    else
        return 0.0f;
}

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
    std::string argv3 = parser.get<std::string>("3");
    std::string bounding_box_string;
    cv::Rect bounding_box;
    std::ifstream gt_file;
    if (exists(argv3))
    {
        gt_file.open(argv3.c_str(), std::ifstream::in);
        std::getline(gt_file, bounding_box_string);
        bounding_box = parseRect(bounding_box_string);
    }
    else
    {
        bounding_box = gui.initBoundingBox(parseRect(argv3), frame);
    }

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
    int num_correct = 0;
    int num_responses = 0;
    int num_objects = 0;
    while (true)
    {
        cap >> frame;
        if(frame.empty())
            break;

        cv::Rect position;
        bool found = tracker->track(frame, position);

        cv::Scalar rect_color = cv::Scalar(0, 255, 0);
        if (gt_file.is_open())
        {
            std::getline(gt_file, bounding_box_string);
            bounding_box = parseRect(bounding_box_string);

            if (found)
                num_responses++;

            if (bounding_box != cv::Rect())
                num_objects++;

            if (overlap(position, bounding_box) >= 0.25)
                num_correct++;
            else
                rect_color = cv::Scalar(0, 0, 255);
        }

        if (!gui.displayImage(frame,
                              found ? position : cv::Rect(),
                              gt_file.is_open() ? bounding_box : cv::Rect(),
                              rect_color))
            break;
    }

    std::cout << "Precision:\t" << num_correct / (float)num_responses << std::endl;
    std::cout << "Recall   :\t" << num_correct / (float)num_objects << std::endl;

    return 0;
}
