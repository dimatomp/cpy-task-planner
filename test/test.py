from taskplanner import TaskPlanner

planner = TaskPlanner()
n, m = map(int, input().split())
planner.addDays(*map(int, input().split()))
for i in range(n):
    data = list(map(int, input().split()))
    planner.addTask(data[0], data[2:])
print(*planner.planSchedule())
