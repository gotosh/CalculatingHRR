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
    Mat img_plif;
    Mat img_oh;
    Geometry geometry;
public:
    CalculateHRR(Mat& image_plif, Geometry& geometry);
    void Product_HRR(CalculateHRR& other_class, std::string saved_path);
    void convert_geometry(const int flame_position, const int horizontal_mm, const int vertical_mm);
    ~CalculateHRR();
};




#endif