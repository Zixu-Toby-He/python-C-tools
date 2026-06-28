#define putsdoc									\
	"头文件：<stdio.h>\n"							\
	"函数原型：int puts(const char*)\n"			\
	"功能：打印参数，成功返回0，失败返回EOF\n"

#define systemdoc								\
	"头文件：<stdlib.h>\n"						\
	"函数原型：int system(const char*)\n"			\
	"功能：执行系统命令，返回操作系统参数\n"

#define qsortdoc																\
	"头文件：<stdlib.h>\n"														\
	"函数原型：void qsort(void *base,\n"											\
	"                   size_t nitems,\n"										\
	"                   size_t size, "											\
	"                   int (*compare)(const void *, const void*))\n"			\
	"功能：基于 compare 给 base 数组排序，\n"										\
	"\t其中 base 数组总共有 nitems 个长度为 size 的元素\n"							\
	"说明：\n"																	\
	"\tint compare(const void* p1,const void* p2)\n"							\
	"\t{\n"																		\
	"\t   numtype value1 = getValue(p1);\n"										\
	"\t   numtype value2 = getValue(p2);\n"										\
	"\t   return ((value1>value2)?1:-1) * (value1!=value2);\n"					\
	"\t}\n"																		\
	"注：type(base) <=> list 或 numpy.array\n"

#define silentdoc								\
	"模块全局变量：控制运行时不显示信息\n"

#define unsilentdoc								\
	"模块全局变量：控制运行时显示信息\n"

#define moduledoc								\
	"该模块保存了C语言常用函数的python调用方法，\n"	\
	"调用这些函数时将提供这些函数所处的头文件信息。\n"
