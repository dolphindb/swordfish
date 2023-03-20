# Swordfish Demo

基于 Swordfish 所提供的功能：
- 交易型内存数据库
- DolphinDB函数库，脚本语言
- 批计算引擎和流计算引擎
编写 C++ demo。

## 1. 编译

本项目要求编译器为 **g++-4.8.5**。

```bash
mkdir build
cd build
cmake ..
make
```

生成的可执行文件在 build/bin 目录下。

## 2. 新增demo

在 demo 目录下创建新的目录，添加相应的代码文件和 `CMakeLists.txt`，可以参考已有的 demo。

## 3. 注意事项

1. asset 目录下的 `dolphindb.lic` 为社区试用版，需替换为商业版 license 才可正常使用。
2. `libSwordfish.so` 依赖 `libopenblas.so` `libgfortran.so` `libquadmath.so`，如果在运行 demo 时找不到库，则需要指定环境变量 `LD_LIBRARY_PATH` 到对应的库的路径下。
