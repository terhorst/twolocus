# distutils: language = c++

cdef class twolocus:
    cdef Population* thisptr

    def __cinit__(self, vector[int] pi, double recomb, double sX, double sY):
        self.thisptr = new Population(pi, recomb, sX, sY)

    def simulate(self, int n, int T):
        return self.thisptr.simulate(n, T);

import_array()
