# python-C-tools

不想装 Visual Studio 集成开发工具、又想用 C 语言给 Python 写扩展时用的小工具集合。

目前只有一个子项目：[`C语言函数帮助查询`](C语言函数帮助查询) —— 把几个常用 C 函数
包成 Python 模块 `CFuncHelper`，调用时顺带告诉你这个函数在哪个头文件里，方便对照手册。

## 目录结构

```
python-C-tools/
├─ README.md
├─ LICENSE
└─ C语言函数帮助查询/
   ├─ Makefile              构建入口（mingw32-make + gcc，配方由 PowerShell 执行）
   ├─ source/
   │  ├─ include/           docs.h、Original_funcs.h
   │  └─ src/               CFuncHelper.c、docs.c、Original_funcs.c
   ├─ tools/
   │  ├─ smoke.py           冒烟测试（make test 调用）
   │  └─ wrap.py            setuptools 备用打包脚本（平时用不到）
   └─ build/                构建产物，已 git 忽略
      ├─ obj/*.o
      └─ CFuncHelper.pyd
```

## 构建

构建入口是 [`C语言函数帮助查询/Makefile`](C语言函数帮助查询/Makefile)：
由 MinGW-w64 的 `mingw32-make` 读取，用 `gcc` 直接编译链接出 `CFuncHelper.pyd`，
每行配方交给 PowerShell 执行。不需要 Visual Studio，也不需要批处理脚本。

```powershell
cd C语言函数帮助查询
mingw32-make            # 编译 + 链接 -> build/CFuncHelper.pyd
mingw32-make test       # 构建后跑 tools/smoke.py 冒烟测试
mingw32-make clean      # 删除 build/
mingw32-make rebuild    # clean + all
mingw32-make info       # 打印探测到的 gcc / python / 头文件和库路径
```

前置条件：

- `gcc.exe`、`mingw32-make.exe`（MinGW-w64）在 PATH 里；
- 系统 `python`（默认取 PATH 里的 `python.exe`，需装 numpy），
  用 `mingw32-make PYTHON=<别的解释器>` 可以覆盖。

## 使用

```python
import sys

sys.path.insert(0, r"...\C语言函数帮助查询\build")
import CFuncHelper

CFuncHelper.puts("hello")          # 打印 hello，返回 0
CFuncHelper.system("ver")          # 交给系统执行一条命令
CFuncHelper.silent()               # 之后不再打印「函数所在头文件」的提示
CFuncHelper.unsilent()             # 恢复提示

lst = [3, 1, 2]
CFuncHelper.qsort(lst, len(lst), 8, lambda a, b: (a > b) - (a < b))
print(lst)                         # [1.0, 2.0, 3.0]
```

模块和每个函数的 `__doc__` 里有完整的参数、返回值、异常与示例说明：

```python
print(CFuncHelper.__doc__)
print(CFuncHelper.qsort.__doc__)
```

## API 一览

| 函数 | 头文件 | 说明 |
| --- | --- | --- |
| `puts(s)` | `<stdio.h>` | 输出字符串并换行，返回 0 或 EOF(-1) |
| `system(cmd)` | `<stdlib.h>` | 执行系统命令，返回退出状态 |
| `qsort(array, length, size, cmp)` | `<stdlib.h>` | 用 C 的 qsort 排序，比较交给 Python 回调；list/ndarray 原地改，tuple 返回新 list |
| `silent()` | — | 关闭各函数的提示信息 |
| `unsilent()` | — | 恢复提示信息 |

## 两条硬约定

改目录结构或改源码前请先看 `Makefile` 顶部注释，核心是这两条：

1. **路径必须是 ASCII**。make 把命令行交给系统时按 ANSI 代码页转换，中文会变成
   乱码从而定位失败（已实测），所以源码目录、文件名、输出目录都不能带中文。
   注释可以用中文，因为注释不进命令行。
2. **源文件一律 UTF-8**。编译时用 `-finput-charset=UTF-8` 显式指定输入编码，
   `-fexec-charset=UTF-8` 保证中文字符串以 UTF-8 写进 `.pyd`，与 Python 读取
   文档字符串的默认编码一致。

## 说明

- `编译.bat` 已删除；旧版本设想的 `build_c/`、`build_py/` 两个空目录也已删除，
  构建产物统一放在 `build/`。
- `tools/wrap.py` 是 setuptools 的备用打包路径（原 `打包.py`），平时用不到：
  `python tools/wrap.py build_ext --inplace --compiler=mingw32`。
- 本机若由受限沙箱启动，项目子目录可能对当前用户不可写，导致构建失败；
  在自己的终端里直接运行 `mingw32-make` 不受影响。
