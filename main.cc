#include <lemon/preflow.h>
#include <lemon/list_graph.h>
#include <iostream>
#include <queue>
#include <list>
#include <lemon/cost_scaling.h>

using namespace lemon;
using namespace std;

typedef ListDigraph::Node Node;
typedef ListDigraph::ArcMap<int> ArcMap;
typedef ListDigraph::Arc Edge;

class TaskPlanner {
private:
    ListDigraph digraph;
    ArcMap capMap;
    ArcMap costMap;
    Node source, dest;

    struct Task {
        Node node;
        Edge weight;
        list<pair<Edge, int>> availableDays;
    };

    struct Day {
        Node node;
        Edge capacity;
        list<shared_ptr<Task>> availableTasks;
    };

    deque<shared_ptr<Task>> tasks;
    vector<Day> days;

public:
    TaskPlanner(): digraph(), capMap(digraph), costMap(digraph), source(digraph.addNode()), dest(digraph.addNode()) {}

    void addTask(int weight) {
        Node node = digraph.addNode();
        Edge edge = digraph.addArc(source, node);
        capMap[edge] = weight;
        tasks.push_back(make_shared<Task>(Task{ node, edge }));
    }

    void addDay(int capacity) {
        Node node = digraph.addNode();
        Edge edge = digraph.addArc(node, dest);
        capMap[edge] = capacity;
        costMap[edge] = days.size();
        days.push_back({node, edge});
    }

    void addAvailableDay(int taskIdx, int dayIdx) {
        auto task = tasks[taskIdx];
        Day &day = days[dayIdx];
        Edge edge = digraph.addArc(task->node, day.node);
        capMap[edge] = capMap[task->weight];
        task->availableDays.push_back(make_pair(edge, dayIdx));
        day.availableTasks.push_back(task);
    }

    void planSchedule(queue<int> &results) {
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
                    results.push(edge.second);
                    digraph.erase(task->node);
                    while (!day.availableTasks.empty() && !digraph.valid(day.availableTasks.front()->weight))
                        day.availableTasks.pop_front();
                    for (auto edgeIter = day.availableTasks.begin(); edgeIter != day.availableTasks.end();) {
                        auto otherTask = *edgeIter;
                        if (capMap[otherTask->weight] > capMap[day.capacity]) {
                            day.availableTasks.erase(edgeIter++);
                            auto revEdge = find_if(otherTask->availableDays.begin(), otherTask->availableDays.end(), [&](pair<Edge, int> revEdge) {return revEdge.second == edge.second;});
                            digraph.erase(revEdge->first);
                            otherTask->availableDays.erase(revEdge);
                        } else
                            edgeIter++;
                    }
                    break;
                }
                if (!isScheduled)
                    results.push(-1);
                tasks.pop_front();
                if (!isFullyScheduled)
                    break;
            }
        }
    }
};

int main() {
    TaskPlanner planner;
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        int weight;
        cin >> weight;
        planner.addTask(weight);
    }
    for (int i = 0; i < m; i++) {
        int capacity;
        cin >> capacity;
        planner.addDay(capacity);
    }
    for (int i = 0; i < n; i++) {
        int c;
        cin >> c;
        for (int j = 0; j < c; j++) {
            int d;
            cin >> d;
            planner.addAvailableDay(i, d);
        }
    }
    queue<int> results;
    planner.planSchedule(results);
    while (!results.empty()) {
        cout << results.front() << ' ';
        results.pop();
    }
    return 0;
}