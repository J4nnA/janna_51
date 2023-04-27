// 接受上层数据，处理后，转换成qstring
// 日后可维护多个设备
// 返回————————
#ifndef SERVICE_H
#define SERVICE_H

#include <QString>
#include <QDebug>
#include "visa.h"
#include "visatype.h"
#include "Device.h"

enum class DEVICE_TYPE{
    devicetype_analyzer = 0
};

class Server{
public:

    // 连接设备
    qint32 connectDevice(QString ip, DEVICE_TYPE deviceType);

    // 设置起始频率
    qint32 setStartFreq(double startFreq, DEVICE_TYPE deviceType);

    // 查询起始频率
    QString queryStartFreq(DEVICE_TYPE deviceType);

    // 设置终止频率
    qint32 setStopFreq(double stopFreq, DEVICE_TYPE deviceType);

    // 查询终止频率
    QString queryStopFreq(DEVICE_TYPE deviceType);

    // 查询当前迹线格式化数据
    qint32 queryCurTraceFmtData(ViReal32 dataArray[], ViInt32 &dataNum, DEVICE_TYPE deviceType);

public:
    const static qint32 DEVICE_MAX_POINT_NUM = 1600;          // 所有设备一次所能获得的最大数据个数

private:
    Analyzer m_analyzer;            // 矢网分析仪
};


#endif // SERVICE_H
