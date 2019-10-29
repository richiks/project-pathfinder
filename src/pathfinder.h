/**
 * @brief This file declares the graph search algorithms for the project.
 * @class pathfinder.cpp
 * @author Richik Vivek Sen
 * @version 2019/04/08
 * @attention All changes to be made in new copies of this file. Comment "TODO",
 * "EDITED", and "FIXME" wherever necessary.
 */

#ifndef _pathfinder_h
#define _pathfinder_h

#include "vector.h"
#include "RoadGraph.h"
#include <unordered_map>

/**
 * Type: Path
 *
 * A type representing a path. This type literally is a Vector<Vertex*>, but we've given
 * it the convenient name "Path" for simplicity.
 */
using Path = Vector<RoadNode*>;

Path a_star(const RoadGraph& graph, RoadNode* source, RoadNode* target);
Path periphery_sweep(const RoadGraph& graph, RoadNode* source, RoadNode* target);
Path memory_optimized_ida_star(const RoadGraph& graph, RoadNode* source, RoadNode* target);
Path ida_star(const RoadGraph& graph, RoadNode* source, RoadNode* target);

#endif
