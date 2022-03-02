# reconstruction

##
`main` 分支是在qt中编写的，功能不全。
`product`是产品分支，完整了某一阶段的功能。
`clion`分支是在clion中引用qt库和opencv库写的，具体配置步骤在CSDN。

## 在CMake中包含Open CV库

首先，把动态库的路径包含到环境变量。

其次，在CMake中find package，并且链接头文件路径，库文件路径，以及库文件。

```cmake
find_package(OpenCV CONFIG REQUIRED PATHS)

target_link_directories(reconstruction PUBLIC
    ${OpenCV_LIB_DIRS}
    )
target_link_libraries(reconstruction
    ${OpenCV_LIBS}
    )
target_include_directories(reconstruction PUBLIC
    ${OpenCV_INCLUDE_DIRS}
    )
```

## 2022.0203
今天把所有的加速度和角度数据解算出来了。和时间对应成功。磁的数据用来解算z轴用的。已经用在传过来的角度值上了，磁数据没必要再用了。
代码任务：
-- 这个调试助手要修改为我自己用的调试助手。文本框显示应该是时间：三个参数，三个值。
-- 然后我的输出可能需要独特地设置为初始化坐标。
-- 打包到笔记本上？打包之后如何调试呢？远程修改代码，再传回来？



