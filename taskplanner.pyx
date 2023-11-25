from taskplanner cimport TaskPlanner
from cpython cimport array

cdef class TaskPlanner:
    cdef CTaskPlanner planner

    def addTask(self, int weight, availableDays):
        cdef array.array[int] availableDaysArr = array.array('i', availableDays)
        self.planner.addTask(weight, len(availableDays), availableDaysArr.data.as_ints)

    def addDays(self, *capacities):
        cdef array.array[int] capacitiesArr = array.array('i', capacities)
        self.planner.addDays(len(capacities), capacitiesArr.data.as_ints)

    def planSchedule(self):
        cdef array.array[int] results = array.array('i', [-1] * self.planner.tasksCount())
        self.planner.planSchedule(results.data.as_ints)
        return results