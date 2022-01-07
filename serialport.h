#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QDialog>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTime>
#include <QMessageBox>

namespace Ui {
class SerialPort;
}

class SerialPort : public QDialog
{
    Q_OBJECT

public:
    explicit SerialPort(QWidget *parent = nullptr);
    ~SerialPort();

private slots:
    void on_send_button_clicked();
    void on_open_port_clicked();
    void on_close_port_clicked();
    void on_clear_button_1_clicked();
    void on_clear_button2_clicked();
    void on_receive_modl_clicked();
    void on_send_modl_clicked();



private:
    Ui::SerialPort *ui;
    QSerialPort *serialport;
    bool textstate_receive;
    bool textstate_send;

    void Read_Data();       //读取串口数据
    void find_port();       //查找可用串口
    void sleep(int msec);      //延时函数
};

#endif // SERIALPORT_H
