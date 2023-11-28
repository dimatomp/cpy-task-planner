#include "../taskplanner.h"
#include <iostream>

using namespace std;
using namespace taskplanner;

int main() {
    CTaskPlanner planner;
    int n, m;
    cin >> n >> m;
    int capacities[m];
    for (int i = 0; i < m; i++)
        cin >> capacities[i];
    planner.addDays(m, capacities);
    for (int i = 0; i < n; i++) {
        int weight, daysCnt;
        cin >> weight >> daysCnt;
        int days[daysCnt];
        for (int j = 0; j < daysCnt; j++)
            cin >> days[j];
        planner.addTask(weight, daysCnt, days);
    }
    int results[n];
    planner.planSchedule(results);
    for (int i = 0; i < n; i++)
        cout << results[i] << " ";
    return 0;
}