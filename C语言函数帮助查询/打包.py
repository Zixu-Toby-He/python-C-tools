import setuptools
import numpy
import os
numpy目录=numpy.get_include()
源文件目录=r".\源文件"
源文件_文件名=[
	"Original funcs.c",
#	"docs.c",
	"CFuncHelper.c",
	]
源文件_引用路径=[]
for 单个文件名 in 源文件_文件名:
	源文件_引用路径+=[os.path.join(源文件目录,单个文件名)]
模块 = setuptools.Extension(name="CFuncHelper",sources=源文件_引用路径)
文档 = """
	该模块保存了C语言常用函数的python调用方法，\n
	调用这些函数时将提供这些函数所处的头文件信息。\n
	"""
setuptools.setup(name="CFuncHelper",
	description=文档,
	ext_modules=[模块],
	include_dirs=[numpy目录]
	)