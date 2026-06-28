#include "CMain.h"
#include <Python.h>

PyObject* pythonRun()
{
	return PyLong_FromLong(run());
}

PyMethodDef MakePythonMethords[]={
	{"pythonRun",pythonRun,METH_VARARGS,"The warp for C main function \"int run(void)\". In formal writing, it should be \"int main(void)\""},
	{NULL,NULL,0,NULL}
};

struct PyModuleDef MakePythonModule=
{
	PyModuleDef_HEAD_INIT,
	"CCall",
	"用于Python调用的C语言模块",
	-1,
	MakePythonMethords
};

// 模块初始化函数
PyMODINIT_FUNC PyInit_CCall(void)
{
	return PyModule_Create(&MakePythonModule);
};