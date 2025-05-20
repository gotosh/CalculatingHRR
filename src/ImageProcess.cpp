#include "ImageProcess.hpp"

ImageProcess::ImageProcess(const std::string fileName_plif,
    const std::string fileName_chemilumi, Geometry& geometry) : geometry(geometry)
{
    img_plif = imread(fileName_plif, IMREAD_UNCHANGED);
    img_chemilumi = imread(fileName_chemilumi, IMREAD_UNCHANGED);

    centerwidth_pixel = static_cast<int>(0.5 / geometry.scale_calibration);
    /* 1 mm width center subtraction. To find flame position, considering strip shape square */
    aroundcenter_img_chemilumi = img_chemilumi.colRange(geometry.burner_center_x - centerwidth_pixel, geometry.burner_center_x + centerwidth_pixel).clone();
    aroundcenter_img_plif = img_plif.colRange(geometry.burner_center_x - centerwidth_pixel, geometry.burner_center_x + centerwidth_pixel).clone();

}

ImageProcess::ImageProcess(const std::string fileName_plif,
    const std::string fileName_chemilumi,
    Geometry& geometry, std::unique_ptr<Geometry> geometry_chemilumi)
    : geometry(geometry), geometry_chemilumi(std::move(geometry_chemilumi))
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

double ImageProcess::get_flame_position_otherref(bool is_mm) {
    if (is_mm)
    {
        flame_position = get_flame_position();
        double flame_position_mm = geometry_chemilumi->scale_calibration
                                   * (geometry_chemilumi->burner_inlet_y - flame_position);
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

void ImageProcess::normalized_intensity(int normalizeValue){
    // Subtract offset value
    Mat rowMean;
    double minVal, maxVal;
    Point minLoc, maxLoc;
    // std::cout << "Image type: " << img_plif.type() << std::endl;
    
    reduce(aroundcenter_img_plif, rowMean, 1, REDUCE_AVG, CV_32F); // Average row direction
    minMaxLoc(rowMean, &minVal, &maxVal, &minLoc, &maxLoc);
    maxVal_onCenter = maxVal;
    img_plif.convertTo(img_plif, CV_32F);
    img_plif = img_plif - static_cast<float>(minVal);
    // Normalize
    if (normalizeValue == 0) {
        maxVal = static_cast<float>(maxVal);
        img_plif = img_plif / maxVal;
    }
    else if (normalizeValue == 1) {
        normalizeValue = static_cast<float>(normalizeValue);
        img_plif = img_plif / normalizeValue;
    } else {
        normalizeValue = static_cast<float>(normalizeValue);
        img_plif = img_plif / normalizeValue;
    }

    // std::cout << img_plif.col(geometry.burner_center_x) << std::endl;
    
}

float ImageProcess::get_maxVal_onCenter() {
    if (maxVal_onCenter < 0)
    {
        std::cerr << "Max value is not calculated!" << std::endl;
        return -1.0;
    }
    return maxVal_onCenter;
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
    return static_cast<double>(flame_position) * geometry.scale_calibration;
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