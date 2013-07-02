from distutils.core import setup, Extension
from distutils.ccompiler import new_compiler
from Cython.Build import cythonize
import numpy

setup(
    ext_modules = cythonize([Extension(
        'twolocus',
        ['twolocus.pyx', 'Population.cpp'],
        extra_compile_args = ['-std=c++11', '-Wno-unknown-warning'],
        include_dirs = [numpy.get_include()]
        )
    ])
)
