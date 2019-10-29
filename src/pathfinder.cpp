/**
 * @brief This file implements the graph search algorithms for the project.
 * @headerfile pathfinder.h
 * @author Richik Vivek Sen
 * @version 2019/04/08
 * @attention All changes to be made in new copies of this file. Comment "TODO"
 * and "EDITED" wherever necessary.
 */

#include "pathfinder.h"
#include "queue.h"
#include "pqueue.h"
#include <list>
#include <map>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include "Color.h"
#include "World.h"

using namespace std;

const int FOUND_END = -1;

Path reconstruct_path(std::unordered_map<Vertex*, Vertex*>& node_predecessor, Vertex* current);
Path iterative_deepening_weighted_path_helper(const BasicGraph& graph, Vertex*
        start, Vertex* end, bool is_periphery_sweep);
double ida_star_helper(const BasicGraph& graph, double g_score, double
        f_threshold, Vertex* target, Path& best_path, std::unordered_set<Vertex*>
        visited);

/*
Path a_star(const BasicGraph& graph, Vertex* source, Vertex* target) {
    PriorityQueue<Path> remaining;
    unordered_set<Vertex*> visited;
    Path initial_path;

    initial_path.add(source);
    double source_f_score = heuristicFunction(source, target);

    remaining.enqueue(initial_path, source_f_score);

    while (!remaining.isEmpty()) {
        double current_f_score = remaining.peekPriority();
        Path current_path = remaining.dequeue();
        Vertex* current = current_path[current_path.size() - 1];
        double current_heuristic = heuristicFunction(current, target);

        double current_g_score = current_f_score - current_heuristic;
        visited.insert(current);
        current->setColor(GREEN);

        if (current == target) {
            return current_path;
        }

        for(Vertex* successor : graph.getNeighbors(current)) {
            Edge* next_edge = graph.getEdge(current, successor);
            if (!visited.count(successor)) {
                successor->setColor(YELLOW);
                Path new_path = current_path;
                new_path.add(successor);

                double successor_heuristic = heuristicFunction(successor, target);
                double successor_g_score = current_g_score + next_edge->cost;

                double successor_f_score = successor_g_score + successor_heuristic;
                remaining.enqueue(new_path, successor_f_score);
            }
        }
    }
    Path no_path;
    return no_path;
}
*/

Path a_star(const BasicGraph& graph, Vertex* source, Vertex* target) {
    PriorityQueue<Vertex*> frontier;
    double source_f_score = heuristicFunction(source, target);
    frontier.enqueue(source, source_f_score);

    unordered_map<Vertex*, double> g_score;
    std::unordered_map<Vertex*, Vertex*> node_predecessor;
    g_score[source] = 0;
    node_predecessor[source] = nullptr;

    while (!frontier.isEmpty()) {
        Vertex* current = frontier.dequeue();
        current->setColor(GREEN);

        if (current == target) {
            return reconstruct_path(node_predecessor, current);
        }

        for (Vertex* successor : graph.getNeighbors(current)) {
            Edge* next_edge = graph.getEdge(current, successor);
            double successor_g_score = g_score[current] + next_edge->cost;
            if (!g_score.count(successor) || successor_g_score < g_score[successor]) {
                successor->setColor(YELLOW);
                g_score[successor] = successor_g_score;
                double successor_heuristic = heuristicFunction(successor, target);
                double successor_f_score = successor_g_score + successor_heuristic;
                frontier.enqueue(successor, successor_f_score);
                node_predecessor[successor] = current;
            }
        }
    }
    Path no_path;
    return no_path;
}


Path periphery_sweep(const BasicGraph& graph, Vertex* source, Vertex* target) {
    return iterative_deepening_weighted_path_helper(graph, source, target, true);
}

Path memory_optimized_ida_star(const BasicGraph& graph, Vertex* source, Vertex* target) {
    return iterative_deepening_weighted_path_helper(graph, source, target, false);
}

Path iterative_deepening_weighted_path_helper(const BasicGraph& graph, Vertex*
        source, Vertex* target, bool is_periphery_sweep) {
    list<Vertex*> frontier;
    unordered_map<Vertex*, list<Vertex*> :: iterator> node_location;
    frontier.insert(frontier.begin(), source);
    node_location[source] = frontier.begin();

    unordered_map<Vertex*, double> g_score;
    std::unordered_map<Vertex*, Vertex*> node_predecessor;
    g_score[source] = 0;
    node_predecessor[source] = nullptr;

    double f_threshold = heuristicFunction(source, target);

    while(!frontier.empty()) {
        double f_min = INFINITY;
        for(Vertex* current : frontier) {
            double current_g_score = g_score[current];
            double current_f_score = current_g_score + heuristicFunction(current, target);

            if(current_f_score > f_threshold) {
                f_min = min(current_f_score, f_min);
                continue;
            }

            current->setColor(GREEN);
            if(current == target) {
                return reconstruct_path(node_predecessor, current);
            }

           for(Vertex* successor : graph.getNeighbors(current)) {
                Edge* next_edge = graph.getEdge(current, successor);
                double successor_g_score = current_g_score + next_edge->cost;
                if(g_score.count(successor)) {
                    if(successor_g_score >= g_score[successor]) {
                        continue;
                    }
                }

                if(node_location.count(successor)) {
                    frontier.erase(node_location[successor]);
                    node_location.erase(successor);
                }
                frontier.insert(frontier.end(), successor);
                node_location[successor]= --frontier.end();
                g_score[successor] = successor_g_score;
                node_predecessor[successor] = current;
                successor->setColor(YELLOW);
            }
            if(is_periphery_sweep) {
               frontier.erase(node_location[current]);
               node_location.erase(current);
            }
        }
        f_threshold = f_min;
    }
    Path no_path;
    return no_path;
}

Path reconstruct_path(std::unordered_map<Vertex*, Vertex*>& node_predecessor, Vertex* current) {
    Path best_path;
    while(current) {
        best_path.insert(0, current);
        current = node_predecessor[current];
    }
    return best_path;
}

Path ida_star(const BasicGraph& graph, Vertex* source, Vertex* target) {
    double f_threshold = heuristicFunction(source, target);
    Path best_path;
    unordered_set<Vertex*> visited;
    best_path.add(source);
    visited.insert(source);

    while(true) {
        double cost = ida_star_helper(graph, 0, f_threshold, target, best_path,
                visited);
        if(cost == FOUND_END) {
            return best_path;
        }
        if(cost == INFINITY) {
            break;
        }
        f_threshold = cost;
    }
    Path no_path;
    return no_path;
}

double ida_star_helper(const BasicGraph& graph, double current_g_score, double f_threshold,
        Vertex* target, Path& best_path, unordered_set<Vertex*> visited) {
    Vertex* current = best_path[best_path.size() - 1];
    double current_f_score = current_g_score + heuristicFunction(current, target);

    if(current_f_score > f_threshold) {
        return current_f_score;
    }
    current->setColor(GREEN);

    if(current == target) {
        return FOUND_END;
    }

    double f_min = INFINITY;

    for(Vertex* successor : graph.getNeighbors(current)) {
        if(!visited.count(successor)) {
            successor->setColor(YELLOW);
            best_path.add(successor);
            visited.insert(successor);
            Edge* next_edge = graph.getEdge(current, successor);
            double temp_min = ida_star_helper(graph, current_g_score + next_edge->cost,
                    f_threshold, target, best_path, visited);
            if(temp_min == FOUND_END) {
                return FOUND_END;
            }
            f_min = min(f_min, temp_min);
            best_path.remove(best_path.size() - 1);
        }
    }
    return f_min;
}

Set<Edge*> kruskal(BasicGraph& graph) {
    // put each vertex into a 'cluster', initially containing only itself
    Map<Vertex*, Set<Vertex*>* > clusters;
    Set<Vertex*> allVertices = graph.getVertexSet();
    Vector<Set<Vertex*>* > allSets;   // for freeing later

    for (Vertex* v : allVertices) {
        Set<Vertex*>* set = new Set<Vertex*>();
        set->add(v);
        clusters[v] = set;
        allSets.add(set);
    }

    // put all edges into a priority queue, sorted by weight
    PriorityQueue<Edge*> pq;
    Set<Edge*> allEdges = graph.getEdgeSet();
    for (Edge* edge : allEdges) {
        pq.enqueue(edge, edge->cost);
    }

    // repeatedly pull min-weight edge out of PQ and add it to MST if its
    // endpoints are not already connected
    Set<Edge*> mstEdges;
    while (!pq.isEmpty()) {
        Edge* e = pq.dequeue();
        Set<Vertex*>* set1 = clusters[e->start];
        Set<Vertex*>* set2 = clusters[e->finish];
        if (set1 != set2) {
            mstEdges.add(e);

            // merge the two sets
            set1->addAll(*set2);
            for (Vertex* v : *set1) {
                Set<Vertex*>* setv = clusters[v];
                if (setv != set1) {
                    clusters[v] = set1;
                }
            }
        }
    }

    for (Set<Vertex*>* set : allSets) {
        delete set;
    }
    return mstEdges;
}
