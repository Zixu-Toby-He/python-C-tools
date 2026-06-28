#define expy2(func) extern PyObject* func(PyObject* self, PyObject* args)
#define expy1(func) extern PyObject* func(PyObject* self, PyObject* args)

#define FuncNum 3

expy2(pyputs);
expy2(pysystem);
expy2(pyqsort);

expy1(setsilent);
expy1(setunsilent);
