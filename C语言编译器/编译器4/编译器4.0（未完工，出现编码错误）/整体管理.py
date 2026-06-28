import os
import sys

def pause():
	os.system("pause")
def cls():
	os.system("cls")

指令总数=4
指令名称={
	1	:	"编译运行一条龙",
	2	:	"运行",
	3	:	"编译",
	4	:	"清除编译文件",
	0	:	"退出"
	}

指令内容={
	1	:	r"call .\bat辅助文件\编译运行一体.bat",
	2	:	r"call .\bat辅助文件\运行.bat",
	3	:	r"call .\bat辅助文件\编译.bat",
	4	:	r"call .\bat辅助文件\清除编译文件.bat",
	0	:	r"exit"
	}

def 从窗口获取指令():
	print("请选择操作操作方式")
	print("")
	for 指令编号 in 指令名称:
		print("\t<{编号}> {指令名称}".format(编号=指令编号, 指令名称=指令名称[指令编号]))
	print("")
	输入值=(input("请输入：")).lower()
	输入转指令={
		"1"						:	1,
		"<1>"					:	1,
		"< 1 >"					:	1,
		"<1>编译运行一条龙"		:	1,
		"<1> 编译运行一条龙"		:	1,
		"< 1 > 编译运行一条龙"	:	1,
		"<1>编译运行一体"			:	1,
		"<1> 编译运行一体"		:	1,
		"< 1 > 编译运行一体"		:	1,
		"编译运行一条龙"			:	1,
		"编译运行"				:	1,
		"编译运行一体"			:	1,
		"compile and run"		:	1,
		"compileand run"		:	1,
		"compile andrun"		:	1,
		"compile & run"			:	1,
		"compile&run"			:	1,
		"cr"					:	1,

		"2"						:	2,
		"<2>"					:	2,
		"< 2 >"					:	2,
		"<2>运行"				:	2,
		"<2> 运行"				:	2,
		"< 2 > 运行"				:	2,
		"运行"					:	2,
		"run"					:	2,

		"3"						:	3,
		"<3>"					:	3,
		"< 3 >"					:	3,
		"<3>编译"				:	3,
		"<3> 编译"				:	3,
		"< 3 > 编译"				:	3,
		"编译"					:	3,
		"compile"				:	3,

		"4"						:	4,
		"<4>"					:	4,
		"< 4 >"					:	4,
		"<4>清理编译环境"			:	4,
		"<4> 清理编译环境"		:	4,
		"< 4 > 清理编译环境"		:	4,
		"清理编译环境"			:	4,
		"clear"					:	4,
		"clearup"				:	4,

		"0"						:	0,
		"<0>"					:	0,
		"< 0 >"					:	0,
		"<0>退出"				:	0,
		"<0> 退出"				:	0,
		"< 0 > 退出"				:	0,
		"退出"					:	0,
		"exit"					:	0,
		"quit"					:	0,
		"exit()"				:	0
		}
	try:
		return 输入转指令[输入值]
	except:
		print("输入值“{}”无法转化操作指令。请输入{}中任意一个数字".format(输入值, list(range(1,指令总数+1))))
		pause()
		raise

def 从命令行获取指令():
	try:
		return int(sys.argv[1])
	except:
		raise ValueError("参数“{}”无法转化操作指令。".format(sys.argv[1]))

def 获取操作指令():
	if (len(sys.argv)==1):
		return 从窗口获取指令()
	elif (len(sys.argv)==2):
		return 从命令行获取指令()
	else:
		raise ValueError("命令行参数过多，仅需1个参数")

def 获取当前绝对路径():
	return os.getcwd()

def 获取编译环境路径(编译环境上一步路径):
	return os.path.join(编译环境上一步路径, "运行环境")

def main():
	是否操作=True
	while(是否操作):
		操作指令=获取操作指令()
		是否操作=bool(操作指令)
		if (是否操作):
			cls()
			print("现在开始执行“<{编号}> {指令名称}”操作".format(编号=操作指令, 指令名称=指令名称[操作指令]))
			os.system(指令内容[操作指令])
			pause()
			cls()
		else:
			exit()

if __name__=="__main__":
	main()