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
