#include "taskplanner.h"
#include <lemon/preflow.h>
#include <lemon/cost_scaling.h>

namespace taskplanner {
    CTaskPlanner::CTaskPlanner(): digraph(), capMap(digraph), costMap(digraph), source(digraph.addNode()), dest(digraph.addNode()) {}

    void CTaskPlanner::addTask(int weight, int availableDaysCnt, const int* availableDays) {
        Node node = digraph.addNode();
        Edge wEdge = digraph.addArc(source, node);
        capMap[wEdge] = weight;
        auto task = make_shared<Task>(Task{ node, wEdge });
        tasks.push_back(task);
        for (; availableDaysCnt > 0; availableDaysCnt--, availableDays++) {
            Day &day = days[*availableDays];
            Edge edge = digraph.addArc(node, day.node);
            capMap[edge] = weight;
            task->availableDays.push_back(make_pair(edge, *availableDays));
            day.availableTasks.push_back(task);
        }
    }

    void CTaskPlanner::addDays(int count, const int* capacities) {
        for (; count > 0; count--, capacities++) {
            Node node = digraph.addNode();
            Edge edge = digraph.addArc(node, dest);
            capMap[edge] = *capacities;
            costMap[edge] = days.size();
            days.push_back({node, edge});
        }
    }

    int CTaskPlanner::tasksCount() const {
        return tasks.size();
    }

    void CTaskPlanner::planSchedule(int *results) {
        Preflow<ListDigraph> maxFlow(digraph, capMap, source, dest);
        CostScaling<ListDigraph> minCost(digraph);
        while (!tasks.empty()) {
            maxFlow.run();
            minCost.reset();
            minCost.upperMap(capMap);
            minCost.costMap(costMap);
            minCost.stSupply(source, dest, maxFlow.flowValue());
            minCost.run();
            while (!tasks.empty()) {
                bool isScheduled = false;
                bool isFullyScheduled = false;
                auto task = tasks.front();
                for (auto edge: task->availableDays) {
                    int flow = minCost.flow(edge.first);
                    if (flow == 0)
                        continue;
                    isScheduled = true;
                    isFullyScheduled = flow == capMap[task->weight];
                    Day &day = days[edge.second];
                    capMap[day.capacity] -= capMap[task->weight];
                    *(results++) = edge.second;
                    while (!day.availableTasks.empty() && !digraph.valid(day.availableTasks.front()->weight))
                        day.availableTasks.pop_front();
                    for (auto edgeIter = day.availableTasks.begin(); edgeIter != day.availableTasks.end();) {
                        auto otherTask = *edgeIter;
                        if (capMap[otherTask->weight] > capMap[day.capacity]) {
                            day.availableTasks.erase(edgeIter++);
                            auto revEdge = find_if(otherTask->availableDays.begin(), otherTask->availableDays.end(), [&](pair<Edge, int> revEdge) -> bool {return revEdge.second == edge.second;});
                            digraph.erase(revEdge->first);
                            otherTask->availableDays.erase(revEdge);
                        } else
                            edgeIter++;
                    }
                    break;
                }
                if (!isScheduled)
                    *(results++) = -1;
                digraph.erase(task->node);
                tasks.pop_front();
                if (!isFullyScheduled)
                    break;
            }
        }
    }
}