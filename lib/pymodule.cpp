/*
   Copyright (C) 2020  Equinor ASA, Norway.

   The file 'pymodule.cpp' is part of ERT - Ensemble based Reservoir Tool.

   ERT is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   ERT is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.

   See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
   for more details.
*/

#include <Python.h>

namespace {
    PyMethodDef functions[] = {{nullptr, nullptr, 0, nullptr}};

    struct PyModuleDef module = {
        PyModuleDef_HEAD_INIT,
        "libecl",
        nullptr,
        -1,
        functions,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    };
} // namespace

PyMODINIT_FUNC PyInit_libres() {
    return PyModule_Create(&module);
}
