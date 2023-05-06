#include "serialport.h"


SerialPort::SerialPort()
{
    //串口对象初始化
    m_serialPort.setPortName("COM4");
    m_serialPort.setBaudRate(QSerialPort::Baud9600);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setParity(QSerialPort::NoParity);
    m_serialPort.setStopBits(QSerialPort::OneStop);
    m_serialPort.setFlowControl(QSerialPort::NoFlowControl);

    // 基本功能的响应关系
    //

}


void SerialPort::start()
{
    //打开串口
    if (m_serialPort.open(QIODevice::ReadWrite)) {
        qDebug() << "Serial port opened successfully!";
    } else {
        qDebug() << "Failed to open serial port!";
    }
    sendNum();
}
/*
void SerialPort::sendAndRecive(qint32 number)
{
    //打开串口
    if (m_serialPort.open(QIODevice::ReadWrite)) {
        qDebug() << "Serial port opened successfully!";
    } else {
        qDebug() << "Failed to open serial port!";
    }

    // 响应后自动调用readData
    connect(&m_serialPort, &QSerialPort::readyRead, this, &SerialPort::new_readData);

    // 转换数据格式
    QByteArray data;
    data.append(QString::number(number));

    // 发送数据
    m_serialPort.write(data);

    // 清空缓冲区
    //m_serialPort.flush();
}

*/

void SerialPort::sendNum()
{
    //通过串口发送信息
    QByteArray data;
    //int value = 2000;
    //data.append(QString::number(value));
    char temp =  's';
    m_serialPort.write(&temp);
    qDebug() << "send 's'";
    m_serialPort.flush();
    qDebug() << temp;
}

void SerialPort::readData()
{
    QByteArray data = m_serialPort.readAll();
    qDebug() << "recive" << data;
}

/*
void SerialPort::new_readData()
{
    QByteArray data = m_serialPort.readAll();
    int receiveData = QString(data).toInt();
    emit new_retData(receiveData);
}
*/


