#include "CalculateHRR.hpp"

CalculateHRR::CalculateHRR(Mat& image_plif, Geometry& geometry) : img_plif(image_plif), geometry(geometry)
{
    // img_plif = imread(fileName_plif, IMREAD_UNCHANGED);
    // img_oh = imread(fileName_oh, IMREAD_UNCHANGED);
}

void CalculateHRR::convert_geometry(const int flame_position, const int horizontal_mm, const int vertical_mm) {
    int horizontal_pixel_num, vertical_pixel_num;
    _horizontal_mm = horizontal_mm;
    _vertical_mm = vertical_mm;
    horizontal_pixel_num = static_cast<int>(horizontal_mm / geometry.scale_calibration);
    vertical_pixel_num = static_cast<int>(vertical_mm / geometry.scale_calibration);
    img_plif = img_plif(
        Range(flame_position - vertical_pixel_num, flame_position + vertical_pixel_num),
        Range(geometry.burner_center_x - horizontal_pixel_num, geometry.burner_center_x + horizontal_pixel_num)
    );
    _center_aft_crop = horizontal_pixel_num;
    _flame_posi_aft_crop = vertical_pixel_num;

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
        scale_calib_HRRim = geometry.scale_calibration;
    } else {
        resize(img_plif, img_plif, other_class.img_plif.size(), 0, 0, INTER_CUBIC);
        scale_calib_HRRim = other_class.geometry.scale_calibration;
    }

    /* Confirm the size of each image */
    if (img_plif.size() != other_class.img_plif.size() || img_plif.type() != other_class.img_plif.type())
    {
        std::cerr << "Images must have the same size and type!" << std::endl;
        return;
    }

    Mat product, output;
    multiply(img_plif, other_class.img_plif, product);
    result_HRR = product.clone();
    cv::normalize(product, output, 0, 65535, cv::NORM_MINMAX, CV_16U);
    imwrite(saved_path, output);
        
}

void CalculateHRR::setScaleCalib_HRR() {
        scale_calib_HRRim = (
        static_cast<double>(_horizontal_mm)
            / static_cast<double>(_center_aft_crop)
        + static_cast<double>(_vertical_mm)
            / static_cast<double>(_flame_posi_aft_crop)
    ) / 2;
}

void CalculateHRR::image_crop(double half_crop_width_mm) {
    int half_crop_width_px 
        = static_cast<int>(half_crop_width_mm / scale_calib_HRRim);
    result_HRR = result_HRR.colRange(
        _center_aft_crop - half_crop_width_px,
        _center_aft_crop + half_crop_width_px 
    );
}

void CalculateHRR::reduce_cropped_image() {
    reduce(result_HRR, result_HRR, 1, REDUCE_AVG, CV_32F);
}


float CalculateHRR::getMaximum_HRR() {
    double maxVal;  // minMaxLoc cannot accept float value
    Point maxLoc;
    minMaxLoc(result_HRR, nullptr, &maxVal, nullptr, &maxLoc);
    maxLoc_HRR = maxLoc;
    maxVal_HRR = static_cast<float>(maxVal);
    return maxVal;
}


double CalculateHRR::get_e2width() {
    int max_index = maxLoc_HRR.y;
    int length = result_HRR.total();
    std::vector<int> indices;
    Mat result_HRR_maxright = result_HRR.rowRange(max_index, length);
    for (int i = 0; i < length - max_index; i++)
    {
        if (result_HRR_maxright.at<float>(i) 
                > maxVal_HRR / (exp(1.0) * (exp(1.0))))
        {
            indices.push_back(i);
        }
        
    }
    int length_fwhmpx = indices.size();
    float fwhm_mm = length_fwhmpx * 2 * scale_calib_HRRim;
    return static_cast<double>(fwhm_mm);

}

void CalculateHRR::saveCenterdistrib(std::string save_fileName) {
    if (result_HRR.cols != 1)
    {
        std::cerr << "Please use reduce_cropped_image() function first."
                        "For reducing image to One dimensional" << std::endl;
    }
    if (scale_calib_HRRim == 0.0)
    {
        std::cerr << "Please use setScaleCalib_HRR function"
                     " to set mm/px in image"
                  << std::endl;
    }
    
    std::vector<float> center_axis;
    for (size_t i = 0; i < result_HRR.rows; i++)
    {
        float y = scale_calib_HRRim * i;
        center_axis.push_back(y);
    }
    
    std::ofstream file(save_fileName);
    if (!file)
    {
        std::cerr << "Error: Unable to open file for writing" << std::endl;
    }
    
    for (size_t i = 0; i < result_HRR.rows; i++)
    {
        file << center_axis.at(i) << "," << result_HRR.at<float>(i)
             << std::endl;
    }
    
    
    
}

CalculateHRR::~CalculateHRR()
{
}