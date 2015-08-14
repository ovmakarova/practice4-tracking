#include "tracker.hpp"

cv::Ptr<Tracker> createTrackerDummy();
// Declare your implementation here
// cv::Ptr<Tracker> createTrackerYourName();

cv::Ptr<Tracker> createTracker(TRACKER_IMPLEMENTATIONS impl)
{
    switch (impl) {
        case DUMMY:
            return createTrackerDummy();
        // Add case for your implementation
        // case YOUR_NAME:
        //     return createTrackerYourName();
        default:
            return 0;
    }
}
