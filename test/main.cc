#include "../taskplanner.h"
#include <iostream>

using namespace std;
using namespace taskplanner;

int main() {
    CTaskPlanner planner;
    int n, m;
    cin >> n >> m;
    vector<int> capacities(m);
    for (int i = 0; i < m; i++) {
        int c;
        cin >> c;
        capacities.push_back(c);
    }
    planner.addDays(capacities);
    for (int i = 0; i < n; i++) {
        int weight, daysCnt;
        cin >> weight >> daysCnt;
        vector<int> days(daysCnt);
        for (int j = 0; j < daysCnt; j++) {
            int d;
            cin >> d;
            days.push_back(d);
        }
        planner.addTask(weight, days);
    }
    vector<int> results(n);
    planner.planSchedule(results);
    for (int i = 0; i < n; i++)
        cout << results[i] << " ";
    return 0;
}