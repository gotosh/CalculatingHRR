#include "ParseJSON.hpp"


ParseJSON::ParseJSON(std::string fileName_json)
{
    std::ifstream file(fileName_json);
    if (!file.is_open())
    {
        std::cerr << "Failed to open the file." << std::endl;
    }

    // Read entire file
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonContent = buffer.str();

    // Parse json data by manual
    size_t geometryPos = jsonContent.find("\"geometry\":");
    if (geometryPos == std::string::npos)
    {
        std::cerr << "geometry not found!" << std::endl;
    }

    // Extract "geometry" block 
    size_t geoStart = jsonContent.find("{", geometryPos);
    size_t geoEnd = jsonContent.find("}", geoStart);
    std::string geometryData = jsonContent.substr(geoStart + 1, geoEnd - geoStart - 1);
    
    // Extract some keys (center, left, right, 5mm, ten_mm)
    size_t centerPos = geometryData.find("\"center\":");
    if (centerPos != std::string::npos)
    {
        _center = std::stoi(geometryData.substr(centerPos + 9, geometryData.find(",", centerPos) - centerPos - 9));
    }

    size_t leftPos = geometryData.find("\"left\":");
    if (leftPos != std::string::npos) {
        _left = std::stoi(geometryData.substr(leftPos + 7, geometryData.find(",", leftPos) - leftPos - 7));
    }

    size_t rightPos = geometryData.find("\"right\":");
    if (rightPos != std::string::npos) {
        _right = std::stoi(geometryData.substr(rightPos + 8, geometryData.find(",", rightPos) - rightPos - 8));
    }

    size_t mm5Pos = geometryData.find("\"5mm\":");
    if (mm5Pos != std::string::npos) {
        _5mm = std::stoi(geometryData.substr(mm5Pos + 6, geometryData.find(",", mm5Pos) - mm5Pos - 6));
        _10mm = -1;
    }

    size_t mm10Pos = geometryData.find("\"10mm\":");
    if (mm10Pos != std::string::npos)
    {
        _10mm = std::stoi(geometryData.substr(mm10Pos + 7, geometryData.find(",", mm10Pos) - mm10Pos - 7));
        _5mm = -1;
    }
        
}

void ParseJSON::set_geometry(Geometry& geometry){
    geometry.burner_center_x = _center;
    geometry.left = _left;
    geometry.right = _right;
    if (_right != _left)
    {
        geometry.scale_calibration = 20.0 / (_right - _left);
    } else {
        std::cerr << "Error: left and right is the same value" << std::endl;
        geometry.scale_calibration = 0.0;
    }
    
    if (_10mm == -1)
    {
        geometry.fiveORten_mm = _5mm;
        geometry.burner_inlet_y = static_cast<int>(_5mm + 5 * geometry.scale_calibration);
    } else if (_5mm == -1)
    {
        geometry.fiveORten_mm = _10mm;
        geometry.burner_inlet_y = static_cast<int>(_10mm + 10 * geometry.scale_calibration);
    } else {
        std::cerr << "Something wrong has happened when Parse JSON file! " << std::endl;
    }

}

ParseJSON::~ParseJSON()
{
}