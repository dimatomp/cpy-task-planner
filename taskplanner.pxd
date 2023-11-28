from libcpp.vector cimport vector

cdef extern from "taskplanner.cc":
    pass

cdef extern from "taskplanner.h" namespace "taskplanner":
    cdef cppclass CTaskPlanner:
        CTaskPlanner() except +
        void addTask(int weight, const vector[int] &availableDays)
        void addDays(const vector[int] &capacities)
        void planSchedule(vector[int] &results)
