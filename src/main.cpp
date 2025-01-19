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
    Mat img = imread(image_path, IMREAD_COLOR);
    std::cout << typeid(img).name() << std::endl;

    imshow("Display window", img);
    int k = waitKey(1000); // Wait for a keystroke in the window
    destroyAllWindows(); 
    Geometry geometry_OH;
    ParseJSON scale_OH("./scale_oh.json");
    scale_OH.SetGeometry(geometry_OH);
    std::cout << geometry_OH.burner_center_x << std::endl;
    std::cout << geometry_OH.scale_calibration << std::endl;
    std::cout << geometry_OH.fiveORten_mm << std::endl;
    std::cout << geometry_OH.left << std::endl;
    return 0;
}


