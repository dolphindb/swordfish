# Embedded DolphinDB Demo

利用动态库libEmbeddedDolphinDB.so来实现各种对数据库的操作。

## 1. 编译

```bash
cd path/to/demo
mkdir build
cd build
cmake ..
make
```

生成的可执行文件在bin目录下。依赖的动态库在lib目录下


## 2. 新增demo

在demo目录下创建新的目录，可以参考demo/baseDemo的目录。同时要在外层CmakeLists.txt中增加add_subdirectory

## 3. 运行时注意事项
1. asset目录下的dolphindb.lic为社区试用版，需替换为商业版license才可正常使用
2. libEmbeddedDolphinDB.so依赖libopenblas.so libgfortran.so libquadmath.so，如果在运行demo时找不到库，则需要指定环境变量 **LD_LIBRARY_PATH** 到库的路径下
