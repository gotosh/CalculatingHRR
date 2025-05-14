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

    // Geometry of the picture that is from scale picture
    Geometry& geometry;
    // Analytical region of this analysis. It is 1 mm width.
    int centerwidth_pixel;

    Mat aroundcenter_img_chemilumi;
    Mat aroundcenter_img_plif;

    int flame_position = -1;


public:
    /**
     * @brief Construct a new Image Process object
     * 
     * @param fileName_plif PLIF image file name
     * @param fileName_chemilumi Chemiluminescence image file name
     * @param geometry Geometry object
     * 
     * @note This constructor crops the image around the burner center.
     */
    ImageProcess(std::string fileName_plif, std::string fileName_chemilumi, Geometry& geometry);


    /**
     * @brief Get the flame position from OH chemiluminescence image.
     * This function aim to get the flame position 
     * from OH chemiluminescence image.
     * The flame position is defined that the position where
     * maximum intensity of chemiluminescence.
     * @return flame position [pixel]
     */
    int get_flame_position();

    /**
     * @brief Get the flame position from OH chemiluminescence image.
     * and return the flame position from burner exit [mm].
     * @param is_mm if true, return the flame position in mm unit.
     * @return flame position [mm]
     */
    double get_flame_position(bool is_mm);

    /**
     * @brief Receive the flame position in mm unit and 
     * convert to pixel unit.
     */
    int set_flame_position_fromOH(double flame_position_mm);
    



    /**
     * @brief Normalize intensity of PLIF image.
     * This function aim to normalize intensity of PLIF image.
     * The normalization is done by subtracting the offset value and
     * dividing by the maximum value.
     */
    void normalized_intensity();

    /**
     * @brief Convert pixel to coordinate.
     * This function aim to flame position pixel to coordinate.
     * @param flame_position flame position [pixel]
     * @return flame position [mm]
     */
    double pixel_to_coordinate(int flame_position);

    Mat& getImage_plif();
    void cut_threshold_value(double thresholdconst);
    void SaveImgplif(std::string file_path);
    ~ImageProcess();
};


#endif