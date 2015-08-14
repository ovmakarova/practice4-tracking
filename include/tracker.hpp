#include <opencv2/core/core.hpp>

class Tracker
{
 public:
    virtual ~Tracker() {}

    virtual bool init( const cv::Mat& frame, const cv::Rect& initialPosition ) = 0;
    virtual bool track( const cv::Mat& frame, cv::Rect& newPosition ) = 0;
};

enum TRACKER_IMPLEMENTATIONS {
    DUMMY,
    // Add your enum value here
    // YOUR_NAME,
    NUM_IMPLS
};

cv::Ptr<Tracker> createTracker(TRACKER_IMPLEMENTATIONS impl);
