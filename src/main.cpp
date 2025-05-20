#include <iostream>
#include <string>
#include <typeinfo>
#include <fstream>
#include <utility>

#include "opencv2/opencv.hpp"

#include "ImageProcess.hpp"
#include "ParseJSON.hpp"
#include "CalculateHRR.hpp"
#include "Geometry.hpp"

using namespace cv;

std::pair<float, float> run_all_task(
    std::string image_path_OH_PLIF,
    std::string image_path_OH_chemilumi,
    std::string scale_path_OH,
    std::string image_path_CH2O_PLIF,
    std::string image_path_CH2O_chemilumi,
    std::string scale_path_CH2O,
    std::string output_img_path,
    int normalizeValue_OH = 0,
    int normalizeValue_CH2O = 0
) {
    Geometry geometry_OH;
    Geometry geometry_CH2O;

    ParseJSON scale_OH(scale_path_OH);
    scale_OH.set_geometry(geometry_OH);
    ImageProcess imageprocess_OH(image_path_OH_PLIF, image_path_OH_chemilumi, geometry_OH);
    int flame_position_OH = imageprocess_OH.get_flame_position() - static_cast<int>(1.5 / geometry_OH.scale_calibration);
    std::cout << "OH_path: " << image_path_OH_PLIF << std::endl;
    std::cout << "OH flame position [pixel]: " << flame_position_OH << std::endl;
    double flame_position_mm_OH = geometry_OH.scale_calibration * (geometry_OH.burner_inlet_y - flame_position_OH);
    std::cout << "OH flame position [mm]: " << flame_position_mm_OH << std::endl;
    imageprocess_OH.normalized_intensity(normalizeValue_OH);
    float OH_maxVal = imageprocess_OH.get_maxVal_onCenter();
    imageprocess_OH.cut_threshold_value(0.1);
    Mat& OH_plif_ref = imageprocess_OH.getImage_plif();

    ParseJSON scale_CH2O(scale_path_CH2O);
    scale_CH2O.set_geometry(geometry_CH2O);
    ImageProcess imageprocess_CH2O(image_path_CH2O_PLIF, image_path_CH2O_chemilumi, geometry_CH2O);
    double flame_position_CH2O_mm = imageprocess_CH2O.get_flame_position(true);
    int flame_position_CH2O = imageprocess_CH2O.set_flame_position_fromOH(flame_position_CH2O_mm);
    std::cout << "CH2O_path: " << image_path_CH2O_PLIF << std::endl;
    std::cout << "flame position [pixel]: " << flame_position_CH2O << std::endl;
    std::cout << "flame position [mm]: " << flame_position_CH2O_mm << std::endl;
    imageprocess_CH2O.normalized_intensity(normalizeValue_CH2O);
    float CH2O_maxVal = imageprocess_CH2O.get_maxVal_onCenter();
    // imageprocess_CH2O.cut_threshold_value(0.20);
    imageprocess_CH2O.SaveImgplif("test.tif");
    Mat& CH2O_plif_ref = imageprocess_CH2O.getImage_plif();

    CalculateHRR calculate_OH(OH_plif_ref, geometry_OH);
    calculate_OH.convert_geometry(flame_position_OH, 10, 5);
    CalculateHRR calculate_CH2O(CH2O_plif_ref, geometry_CH2O);
    calculate_CH2O.convert_geometry(flame_position_CH2O, 10, 5);
    calculate_CH2O.Product_HRR(calculate_OH, output_img_path);

    return std::make_pair(OH_maxVal, CH2O_maxVal);
}


int main()
{
    /* 0kV HRR 1*/
    auto [OH_max_1, CH2O_max_1] = run_all_task(
        "./image/oh_75Calib/0kV_plif_1_oh.tif",
        "./image/oh_chem/0kV_chem_1_oh.tif",
        "./scale_oh.json",
        "./image/ch2o_plif/AVG_0kv_plif_1.tif",
        "./image/ch2o_chem/AVG_0kV_chem.tif",
        "./scale_ch2o.json",
        "HRR_0kV_1.tif"
    );

    /* 0kV HRR 2*/
    auto [OH_max_2, CH2O_max_2] = run_all_task(
        "./image/oh_75Calib/0kV_plif_2_oh.tif",
        "./image/oh_chem/0kV_chem_2_oh.tif",
        "./scale_oh.json",
        "./image/ch2o_plif/AVG_0kv_plif_2.tif",
        "./image/ch2o_chem/AVG_0kV_chem.tif",
        "./scale_ch2o.json",
        "HRR_0kV_2.tif"
    );

    /* +5kV HRR 1*/
    run_all_task(
        "./image/oh_75Calib/5kV_plif_1_oh.tif",
        "./image/oh_chem/5kV_chem_1_oh.tif",
        "./scale_oh.json",
        "./image/ch2o_plif/AVG_5kv_plif_1.tif",
        "./image/ch2o_chem/AVG_5kV_chem.tif",
        "./scale_ch2o.json",
        "HRR_5kV_1.tif",
        OH_max_1,
        CH2O_max_1
    );

    /* +5kV HRR 2*/
    run_all_task(
        "./image/oh_75Calib/5kV_plif_2_oh.tif",
        "./image/oh_chem/5kV_chem_2_oh.tif",
        "./scale_oh.json",
        "./image/ch2o_plif/AVG_5kv_plif_2.tif",
        "./image/ch2o_chem/AVG_5kV_chem.tif",
        "./scale_ch2o.json",
        "HRR_5kV_2.tif",
        OH_max_2,
        CH2O_max_2
    );

    /* -5kV HRR 1*/ 
    run_all_task(
        "./image/oh_75Calib/-5kV_plif_1_oh.tif",
        "./image/oh_chem/-5kV_chem_1_oh.tif",
        "./scale_oh.json",
        "./image/ch2o_plif/AVG_-5kv_plif_1.tif",
        "./image/ch2o_chem/AVG_-5kV_chem.tif",
        "./scale_ch2o.json",
        "HRR_-5kV_1.tif",
        OH_max_1,
        CH2O_max_1
    );

    // /* -5kV HRR 2*/ 
    run_all_task(
        "./image/oh_75Calib/-5kV_plif_2_oh.tif",
        "./image/oh_chem/-5kV_chem_2_oh.tif",
        "./scale_oh.json",
        "./image/ch2o_plif/AVG_-5kv_plif_2.tif",
        "./image/ch2o_chem/AVG_-5kV_chem.tif",
        "./scale_ch2o.json",
        "HRR_-5kV_2.tif",
        OH_max_2,
        CH2O_max_2
    );

    /* +7.5 kV HRR 1*/
    run_all_task(
        "./image/oh_75Calib/7_5kV_plif_1_oh.tif",
        "./image/oh_chem/7_5kV_chem_1_oh.tif",
        "./scale_oh.json",
        "./image/ch2o_plif/AVG_7_5kv_plif_1.tif",
        "./image/ch2o_chem/AVG_7_5kV_chem.tif",
        "./scale_ch2o.json",
        "HRR_7_5kV_1.tif",
        OH_max_1,
        CH2O_max_1
    );

    // /* +7.5 kV HRR 2*/
    run_all_task(
        "./image/oh_75Calib/7_5kV_plif_2_oh.tif",
        "./image/oh_chem/7_5kV_chem_2_oh.tif",
        "./scale_oh.json",
        "./image/ch2o_plif/AVG_7_5kv_plif_2.tif",
        "./image/ch2o_chem/AVG_7_5kV_chem.tif",
        "./scale_ch2o.json",
        "HRR_7_5kV_2.tif",
        OH_max_2,
        CH2O_max_2
    );

    /* -7_5kV HRR 1*/
    run_all_task(
        "./image/oh_75Calib/-7_5kV_plif_1_oh.tif",
        "./image/oh_chem/-7_5kV_chem_1_oh.tif",
        "./scale_oh.json",
        "./image/ch2o_plif/AVG_-7_5kv_plif_1.tif",
        "./image/ch2o_chem/AVG_-7_5kV_chem.tif",
        "./scale_ch2o.json",
        "HRR_-7_5kV_1.tif",
        OH_max_1,
        CH2O_max_1
    );

    // /* -7_5kV HRR 2*/
    run_all_task(
        "./image/oh_75Calib/-7_5kV_plif_2_oh.tif",
        "./image/oh_chem/-7_5kV_chem_2_oh.tif",
        "./scale_oh.json",
        "./image/ch2o_plif/AVG_-7_5kv_plif_2.tif",
        "./image/ch2o_chem/AVG_-7_5kV_chem.tif",
        "./scale_ch2o.json",
        "HRR_-7_5kV_2.tif",
        OH_max_2,
        CH2O_max_2
    );

    // Geometry geometry;
    // ParseJSON s("./scale_oh.json");
    // s.set_geometry(geometry);

    // ImageProcess imagep("./image/AVG_7_5kV_plif_ch2o.tif", "./image/AVG_7_5kV_chemilumi_ch2o.tif", geometry);
    // imagep.cut_threshold_value(0.4);
    // int flameposi = imagep.get_flame_position();
    // imagep.normalized_intensity();
    // Mat& image = imagep.getImage_plif();
    // CalculateHRR c(image, geometry);
    // c.convert_geometry(flameposi, 10, 5);

    // imagep.SaveImgplif("test_file.tif");
    // std::cout << geometry.scale_calibration;

    return 0;
}


