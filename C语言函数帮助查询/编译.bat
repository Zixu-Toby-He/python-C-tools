@echo off
rem chcp 65001
if exist .\编译文件 (
    echo=
    echo=
    echo=
    echo 正在删除上一次编译结果
    rmdir /s /q .\编译文件
    echo 删除完成
) else (
    echo 上一次编译结果不存在，可以进行这一次编译
)
echo=
echo=
echo=
echo 开始编译
echo=
echo -----------------------------------------------
rem python 打包.py build --build-base 编译文件
python 打包.py build_ext --build-lib 编译文件
rem python 打包.py build_ext 
echo=
echo 编译完成
rem echo 编译完成，复制文件到运行文件夹
rem copy .\Python辅助文件\运行.py .\运行环境
rem copy .\运行环境\build\lib.win-amd64-cpython-39\CCall.cp39-win_amd64.pyd .\运行环境
echo -----------------------------------------------
echo=
rem if exist .\运行环境\CCall.cp39-win_amd64.pyd (
rem     echo 编译过程已完成，请查看编译信息
rem ) else (
rem    echo 编译失败，请重新编辑程序后重新编译
rem )
echo=
echo=
echo=
echo=
pause