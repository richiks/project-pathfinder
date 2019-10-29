/**
 * @brief This file implements code related to colors for the project.
 * @class Color.cpp
 * @author Richik Vivek Sen
 * @version 2019/04/08
 * @attention Do not modify this file.
 */

#ifndef _color_h
#define _color_h

#include <string>

/*
 * An enumerated type representing a color for a node during an execution of
 * graph search algorithms.
 */
enum class Color {
    WHITE, YELLOW, GREEN
};

/*
 * Converts an integer color like 0xffdd88 into three separate red/green/blue
 * components from 0-255.
 */
void colorToRGB(Color color, int& r, int& g, int& b);

/*
 * Converts three RGB values from 0-255 into a hex color string like "#cccccc".
 */
std::string rgbToColor(int r, int g, int b);

#endif // _color_h
