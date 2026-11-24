/* Python 的 PyCFunction 签名固定是两个参数 (self, args)：
   即使方法用 METH_NOARGS 注册，第二个参数也会传进来（值为 NULL）。
   所以声明和实现都必须是两个参数，否则与 PyMethodDef.ml_meth 类型不符。 */
#define expy2(func) extern PyObject* func(PyObject* self, PyObject* args)
#define expy1(func) extern PyObject* func(PyObject* self, PyObject* args)

#define FuncNum 3

expy2(pyputs);
expy2(pysystem);
expy2(pyqsort);

expy1(setsilent);
expy1(setunsilent);

/* 在本编译单元里初始化 numpy 的 C API 接口表，失败返回 -1 */
extern int import_numpy(void);
