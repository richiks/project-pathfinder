/**
 * @brief This file contains the main method to run the overall program.
 * @author Richik Vivek Sen
 * @version 2019/04/08
 * @attention Do not modify this file.
 */

#include "gevents.h"
#include "gwindow.h"
#include "PathfinderGUI.h"
#include "console.h"

/* Constants and functions local to this function. */
namespace {
    const std::string WINDOW_TITLE = "Project Pathfinder";

    /*
     * Prints an introductory text message on the screen.
     */
    void intro() {
        std::cout << "Welcome to Project Pathfinder!" << std::endl;
        std::cout << "This program searches for paths through graphs" << std::endl;
        std::cout << "representing roadmaps. It demonstrates several" << std::endl;
        std::cout << "graph algorithms for finding paths, such as" << std::endl;
        std::cout << "A* search, IDA* search, memory-optimized IDA*," << std::endl;
        std::cout << "and Periphery Sweep." << std::endl;
    }
}

/*
 * Main program.
 */
int main() {
    intro();

    // create GUI window and position the console to its right
    setConsoleLocation(800 + 6 * WorldDisplay::WINDOW_MARGIN, 20);
    setConsoleEventOnClose(true);

    PathfinderGUI gui(WINDOW_TITLE);
    gui.snapConsoleLocation();

    // main event loop to process events as they happen
    while (true) {
        GEvent e = waitForEvent(ACTION_EVENT | MOUSE_EVENT | WINDOW_EVENT);
        if (e.getEventType() == MOUSE_CLICKED || e.getEventType() == MOUSE_MOVED) {
            gui.processMouseEvent(GMouseEvent(e));
        } else if (e.getEventClass() == ACTION_EVENT) {
            gui.processActionEvent(GActionEvent(e));
        } else if (e.getEventClass() == WINDOW_EVENT) {
            gui.processWindowEvent(GWindowEvent(e));
        }
    }
    return 0;
}
