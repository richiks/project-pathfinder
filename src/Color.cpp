/**
 * @brief This file implements code related to colors for the project.
 * @headerfile Color.h
 * @author Richik Vivek Sen
 * @version 2019/04/08
 * @attention Do not modify this file.
 */

#include "Color.h"
#include "error.h"
#include <iomanip>
#include <sstream>

void colorToRGB(Color color, int& r, int& g, int& b) {
    switch (color) {
        case Color::WHITE:   r = 255; g = 255; b = 255; break;
        case Color::YELLOW: r = 255; g = 255; b = 0;   break;
        case Color::GREEN:  r = 0;   g = 255; b = 0;   break;
        default: error("Unknown color?");
    }
}

std::string rgbToColor(int r, int g, int b) {
    std::ostringstream hexValue;
    hexValue << '#' << std::hex << std::setfill('0');
    hexValue << std::setw(2) << r << std::setw(2) << g << std::setw(2) << b;
    return hexValue.str();
}
