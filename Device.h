// 发送数据进行string类型转换，接收数据不进行转换
#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QThread>
#include <QDebug>
#include "visa.h"
#include "visatype.h"

class Analyzer
{
public:
    //Analyzer();

private:
    ViSession m_analyzerSession;            // 设备会话
    ViSession m_defaultRM;                  // 默认资源管理器会话

};

#endif // DEVICE_H
