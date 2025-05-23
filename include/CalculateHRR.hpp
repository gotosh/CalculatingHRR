#ifndef CALCULATEHRR_HPP
#define CALCULATEHRR_HPP

#include <iostream>
#include <sstream>
#include "opencv2/opencv.hpp"
#include "Geometry.hpp"

using namespace cv;


class CalculateHRR
{
private:
    int l;
    Mat& img_plif;
    Mat result_HRR;
    // Mat& img_oh;
    Geometry& geometry;
    int _center_aft_crop, _flame_posi_aft_crop;

public:
    CalculateHRR(Mat& image_plif, Geometry& geometry);
    void Product_HRR(CalculateHRR& other_class, std::string saved_path);
    void convert_geometry(const int flame_position, const int horizontal_mm, const int vertical_mm);
    float getMaximum_HRR();
    ~CalculateHRR();
};




#endif