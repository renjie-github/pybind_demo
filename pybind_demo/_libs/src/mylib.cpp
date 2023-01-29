#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace pybind11::literals;

int add(int i, int j)
{
    return i + j;
}

// 'mylib' is module name，'m' is module handle
PYBIND11_MODULE(mylib, m)
{
    // optional module docstring
    m.doc() = "my pybind11 demo";
    // name for calling method name in Python
    m.def(
        "add", &add, "A function that adds two numbers",
        py::arg("i") = 1, py::arg("j") = 2 // support kwargs and set default values
    );
    // short version for args, need to include "using namespace pybind11::literals" at first
    m.def("add_1", &add, "i"_a=1, "j"_a=2);
    m.attr("export_attr1") = 66;
    // built-in types and general types will be auto converted when assigning,
    // or you can explicitly convert it using py::cast
    py::object world = py::cast("World");
    m.attr("export_attr2") = world;
}