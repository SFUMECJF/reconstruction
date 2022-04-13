cmake_minimum_required(VERSION 3.5)
project (reconstruction)

# 1 Qt
#注意，在一个大工程里，这几个开关一定要尽量放在前面打开，否则有可能会报无法生成ui文件的错误
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#注意，在一个大工程里，这几个开关一定要尽量放在前面打开，否则有可能会报无法生成ui文件的错误

#为了find_package成功执行，需要提前设置去哪里找qt，所以要设置路径
# 这里需要了解CMake找包时候的顺序。https://stackoverflow.com/questions/50764721/how-to-set-qt5-path-with-cmake-find-package-on-windows#:~:text=You%20can%20set%20click%20the,0%20is%20the%20Qt%20version.
# 先找cache variables: CMAKE_PREFIX_PATH, CMAKE_FRAMEWORK_PATH, CMAKE_APPBUNDLE_PATH
# 再找环境变量里可能存在的路径：比如Qt_DIR
# 再从环境变量里找路径
set(CMAKE_PREFIX_PATH "D:/Qt5.12.11/5.12.11/msvc2017_64")
#set(CMAKE_PREFIX_PATH "D:\\Qt5.12.11\\5.12.11\\mingw73_64\\")# D:\Qt5.12.11\5.12.11\mingw73_64
#set(CMAKE_PREFIX_PATH "~/Qt5.9.9/5.9.9/gcc_64/")#为了不在Qt creator这个软件中也能CMake找到Qt的库
# D:\Qt5.12.11\5.12.11\msvc2017_64\lib 在路径里储存了所有的库，用到的都需要添加。
find_package(Qt5 COMPONENTS Widgets Gui SerialPort REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)

# 2 find open cv
#SET(OpenCV_DIR "D:/Programs/opencv/opencv/build/x64/vc15/lib")#先设置好这个目录，这回就找对opencvconfig.cmake了。
#find_package(OpenCV CONFIG REQUIRED) #


#include_directories(${OpenCV_INCLUDE_DIRS})
#link_directories(${OpenCV_LIB_DIR})

# add exe  如果想在其他ide里使用，需要把所有的.qrc和.ui文件都包含进去
add_executable(reconstruction main.cpp mainwindow.cpp mainwindow.h mainwindow.ui serialport.h serialport.cpp serialport.ui)
# change the codes, add the neeeded libs.
target_link_libraries(reconstruction Qt5::Core Qt5::Widgets Qt5::Gui Qt5::SerialPort)

#target_link_directories(reconstruction PUBLIC
#    ${OpenCV_LIB_DIRS}
#    )
#target_link_libraries(reconstruction
#    ${OpenCV_LIBS}
#    )
#target_include_directories(reconstruction PUBLIC
#    ${OpenCV_INCLUDE_DIRS}
#    )

MESSAGE("--------------------")
# qt官网找到的一些成功found到包之后的一些可能创建好的变量。
MESSAGE("Qt5Widgets_INCLUDE_DIRS : ${Qt5Widgets_INCLUDE_DIRS}")
MESSAGE("Qt5Widgets_LIBRARIES : ${Qt5Widgets_LIBRARIES}")
MESSAGE("Qt5Widgets_VERSION_STRING : ${Qt5Widgets_VERSION_STRING}")
MESSAGE("Qt5Widgets_PRIVATE_INCLUDE_DIRS : ${Qt5Widgets_PRIVATE_INCLUDE_DIRS}")
#MESSAGE("OpenCV version : ${OpenCV_VERSION}")
#MESSAGE("OpenCV_LIB_DIRS : ${OpenCV_LIB_DIRS}")
#MESSAGE("OpenCV_LIBS : ${OpenCV_LIBS}")
#MESSAGE("OpenCV_DIR : ${OpenCV_DIR}")
#MESSAGE("OpenCV_LIBRARIES : ${OpenCV_LIBRARIES}")
#MESSAGE("OpenCV_INCLUDE_DIRS : ${OpenCV_INCLUDE_DIRS}")
MESSAGE("--------------------")
MESSAGE("C++ compiler： ${CMAKE_CXX_COMPILER}")
MESSAGE("-----complete cmake -----")
