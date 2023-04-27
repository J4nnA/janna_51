// 发送数据进行string类型转换，接收数据不进行转换
#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QThread>
#include <QDebug>
#include "visa.h"
#include "visatype.h"

#define BUFFER_SIZE           1024  // 读取单个数据时的缓冲区大小
#define BUFFER_SIZE_PER_READ  2048  // 读取数据块时，每次读取时的缓冲区大小
#define MAX_POINT_NUM      1600    // 矢网迹线的最大数目
/*
 * 数据传输类型
 */
enum class SCPI_DATA_FMT
{
    s_datafmt_ASC = 0,
    s_datafmt_HEX

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
    ViStatus setStopFreq(QString endFreqStr);

    // 查询终止频率
    ViStatus queryStopFreq(ViReal64 &freq);


    // 查询格式化后的当前迹线的数据(返回数据和数据数目)
    ViStatus low2service_queryCurFmtTrace(ViChar charDataArray[], ViInt32 &dataNum);
    ViStatus low2readASCIIDataBuff(ViChar charDataArray[], ViInt32& dataNum);

    // 设置查询数据格式
    ViStatus setQueryDataFmt(SCPI_DATA_FMT queryDataFmt);

    // 查询操作
    ViStatus querySingleData(const QString &cmd, ViReal64 &doubleValue);     // 需要重载一堆函数
    // 发送指令
    ViStatus sendCmd(const QString &cmd);


public:
    const static int TIMEOUT = 10000;

public:
    ViSession m_analyzerSession;            // 设备会话
    ViSession m_defaultRM;                  // 默认资源管理器会话
    ViRsrc m_analyzerName;                  // 分析仪资源符

    static SCPI_DATA_FMT queryDataFmt;      // 查询返回数据类型
};

#endif // DEVICE_H
