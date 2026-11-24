# =============================================================================
#  CFuncHelper —— Python C 扩展构建脚本
#
#  构建工具：mingw32-make（GNU Make 4.x，MinGW-w64 自带）
#  编译器  ：MinGW-w64 gcc.exe（不需要 Visual Studio / MSVC）
#  解释器  ：系统 PATH 里的 python.exe
#  配方外壳：PowerShell 5.1（Windows PowerShell）
#
#  常用命令：
#    mingw32-make               编译 + 链接，产出 build/CFuncHelper.pyd
#    mingw32-make test          构建后做一次导入冒烟测试
#    mingw32-make clean         删除 build 目录
#    mingw32-make rebuild       先 clean 再 all
#    mingw32-make info          打印探测到的工具链、解释器与路径
#    mingw32-make PYTHON=xxx    指定其它解释器（默认用系统的 python）
#
#  两条硬约定（改文件之前先看）：
#   1. 配方行只能出现 ASCII 相对路径。
#      make 把命令行交给系统时按 ANSI 代码页转换，中文会被转成乱码从而
#      定位失败（已实测：配方里 Test-Path 中文目录返回 False）。
#      所以源码目录、文件名、输出目录都必须保持 ASCII，本文件里也不能
#      出现中文命令行。注释可以用中文，因为注释不进命令行。
#   2. 源文件一律以 UTF-8 保存。
#      -finput-charset=UTF-8 显式声明 .c/.h 的输入编码；
#      -fexec-charset=UTF-8 让字符串字面量里的中文以 UTF-8 写进 .pyd，
#      与 Python 读取文档字符串的默认编码一致。
# =============================================================================

# ---- 配方外壳：PowerShell ---------------------------------------------------
SHELL       := powershell.exe
.SHELLFLAGS := -NoProfile -Command

# ---- 构建目标与工具 ---------------------------------------------------------
.DEFAULT_GOAL := all
.DELETE_ON_ERROR:

CC     := gcc
PYTHON ?= python

BUILD  := build
OBJDIR := $(BUILD)/obj
MODULE := CFuncHelper
TARGET := $(BUILD)/$(MODULE).pyd

SRCDIR  := source/src
INCDIR  := source/include
SOURCES := $(SRCDIR)/CFuncHelper.c $(SRCDIR)/docs.c $(SRCDIR)/Original_funcs.c
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))
HEADERS := $(wildcard $(INCDIR)/*.h)

# ---- 从系统 python 探测解释器信息 -------------------------------------------
# 统一换成 / 分隔：既避开反斜杠转义，也让 gcc 的 -I / -L 参数稳定
PY_VER     := $(shell $(PYTHON) -c "import sys;print('%d%d' % sys.version_info[:2])")
PY_INCLUDE := $(shell $(PYTHON) -c "import sysconfig,os;print(sysconfig.get_path('include').replace(os.sep,'/'))")
PY_LIBDIR  := $(shell $(PYTHON) -c "import sysconfig,os;print(os.path.join(sysconfig.get_config_var('installed_base'),'libs').replace(os.sep,'/'))")
NUMPY_INC  := $(shell $(PYTHON) -c "import os,numpy;print(numpy.get_include().replace(os.sep,'/'))")

# ---- 编译 / 链接参数 --------------------------------------------------------
CPPFLAGS := -I$(INCDIR) -I"$(PY_INCLUDE)" -I"$(NUMPY_INC)"
CFLAGS   := -O2 -Wall -std=gnu11 -finput-charset=UTF-8 -fexec-charset=UTF-8
LDFLAGS  := -shared
LDLIBS   := -L"$(PY_LIBDIR)" -lpython$(PY_VER)

.PHONY: all test clean rebuild info

# ---- 默认目标 ---------------------------------------------------------------
all: $(TARGET)
	@Write-Host "[ok] $<"

# ---- 输出目录 ---------------------------------------------------------------
$(BUILD) $(OBJDIR):
	@New-Item -ItemType Directory -Force '$@' | Out-Null

# ---- 编译（每个 .c 单独出 .o） ----------------------------------------------
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
	@Write-Host "[cc] $<"
	$(CC) $(CPPFLAGS) $(CFLAGS) -c '$<' -o '$@'

# ---- 链接（产出 .pyd） ------------------------------------------------------
$(TARGET): $(OBJECTS) | $(OBJDIR)
	@Write-Host "[ld] $@"
	$(CC) $(LDFLAGS) -o '$@' $(OBJECTS) $(LDLIBS)

# ---- 冒烟测试 ---------------------------------------------------------------
test: $(TARGET)
	@Write-Host "[test] $(PYTHON) tools/smoke.py"
	$(PYTHON) tools/smoke.py

# ---- 清理 / 重建 / 信息 ------------------------------------------------------
clean:
	@if (Test-Path '$(BUILD)') { Remove-Item -Recurse -Force '$(BUILD)' }
	@Write-Host "[clean] $(BUILD)/ removed"

rebuild: clean
	@$(MAKE) --no-print-directory all

info:
	@Write-Host "make    : $(MAKE) $(MAKE_VERSION)"
	@Write-Host "gcc     : $(CC)"
	$(CC) --version | Select-Object -First 1
	@Write-Host "python  : $(PYTHON)"
	$(PYTHON) -c "import sys;print('  version :',sys.version.split()[0]);print('  exe     :',sys.executable)"
	@Write-Host "  include : $(PY_INCLUDE)"
	@Write-Host "  libs    : $(PY_LIBDIR)"
	@Write-Host "numpy   : $(NUMPY_INC)"
	@Write-Host "target  : $(TARGET)"
