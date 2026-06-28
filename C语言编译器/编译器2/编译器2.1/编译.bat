echo off
cls
if exist .\运行环境 (
        echo "                    "
        echo "                    "
        echo "                    "
        echo 正在删除上一次编译结果
        rmdir /s .\运行环境
        echo 已删除
)
echo "                    "
echo "                    "
echo "                    "
echo 开始编译
echo "                    "
echo -----------------------------------------------
mkdir .\运行环境
cd .\运行环境
python ..\Python辅助文件\编译.py build
echo "                    "
echo 编译完成，复制文件到运行文件夹
copy .\build\lib.win-amd64-cpython-39\CCall.cp39-win_amd64.pyd .
copy ..\Python辅助文件\运行.py .
echo -----------------------------------------------
echo "                    "
echo 编译过程已完成，请查看编译信息
echo "                    "
echo "                    "
echo "                    "
echo "                    "

pause