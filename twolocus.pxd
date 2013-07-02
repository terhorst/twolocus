from libcpp.vector cimport vector

cdef extern from "twolocus_helper.hpp":
    void import_array()

cdef extern from "Population.hpp":
    cdef cppclass Population:
        Population(vector[int], double, double, double)
        object simulate(int, int)
