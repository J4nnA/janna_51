#include "myserialport.h"

MySerialPort::MySerialPort(QObject *parent)
    : QObject{parent}
{
    //串口对象初始化
    m_serialPort.setPortName("COM4");
    m_serialPort.setBaudRate(QSerialPort::Baud9600);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setParity(QSerialPort::NoParity);
    m_serialPort.setStopBits(QSerialPort::OneStop);
    m_serialPort.setFlowControl(QSerialPort::NoFlowControl);

    // 建立收发机制
    connect(&m_serialPort, &QSerialPort::readyRead, this, &MySerialPort::reciveData);
}


void MySerialPort::sendRandomNum(qint32 number)
{

    // 转换数据格式
    QByteArray data;
    data.append(QString::number(number));

    // 发送数据
    m_serialPort.write(data);

    // 清空缓冲区
    m_serialPort.flush();
}

bool MySerialPort::connectSerialPort()
{
    //打开串口
    if (m_serialPort.open(QIODevice::ReadWrite)) {
        qDebug() << "Serial port opened successfully!";
    } else {
        qDebug() << "Failed to open serial port!";
        return 0;
    }
    return 1;
}

void MySerialPort::reciveData()
{
    // 接收数据
    QByteArray data = m_serialPort.readAll();
    int dataRet = QString(data).toInt();

    // 发射信号
    emit retData(dataRet);
}
