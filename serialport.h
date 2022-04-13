#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QWidget>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTime>
#include <QMessageBox>
#include <QVector>
#include <QPair>
#include <QFile>

namespace Ui {
class SerialPort;
}

class SerialPort : public QWidget
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
    void on_save_button_clicked();
    void on_load_button_clicked();



private:
    Ui::SerialPort *ui;
    QSerialPort *serialport;
    bool textstate_receive;
    bool textstate_send;
    QByteArray serial_buffer;

    void Read_Data();       //读取串口数据
    void find_port();       //查找可用串口
    void sleep(int msec);      //延时函数
    void parse_data();
    int check_51();

//    QVector<QVector<double>> dof_result;// 按顺序是三轴角度，
//    QVector<QDateTime> time_result;
    QDateTime temp_time;
    QMap<QDateTime,QVector<double>> angle_speed;
    QMap<QDateTime,QVector<double>> acc;
    QMap<QDateTime,QVector<double>> angle;
    QMap<QDateTime,QVector<double>> magnetism;

};

#endif // SERIALPORT_H
