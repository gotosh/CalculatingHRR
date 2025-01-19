#include "ImageProcess.hpp"

ImageProcess::ImageProcess(std::string fileName_plif, std::string fileName_chemilumi, std::string fineName_geometry)
{
    Mat img_plif = imread(fileName_plif, IMREAD_COLOR);
    Mat img_chemilumi = imread(fileName_chemilumi, IMREAD_COLOR);

    
}

ImageProcess::~ImageProcess()
{
}