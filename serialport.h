#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <stdlib.h>
#include <iostream>

class SerialPort
{
public:
    SerialPort();
    void start();

private:
    void sendNum();
    void readData();
private:
    QSerialPort m_serialPort;
};

#endif // SERIALPORT_H
