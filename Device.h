// 发送数据进行string类型转换，接收数据不进行转换
#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QThread>
#include <QDebug>
#include "visa.h"
#include "visatype.h"

#define BUFFER_SIZE           1024  // 缓存区最大尺寸

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

private:

    // 设置查询数据格式
    ViStatus setQueryDataFmt(SCPI_DATA_FMT queryDataFmt);

    // 发送设置命令
    ViStatus sendSetCmd(const ViSession &analyzerSession, QString opStr, QString dataStr);

    // 发送查询命令(包括接收数据）
    ViStatus sendQueryCmd(const ViSession &analyzerSession, QString opStr, ViChar WR_Buff[]);
public:
    const static int TIMEOUT = 10000;

private:
    ViSession m_analyzerSession;            // 设备会话
    ViSession m_defaultRM;                  // 默认资源管理器会话
    ViRsrc m_analyzerName;                  // 分析仪资源符

    static SCPI_DATA_FMT queryDataFmt;      // 查询返回数据类型
};

#endif // DEVICE_H
