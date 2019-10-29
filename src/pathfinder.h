/**
 * @brief This file declares the graph algorithms for the project.
 * @class pathfinder.cpp
 * @author Richik Vivek Sen
 * @version 2019/04/08
 * @attention All changes to be made in new copies of this file. Comment
 * "TODO" and "EDITED" wherever necessary.
 */

#ifndef _pathfinder_h
#define _pathfinder_h

#include "basicgraph.h"
#include "set.h"
#include "vector.h"
#include "Color.h"

using Path = Vector<Vertex*>;

// The path-searching algorithms

Path a_star(const BasicGraph& graph, Vertex* source, Vertex* target);
Path periphery_sweep(const BasicGraph& graph, Vertex* source, Vertex* target);
Path memory_optimized_ida_star(const BasicGraph& graph, Vertex* source,
                               Vertex* target);
Path ida_star(const BasicGraph& graph, Vertex* source, Vertex* target);

// The minimum spanning tree algorithm for random maze generation

Set<Edge*> kruskal(BasicGraph& graph);

/*
 * Returns a 'heuristic' value, or rough estimation, of the distance between
 * this vertex and the given other vertex.
 * The heuristic function is guaranteed to be admissible.
 */

extern double heuristicFunction(Vertex* from, Vertex* to);

#endif

