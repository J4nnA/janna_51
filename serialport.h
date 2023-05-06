#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <stdlib.h>
#include <iostream>

class SerialPort
{

public:
    SerialPort();

    void start();           // 最早的操作

    // 向单片机发送数值并接受
    //void sendAndRecive(qint32 number);

signals:
    // 向上层发送数据
    //void new_retData(qint32 data);

// 私有方法
private:
    void sendNum();
    void readData();


private slots:
    //void new_readData();

    // 私有成员
private:
    QSerialPort m_serialPort;

};

#endif // SERIALPORT_H
