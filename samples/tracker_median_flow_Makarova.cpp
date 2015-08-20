#include <tracker.hpp>
#include <iostream>
#include <fstream>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

class TrackerMakarova : public Tracker
{
public:
   virtual ~TrackerMakarova() {}

   virtual bool init( const cv::Mat& frame, const cv::Rect& initial_position );
   virtual bool track( const cv::Mat& frame, cv::Rect& new_position );

private:
   cv::Mat prev_copy;
   cv::Rect position_;
};

bool TrackerMakarova::init( const cv::Mat& frame, const cv::Rect& initial_position )
{
   position_ = initial_position;
   return true;
}

bool TrackerMakarova::track( const cv::Mat& frame, cv::Rect& new_position )
{
  
 new_position = position_;
  std::vector<cv::Point2f> corners, corners_2;
  vector<uchar> status;
  vector<float> err;
  Mat copy;
  cvtColor(frame, copy, COLOR_BGR2GRAY);
  Mat roi(copy, position_);
  
  goodFeaturesToTrack(roi, corners, 50, 0.01, 10);
  

  for(int i = 0; i< corners.size()-1; i++){
      corners[i].x += position_.x;
      corners[i].y += position_.y;
  }

  if(prev_copy.empty()){
      copy.copyTo(prev_copy);
  }
  calcOpticalFlowPyrLK(prev_copy, copy, corners, corners_2, status, err);

  for(size_t i = 0; i<status.size()-1; i++)
  {
      if(status[i] != 0)
      {
          for(size_t j = i; j < err.size() - 1 - i; j++){
          if(err[i] < err[j]){
              swap(err[i], err[j]);
              swap(corners_2[i], corners_2[j]);
              swap(corners[i], corners[j]);
          }
          }
      }
  }
         
  while(corners_2.size()>corners_2.size()/2){
      corners.pop_back();
      corners_2.pop_back();
      err.pop_back();
      status.pop_back();
  }

 

  return true;
   
 
}

cv::Ptr<Tracker> createTrackerMakarova()
{
   return cv::Ptr<Tracker>(new TrackerMakarova());
}