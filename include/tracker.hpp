#include <opencv2/core/core.hpp>

class Tracker
{
 public:

  // virtual ~Tracker();

  /** @brief Initialize the tracker with a know bounding box that surrounding the target
    @param image The initial frame
    @param boundingBox The initial boundig box

    @return True if initialization went succesfully, false otherwise
     */
  bool init( const cv::Mat& image, const cv::Rect& boundingBox );

  /** @brief Update the tracker, find the new most likely bounding box for the target
    @param image The current frame
    @param boundingBox The boundig box that represent the new target location, if true was returned, not
    modified otherwise

    @return True means that target was located and false means that tracker cannot locate target in
    current frame. Note, that latter *does not* imply that tracker has failed, maybe target is indeed
    missing from the frame (say, out of sight)
     */
  bool update( const cv::Mat& image, cv::Rect& boundingBox );

  /** @brief Creates a tracker by its name.
    @param trackerType Tracker type

    The following detector types are supported:

    -   "MIL" -- TrackerMIL
    -   "BOOSTING" -- TrackerBoosting
     */

 protected:

  // virtual bool initImpl( const Mat& image, const Rect2d& boundingBox ) = 0;
  // virtual bool updateImpl( const Mat& image, Rect2d& boundingBox ) = 0;

  // bool isInit;

  // Ptr<TrackerFeatureSet> featureSet;
  // Ptr<TrackerSampler> sampler;
  // Ptr<TrackerModel> model;
};

cv::Ptr<Tracker> create( const std::string& trackerType );
