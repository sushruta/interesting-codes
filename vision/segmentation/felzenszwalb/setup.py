from distutils.core import setup
from Cython.Build import cythonize
import numpy

setup(
  name = 'operations optimized in cython',
  ext_modules = cythonize("native_code.pyx"),
  include_dirs=[numpy.get_include()]
)
