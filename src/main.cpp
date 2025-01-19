#include <iostream>
#include <string>
#include <typeinfo>
#include <fstream>

#include "opencv2/opencv.hpp"

#include "ImageProcess.hpp"
#include "ParseJSON.hpp"
#include "Geometry.hpp"

using namespace cv;



int main()
{
    /* test */
    std::string image_path = "./image/AVG_0kV_plif_1.tif";
    Mat img = imread(image_path, IMREAD_UNCHANGED);
    switch (img.depth())
    {
    case CV_8U:
        std::cout << "The image is 8 bit" << std::endl;
        break;
    case CV_16U:
        std::cout << "The image is 16 bit" << std::endl;
    
    default:
        break;
    }
    std::cout << img.row(400) << std::endl;

    imshow("Display window", img);
    int k = waitKey(1000); // Wait for a keystroke in the window
    destroyAllWindows(); 
    Geometry geometry_OH;
    Geometry geometry_CH2O;
    ParseJSON scale_OH("./scale_ch2o.json");
    scale_OH.set_geometry(geometry_OH);

    ImageProcess imageprocess(image_path, "./image/AVG_chemilumi_0kV.tif", geometry_OH);
    std::cout << imageprocess.get_flame_position() << std::endl;
    return 0;
}


