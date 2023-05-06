#include "newserialport.h"

NewSerialPort::NewSerialPort(QWidget *parent)
{
    //串口对象初始化
    m_serialPort.setPortName("COM4");
    m_serialPort.setBaudRate(QSerialPort::Baud9600);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setParity(QSerialPort::NoParity);
    m_serialPort.setStopBits(QSerialPort::OneStop);
    m_serialPort.setFlowControl(QSerialPort::NoFlowControl);

}

void NewSerialPort::sendAndRevice(qint32 number)
{
    //打开串口
    if (m_serialPort.open(QIODevice::ReadWrite)) {
        qDebug() << "Serial port opened successfully!";
    } else {
        qDebug() << "Failed to open serial port!";
    }

    // 信号槽关系建立
    QObject::connect(&m_serialPort, &QSerialPort::readyRead, this, &NewSerialPort::reviceData);

    // 转换数据格式
    QByteArray data;
    data.append(QString::number(number));

    // 发送数据
    m_serialPort.write(data);

    // 清空缓冲区
    m_serialPort.flush();

}

void NewSerialPort::reviceData()
{
    QByteArray data = m_serialPort.readAll();
    int receiveData = QString(data).toInt();
    emit newRetData(receiveData);
}
