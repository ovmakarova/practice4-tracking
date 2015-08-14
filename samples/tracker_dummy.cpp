#include <tracker.hpp>

class TrackerDummy : public Tracker
{
 public:
    virtual ~TrackerDummy() {}

    virtual bool init( const cv::Mat& frame, const cv::Rect& initialPosition );
    virtual bool track( const cv::Mat& frame, cv::Rect& newPosition );

 private:
    cv::Rect position_;
};

bool TrackerDummy::init( const cv::Mat& frame, const cv::Rect& initialPosition )
{
    position_ = initialPosition;
}

bool TrackerDummy::track( const cv::Mat& frame, cv::Rect& newPosition )
{
    newPosition = position_;
}

cv::Ptr<Tracker> createTrackerDummy()
{
    return cv::Ptr<Tracker>(new TrackerDummy());
}
