import ctypes
import numpy as np
import multiprocessing as mp
from multiprocessing.sharedctypes import RawArray, Value
from contextlib import closing
import twolocus
import logging
from itertools import product

T = 75
N = 5000
NW = 24
R = 1e-6
info = mp.get_logger().info
ss = np.linspace(-0.1, 0.1, 30)
ssn = ss.shape[0]
dims = (ssn, ssn, NW, N, T, 4)

def main():
    logger = mp.log_to_stderr()
    logger.setLevel(logging.INFO)
    shared_arr = RawArray(ctypes.c_uint16, np.prod(dims).item())
    sel_inds = product(range(ssn), repeat=2)
    arr = tonumpyarray(shared_arr)
    with closing(mp.Pool(NW, initializer=init, initargs=(shared_arr,))) as p:
        # many processes access the same slice
        p.map(f, sel_inds)
    np.save('data/sims', arr)
    
def init(shared_arr_):
    global shared_arr
    shared_arr = shared_arr_ # must be inhereted, not passed as an argument

def tonumpyarray(mp_arr):
    return np.frombuffer(mp_arr, ctypes.c_uint16).reshape(dims)

def f(s):
    """no synchronization."""
    sX = ss[s[0]]
    sY = ss[s[1]]
    info("start %s %g %g" % (str(s), sX, sY))
    arr = tonumpyarray(shared_arr)[s[0], s[1]]
    arr[...] = twolocus.twolocus([500]*4, R, sX, sY).simulate(N, T)

if __name__=="__main__":
    main()
