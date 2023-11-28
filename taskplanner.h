#ifndef TASKPLANNER_H
#define TASKPLANNER_H
#include <lemon/list_graph.h>
#include <list>
#include <deque>
#include <optional>

namespace taskplanner {
    class CTaskPlanner {
    private:
        typedef lemon::ListDigraph Digraph;
        typedef Digraph::Node Node;
        typedef Digraph::ArcMap<int> ArcMap;
        typedef Digraph::Arc Edge;

        Digraph digraph;
        ArcMap capMap;
        ArcMap costMap;
        Node source, dest;

        struct Task {
            Node node;
            Edge weight;
            std::list<std::pair<Edge, int>> availableDays;
        };

        struct Day {
            Node node;
            Edge capacity;
            std::list<std::shared_ptr<Task>> availableTasks;
        };

        std::deque<std::shared_ptr<Task>> tasks;
        std::vector<std::optional<Day>> days;

    public:
        CTaskPlanner();
        void addTask(int weight, const std::vector<int> &availableDays);
        void addDays(const std::vector<int> &capacities);
        void planSchedule(std::vector<int> &results);
    };
}
#endif