#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>

#include <Python.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
#include "Original_funcs.h"

#ifndef true
#define true  ((char)1)
#endif
#ifndef false
#define false ((char)0)
#endif
char silent = false;

/* numpy 的 PyArray_API 是每个编译单元各一份的静态指针，必须在真正调用
   PyArray_* 的编译单元里执行 import_array 才会被填好。少了这一步，本文件
   的 PyArray_API 始终是 NULL，一调用 qsort 就会访问空指针崩溃。 */
int import_numpy(void)
{
	import_array1(-1);
	return 0;
}

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


/* ============================================================================
 * qsort：调用 C 标准库的 qsort 排序，比较逻辑交给 Python 回调
 *
 * 实现要点：
 *   1. 数据先统一转成一份私有的 double 连续缓冲区再排序，元素宽度固定是
 *      sizeof(double)。size 参数只为兼容旧调用保留（0/1/2/4 都按 8 处理）。
 *   2. 比较回调可能抛异常，而 C 的 qsort 没有中断机制：一旦出错就置
 *      qsort_failed，后续比较立刻返回 0，排序结束再把异常抛给 Python。
 *      写回发生在排序之后，所以回调出错时原数组保持原样。
 *   3. PyList_SetItem 会接管新引用并释放旧引用，写回时不能再手动减引用，
 *      否则就是重复释放（旧实现的问题）。
 *   4. 回调里可能再次调用本函数，所以排序前保存全局状态、结束后恢复。
 * ========================================================================== */

static PyObject* qsort_cmpfunc = NULL;	/* 排序期间生效的比较回调 */
static int	 qsort_failed  = 0;	/* 回调出错标记：1 表示已经出错 */

static int pyqsort_cmp(const void* p1, const void* p2)
{
	double v1, v2, rf;
	PyObject* robj;

	if (qsort_failed) return 0;

	v1 = *(const double*)p1;
	v2 = *(const double*)p2;
	robj = PyObject_CallFunction(qsort_cmpfunc, "dd", v1, v2);
	if (robj == NULL)
	{
		qsort_failed = 1;
		return 0;
	}
	rf = PyFloat_AsDouble(robj);
	Py_DECREF(robj);
	if (rf == -1.0 && PyErr_Occurred())
	{
		qsort_failed = 1;
		return 0;
	}
	if (rf > 0.0) return 1;
	if (rf < 0.0) return -1;
	return 0;
}

PyObject* pyqsort(PyObject* self, PyObject* args)
{
	PyObject* arrayObj = NULL;
	PyObject* cmpobj   = NULL;
	Py_ssize_t length  = 0;
	Py_ssize_t size    = 0;
	Py_ssize_t count   = 0;
	Py_ssize_t i;
	PyArrayObject* buffobj = NULL;
	double* data;
	PyObject* prev_func;
	int prev_failed;
	int sort_failed;

	if(!silent) puts("《qsort函数位于头文件 <stdlib.h> 中。》");

	if (!PyArg_ParseTuple(args, "OnnO:qsort", &arrayObj, &length, &size, &cmpobj))
		return NULL;

	if (!PyCallable_Check(cmpobj))
	{
		PyErr_SetString(PyExc_TypeError, "参数 4 需要可调用的比较函数");
		return NULL;
	}
	if (length < 0)
	{
		PyErr_SetString(PyExc_ValueError, "参数 2 长度不能为负数");
		return NULL;
	}
	/* 内部统一按 double 排序：0 表示自动，1/2/4 是旧版本写法，都等价于 8 */
	if (!(size == 0 || size == 1 || size == 2 || size == 4
	      || size == (Py_ssize_t)sizeof(double)))
	{
		PyErr_Format(PyExc_ValueError,
			"参数 3 元素字节数只支持 %zd（double），收到 %zd",
			(Py_ssize_t)sizeof(double), size);
		return NULL;
	}
	if (!PyList_Check(arrayObj) && !PyTuple_Check(arrayObj) && !PyArray_Check(arrayObj))
	{
		PyErr_SetString(PyExc_TypeError,
			"参数 1 需要 list、tuple 或 numpy.ndarray");
		return NULL;
	}

	/* ENSURECOPY：保证缓冲区与调用者的数组不共用内存，
	   排序过程中不会提前动到原数组，出错时也就能保持原样 */
	buffobj = (PyArrayObject*)PyArray_FROM_OTF(
		arrayObj, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY | NPY_ARRAY_ENSURECOPY);
	if (buffobj == NULL)
		return NULL;

	count = (Py_ssize_t)PyArray_SIZE(buffobj);
	if (length > count)
	{
		PyErr_Format(PyExc_ValueError,
			"参数 2 长度 %zd 超过数组实际元素个数 %zd", length, count);
		Py_DECREF(buffobj);
		return NULL;
	}

	data = (double*)PyArray_DATA(buffobj);
	prev_func   = qsort_cmpfunc;
	prev_failed = qsort_failed;
	qsort_cmpfunc = cmpobj;
	qsort_failed  = 0;
	qsort(data, (size_t)length, sizeof(double), pyqsort_cmp);
	sort_failed   = qsort_failed;
	qsort_cmpfunc = prev_func;
	qsort_failed  = prev_failed;
	if (sort_failed)
	{
		Py_DECREF(buffobj);
		return NULL;
	}

	/* list：原地写回（元素会被替换成 float），返回 None */
	if (PyList_Check(arrayObj))
	{
		for (i = 0; i < length; i++)
		{
			PyObject* item = PyFloat_FromDouble(data[i]);
			if (item == NULL)
			{
				Py_DECREF(buffobj);
				return NULL;
			}
			/* PyList_SetItem 会自动释放位置 i 上的旧元素并接管 item */
			if (PyList_SetItem(arrayObj, i, item) < 0)
			{
				Py_DECREF(item);
				Py_DECREF(buffobj);
				return NULL;
			}
		}
		Py_DECREF(buffobj);
		Py_RETURN_NONE;
	}

	/* numpy.ndarray：先确认可写，再把整块缓冲区写回去 */
	if (PyArray_Check(arrayObj))
	{
		PyArrayObject* arr = (PyArrayObject*)arrayObj;
		if (!PyArray_ISWRITEABLE(arr))
		{
			PyErr_SetString(PyExc_ValueError, "传入的 numpy 数组不可写（read-only）");
			Py_DECREF(buffobj);
			return NULL;
		}
		if (PyArray_CopyInto(arr, buffobj) < 0)
		{
			Py_DECREF(buffobj);
			return NULL;
		}
		Py_DECREF(buffobj);
		Py_RETURN_NONE;
	}

	/* tuple 不可变，直接返回排序后的新 list */
	{
		PyObject* result = PyList_New(length);
		if (result == NULL)
		{
			Py_DECREF(buffobj);
			return NULL;
		}
		for (i = 0; i < length; i++)
		{
			PyObject* item = PyFloat_FromDouble(data[i]);
			if (item == NULL)
			{
				Py_DECREF(result);
				Py_DECREF(buffobj);
				return NULL;
			}
			PyList_SET_ITEM(result, i, item);	/* 偷引用，不检查返回值 */
		}
		Py_DECREF(buffobj);
		return result;
	}
}


/* METH_NOARGS 注册的函数同样收到 (self, args) 两个参数，args 为 NULL，
   所以这里必须写成两个参数，才能匹配 PyMethodDef.ml_meth 的 PyCFunction 类型。 */
PyObject* setsilent(PyObject* self, PyObject* args)
{
	puts("已设置不显示提示信息");
	silent = (char)true;
	Py_RETURN_NONE;
}
PyObject* setunsilent(PyObject* self, PyObject* args)
{
	puts("已设置显示提示信息");
	silent = (char)false;
	Py_RETURN_NONE;
}
