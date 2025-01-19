#include "ImageProcess.hpp"

ImageProcess::ImageProcess(std::string fileName_plif, std::string fileName_chemilumi, Geometry& geometry) : geometry(geometry)
{
    img_plif = imread(fileName_plif, IMREAD_UNCHANGED);
    img_chemilumi = imread(fileName_chemilumi, IMREAD_UNCHANGED);


}

/* Get flame position function. Flame position is defined that the position where maximum intensity of chemiluminescence */
int ImageProcess::get_flame_position() {
    int flameposition;
    
    int centerwidth_pixel = static_cast<int>(0.5 / geometry.scale_calibration)  ;
    /* 1 mm width center subtraction. To find flame position, considering strip shape square */
    Mat aroundcenter_img = img_chemilumi.colRange(geometry.burner_center_x - centerwidth_pixel, geometry.burner_center_x + centerwidth_pixel).clone();
    /* Summing up for the direction of row */
    Mat rowSum;
    reduce(aroundcenter_img, rowSum, 1, REDUCE_SUM, CV_32F);
    /* Find max value from aroundcenter_img */
    double maxVal;
    Point maxLoc; // position of max value
    minMaxLoc(rowSum, nullptr, &maxVal, nullptr, &maxLoc);
    flameposition = maxLoc.y;

    return flameposition;
}

ImageProcess::~ImageProcess()
{
}