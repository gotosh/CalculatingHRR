#ifndef CALCULATEHRR_HPP
#define CALCULATEHRR_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <typeinfo>
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
    std::vector<float> result_1D_vec;

public:
    /**
     * @brief Calculate Heat Release Rate by using PLIF image
     * This class cannot work propery itself. Referencing the other
     * CalculateHRR class is necessary.
     * 
     * @param   image_plif PLIF image which is read by OpenCV
     * @param   geometry Geometry of PLIF image
     */
    CalculateHRR(Mat& image_plif, Geometry& geometry);

    /**
     * @brief Product images of OH-PLIF and CH2O-PLIF for calculating HRR
     * 
     * @param   other_class Another CalculateHRR class
     * @param   saved_path Save path of result image
     */
    void Product_HRR(CalculateHRR& other_class, std::string saved_path);

    /**
     * @brief Cropping 2*horizontal_mm times 2*vertical_mm mm image
     * by referencing to flame position and update some geometry data
     * 
     * @param   flame_position Position of flame [px(int)]
     * @param   horizotal_mm Cropping region of half horizontal [mm]
     * @param   vertical_mm Cropping region of half vertical [mm]
     */
    void convert_geometry(const int flame_position,
            const int horizontal_mm, const int vertical_mm);
    
    /**
     * @brief Get maximum intensity of HRR image on center axis
     * 
     * @return  Max value of distribution
     */
    float getMaximum_HRR();

    void image_crop(double half_crop_width_mm);
    void reduce_cropped_image();

    /**
     * @brief Interpolate 1d result array 
     * @param   mag magnification of interpolate point
     * 
     */
    void liner_interp_distribution(int mag);

    double get_e2width();
    double get_e2width(bool interpolation);
    void saveCenterdistrib(std::string save_fileName,
                           double flame_position_mm = 0.0);
    ~CalculateHRR();

protected:
    Mat& img_plif;
    int _center_aft_crop = 0, _flame_posi_aft_crop = 0;
    int _horizontal_mm = 0, _vertical_mm = 0;
    float scale_calib_HRRim = 0.;
};




#endif