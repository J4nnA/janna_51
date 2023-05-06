#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <stdlib.h>
#include <iostream>

class MySerialPort : public QObject
{
    Q_OBJECT
public:
    explicit MySerialPort(QObject *parent = nullptr);

    void sendRandomNum(qint32 number);

    bool connectSerialPort();

signals:
    void retData(qint32 data);

private slots:
    // 接收数据，并发射信号retData
    void reciveData();

private:
    static QString dataBuffer;
    QSerialPort m_serialPort;

};

#endif // MYSERIALPORT_H
