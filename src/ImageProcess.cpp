#include "ImageProcess.hpp"

ImageProcess::ImageProcess(std::string fileName_plif, std::string fileName_chemilumi, Geometry& geometry) : geometry(geometry)
{
    img_plif = imread(fileName_plif, IMREAD_UNCHANGED);
    img_chemilumi = imread(fileName_chemilumi, IMREAD_UNCHANGED);

    centerwidth_pixel = static_cast<int>(0.5 / geometry.scale_calibration);
    /* 1 mm width center subtraction. To find flame position, considering strip shape square */
    aroundcenter_img_chemilumi = img_chemilumi.colRange(geometry.burner_center_x - centerwidth_pixel, geometry.burner_center_x + centerwidth_pixel).clone();
    aroundcenter_img_plif = img_plif.colRange(geometry.burner_center_x - centerwidth_pixel, geometry.burner_center_x + centerwidth_pixel).clone();

}

int ImageProcess::get_flame_position() { 
    /* Summing up for the direction of row */
    Mat rowSum;
    reduce(aroundcenter_img_chemilumi, rowSum, 1, REDUCE_SUM, CV_32F);
    /* Find max value from aroundcenter_img */
    double maxVal;
    Point maxLoc; // position of max value
    minMaxLoc(rowSum, nullptr, &maxVal, nullptr, &maxLoc);
    flame_position = maxLoc.y;

    return flame_position;
}

double ImageProcess::get_flame_position(bool is_mm) {
    if (is_mm)
    {
        flame_position = get_flame_position();
        double flame_position_mm = geometry.scale_calibration
                                   * (geometry.burner_inlet_y - flame_position);
        return flame_position_mm;
        
    } 
    else
    {
        return 0.0;
    }
    
}

int ImageProcess::set_flame_position_fromOH(double flame_position_mm) {
    flame_position 
    = static_cast<int>(geometry.burner_inlet_y - 
                       flame_position_mm / geometry.scale_calibration);
    return flame_position;
}

void ImageProcess::normalized_intensity(){
    // Subtract offset value
    Mat rowMean;
    double minVal, maxVal;
    Point minLoc, maxLoc;
    // std::cout << "Image type: " << img_plif.type() << std::endl;
    
    reduce(aroundcenter_img_plif, rowMean, 1, REDUCE_AVG, CV_32F); // Average row direction
    minMaxLoc(rowMean, &minVal, &maxVal, &minLoc, &maxLoc);
    img_plif.convertTo(img_plif, CV_32F);
    img_plif = img_plif - static_cast<float>(minVal);
    // Normalize
    float normalizeValue = img_plif.at<float>(maxLoc.y, geometry.burner_center_x);
    if (normalizeValue != 0)
    {
        img_plif = img_plif / normalizeValue;    
    } else {
        std::cerr << "Warning: normalizeValue is zero, skkipping normalization." << std::endl;
    }

    // std::cout << img_plif.col(geometry.burner_center_x) << std::endl;
    
}

Mat& ImageProcess::getImage_plif() {
    return img_plif;
}

void ImageProcess::cut_threshold_value(double thresholdconst) {
    double maxVal, thresholdVal;
    Point maxLoc;
    minMaxLoc(img_plif, nullptr, &maxVal, nullptr, &maxLoc);
    thresholdVal = maxVal * thresholdconst;
    threshold(img_plif, img_plif, thresholdVal, maxVal, 3);
}

double ImageProcess::pixel_to_coordinate(int flame_position) {
    // Recast region is 20 mm * 10 mm
}

void ImageProcess::SaveImgplif(std::string file_path) {
    imwrite(file_path, img_plif);
}


ImageProcess::~ImageProcess()
{
    if (flame_position < 0)
    {
        std::cerr << "Flame position must be positive!" << std::endl;
    }
}