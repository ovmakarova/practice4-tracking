#include <tracker.hpp>

// #include <stdio.h>
#include <iostream>

#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

const std::string windowName = "Tracking Sample";

static Mat screenImage;
static Rect boundingBox;
static bool isObjectSelected = false;
static bool startSelection = false;

static void onMouse( int event, int x, int y, int, void* )
{
    if( !isObjectSelected )
    {
        switch ( event )
        {
            case EVENT_LBUTTONDOWN:
                //set origin of the bounding box
                startSelection = true;
                boundingBox.x = x;
                boundingBox.y = y;
                break;
            case EVENT_LBUTTONUP:
                //set width and height of the bounding box
                boundingBox.width = std::abs( x - boundingBox.x );
                boundingBox.height = std::abs( y - boundingBox.y );
                // paused = false;
                isObjectSelected = true;
                break;
            case EVENT_MOUSEMOVE:
              if( startSelection && !isObjectSelected )
              {
                  //draw the bounding box
                  Mat currentFrame;
                  screenImage.copyTo( currentFrame );
                  rectangle( currentFrame,
                             Point((int) boundingBox.x, (int)boundingBox.y ),
                             Point( x, y ),
                             Scalar( 255, 0, 0 ), 2, 1 );
                  imshow( windowName, currentFrame );
              }
              break;
        }
    }
}

static void help()
{
    cout <<
    "\nThis example shows the functionality of \"Long-term optical tracking API\""
    "-- pause video [p] and draw a bounding box around the target to start the tracker\n"
    "Example of <video_name> is in opencv_extra/testdata/cv/tracking/\n"
    "Call:\n"
    "./tracker <tracker_algorithm> <video_name> <start_frame> [<bounding_frame>]\n"
    << endl;

    cout << "\n\nHot keys: \n"
    "\tEsc, q - quit the program\n"
    "\tp - pause video\n";
}

static const char* keys =
{
    "{1 | |                 | Tracker algorithm }"
    "{2 | |                 | video name        }"
    "{3 | | 0,0,0,0 | Initial bounding frame    }"
};

int main( int argc, char** argv )
{
    CommandLineParser parser( argc, argv, keys );

    string tracker_algorithm = parser.get<string>("1");
    string video_name = parser.get<string>( "2" );

    cout << "Tracker algorithm: " << tracker_algorithm << endl;
    cout << "Video name: " << video_name << endl;

    if( tracker_algorithm.empty() || video_name.empty() )
    {
        cout << "Error: can't recognize tracking algorithm or open video" << endl;
        help();
        return -1;
    }

    int coords[4] = {0, 0, 0, 0};
    bool initBoxWasGivenInCommandLine = false;
    {
        String initBoundingBox = parser.get<string>("3");

        for(size_t npos=0, pos=0, ctr=0; ctr < 4; ctr++)
        {
            npos = initBoundingBox.find_first_of(',', pos);
            if(npos == string::npos && ctr < 3)
            {
                break;
            }

            int num = atoi(initBoundingBox.substr(pos,(ctr==3)?(string::npos):(npos-pos)).c_str());
            if(num <= 0)
            {
                break;
            }
            coords[ctr] = num;
            pos = npos + 1;
        }
        if(coords[0]>0 && coords[1]>0 && coords[2]>0 && coords[3]>0)
        {
            initBoxWasGivenInCommandLine = true;
        }
        else
        {
            cout << "\nBounding box should be given in format \"x1,y1,x2,y2\",\n"
                    "where x's and y's are integer cordinates of opposed corners of bdd box" << endl;
            cout << "Manual selection of bounding box will be used\n" << endl;
        }
    }

    // Open the video file
    VideoCapture cap;
    cap.open( video_name );

    if( !cap.isOpened() )
    {
        cout << "Error: could not initialize video capturing...\n";
        help();
        return -1;
    }

    // paused = true;
    namedWindow( windowName, 1 );
    setMouseCallback( windowName, onMouse, 0 );

    // Get the first frame
    Mat frame;
    cap >> frame;
    frame.copyTo( screenImage );

    // Init bounding box
    if(initBoxWasGivenInCommandLine)
    {
        isObjectSelected = true;
        // paused = false;
        boundingBox.x = coords[0];
        boundingBox.y = coords[1];
        boundingBox.width  = std::abs( coords[2] - coords[0] );
        boundingBox.height = std::abs( coords[3] - coords[1] );
        cout << "Bounding box with vertices ("
             << coords[0] << ", " << coords[1] << ") and ("
             << coords[2] << ", " << coords[3] << ") was given in command line" << endl;
        rectangle( screenImage, boundingBox, Scalar( 255, 0, 0 ), 2, 1 );
    }

    imshow( windowName, screenImage );

    // Instantiate Tracker
    Ptr<Tracker> tracker = create( tracker_algorithm );
    if( tracker == NULL )
    {
        cout << "Error: can't create tracker...\n";
        return -1;
    }

    // Main loop of the application
    bool initialized = false;
    for ( ;; )
    {
        // if( !paused )
        {
            if(initialized)
            {
                cap >> frame;
                if(frame.empty()){
                    break;
                }
                frame.copyTo( screenImage );
            }

            if( !initialized && isObjectSelected )
            {
                    //initializes the tracker
                if( !tracker->init( frame, boundingBox ) )
                {
                    cout << "***Could not initialize tracker...***\n";
                    return -1;
                }
                initialized = true;
            }
            else if( initialized )
            {
                    //updates the tracker
                if( tracker->update( frame, boundingBox ) )
                {
                    rectangle( screenImage, boundingBox, Scalar( 255, 0, 0 ), 2, 1 );
                }
            }
            imshow( windowName, screenImage );
        }

        char c = (char) waitKey( 2 );
        if( c == 27 ) // Esc
            break;
        if( c == 'q' )
            break;
        // if( c == 'p' )
        //     paused = !paused;

    }

    return 0;
}
