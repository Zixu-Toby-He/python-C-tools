#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>

#include <Python.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
#include "Original funcs.h"

#ifndef true
#define true  ((char)1)
#endif
#ifndef false
#define false ((char)0)
#endif
char silent = false;

PyObject* pyputs(PyObject* self, PyObject* args)
{
	char* str;
	if(!silent) puts("《puts函数位于头文件 <stdio.h> 中。》");
	if(!(PyArg_ParseTuple(args,"s",&str)))
	{
		puts("错误：传参失败");
		Py_RETURN_NONE;
	}
	return PyLong_FromLong(puts(str));
}

PyObject* pysystem(PyObject* self, PyObject* args)
{
	char* str;
	if(!silent) puts("《system函数位于头文件 <stdlib.h> 中。》");
	if(!PyArg_ParseTuple(args,"s",&str))
	{
		puts("错误：传参失败");
		Py_RETURN_NONE;
	}
	return PyLong_FromLong(system(str));
}


PyObject* cmpobj = NULL;
int cmp(const void* p1,const void* p2)
{
	double v1 = *((double*)p1);
	double v2 = *((double*)p2);
	PyObject* v1obj = PyFloat_FromDouble(v1);
	PyObject* v2obj = PyFloat_FromDouble(v2);
	PyObject* args = Py_BuildValue("dd",v1,v2);
	PyObject* robj  = PyObject_CallObject(cmpobj,args);
	int r;
	if(PyLong_Check(robj))
	{
		r = PyLong_AsLong(robj);
	}
	else if (PyFloat_Check(robj))
	{
		double rf = PyFloat_AsDouble(robj);
		r = ((rf>0)?1:-1) * (rf!=0);
	}
	else
	{
		r = 0;
	}
	Py_XDECREF(v1obj);
	Py_XDECREF(v2obj);
	Py_XDECREF(args);
	Py_XDECREF(robj);
	return r;
}

PyObject* pyqsort(PyObject* self, PyObject* args)
{
	const char LST = (char)0;
	const char NPY = (char)1;
	if(!silent) puts("《qsort函数位于头文件 <stdlib.h> 中。》");
	PyObject* arrayObj = NULL;
	PyArrayObject* arrayAdjObj = NULL;
	void* data;
	size_t copylength;
	char type;
	size_t length,size;
	if(!PyArg_ParseTuple(args,"OiiO",&arrayObj,&length,&size,&cmpobj))
	{
		puts("错误：传参失败");
		Py_RETURN_NONE;
	}

	// 获取数组的数据和长度信息
	if (PyList_Check(arrayObj))
	{
		// 将 Python 列表转换为 NumPy 数组
		arrayAdjObj = (PyArrayObject*)PyArray_FROM_OTF(arrayObj, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
		if (arrayAdjObj == NULL)
		{
			puts("传入数组，但无法转化为(PyArrayObject*)");
			Py_RETURN_NONE;
		}
		else
		{
			type = LST;
			data = PyArray_DATA(arrayAdjObj);
			copylength = PyArray_NBYTES(arrayAdjObj);
			if ((size == 1) or (size == 2) or (size == 4))
			{
				size = 8;
			}
			else
			{
				printf("size = %zi 不是完整数据类型的操作，请慎重使用\n",size);
			}
		}
	}
	else if (PyArray_Check(arrayObj))
	{
		arrayAdjObj = (PyArrayObject*)PyArray_FROM_OTF(arrayObj, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
		if (arrayAdjObj == NULL)
		{
			puts("传入 numpy.array，但无法排序");
			Py_RETURN_NONE;
		}
		else
		{
			type = NPY;
			data = PyArray_DATA(arrayAdjObj);
			copylength = PyArray_NBYTES(arrayAdjObj);
		}
	}
	else
	{
		puts("参数 1 类型错误：需要接收数组类型，即python的列表、元组与numpy.ndarray");
		Py_RETURN_NONE;
	}
	
	// ... 判断 length*size 与 copylength 的关系等
	if (copylength<length*size)
	{
		if(arrayAdjObj)
		{
			Py_XDECREF(arrayAdjObj);
		}
		puts("错误：修改参数超过数组长度");
		Py_RETURN_NONE;
	}

	// 调用 qsort 函数
	qsort(data, length, size, cmp);
	cmpobj = NULL;
	
	// 根据类型将排序后的数据还原到 arrayObj 中
	if (type == LST)
	{
		PyObject* oldobj = NULL;
		for(size_t counting=0; counting<length;counting++)
		{
			oldobj = PyList_GetItem(arrayObj, counting);
			PyList_SetItem(arrayObj, counting, PyFloat_FromDouble(*((double*)data+counting)));
			Py_XDECREF(oldobj);
		}
		Py_XDECREF(arrayAdjObj);
	}
	else
	{
		puts("参数 1 类型错误：需要接收数组类型，即python的列表、元组与numpy.ndarray");
	}
	Py_RETURN_NONE;
}


PyObject* setsilent(PyObject* self)
{
	puts("已设置不显示提示信息");
	silent = (char)true;
	Py_RETURN_NONE;
}
PyObject* setunsilent(PyObject* self)
{
	puts("已设置显示提示信息");
	silent = (char)false;
	Py_RETURN_NONE;
}