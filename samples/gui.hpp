#pragma once

#include <sstream>
#include <opencv2/highgui/highgui.hpp>

class GUI
{
private:
    std::string window_name_;

    cv::Rect bounding_box_;
    cv::Point p1_, p2_;
    cv::Mat  init_image_;
    cv::Mat  display_image_;
    bool start_selection_;
    bool object_selected_;

public:
    GUI(const std::string &window_name = "Tracking Sample")
        : window_name_(window_name)
    {
        cv::namedWindow(window_name_);
        cv::setMouseCallback(window_name_, onMouse, this);
    }

    ~GUI()
    {
        cv::destroyWindow(window_name_);
    }

    cv::Rect initBoundingBox(std::string init_string,
                             const cv::Mat &init_image)
    {
        // Parse init_string
        std::replace(init_string.begin(), init_string.end(), ',', ' ');
        std::istringstream init_stream(init_string);
        std::vector<int> coords(4, 0);
        bool valid = true;
        for (size_t i = 0; i < coords.size(); i++)
            init_stream >> coords[i];

        // Validate coordinates
        if (coords[0] <= 0 || coords[1] <= 0 || coords[2] <= 0 || coords[3] <= 0 ||
            coords[0] >= init_image.cols || coords[2] >= init_image.cols ||
            coords[1] >= init_image.rows || coords[3] >= init_image.rows)
            valid = false;

        if (valid)
        {
            bounding_box_ = cv::Rect(cv::Point(coords[0], coords[1]),
                                     cv::Point(coords[2], coords[3]));
            object_selected_ = true;
        }
        else
        {
            // ask user to draw the bounding box if init_string doesn't provide
            // a valid one

            bounding_box_ = cv::Rect();

            init_image.copyTo(init_image_);
            init_image.copyTo(display_image_);

            start_selection_ = false;
            object_selected_ = false;

            while(!object_selected_ &&
                  displayImage(init_image_, bounding_box_))
                continue;
        }

        return bounding_box_;
    }

    bool displayImage(const cv::Mat &image, const cv::Rect &rect)
    {
        image.copyTo(display_image_);
        cv::rectangle(display_image_, rect, cv::Scalar( 255, 0, 0 ), 2, 1);
        cv::imshow(window_name_, display_image_);
        char c = cv::waitKey(30) & 0xFF;
        if (c == 27) // ESC
            return false;

        return true;
    }

    static void onMouse( int event, int x, int y, int, void *s)
    {
        GUI *self = reinterpret_cast<GUI *>(s);

        if( !self->object_selected_ )
        {
            switch ( event )
            {
            case cv::EVENT_LBUTTONDOWN:
                self->start_selection_ = true;
                self->p1_ = cv::Point(x, y);
                break;
            case cv::EVENT_LBUTTONUP:
                self->object_selected_ = true;
                self->start_selection_ = false;
                break;
            case cv::EVENT_MOUSEMOVE:
                if( self->start_selection_ && !self->object_selected_ )
                {
                    self->p2_ = cv::Point(x, y);
                    self->bounding_box_ = cv::Rect(self->p1_, self->p2_);
                }
                break;
            }
        }
    }
};
