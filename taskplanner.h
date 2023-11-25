#ifndef TASKPLANNER_H
#define TASKPLANNER_H
#include <lemon/list_graph.h>
#include <list>
#include <deque>

using namespace lemon;
using namespace std;

typedef ListDigraph::Node Node;
typedef ListDigraph::ArcMap<int> ArcMap;
typedef ListDigraph::Arc Edge;

namespace taskplanner {
    class CTaskPlanner {
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
        CTaskPlanner();
        void addTask(int weight, int availableDaysCnt, const int *availableDays);
        void addDays(int count, const int *capacities);
        int tasksCount() const;
        void planSchedule(int* results);
    };
}
#endif