#ifndef CALCULATEHRR_HPP
#define CALCULATEHRR_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include "opencv2/opencv.hpp"
#include "Geometry.hpp"

using namespace cv;


class CalculateHRR
{
private:
    int l;
    Mat result_HRR;
    // Mat& img_oh;
    Geometry& geometry;
    Point maxLoc_HRR;
    float maxVal_HRR;

public:
    CalculateHRR(Mat& image_plif, Geometry& geometry);
    void Product_HRR(CalculateHRR& other_class, std::string saved_path);
    void convert_geometry(const int flame_position, const int horizontal_mm, const int vertical_mm);
    float getMaximum_HRR();
    void setScaleCalib_HRR();
    void image_crop(double half_crop_width_mm);
    void reduce_cropped_image();
    double get_e2width();
    void saveCenterdistrib(std::string save_fileName);
    ~CalculateHRR();

protected:
    Mat& img_plif;
    int _center_aft_crop = 0, _flame_posi_aft_crop = 0;
    int _horizontal_mm = 0, _vertical_mm = 0;
    float scale_calib_HRRim = 0.;
};




#endif