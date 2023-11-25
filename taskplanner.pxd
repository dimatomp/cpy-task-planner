cdef extern from "taskplanner.cc":
    pass

cdef extern from "taskplanner.h" namespace "taskplanner":
    cdef cppclass CTaskPlanner:
        CTaskPlanner() except +
        void addTask(int weight, int availableDaysCnt, const int *availableDays)
        void addDays(int count, const int *capacities)
        int tasksCount() const
        void planSchedule(int* results)
