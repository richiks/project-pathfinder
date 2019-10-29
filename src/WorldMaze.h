/**
 * @brief WorldMaze is a class representing world graphs that are 2D maze grids.
 * Every square of a maze is either 0 (MAZE_WALL) or 1 (MAZE_FLOOR).Every edge
 * in a maze is between neighboring squares and costs exactly 1.
 * @class WorldMaze.cpp
 * @author Richik Vivek Sen
 * @version 2019/04/08
 * @attention Do not modify this file.
 */

#ifndef _WorldMaze_h
#define _WorldMaze_h

#include "WorldGrid.h"

class WorldMaze : public WorldGrid {
public:
    /* Constants representing the value of a wall and floor cell in a maze. */
    static const int MAZE_WALL;
    static const int MAZE_FLOOR;
    
    /*
     * Constructor; creates a new empty world on the given graphical window with
     * the given world size.
     */
    WorldMaze(GWindow* gwnd, WorldSize size);
    
    // implementations of pure virtual functions from World and WorldGrid classes
    // (see World.h and WorldGrid.h for documentation)
    virtual double costFunction(int r1, int c1, int r2, int c2) const;
    virtual void createRandomMaze(WorldSize size);
    virtual std::string getType() const;
    virtual int getRowsCols(WorldSize size) const;
    virtual double heuristic(int r1, int c1, int r2, int c2);
};

#endif // _WorldMaze_h
