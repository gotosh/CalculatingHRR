#ifndef IMAGEPROCESS_HPP
#define IMAGEPROCESS_HPP

#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"
#include "Geometry.hpp"

using namespace cv;



class ImageProcess
{
private:
    /* data */
    Mat img_plif;
    Mat img_chemilumi;
    Geometry& geometry;
    int centerwidth_pixel;
    Mat aroundcenter_img_chemilumi;
    Mat aroundcenter_img_plif;


public:
    ImageProcess(std::string fileName_plif, std::string fileName_chemilumi, Geometry& geometry);
    int get_flame_position();
    void normalized_intensity();
    void pixel_to_coordinate(int flame_position);
    Mat& getImage_plif();
    ~ImageProcess();
};


#endif