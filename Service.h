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

enum class M_DEVICE_TYPE{
    devicetype_analyzer = 0
};

class Server{
public:

    // 连接设备
    qint32 connectDevice(QString ip, M_DEVICE_TYPE deviceType);

    // 设置起始频率
    qint32 setStartFreq(double startFreq, M_DEVICE_TYPE deviceType);

    // 查询起始频率
    QString queryStartFreq(M_DEVICE_TYPE deviceType);

    // 设置终止频率
    qint32 setStopFreq(double stopFreq, M_DEVICE_TYPE deviceType);

    // 查询终止频率
    QString queryStopFreq(M_DEVICE_TYPE deviceType);

    // 设置扫描点数
    qint32 setSweepPoint(qint32 sweepPoint, M_DEVICE_TYPE deviceType);

    // 查询扫描点数
    QString querySweepPoint(M_DEVICE_TYPE deviceType);

    // 查询当前迹线格式化数据
    qint32 queryCurTraceFmtData(ViReal32 dataArray[], ViInt32 &dataNum, M_DEVICE_TYPE deviceType);

    // 设置测试模式
    qint32 setMeasMode(SCPI_MEAS_MODE s_measmode, M_DEVICE_TYPE deviceType);


public:
    const static qint32 DEVICE_MAX_POINT_NUM = 1600;          // 所有设备一次所能获得的最大数据个数

private:
    Analyzer m_analyzer;            // 矢网分析仪
};


#endif // SERVICE_H
