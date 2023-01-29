# pybind11_add_module is a func provided by pybind11.  It will take care 
# of all the details needed to build a Python extension module on any platform. 
pybind11_add_module(mylib MODULE mylib.cpp)
pybind11_add_module(mylib_object MODULE mylib_object.cpp)