文件名="编译打包信息.txt"
文件=open(文件名,"r",encoding="ANSI")
内容=文件.readlines()
for i in range(len(内容)):
	if (内容[i].strip()=="CCall.c"):
		报错信息=内容[i:]
		break
	else:
		pass

for i in range(len(报错信息)):
	if ((报错信息[i].strip()).find("creating")!=-1):
		报错信息=报错信息[:i]
		break
	else:
		pass
print("".join(报错信息))