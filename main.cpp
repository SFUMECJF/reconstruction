//c++
#include <iostream>

// qt
#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <thread>
#include <QTime>

// mainwindow
#include "mainwindow.h"

// opencv
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

// serial port
#include "serialport.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
   cout << "Hello Qt-CMake!" << endl;

   QApplication a(argc, argv);
//
   MainWindow mainWin;
   mainWin.show();

//   Mat image(Size(100, 100), CV_8UC1);
//   imshow("black",image);

   return a.exec();
}
