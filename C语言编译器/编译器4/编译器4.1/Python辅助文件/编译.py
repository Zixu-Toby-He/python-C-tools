import setuptools

模块=setuptools.Extension("CCall",sources=[".\C语言文件\CCall.c",".\C语言文件\CMain.c"])
setuptools.setup(name="C语言试验包",description="这是利用C语言建立的包",ext_modules=[模块])