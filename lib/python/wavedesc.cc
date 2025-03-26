#include <Python.h>
#include "wavedesc.h"

// Define wavedesc Python type
typedef struct {
    PyObject_HEAD
    wavedesc* wd;
} wavedescObject;

// wavedesc deallocator
static void wavedesc_dealloc(wavedescObject* self) {
    delete self->wd;
    Py_TYPE(self)->tp_free(reinterpret_cast<PyObject*>(self));
}

// wavedesc constructor
static PyObject* wavedesc_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    wavedescObject* self;

    self = reinterpret_cast<wavedescObject*>(type->tp_alloc(type, 0));
    if (self != nullptr) {
        const char* filename;

        if (PyArg_ParseTuple(args, "s", &filename)) {
            self->wd = new wavedesc(filename);
        } else {
            Py_DECREF(self);
            return nullptr;
        }
    }

    return reinterpret_cast<PyObject*>(self);
}

// wavedesc rewind method
static PyObject* wavedesc_rewind(wavedescObject* self) {
    self->wd->rewind();
    Py_RETURN_NONE;
}

// wavedesc next method
static PyObject* wavedesc_next(wavedescObject* self) {
  bool result = self->wd->next();
  return PyBool_FromLong(result);
}

// wavedesc get method
static PyObject* wavedesc_get(wavedescObject* self) {
    std::vector<std::array<float, 2>>& data = self->wd->get();

    // Convert the result to a Python list of tuples
    PyObject* pyList = PyList_New(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        float x = data[i][0];
        float y = data[i][1];

        // Convert the elements of std::array to Python float objects
        PyObject* pyTuple = PyTuple_Pack(2, PyFloat_FromDouble(x), PyFloat_FromDouble(y));
        PyList_SetItem(pyList, i, pyTuple);
    }

    return pyList;
}

// wavedesc methods
static PyMethodDef wavedesc_methods[] = {
    {"rewind", reinterpret_cast<PyCFunction>(wavedesc_rewind), METH_NOARGS, "Rewind the wavedesc."},
    {"next", reinterpret_cast<PyCFunction>(wavedesc_next), METH_NOARGS, "Move to the next element in the wavedesc."},
    {"get", reinterpret_cast<PyCFunction>(wavedesc_get), METH_NOARGS, "Get the data from the wavedesc."},
    {nullptr, nullptr, 0, nullptr} // Sentinel
};

// wavedesc type definition
static PyTypeObject wavedescType = {
    PyVarObject_HEAD_INIT(nullptr, 0)
    "wavedesc.wavedesc",         // tp_name
    sizeof(wavedescObject),      // tp_basicsize
    0,                            // tp_itemsize
    reinterpret_cast<destructor>(wavedesc_dealloc), // tp_dealloc
    0,                            // tp_print
    nullptr,                      // tp_getattr
    nullptr,                      // tp_setattr
    nullptr,                      // tp_reserved
    nullptr,                      // tp_repr
    nullptr,                      // tp_as_number
    nullptr,                      // tp_as_sequence
    nullptr,                      // tp_as_mapping
    nullptr,                      // tp_hash
    nullptr,                      // tp_call
    nullptr,                      // tp_str
    nullptr,                      // tp_getattro
    nullptr,                      // tp_setattro
    nullptr,                      // tp_as_buffer
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, // tp_flags
    nullptr,                      // tp_doc
    nullptr,                      // tp_traverse
    nullptr,                      // tp_clear
    nullptr,                      // tp_richcompare
    0,                            // tp_weaklistoffset
    nullptr,                      // tp_iter
    nullptr,                      // tp_iternext
    wavedesc_methods,             // tp_methods
    nullptr,                      // tp_members
    nullptr,                      // tp_getset
    nullptr,                      // tp_base
    nullptr,                      // tp_dict
    nullptr,                      // tp_descr_get
    nullptr,                      // tp_descr_set
    0,                            // tp_dictoffset
    nullptr,                      // tp_init
    nullptr,                      // tp_alloc
    wavedesc_new,                 // tp_new
};

// Module initialization function
static PyModuleDef wavedesc_module = {
    PyModuleDef_HEAD_INIT,
    "wavedesc",                  // Module name
    nullptr,                     // Module documentation, may be NULL
    -1,                          // Size of per-interpreter state of the module, or -1 if the module keeps state in global variables
    nullptr,                     // m_methods
    nullptr,                     // m_reload
    nullptr,                     // m_traverse
    nullptr,                     // m_clear
    nullptr,                     // m_free
};

// Module initialization function
PyMODINIT_FUNC PyInit_wavedesc(void) {
    PyObject* module;

    if (PyType_Ready(&wavedescType) < 0) {
        return nullptr;
    }

    module = PyModule_Create(&wavedesc_module);
    if (module == nullptr) {
        return nullptr;
    }

    Py_INCREF(&wavedescType);
    PyModule_AddObject(module, "wavedesc", reinterpret_cast<PyObject*>(&wavedescType));

    return module;
}
