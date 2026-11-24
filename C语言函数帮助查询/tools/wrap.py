"""CFuncHelper 的 setuptools 打包脚本（备选路径）。

主构建入口是同目录上一级的 Makefile：
    mingw32-make
本脚本只在没有 make 的环境里作为备用，例如：
    python tools/wrap.py build_ext --inplace --compiler=mingw32
"""

import os

import numpy
import setuptools

项目根目录 = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
源文件目录 = os.path.join(项目根目录, "source", "src")
头文件目录 = os.path.join(项目根目录, "source", "include")
源文件_文件名 = [
	"CFuncHelper.c",
	"docs.c",
	"Original_funcs.c",
]
源文件_引用路径 = [os.path.join(源文件目录, 单个文件名) for 单个文件名 in 源文件_文件名]

模块 = setuptools.Extension(
	name="CFuncHelper",
	sources=源文件_引用路径,
	include_dirs=[numpy.get_include(), 头文件目录],
)
文档 = """
	该模块保存了C语言常用函数的python调用方法，\n
	调用这些函数时将提供这些函数所处的头文件信息。\n
	"""
setuptools.setup(
	name="CFuncHelper",
	description=文档,
	ext_modules=[模块],
)
