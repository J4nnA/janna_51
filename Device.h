// 发送数据进行string类型转换，接收数据不进行转换
#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QThread>
#include <QDebug>
#include "visa.h"
#include "visatype.h"

/*
 * 数据传输类型
 */
enum class SCPI_DATA_FMT
{
    s_datafmt_ASC = 0,
    s_datafmt_HEX

};

enum class SCPI_MEAS_MODE
{
    s_measmode_CAT = 0,
    s_measmode_SA,
    s_measmode_VNA,
    s_measmode_USBPM,
    s_measmode_VVM
};

class Analyzer
{
public:
    Analyzer();

    // 分析仪连接与初始化
    ViStatus connectAnalyzer(QString nameStr);

    // 设置起始频率
    ViStatus setStartFreq(QString startFreqStr);

    // 查询起始频率
    ViStatus queryStartFreq(ViReal64 &freq);

    // 设置终止频率
    ViStatus setStopFreq(QString stopFreqStr);

    // 查询终止频率
    ViStatus queryStopFreq(ViReal64 &freq);

    // 设置扫描点数
    ViStatus setSweepPoint(QString sweepPoint);

    // 查询扫描点数
    ViStatus querySweepPoint(ViInt32& sweepPoint);

    // 查询格式化后的当前迹线的数据(返回数据和数据数目)
    ViStatus queryCurTraceFmtData(ViChar charDataArray[], ViInt32 &dataNum);

    // 设置测量模式
    ViStatus setMeasMode(SCPI_MEAS_MODE s_measmode);


private:
    // 设置查询数据格式
    ViStatus setQueryDataFmt(SCPI_DATA_FMT queryDataFmt);

    // 查询操作
    ViStatus querySingleData(const QString &cmd, ViInt32 &longValue);
    ViStatus querySingleData(const QString &cmd, ViReal64 &doubleValue);     // 需要重载一堆函数
    ViStatus queryArrayData(const QString &cmd, ViChar charDataArray[], ViInt32& dataNum);
                                    // 目前只能实现ascii码的读取
    // 发送指令
    ViStatus sendCmd(const QString &cmd);


public:
    const static qint32 TIMEOUT = 10000;
    const static qint32 BUFFER_SIZE = 1024;
    const static qint32 BUFFER_SIZE_PER_READ = 2048;
    const static qint32 MAX_POINT_NUM = 1600;

private:
    ViSession m_analyzerSession;            // 设备会话
    ViSession m_defaultRM;                  // 默认资源管理器会话
    ViRsrc m_analyzerName;                  // 分析仪资源符
    static SCPI_DATA_FMT queryDataFmt;      // 查询返回数据类型
};

#endif // DEVICE_H
