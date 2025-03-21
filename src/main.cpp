#include <iostream>
#include <string>
#include <typeinfo>
#include <fstream>

#include "opencv2/opencv.hpp"

#include "ImageProcess.hpp"
#include "ParseJSON.hpp"
#include "CalculateHRR.hpp"
#include "Geometry.hpp"

using namespace cv;

void run_all_task(
    std::string image_path_OH_PLIF,
    std::string image_path_OH_chemilumi,
    std::string scale_path_OH,
    std::string image_path_CH2O_PLIF,
    std::string image_path_CH2O_chemilumi,
    std::string scale_path_CH2O,
    std::string output_img_path
) {
    Geometry geometry_OH;
    Geometry geometry_CH2O;

    ParseJSON scale_OH(scale_path_OH);
    scale_OH.set_geometry(geometry_OH);
    ImageProcess imageprocess_OH(image_path_OH_PLIF, image_path_OH_chemilumi, geometry_OH);
    int flame_position_OH = imageprocess_OH.get_flame_position() - static_cast<int>(1.0 / geometry_OH.scale_calibration) ;
    std::cout << "OH_path: " << image_path_OH_PLIF << std::endl;
    std::cout << "OH flame position [pixel]: " << flame_position_OH << std::endl;
    double flame_position_mm_OH = geometry_OH.scale_calibration * (geometry_OH.burner_inlet_y - flame_position_OH);
    std::cout << "OH flame position [mm]: " << flame_position_mm_OH << std::endl;
    imageprocess_OH.normalized_intensity();
    // imageprocess_OH.cut_threshold_value(0.1);
    Mat& OH_plif_ref = imageprocess_OH.getImage_plif();

    ParseJSON scale_CH2O(scale_path_CH2O);
    scale_CH2O.set_geometry(geometry_CH2O);
    ImageProcess imageprocess_CH2O(image_path_CH2O_PLIF, image_path_CH2O_chemilumi, geometry_CH2O);
    int flame_position_CH2O = imageprocess_CH2O.get_flame_position();
    std::cout << "CH2O_path: " << image_path_CH2O_PLIF << std::endl;
    std::cout << "flame position [pixel]: " << flame_position_CH2O << std::endl;
    double flame_position_mm_CH2O = geometry_CH2O.scale_calibration * (geometry_CH2O.burner_inlet_y - flame_position_CH2O );
    std::cout << "flame position [mm]: " << flame_position_mm_CH2O << std::endl;
    imageprocess_CH2O.normalized_intensity();
    imageprocess_CH2O.cut_threshold_value(0.35);
    Mat& CH2O_plif_ref = imageprocess_CH2O.getImage_plif();

    CalculateHRR calculate_OH(OH_plif_ref, geometry_OH);
    calculate_OH.convert_geometry(flame_position_OH, 10, 5);
    CalculateHRR calculate_CH2O(CH2O_plif_ref, geometry_CH2O);
    calculate_CH2O.convert_geometry(flame_position_CH2O, 10, 5);
    calculate_CH2O.Product_HRR(calculate_OH, output_img_path);

    return;
}


int main()
{
    /* 0kV HRR */
    run_all_task(
        "./image/AVG_0kV_ohplif.tif",
        "./image/AVG_0kv_ohchemi.tif",
        "./scale_oh.json",
        "./image/AVG_0kV_plif_ch2o.tif",
        "./image/AVG_0kV_chemilumi_ch2o.tif",
        "./scale_ch2o.json",
        "HRR_0kV.tif"
    );

    /* +5kV HRR */
    run_all_task(
        "./image/AVG_5kV_plif_oh.tif",
        "./image/AVG_5kV_chemilumi_oh.tif",
        "./scale_oh.json",
        "./image/AVG_5kV_plif_ch2o.tif",
        "./image/AVG_5kV_chemilumi_ch2o.tif",
        "./scale_ch2o.json",
        "HRR_5kV.tif"
    );

    /* -5kV HRR */ 
    run_all_task(
        "./image/AVG_-5kV_plif_oh.tif",
        "./image/AVG_-5kV_chemilumi_oh.tif",
        "./scale_oh.json",
        "./image/AVG_-5kV_plif_ch2o.tif",
        "./image/AVG_-5kV_chemilumi_ch2o.tif",
        "./scale_ch2o.json",
        "HRR_-5kV.tif"
    );

    /* +7.5 kV HRR */
    run_all_task(
        "./image/AVG_7_5kV_plif_oh.tif",
        "./image/AVG_7_5kV_chemilumi_oh.tif",
        "./scale_oh.json",
        "./image/AVG_7_5kV_plif_ch2o.tif",
        "./image/AVG_7_5kV_chemilumi_ch2o.tif",
        "./scale_ch2o.json",
        "HRR_7_5kV.tif"
    );
    /* -7_5kV HRR */
    run_all_task(
        "./image/AVG_-7_5kV_plif_oh.tif",
        "./image/AVG_-7_5kV_chemilumi_oh.tif",
        "./scale_oh.json",
        "./image/AVG_-7_5kV_plif_ch2o.tif",
        "./image/AVG_-7_5kV_chemilumi_ch2o.tif",
        "./scale_ch2o.json",
        "HRR_-7_5kV.tif"
    );
    Geometry geometry;
    ParseJSON s("./scale_oh.json");
    s.set_geometry(geometry);

    ImageProcess imagep("./image/AVG_7_5kV_plif_ch2o.tif", "./image/AVG_7_5kV_chemilumi_ch2o.tif", geometry);
    imagep.cut_threshold_value(0.4);
    int flameposi = imagep.get_flame_position();
    imagep.normalized_intensity();
    Mat& image = imagep.getImage_plif();
    CalculateHRR c(image, geometry);
    c.convert_geometry(flameposi, 10, 5);

    imagep.SaveImgplif("test_file.tif");
    std::cout << geometry.scale_calibration;

    return 0;
}


