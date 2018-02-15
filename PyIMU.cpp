//
// Created by artem on 12.02.18.
//
//#include "MajvikFilter.h"
#include <python3.5/Python.h>
#include "python3.5/structmember.h"

static PyObject* PyIMUError;

//static MajvikFilter filter;

static PyTypeObject PyMFilter_Type;   // тип данных фильтра
#define PyMFilter_Check(x) ((x) -> ob_type == &PyMFilter_Type)        // ф-ия проверки фильтр ли это


/*
static PyObject* MajvikFilterInit(PyObject* self, PyObject *args)   // инициализация
{
    float beta;
    if(!PyArg_ParseTuple(args, "f", &beta)) return NULL;
    filter.setBeta(beta);
}

static PyObject* MajvikFilterUpdate(PyObject* self, PyObject* args)
{
    PyObject* aList;
    PyObject* wList;
}
*/

typedef struct                              // структура объекта
{
    PyObject_HEAD
    float beta;
    float yaw;
    float pitch;
    float roll;
} PyMFilterObject;

/*
static void MFilter_dealloc(PyMFilterObject* self)
{
    PyObject_Del(self);
}
*/
static void MFilter_dealloc(PyMFilterObject* self)
{
    Py_TYPE(self)->tp_free((PyObject*)self);
}

/*
static PyObject* PyMFilter_new(float beta)
{
    PyMFilterObject* self;

    self = PyObject_New(PyMFilterObject, &PyMFilter_Type);
    if(self != NULL)
    {
        self -> yaw = 0.f;
        self -> pitch = 0.f;
        self -> roll = 0.f;
        self -> beta = 0.f;
    }
    return (PyObject*)self;
}
*/


static PyObject* PyMFilter_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyMFilterObject* self;

    self = (PyMFilterObject *)type->tp_alloc(type, 0);
    if(self != NULL)
    {
        self -> yaw = 0.f;
        self -> pitch = 0.f;
        self -> roll = 0.f;
        self -> beta = 0.f;
    }
    return (PyObject*)self;
}

/*
static PyObject* MFilter(PyMFilterObject *self, PyObject *args)
{
    float beta;
    if(!PyArg_ParseTuple(args, "f", &beta)) return NULL;
    return PyMFilter_new(beta);
}
 */

static int MFilter_init(PyMFilterObject *self, PyObject *args)
{
    if(! PyArg_ParseTuple(args, "f", &self -> beta)) return -1;
    return 0;
}
/*
static PyObject* MFilter_updateAngle(PyMFilterObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"yaw", "pitch", "roll", NULL};
    if(!PyArg_ParseTupleAndKeywords(args, kwds, "fff", kwlist, self->yaw, self->pitch, self->roll))
        return NULL;
    return PyLong_FromLong(1);
}
*/
static PyObject* MFilter_updateAngle(PyMFilterObject *self, PyObject *args)
{
    if(!PyArg_ParseTuple(args, "|fff", &self->yaw, &self->pitch, &self->roll))
        return NULL;
    return PyLong_FromLong(1);
}

static PyMemberDef PyIMU_members[] =        // переменные
        {
                {"beta",  T_FLOAT, offsetof(PyMFilterObject, beta),  READONLY, "beta parameter"},
                {"yaw",   T_FLOAT, offsetof(PyMFilterObject, yaw),   READONLY, "yaw angle"},
                {"pitch", T_FLOAT, offsetof(PyMFilterObject, pitch), READONLY, "pitch angle"},
                {"roll",  T_FLOAT, offsetof(PyMFilterObject, roll),  READONLY, "roll angle"},
                {NULL}  // Sentinel
        };

static PyMethodDef PyMFilter_methods[] =
        {
                {"updateAngle", (PyCFunction)MFilter_updateAngle, METH_VARARGS, "TempUpdateAngle"},
                {NULL} // Sentinel
        };


static PyTypeObject PyMFilter_Type = {
        PyVarObject_HEAD_INIT(NULL, 0)
        "PyIMU.MFilter",            // tp_name
        sizeof(PyMFilterObject),    // basic size
        0,                          // tp_itemsize
        (destructor)MFilter_dealloc, // tp_dealloc
        0,                          // tp_print
        0,                         // tp_getattr
        0,                          // tp_setattr
        0,                          // tp_reserved
        0,                          // tp_repr
        0,                          // tp_as_number
        0,                          // tp_as_sequence
        0,                          // tp_as_mapping
        0,                          // tp_hash
        0,                          // tp_call
        0,                          // tp_str
        0,                          // tp_getattro
        0,                          // tp_setattro
        0,                          // tp_as_buffer
        Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,         // tp_flags
        "MajvikFilter object",      // tp_doc
        0,                         // tp_traverse
        0,                         // tp_clear
        0,                         // tp_richcompare
        0,                         // tp_weaklistoffset
        0,                         // tp_iter
        0,                         // tp_iternext
        PyMFilter_methods,         // tp_methods
        PyIMU_members,             // tp_members
        0,                         // tp_getset
        0,                         // tp_base
        0,                         // tp_dict
        0,                         // tp_descr_get
        0,                         // tp_descr_set
        0,                         // tp_dictoffset
        (initproc)MFilter_init,    // tp_init
        0,                         // tp_alloc
        PyMFilter_new,               // tp_new
};

/*
static PyMethodDef PyIMU_methods[] =
        {
                { "MFilter", (PyCFunction)MFilter, METH_VARARGS, "method for initialize Majvik filter"},
                { NULL, NULL, 0, NULL }
        };
*/

static PyModuleDef PyIMUModule =
        {
            PyModuleDef_HEAD_INIT,
            "PyIMU",
            "Module for IMU sensor"
            -1,
            NULL,
            NULL, NULL, NULL, NULL
        };

PyMODINIT_FUNC PyInit_PyIMU(void)
{
    PyObject* m;
    //PyMFilter_Type.tp_new = PyType_GenericNew;
    if(PyType_Ready(&PyMFilter_Type) < 0) return NULL;

    m = PyModule_Create(&PyIMUModule);
    if(m == NULL) return NULL;

    PyIMUError = PyErr_NewException("PyIMU.error", NULL, NULL);
    Py_INCREF(PyIMUError);
    PyModule_AddObject(m, "error", PyIMUError);

    Py_INCREF(&PyMFilter_Type);
    PyModule_AddObject(m, "MFilter", (PyObject *)&PyMFilter_Type);
    return m;
}

int main(int argc, char *argv[])
{
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL)
    {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }
    /*добавляем встроенный модуль перед инициализацией питона*/
    PyImport_AppendInittab("PyIMU", PyInit_PyIMU);
    /*передаем argv[0] интерпретатору питона */
    Py_SetProgramName(program);
    /*инициализируем интерпретатор питона*/
    Py_Initialize();
    /*опционально - импортируем модуль? модуль может импортировать также внешний скрипт*/
    PyImport_ImportModule("PyIMU");
    PyMem_RawFree(program);
    return 0;
}