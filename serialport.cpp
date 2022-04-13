#include "serialport.h"
#include "ui_serialport.h"

SerialPort::SerialPort(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPort)
{
    ui->setupUi(this);

    textstate_receive = false;
    textstate_send = false;
    serial_buffer = "";
    angle.clear();
    angle_speed.clear();
    acc.clear();
    magnetism.clear();

    setWindowTitle("医学图像调试助手");
    serialport = new QSerialPort;
    find_port();                    //查找可用串口

    ui->send_button->setEnabled(false);     //设置控件不可用
    ui->close_port->setEnabled(false);

    // 如果名字已经是on_控件名_信号，这样的名字，那么就不需要重复连接。如果重复输入的话，只会令按下信号之后，执行两次函数。
//    connect(this->ui->send_button, SIGNAL(clicked(bool)), this, SLOT(on_send_button_clicked()));
//    connect(this->ui->save_button, SIGNAL(clicked(bool)), this, SLOT(on_save_button_clicked()));
//    connect(this->ui->load_button, SIGNAL(clicked(bool)), this, SLOT(on_load_button_clicked()));
}

SerialPort::~SerialPort()
{
    delete ui;
}

// return the first index followed by 10 valid bytes
int SerialPort::check_51() {
//  qDebug() << hex << serial_buffer.toHex();

  for (int i = 0; i < serial_buffer.size(); i++) {
    if (serial_buffer.at(i) == QChar(0x55) && i + 11 < serial_buffer.size() && serial_buffer.at(i + 11) == QChar(0x55)) {
//      qDebug() << "i = " << i << "serial_buffer.at(i) = "<< hex << serial_buffer.at(i) << "serial_buffer.at(i + 1) = " << serial_buffer.at(i + 1);
      return i;
    }
  }

  return -1;
}
// 51, 52, 53的数据都需要 / 32768，不过再乘多少量程需要分别处理
double int2double(char high, char low) {
  // 注意，这里的数据是补码表示的负数的高4位和低4位，所以直接用位操作。

  unsigned char high_byte = high;
  unsigned char low_byte = low;
  quint16  result_uint = (high_byte << 8) | low_byte;
  qint16 result = *(qint16 *)&result_uint;
  return result / 32768.0;

}

/*
 * 每一次获取数据之后，在这里进行处理。
 * 目的：读取数据转换为 传感器的：  以及对应的时间。
 * 储存到一个二维列表里，最后输出到文件中。
 */
void SerialPort::Read_Data()
{
    QByteArray buf;
    buf = serialport->readAll();
    temp_time = QDateTime::currentDateTime();
    bool display = false;
    // this display operation will consume a lot of time!
    if(!buf.isEmpty() && display)          //将数据显示到文本串口
    {

        if(textstate_receive)   //文本模式
        {
            QString str = ui->Receive_text_window->toPlainText();
            str+=tr(buf);
//            str += "  ";
            ui->Receive_text_window->clear();
            ui->Receive_text_window->append(str);
        }
        if(!textstate_receive)   //Hex模式
        {
            QString str = ui->Receive_text_window->toPlainText();
            // byteArray 转 16进制
            QByteArray temp = buf.toHex();
            str+=tr(temp);
//            str += "  ";
            ui->Receive_text_window->clear();
            ui->Receive_text_window->append(str);
        }
    }

    serial_buffer += buf;
    buf.clear();    //清空缓存区

    parse_data();


}
QVector<double> parse_one_record(const QByteArray &data,const double & scope) {
  QVector<double> result;
  result.resize(3);
  result[0] = int2double(data[3], data[2]) * scope;
  result[1] = int2double(data[5], data[4]) * scope;
  result[2] = int2double(data[7], data[6]) * scope;
//  result[3] = int2double(data[9], data[8]);
  return result;
}
bool check_parity(const QByteArray & data, int index) {
  quint8 sum = 0;
  for (int i = index; i < index + 10; i++) {
    sum += data[i];
  }
  if ((quint8)data[index + 10] ==sum) {
    qDebug() << "sum : " << sum << "data[index + 10] : " << (quint8)data[index + 10];
    return true;
  }
  else {
    qDebug() << "sum : " << sum << "data[index + 10] : " << (quint8)data[index + 10];
    return false;
  }

}
/*
 * 修改 ：serial_buffer
 */
void SerialPort::parse_data() {
  // 循环选择可用的11个byte
  int index = check_51();
  while (index != -1) {
    //    qDebug() << "while index = " << index;

    // 解析数据，添加时间
    // 51Q 52R 53S 54T 55U serial_buffer.at(0) serial_buffer.at(1) serial_buffer.at(2) serial_buffer.at(3)
    // sum parity check, 绝大部分数据都是正确的。
//    if (!check_parity(serial_buffer, index)) {
//      qDebug() << "parity check failed  " << serial_buffer.toHex();
//
//    }
    if (serial_buffer.at(index + 1) == 0x53) {
      // parse angle

        angle[QDateTime::currentDateTime()] = parse_one_record(serial_buffer.mid(index), 180);
//        ui->
//        ui->
        ui->xangle->setText(QString::number(angle.last().value(0),'f', 3 ));
        ui->yangle->setText(QString::number(angle.last().value(1),'f', 3 ));
        ui->zangle->setText(QString::number(angle.last().value(2),'f', 3 ));
    } else if (serial_buffer.at(index + 1) ==0x52) {// 角速度  度/秒
       angle_speed[QDateTime::currentDateTime()] = parse_one_record(serial_buffer.mid(index), 2000);
       ui->xangle_speed->setText(QString::number(angle_speed.last().value(0), 'f', 3));
       ui->yangle_speed->setText(QString::number(angle_speed.last().value(1), 'f', 3));
       ui->zangle_speed->setText(QString::number(angle_speed.last().value(2), 'f', 3));

    } else if (serial_buffer.at(index + 1) == 0x51) {// 加速度 单位1g=9.8m/s2

      acc[QDateTime::currentDateTime()] = parse_one_record(serial_buffer.mid(index), 16);
      ui->xacc->setText(QString::number(acc.last().value(0), 'f', 3));
      ui->yacc->setText(QString::number(acc.last().value(1), 'f', 3));
      ui->zacc->setText(QString::number(acc.last().value(2), 'f', 3));

    } else if (serial_buffer.at(index + 1) == 0x54) {
      // magnetism is no use
    }

    // 删除前面的缓存，使用文本框里的内容作为整个原始数据
    serial_buffer = serial_buffer.mid(index + 11);
    index = check_51();
  }
//  auto i = angle.constBegin();
//  while (i != angle.constEnd()) {
//    qDebug() << i.key() << ": " << i.value();
//    ++i;
//  }
//
//  auto j = angle_speed.constBegin();
//  while (j != angle_speed.constEnd()) {
//    qDebug() << j.key() << ": " << j.value();
//    ++j;
//  }
//
//  auto k = acc.constBegin();
//  while (k != acc.constEnd()) {
//    qDebug() << k.key() << ": " << k.value();
//    ++k;
//  }



}

void save_file(const QMap<QDateTime,QVector<double>> & data, const QString & file_name) {
  QFile file(file_name+".txt");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qDebug() << "open file failed";
    return;
  }
  QTextStream out(&file);
  auto i = data.constBegin();
  while (i != data.constEnd()) {
        out << i.key().toLocalTime().toString() << " " << i.value()[0] << " " << i.value()[1] << " " << i.value()[2] << endl;
    ++i;
  }
  file.close();
}

void serialize_save(const QMap<QDateTime,QVector<double>> & data, const QString & file_name) {
  QFile file(file_name+".dat");
  if (!file.open(QIODevice::WriteOnly)) {
    QMessageBox::critical(nullptr, "错误", "保存文件失败" + file_name);
    return;
  }
  QDataStream out(&file);
  out << data;
  file.close();
}

void SerialPort::on_save_button_clicked() {
  save_file(angle, "angle");
  save_file(angle_speed, "angle_speed");
  save_file(acc, "acc");

  serialize_save(angle, "angle");
  serialize_save(angle_speed, "angle_speed");
  serialize_save(acc, "acc");

  //
  QMessageBox::information(this, "保存文件", "保存文件成功");

}

QMap<QDateTime, QVector<double>> load_onefile(const QString & file_name) {
  QFile file(file_name);
  QMap<QDateTime, QVector<double>> data;
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::critical(nullptr, "错误", "打开文件失败" + file_name);
  }
  else {
    QDataStream in(&file);    // read the data serialized from the file
    in >> data;
  }

  return data;
}

void SerialPort::on_load_button_clicked() {
  // test
  auto a = load_onefile("angle.dat");
  auto b = load_onefile("acc.dat");
  auto c = load_onefile("angle_speed.dat");

    auto i = a.constBegin();
    while (i != a.constEnd()) {
      qDebug() << i.key() << ": " << i.value();
      ++i;
    }
    qDebug() << "-----------------";
    i = b.constBegin();
    while (i != b.constEnd()) {
      qDebug() << i.key() << ": " << i.value();
      ++i;
    }
    qDebug() << "-----------------";

    i = c.constBegin();
    while (i != c.constEnd()) {
      qDebug() << i.key() << ": " << i.value();
      ++i;
    }
    QMessageBox::information(this, "加载", "加载成功");
}


//查找串口
void SerialPort::find_port()
{
    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);   //设置串口
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->com->addItem(serial.portName());        //显示串口name
            serial.close();
        }
    }
}

////延时函数
void SerialPort::sleep( int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


//发送数据
void SerialPort::on_send_button_clicked()
{
    if(textstate_send == true)  //文版模式
    {
        serialport->write(ui->send_text_window->toPlainText().toLatin1());
    }

    if(textstate_send == false)     //16进制
    {
        QString str = ui->send_text_window->toPlainText();
        int num = str.toInt();
        str = str.setNum(num,16);
        ui->send_text_window->clear();
        ui->send_text_window->append(str);
        serialport->write(ui->send_text_window->toPlainText().toLatin1());
    }


}
//打开串口
void SerialPort::on_open_port_clicked()
{
   update();
   sleep(100);      //延时100ms
   find_port();     //重新查找com
    //初始化串口
        serialport->setPortName(ui->com->currentText());        //设置串口名
        if(serialport->open(QIODevice::ReadWrite))              //打开串口成功
        {
            serialport->setBaudRate(ui->baud->currentText().toInt());       //设置波特率
            switch(ui->bit->currentIndex())                   //设置数据位数
            {
                case 8:serialport->setDataBits(QSerialPort::Data8);break;
                default: break;
            }
            switch(ui->jiaoyan->currentIndex())                   //设置奇偶校验
            {
                case 0: serialport->setParity(QSerialPort::NoParity);break;
                default: break;
            }
            switch(ui->stopbit->currentIndex())                     //设置停止位
            {
                case 1: serialport->setStopBits(QSerialPort::OneStop);break;
                case 2: serialport->setStopBits(QSerialPort::TwoStop);break;
                default: break;
            }
            serialport->setFlowControl(QSerialPort::NoFlowControl);     //设置流控制
            //连接槽函数
            QObject::connect(serialport, &QSerialPort::readyRead, this, &SerialPort::Read_Data);
            // 设置控件可否使用
            ui->send_button->setEnabled(true);
            ui->close_port->setEnabled(true);
            ui->open_port->setEnabled(false);
        }
        else    //打开失败提示
        {
            sleep(100);

            QMessageBox::information(this,tr("Erro"),tr("Open the failure"),QMessageBox::Ok);
        }
}
//关闭串口
void SerialPort::on_close_port_clicked()
{
    serialport->clear();        //清空缓存区
    serialport->close();        //关闭串口

    ui->send_button->setEnabled(false);
    ui->open_port->setEnabled(true);
    ui->close_port->setEnabled(false);
}


void SerialPort::on_clear_button_1_clicked()
{
    ui->send_text_window->clear();
}

void SerialPort::on_clear_button2_clicked()
{
    ui->Receive_text_window->clear();
}

//接收框文本模式转换
void SerialPort::on_receive_modl_clicked()
{
    if(ui->receive_modl->text() == "文本模式")
    {
        textstate_receive = true;
        ui->receive_modl->setText("hex模式");
    }
    else
    {
        ui->receive_modl->setText("文本模式");
        textstate_receive = false;
    }
}

//发送框文本转换
void SerialPort::on_send_modl_clicked()
{
    if(ui->send_modl->text() == "文本模式")
    {
        textstate_send = true;
        ui->send_modl->setText("hex模式");
    }
    else
    {
        ui->send_modl->setText("文本模式");
        textstate_send = false;
    }
}


