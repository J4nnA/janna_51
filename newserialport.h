#ifndef NEWSERIALPORT_H
#define NEWSERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <stdlib.h>
#include <iostream>
#include <QWidget>
class NewSerialPort:public QWidget
{
    Q_OBJECT
public:
    explicit NewSerialPort(QWidget *parent = nullptr);

   // 发送和接收数据
   void sendAndRevice(qint32 number);


signals:
   // 向上层发送数据
    void newRetData(qint32 data);

private slots:
    // 接收数据并发送传输信号
    void reviceData();

private:
    QSerialPort m_serialPort;
};

#endif // NEWSERIALPORT_H
