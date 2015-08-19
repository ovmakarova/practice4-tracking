#pragma once

#include <opencv2/core/core.hpp>

class Tracker
{
 public:
    virtual ~Tracker() {}

    virtual bool init( const cv::Mat& frame, const cv::Rect& initial_position ) = 0;
    virtual bool track( const cv::Mat& frame, cv::Rect& new_position ) = 0;
};

cv::Ptr<Tracker> createTracker(const std::string &impl_name);
