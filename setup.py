from setuptools import setup, Extension
from Cython.Build import cythonize
import os

setup(
    name="cpy-task-planner",
    ext_modules = cythonize([
        Extension('taskplanner', ["taskplanner.pyx"], language="c++", extra_compile_args=["-I" + os.getenv('LEMON_INCLUDE_PATH'), "-std=c++11"])
]))
