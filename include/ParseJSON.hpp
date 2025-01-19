#ifndef PARSEJSON_HPP
#define PARSEJSON_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Geometry.hpp"

class ParseJSON
{
private:
    int _center;
    int _left;
    int _right;
    int _5mm;
    int _10mm;

    
public:
    ParseJSON(std::string fileName_json);
    void SetGeometry(Geometry& geometry);
    ~ParseJSON();
};



#endif