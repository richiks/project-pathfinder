/**
 * @brief This file implements the graph search algorithms for the project.
 * @headerfile pathfinder.h
 * @author Richik Vivek Sen
 * @version 2019/04/08
 * @attention All changes to be made in new copies of this file. Comment "TODO",
 * "EDITED", and "FIXME" wherever necessary.
 */

#include "pathfinder.h"
#include "pqueue.h"
#include <list>
#include <map>
#include <cmath>
#include <unordered_set>
#include <unordered_map>

using namespace std;

static const int FOUND_END = -1;

// forward declaring helper functions

Path retrace_path(std::unordered_map<RoadNode*, RoadNode*>& predecessor_of,
        RoadNode* current);
Path iterative_deepening_weighted_path_helper(const RoadGraph& graph, RoadNode* source,
        RoadNode* target, bool is_periphery_sweep);
double ida_star_helper(const RoadGraph& graph, double g_score, double f_threshold,
                       double max_speed, RoadNode* target, Path& best_path,
                       std::unordered_set<RoadNode*> visited);

Path a_star(const RoadGraph& graph, RoadNode* source, RoadNode* target) {
    PriorityQueue<Path> remaining;
    unordered_set<RoadNode*> visited;
    Path initial_path;

    initial_path.add(source);
    double max_speed = graph.maxRoadSpeed();
    double source_f_score = graph.crowFlyDistanceBetween(source, target) / max_speed;
    remaining.enqueue(initial_path, source_f_score);

    while (!remaining.isEmpty()) {
        double current_f_score = remaining.peekPriority();
        Path current_path = remaining.dequeue();
        RoadNode* current = current_path[current_path.size() - 1];

        double current_heuristic = graph.crowFlyDistanceBetween(current, target)
                / max_speed;
        double current_g_score = current_f_score - current_heuristic;

        visited.insert(current);
        current->setColor(Color::GREEN);

        if (current == target) {
            return current_path;
        }

        for (RoadNode* successor : graph.neighborsOf(current)) {
            RoadEdge* next_edge = graph.edgeBetween(current, successor);
            if (!visited.count(successor)) {
                successor->setColor(Color::YELLOW);
                Path new_path = current_path;
                new_path.add(successor);

                double successor_heuristic = graph.crowFlyDistanceBetween(successor, target)
                        / max_speed;
                double successor_g_score = current_g_score + next_edge->cost();
                double successor_f_score = successor_g_score + successor_heuristic;
                remaining.enqueue(new_path, successor_f_score);
            }
        }
    }
    Path no_path;
    return no_path;
}

Path periphery_sweep(const RoadGraph& graph, RoadNode *source, RoadNode *target) {
    return iterative_deepening_weighted_path_helper(graph, source, target, true);
}

Path memory_optimized_ida_star(const RoadGraph& graph, RoadNode* source, RoadNode* target) {
    return iterative_deepening_weighted_path_helper(graph, source, target, false);
}

Path iterative_deepening_weighted_path_helper(const RoadGraph& graph, RoadNode* source,
        RoadNode* target, bool is_periphery_sweep) {
    list<RoadNode*> frontier;
    unordered_map<RoadNode*, list<RoadNode*>::iterator> node_location_in_frontier;
    frontier.insert(frontier.begin(), source);
    node_location_in_frontier[source] = frontier.begin();

    unordered_map<RoadNode*, double> g_score;
    std::unordered_map<RoadNode*, RoadNode*> predecessor_of;
    g_score[source] = 0;
    predecessor_of[source] = nullptr;
    double max_speed = graph.maxRoadSpeed();

    double f_threshold = graph.crowFlyDistanceBetween(source, target) / max_speed;

    while (!frontier.empty()) {
        double f_min = INFINITY;
        for (RoadNode* current : frontier) {
            double current_g_score = g_score[current];
            double current_f_score = current_g_score +
                    graph.crowFlyDistanceBetween(current, target) / max_speed;

            if (current_f_score > f_threshold) {
                f_min = min(current_f_score, f_min);
                continue;
            }

            current->setColor(Color::GREEN);
            if (current == target) {
                return retrace_path(predecessor_of, current);
            }

           for (RoadNode* successor : graph.neighborsOf(current)) {
                RoadEdge* next_edge = graph.edgeBetween(current, successor);
                double successor_g_score = current_g_score + next_edge->cost();
                if (g_score.count(successor)) {
                    if (successor_g_score >= g_score[successor]) {
                        continue;
                    }
                }

                if (node_location_in_frontier.count(successor)) {
                    frontier.erase(node_location_in_frontier[successor]);
                    node_location_in_frontier.erase(successor);
                }
                frontier.insert(frontier.end(), successor);
                node_location_in_frontier[successor]= --frontier.end();
                g_score[successor] = successor_g_score;
                predecessor_of[successor] = current;
                successor->setColor(Color::YELLOW);
            }
            if (is_periphery_sweep) {
               frontier.erase(node_location_in_frontier[current]);
               node_location_in_frontier.erase(current);
            }
        }
        f_threshold = f_min;
    }
    Path no_path;
    return no_path;
}

Path retrace_path(std::unordered_map<RoadNode*, RoadNode*>& predecessor_of,
        RoadNode* current) {
    Path best_path;
    while (current) {
        best_path.insert(0, current);
        current = predecessor_of[current];
    }
    return best_path;
}

Path ida_star(const RoadGraph& graph, RoadNode* source, RoadNode* target) {
    double max_speed = graph.maxRoadSpeed();
    double f_threshold = graph.crowFlyDistanceBetween(source, target) / max_speed;
    Path best_path;
    unordered_set<RoadNode*> visited;
    best_path.add(source);
    visited.insert(source);

    while (true) {
        double cost = ida_star_helper(graph, 0, f_threshold, max_speed, target,
                best_path, visited);
        if (cost == FOUND_END) {
            return best_path;
        }
        if (cost == INFINITY) {
            break;
        }
        f_threshold = cost;
    }
    Path no_path;
    return no_path;
}

double ida_star_helper(const RoadGraph& graph, double current_g_score, double f_threshold,
        double max_speed, RoadNode* target, Path& best_path, unordered_set<RoadNode*> visited) {
    RoadNode* current = best_path[best_path.size() - 1];
    double current_f_score = current_g_score + graph.crowFlyDistanceBetween(current, target)
            / max_speed;

    if (current_f_score > f_threshold) {
        return current_f_score;
    }
    current->setColor(Color::GREEN);

    if (current == target) {
        return FOUND_END;
    }

    double f_min = INFINITY;

    for (RoadNode* successor : graph.neighborsOf(current)) {
        if (!visited.count(successor)) {
            successor->setColor(Color::YELLOW);
            best_path.add(successor);
            visited.insert(successor);
            RoadEdge* next_edge = graph.edgeBetween(current, successor);
            double temp_min = ida_star_helper(graph, current_g_score + next_edge->cost(),
                    f_threshold, max_speed, target, best_path, visited);
            if (temp_min == FOUND_END) {
                return FOUND_END;
            }
            f_min = min(f_min, temp_min);
            best_path.remove(best_path.size() - 1);
        }
    }
    return f_min;
}

