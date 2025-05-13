#ifndef PARSEJSON_HPP
#define PARSEJSON_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Geometry.hpp"

/**
 * @brief For parsing JSON file that contains geometry information.
 */
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
    void set_geometry(Geometry& geometry);
    ~ParseJSON();
};



#endif