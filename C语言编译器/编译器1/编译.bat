echo off
cls
if exist .\build (
        echo "                    "
        echo "                    "
        echo "                    "
        echo 正在删除上一次编译结果
        rmdir /s build
        del .\Python辅助文件\CCall.cp39-win_amd64.pyd
        echo 已删除
)
echo "                    "
echo "                    "
echo "                    "
echo 开始编译
echo "                    "
echo -----------------------------------------------
python .\Python辅助文件\编译.py build
echo "                    "
echo 编译完成，复制文件到运行文件夹
copy .\build\lib.win-amd64-cpython-39\CCall.cp39-win_amd64.pyd .\Python辅助文件
echo -----------------------------------------------
echo "                    "
echo 编译过程已完成，请查看编译信息
echo "                    "
echo "                    "
echo "                    "
echo "                    "

pause