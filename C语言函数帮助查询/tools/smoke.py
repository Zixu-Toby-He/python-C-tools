"""CFuncHelper 冒烟测试。

在项目根目录下运行：
    mingw32-make test        # 先构建，再跑本脚本
    python tools/smoke.py    # 已经构建过时可以直接跑

脚本会验证模块能否导入、每个函数的基本行为、以及 qsort 的边界情况，
全部通过时退出码为 0，否则为 1。
"""

import os
import sys

项目根目录 = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(项目根目录, "build"))

import CFuncHelper as m  # noqa: E402  （必须先改 sys.path 才能导入）

通过 = 0
失败 = []


def 检查(标题, 条件, 详情=""):
    global 通过
    if 条件:
        通过 += 1
        print("  [pass] " + 标题)
    else:
        失败.append(标题)
        print("  [FAIL] " + 标题 + ("  <- " + 详情 if 详情 else ""))


def 期望异常(标题, 异常类型, 函数):
    try:
        函数()
    except 异常类型 as 错误:
        检查(标题, True)
        return
    except Exception as 错误:  # 抛了别的类型也算不通过
        检查(标题, False, "抛出了 %s" % type(错误).__name__)
        return
    检查(标题, False, "没有抛异常")


def 升序(a, b):
    return (a > b) - (a < b)


print("== 导入 ==")
检查("模块名", m.__name__ == "CFuncHelper")
检查("模块文档非空", bool(m.__doc__))
检查(
    "成员齐全",
    {"puts", "system", "qsort", "silent", "unsilent"} <= set(dir(m)),
    str([n for n in ("puts", "system", "qsort", "silent", "unsilent") if not hasattr(m, n)]),
)
检查("中文字符串按 UTF-8 解码", "头文件" in m.puts.__doc__)

print("== 提示信息开关 ==")
m.silent()      # 后面不再打印头文件提示，输出更干净
检查("silent 可调用", True)

print("== puts / system ==")
检查("puts 返回 0", m.puts("smoke: puts") == 0)
检查("system 返回 0", m.system("echo smoke: system") == 0)

print("== qsort：list ==")
lst = [3, 1, 2]
返回值 = m.qsort(lst, len(lst), 8, 升序)
检查("原地排序", lst == [1.0, 2.0, 3.0], repr(lst))
检查("返回 None", 返回值 is None)

lst = [5, 9, 1]
m.qsort(lst, len(lst), 8, lambda a, b: (b > a) - (b < a))
检查("降序回调", lst == [9.0, 5.0, 1.0], repr(lst))

lst = [2, 1, 4, 3]
m.qsort(lst, 2, 8, 升序)
检查("只排序前 length 个", lst == [1.0, 2.0, 4.0, 3.0], repr(lst))

lst = [1.5, -2.0, 0.0]
m.qsort(lst, len(lst), 8, 升序)
检查("浮点数排序", lst == [-2.0, 0.0, 1.5], repr(lst))

print("== qsort：tuple ==")
结果 = m.qsort((3, 1, 2), 3, 8, 升序)
检查("tuple 返回新 list", isinstance(结果, list), repr(结果))
检查("tuple 排序结果", 结果 == [1.0, 2.0, 3.0], repr(结果))

print("== qsort：numpy.ndarray ==")
try:
    import numpy as np

    数组 = np.array([3.0, 1.0, 2.0])
    m.qsort(数组, 3, 8, 升序)
    检查("ndarray 原地排序", list(数组) == [1.0, 2.0, 3.0], repr(数组))
    检查("ndarray 类型不变", 数组.dtype == np.float64, str(数组.dtype))

    整数数组 = np.array([30, 10, 20], dtype=np.int64)
    m.qsort(整数数组, 3, 8, 升序)
    检查("整数 ndarray 原地排序", list(整数数组) == [10, 20, 30], repr(整数数组))

    只读数组 = np.array([2.0, 1.0])
    只读数组.flags.writeable = False
    期望异常("只读 ndarray 报 ValueError", ValueError,
             lambda: m.qsort(只读数组, 2, 8, 升序))
except ImportError:
    检查("numpy 可用", False, "缺少 numpy")

print("== qsort：错误处理 ==")
期望异常("长度超界 -> ValueError", ValueError,
         lambda: m.qsort([1, 2], 5, 8, 升序))
期望异常("长度为负 -> ValueError", ValueError,
         lambda: m.qsort([1, 2], -1, 8, 升序))
期望异常("元素字节数非法 -> ValueError", ValueError,
         lambda: m.qsort([1, 2], 2, 3, 升序))
期望异常("比较函数不可调用 -> TypeError", TypeError,
         lambda: m.qsort([1, 2], 2, 8, "不是函数"))
期望异常("数组类型不支持 -> TypeError", TypeError,
         lambda: m.qsort(123, 1, 8, 升序))


def 会抛异常(a, b):
    raise RuntimeError("比较回调故意失败")


原数组 = [3, 1, 2]
期望异常("回调异常原样抛出", RuntimeError,
         lambda: m.qsort(原数组, len(原数组), 8, 会抛异常))
检查("回调出错时原数组未被改动", 原数组 == [3, 1, 2], repr(原数组))


def 返回非数值(a, b):
    return "不是数字"


期望异常("回调返回值不可转换 -> TypeError", TypeError,
         lambda: m.qsort([1, 2], 2, 8, 返回非数值))

m.unsilent()

print("")
print("通过 %d 项，失败 %d 项" % (通过, len(失败)))
if 失败:
    for 项 in 失败:
        print("  - " + 项)
    sys.exit(1)
print("全部通过")
