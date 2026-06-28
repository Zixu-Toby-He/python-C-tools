echo off
cls
if exist .\运行环境 call 清除编译文件.bat
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
copy ..\Python辅助文件\运行.py .
copy .\build\lib.win-amd64-cpython-39\CCall.cp39-win_amd64.pyd .
echo -----------------------------------------------
echo "                    "
if exist CCall.cp39-win_amd64.pyd (
        echo 编译过程已完成，请查看编译信息
) else (
       echo 编译失败，请重新编辑程序后重新编译
)
echo "                    "
echo "                    "
echo "                    "
echo "                    "
cd ..
pause