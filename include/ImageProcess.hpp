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
public:
    ImageProcess(std::string fileName_plif, std::string fileName_chemilumi, std::string fineName_geometry);
    ~ImageProcess();
};


#endif