#include <lemon/preflow.h>
#include <lemon/list_graph.h>
#include <iostream>
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
    vector<Edge> weights;
    vector<Node> tasks;
    Node source, dest;

public:
    ListDigraph(): digraph(), capMap(digraph), costMap(digraph), source(digraph.addNode()), dest(digraph.addNode()) {}

    void addTask(int weight) {
        Node node = digraph.addNode();
        tasks.push_back(node);
        Edge edge = digraph.addArc(source, node);
        weights.push_back(edge);
        capMap[edge] = weight;
        return tasks.size() - 1;
    }

    void addDay()
};

int main() {
    int n, m;
    cin >> n >> m;
    ListDigraph digraph;
    digraph.reserveNode(2 + n + m);
    auto source = digraph.addNode();
    auto dest = digraph.addNode();
    ArcMap capMap(digraph);
    ArcMap costMap(digraph);
    Node tasks[n];
    int weights[n];
    for (int i = 0; i < n; i++) {
        tasks[i] = digraph.addNode();
        cin >> weights[i];
        auto edge = digraph.addArc(source, tasks[i]);
        capMap[edge] = weights[i];
    }
    Node days[m];
    Edge dayCaps[m];
    for (int i = 0; i < m; i++) {
        days[i] = digraph.addNode();
        dayCaps[i] = digraph.addArc(days[i], dest);
        cin >> capMap[dayCaps[i]];
        costMap[dayCaps[i]] = i;
    }
    vector<pair<Edge, int>> availableDays[n];
    deque<pair<Edge, int>> availableTasks[m];
    for (int i = 0; i < n; i++) {
        int c;
        cin >> c;
        availableDays[i].reserve(c);
        for (int j = 0; j < c; j++) {
            int d;
            cin >> d;
            if (weights[i] > capMap[dayCaps[d]])
                continue;
            auto edge = digraph.addArc(tasks[i], days[d]);
            availableDays[i].push_back(make_pair(edge, d));
            availableTasks[d].push_back(make_pair(edge, i));
            capMap[edge] = weights[i];
        }
    }
    Preflow<ListDigraph> maxFlow(digraph, capMap, source, dest);
    CostScaling<ListDigraph> minCost(digraph);
    int cTask = 0;
    while (cTask < n) {
        maxFlow.run();
        minCost.reset();
        minCost.upperMap(capMap);
        minCost.costMap(costMap);
        minCost.stSupply(source, dest, maxFlow.flowValue());
        minCost.run();
        while (cTask < n) {
            bool isScheduled = false;
            bool isFullyScheduled = false;
            for (auto edge: availableDays[cTask]) {
                int flow = minCost.flow(edge.first);
                if (flow == 0)
                    continue;
                isScheduled = true;
                isFullyScheduled = flow == weights[cTask];
                cout << edge.second << endl;
                digraph.erase(tasks[cTask]);
                capMap[dayCaps[edge.second]] -= weights[cTask];
                while (!availableTasks[edge.second].empty() && availableTasks[edge.second].front().second <= cTask)
                    availableTasks[edge.second].pop_front();
                for (int i = 0; i < availableTasks[edge.second].size();) {
                    auto revEdge = availableTasks[edge.second][i];
                    if (weights[revEdge.second] > capMap[dayCaps[edge.second]]) {
                        digraph.erase(revEdge.first);
                        availableTasks[edge.second].erase(availableTasks[edge.second].begin() + i);
                        auto &revRevEdges = availableDays[revEdge.second];
                        revRevEdges.erase(find_if(revRevEdges.begin(), revRevEdges.end(), [&](pair<Edge, int> revRevEdge) {return revRevEdge.second == edge.second;}));
                    } else
                        i++;
                }
                break;
            }
            if (!isScheduled)
                break;
            cTask++;
            if (!isFullyScheduled)
                break;
        }
    }
    return 0;
}