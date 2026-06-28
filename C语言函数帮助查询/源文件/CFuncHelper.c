#include <Python.h>
#include <numpy/arrayobject.h>
#include "docs.h"
#include "Original funcs.h"

/*
struct PyMethodDef {
    const char  *ml_name;   // The name of the built-in function/method
    PyCFunction ml_meth;    // The C function that implements it
    int         ml_flags;   // Combination of METH_xxx flags, which mostly
                               describe the args expected by the C func
    const char  *ml_doc;    // The __doc__ attribute, or NULL
};
typedef struct PyMethodDef PyMethodDef;
*/

PyMethodDef MakePythonMethords[FuncNum+3];

struct PyModuleDef MakePythonModule=
{
	PyModuleDef_HEAD_INIT,
	"CFuncHelper",
	moduledoc,
	-1,
	MakePythonMethords
};

void writePyMethodDef(PyMethodDef* object, const char* name, PyCFunction meth, int flags, const char* doc)
{
	(*object).ml_name  = name;
	(*object).ml_meth  = meth;
	(*object).ml_flags = flags;
	(*object).ml_doc   = doc;
}

PyMODINIT_FUNC PyInit_CFuncHelper()
{
	writePyMethodDef(&MakePythonMethords[0],	"puts",		pyputs,			METH_VARARGS,		putsdoc);
	writePyMethodDef(&MakePythonMethords[1],	"system",	pysystem,		METH_VARARGS,		systemdoc);
	writePyMethodDef(&MakePythonMethords[2],	"qsort",	pyqsort,		METH_VARARGS,		qsortdoc);

	writePyMethodDef(&MakePythonMethords[FuncNum],  "silent",	setsilent,		METH_NOARGS,		silentdoc);
	writePyMethodDef(&MakePythonMethords[FuncNum+1],"unsilent",	setunsilent,	METH_NOARGS,		unsilentdoc);

	writePyMethodDef(&MakePythonMethords[FuncNum+2], NULL,NULL,0,NULL);
#ifdef Py_ARRAYOBJECT_H
	import_array();
#endif
	return PyModule_Create(&MakePythonModule);
}
