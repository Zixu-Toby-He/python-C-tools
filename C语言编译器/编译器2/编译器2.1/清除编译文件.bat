echo off
cls
if exist .\运行环境 (
        echo "                    "
        echo "                    "
        echo "                    "
        echo 正在删除上一次编译结果
        rmdir /s .\运行环境
        echo 删除完成
) else (
        echo 上一次编译结果不存在，可以进行这一次编译
)

pause