#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialport.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_pressed()
{
    ui->label_2->setText(ui->lineEdit->text());

}

void MainWindow::on_actionSerialPort_triggered()
{
    serialPort.show();
}

