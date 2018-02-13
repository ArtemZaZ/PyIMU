//
// Created by artem on 12.02.18.
//
//#include "MajvikFilter.h"
#include <python3.5/Python.h>
#include "python3.5/structmember.h"

static PyObject* PyIMUError;

//static MajvikFilter filter;

//static PyTypeObject PyFilter_Type;   // тип данных фильтра
//#define PyFilter_Check(x) ((x) -> ob_type == &PyFilter_Type)        // ф-ия проверки фильтр ли это


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


static int MFilter_init(PyMFilterObject *self, PyObject *args)
{
    if(!PyArg_ParseTuple(args, "f", self->beta)) return -1;
    return 0;
}

static PyObject* updateAngle(PyMFilterObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"yaw", "pitch", "roll", NULL};
    if(!PyArg_ParseTupleAndKeywords(args, kwds, "fff", kwlist, self->yaw, self->pitch, self->roll))
        return NULL;
}

static PyMemberDef PyIMU_members[] =        // переменные
        {
                {"beta",  T_FLOAT, offsetof(PyMFilterObject, beta),  0, "beta parameter"},
                {"yaw",   T_FLOAT, offsetof(PyMFilterObject, yaw),   0, "yaw angle"},
                {"pitch", T_FLOAT, offsetof(PyMFilterObject, pitch), 0, "pitch angle"},
                {"roll",  T_FLOAT, offsetof(PyMFilterObject, roll),  0, "roll angle"},
                {NULL}  /* Sentinel */
        };

static PyMethodDef PyIMU_methods[] =
        {
                {"updateAngle", (PyCFunction)updateAngle, METH_VARARGS, "TempUpdateAngle"},
                {NULL} /* Sentinel */
        };


static PyTypeObject PyFilter_Type = {
        PyVarObject_HEAD_INIT(NULL, 0)
        "PyIMU.MFilter",            /* tp_name */
        sizeof(PyMFilterObject),    /* basic size */
        0,                          /* tp_itemsize */
        0,                          /* tp_dealloc */
        0,                          /* tp_print */
        0,                          /* tp_getattr */
        0,                          /* tp_setattr */
        0,                          /* tp_reserved */
        0,                          /* tp_repr */
        0,                          /* tp_as_number */
        0,                          /* tp_as_sequence */
        0,                          /* tp_as_mapping */
        0,                          /* tp_hash  */
        0,                          /* tp_call */
        0,                          /* tp_str */
        0,                          /* tp_getattro */
        0,                          /* tp_setattro */
        0,                          /* tp_as_buffer */
        0,                          /* tp_flags */
        "MajvikFilter object",      /* tp_doc */
        0,                         /* tp_traverse */
        0,                         /* tp_clear */
        0,                         /* tp_richcompare */
        0,                         /* tp_weaklistoffset */
        0,                         /* tp_iter */
        0,                         /* tp_iternext */
        PyIMU_methods,             /* tp_methods */
        PyIMU_members,             /* tp_members */
        0,                         /* tp_getset */
        0,                         /* tp_base */
        0,                         /* tp_dict */
        0,                         /* tp_descr_get */
        0,                         /* tp_descr_set */
        0,                         /* tp_dictoffset */
        (initproc)MFilter_init,    /* tp_init */
        0,                         /* tp_alloc */
        0,                         /* tp_new */
};

static PyModuleDef PyIMUModule =
        {
            PyModuleDef_HEAD_INIT,
            "PyIMU",
            "Module for IMU sensor"
            -1,
            NULL, NULL, NULL, NULL, NULL
        };

PyMODINIT_FUNC PyInit_PyIMU(void)
{
    PyObject* m;
    PyFilter_Type.tp_new = PyType_GenericNew;
    if(PyType_Ready(&PyFilter_Type) < 0) return NULL;

    m = PyModule_Create(&PyIMUModule);
    if(m == NULL) return NULL;

    Py_INCREF(&PyFilter_Type);
    PyModule_AddObject(m, "MFilter", (PyObject*)&PyFilter_Type);
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