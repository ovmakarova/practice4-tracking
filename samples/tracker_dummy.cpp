#include <tracker.hpp>

bool Tracker::init( const cv::Mat& image, const cv::Rect& boundingBox )
{

}

bool Tracker::update( const cv::Mat& image, cv::Rect& boundingBox )
{

}

cv::Ptr<Tracker> create( const std::string& trackerType )
{
    return new Tracker();
}