#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <stdlib.h>
#include <iostream>


// 对于上位机，控制速度是根据角度来的，即时间和角度的关系
// 对于下位机，控制速度则是根据脉冲的频率
// 需要弄一个转换器
// 可以学会float和int的转换————对于实际工程项目

class MySerialPort : public QObject
{
    Q_OBJECT
public:
    explicit MySerialPort(QObject *parent = nullptr);

    void sendRandomNum(qint32 number);

    bool connectSerialPort();

    // 开启工作
    void startWork();

    // 设置最大角速度
    void setMaxAngleVolecity(const float angleVolecity);

    // 设置最小角速度
    void setMinAngleVolecity(const float angleVolecity);

    // 查询最大角速度
    float queryMaxAngleVolecity();

    // 查询最小角速度
    float queryMinAngleVolecity();

signals:
    void retData(qint32 data);

private slots:
    // 接收数据，并发射信号retData
    void reciveData();

private:
    static QString dataBuffer;
    QSerialPort m_serialPort;

};


// 角度每秒换算成脉冲时延
class Converter
{
    // 角速度变脉冲
    int angleToDelay(const float angleVolecity, const long stepsPerRev);

    // 脉冲变角速度
    float delayToAngle(const int delay, const long stepsPerRev);
};

#endif // MYSERIALPORT_H
