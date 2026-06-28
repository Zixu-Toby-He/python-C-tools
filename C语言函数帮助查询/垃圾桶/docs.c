#include "docs.h"
char* putsdoc =
	"头文件：<stdio.h>\n"
	"函数原型：int puts(const char*)\n"
	"功能：打印参数，成功返回0，失败返回EOF\n"
;
char* systemdoc =
	"头文件：<stdlib.h>\n"
	"函数原型：int system(const char*)\n"
	"功能：执行系统命令，返回操作系统参数\n"
;

char* moduledoc = 
	"该模块保存了C语言常用函数的python调用方法，\n"
	"调用这些函数时将提供这些函数所处的头文件信息。\n"
;
