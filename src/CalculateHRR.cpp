#include "CalculateHRR.hpp"

CalculateHRR::CalculateHRR(Mat& image_plif, Geometry& geometry) : img_plif(image_plif), geometry(geometry)
{
    // img_plif = imread(fileName_plif, IMREAD_UNCHANGED);
    // img_oh = imread(fileName_oh, IMREAD_UNCHANGED);
}

void CalculateHRR::convert_geometry(const int flame_position, const int horizontal_mm, const int vertical_mm) {
    int horizontal_pixel_num, vertical_pixel_num;
    horizontal_pixel_num = static_cast<int>(horizontal_mm / geometry.scale_calibration);
    vertical_pixel_num = static_cast<int>(vertical_mm / geometry.scale_calibration);
    img_plif = img_plif(
        Range(flame_position - vertical_pixel_num, flame_position + vertical_pixel_num),
        Range(geometry.burner_center_x - horizontal_pixel_num, geometry.burner_center_x + horizontal_pixel_num)
    );
    return;
}

/* Use this function after using convert_geometry for both images, retaining member variables. */
void CalculateHRR::Product_HRR(CalculateHRR& other_class, std::string saved_path) {
    if (other_class.img_plif.empty())
    {
        std::cerr << "Failed to load images from other calss!" << std::endl;
        return; 
    }
    
    
    if (other_class.geometry.scale_calibration > geometry.scale_calibration)
    {
        resize(other_class.img_plif, other_class.img_plif, img_plif.size(), 0, 0, INTER_CUBIC);
    } else {
        resize(img_plif, img_plif, other_class.img_plif.size(), 0, 0, INTER_CUBIC);
    }

    /* Confirm the size of each image */
    if (img_plif.size() != other_class.img_plif.size() || img_plif.type() != other_class.img_plif.type())
    {
        std::cerr << "Images must have the same size and type!" << std::endl;
        return;
    }

    Mat product;
    multiply(img_plif, other_class.img_plif, product);
    cv::normalize(product, product, 0, 65535, cv::NORM_MINMAX, CV_16U);
    imwrite(saved_path, product);
        
}

CalculateHRR::~CalculateHRR()
{
}