echo off
cls
if exist .\build (
        echo "                    "
        echo "                    "
        echo "                    "
        echo 正在删除上一次编译结果
        rmdir /s build
        del .\Python辅助文件\CCall.cp39-win_amd64.pyd
        echo 删除完成
) else (
        echo 编译结果不存在
)

pause