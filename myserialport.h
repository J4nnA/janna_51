#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <stdlib.h>
#include <iostream>
#include <QThread>
#include "common.h"

const qint64 STEPS_PER_REV = 66000;

// 对于上位机，控制速度是根据角度来的，即时间和角度的关系
// 对于下位机，控制速度则是根据脉冲的频率
// 需要弄一个转换器
// 可以学会float和int的转换————对于实际工程项目

// 角度每秒换算成脉冲时延
class Converter : public QObject
{
    Q_OBJECT
public:
    explicit Converter(QObject *parent = nullptr);
    // 角速度变脉冲
    int angleToDelay(const float angleVolecity, const long stepsPerRev);

    // 脉冲变角速度
    float delayToAngle(const int delay, const long stepsPerRev);

};


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
    void queryMaxAngleVolecity();

    // 查询最小角速度
    void queryMinAngleVolecity();

    // 查询最大间隔
    qint32 queryMaxDelayForUpper();

    // 查询最小间隔
    qint32 queryMinDelayForUpper();

    // 查询开始时间
    qint64 queryStartTimeForUpper();

    // 查询工作时长
    qint64 queryWorkTimeForUpper();

signals:
    void retData(qint32 data);
    void retMaxAngleVelocity(float value);
    void retMinAngleVelocity(float value);
/*****/
signals:
    void firstQueryFinished();
/**打的补丁***/
private slots:
    // 接收数据，并发射信号retData
    void reciveData();
/*****/
public slots:
   void onFirstQueryFinished();
/**打的补丁***/
private:
    void sendCmd();

    void initQuery();

    void queryMinDelay();

    void queryMaxDelay();   //这两个可以被用到设置角速度函数中

    qint64 calAccTime(const qint64 startDelay, const qint64 stopDelay);

    qint64 calStartTime(); //start work

    qint64 calWorkTime();
private:
    enum RequestType{
        MaxAngleVelocity = 0,
        MinAngleVelocity,
        InitMinDelay,
        InitMaxDelay
    };

    static QString dataBuffer;

    QSerialPort m_serialPort;
    Converter m_converter;
    RequestType curRequestType;

    qint32 m_minDelay;
    qint32 m_maxDelay;      // 单位：毫秒
    float m_maxAngVol;      // 单位：度每秒
    float m_minAngVol;      //
    qint64 startTime;
    qint64 workTime;
};




#endif // MYSERIALPORT_H
