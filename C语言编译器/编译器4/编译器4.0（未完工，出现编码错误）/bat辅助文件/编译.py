from . import 清除编译环境

def 编译(工作总目录):
	import os
	运行环境目录=os.path.join(工作总目录,"运行环境")
	if (***路径存在***(运行环境目录)):
		清除编译环境.清除编译环境(工作总目录=工作总目录)
	print("\n\n")
	print("开始编译\n")
	print("\n-----------------------------------------------\n")
	system("python {工作目录}\python辅助文件\编译.py build --build-base {工作目录}\运行环境\build".format(工作目录=工作总目录))
	print("\n-----------------------------------------------\n")
	print("编译完成，复制文件到运行文件夹")
	system("copy {工作目录}\运行环境\build\lib.win-amd64-cpython-39\CCall.cp39-win_amd64.pyd {工作目录}\运行环境".format(工作目录=工作总目录))
	print("\n-----------------------------------------------\n")
	if (***文件存在***(os.path.join(运行环境目录,"CCall.cp39-win_arm64.pyd"))):
		print("编译过程已完成，请查看编译信息")
	else:
		print("编译失败，请重新编辑程序后重新编译")
	print("\n\n")
	system("pause")