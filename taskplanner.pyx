from taskplanner cimport CTaskPlanner

cdef class TaskPlanner:
    cdef CTaskPlanner planner

    def addTask(self, int weight, availableDays):
        self.planner.addTask(weight, availableDays)

    def addDays(self, *capacities):
        self.planner.addDays(capacities)

    def planSchedule(self):
        cdef vector[int] results
        self.planner.planSchedule(results)
        return results