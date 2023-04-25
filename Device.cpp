#include "Device.h"
SCPI_DATA_FMT Analyzer::queryDataFmt;

Analyzer::Analyzer()
{
    // 设置读取
    queryDataFmt = SCPI_DATA_FMT::s_datafmt_HEX;

    //与默认资源管理器建立会话
    if(VI_SUCCESS != viOpenDefaultRM(&m_defaultRM))
    {
        qDebug() << "openDefaultRM error.";
    }
}

ViStatus Analyzer::connectAnalyzer(QString nameStr)
{
    ViStatus status;

    // 构建资源符
    QByteArray tempByteArr = nameStr.toLocal8Bit();
    m_analyzerName = tempByteArr.data();

    // 建立设备会话
    status = viOpen(m_defaultRM, m_analyzerName, VI_NULL, VI_NULL, &m_analyzerSession);
    if(status != VI_SUCCESS)
    {
        qDebug() << "viOpen error.";
    }

    // 初始化设置
    viSetAttribute(m_analyzerSession, VI_ATTR_TMO_VALUE, 2000);
    viSetAttribute(m_analyzerSession, VI_ATTR_SEND_END_EN, VI_TRUE);
    viSetAttribute(m_analyzerSession, VI_ATTR_SUPPRESS_END_EN, VI_FALSE);
    viSetAttribute(m_analyzerSession, VI_ATTR_TERMCHAR_EN, VI_FALSE);

    // 修改数据读取格式
    status = setQueryDataFmt(SCPI_DATA_FMT::s_datafmt_ASC);
    if(status != VI_SUCCESS)
    {
        qDebug() << "setQueryDataFmt error.";
        return status;
    }

    return status;
}

ViStatus Analyzer::setQueryDataFmt(SCPI_DATA_FMT queryDataFmt)
{
    ViStatus status;

    switch(queryDataFmt)
    {
    case SCPI_DATA_FMT::s_datafmt_ASC:
        status = sendSetCmd(m_analyzerSession, QString(":FORM"),QString("ASC"));
        break;
    case SCPI_DATA_FMT::s_datafmt_HEX:
        status = sendSetCmd(m_analyzerSession, QString(":FORM"),QString("HEX"));
        break;
    }

    if(status != VI_SUCCESS)
    {
        qDebug() << "setQueryDataFmt error.";
        return status;
    }

    // 修改静态变量
    Analyzer::queryDataFmt = SCPI_DATA_FMT::s_datafmt_ASC;
    return status;
}

ViStatus Analyzer::sendSetCmd(const ViSession &analyzerSession, QString opStr, QString dataStr)
{
    ViStatus status;
    ViUInt32 retCnt;
    ViChar wrtBuff[BUFFER_SIZE];
    QString cmdStr;

    // 组合指令字符串
    if(dataStr == nullptr)
    {
        cmdStr = opStr;
    }
    else
    {
        cmdStr = opStr + " " + dataStr;
    }

    // 转换数据类型
    QByteArray tempArray = cmdStr.toLocal8Bit();

    // 写入缓冲区
    std::memcpy(wrtBuff, tempArray, BUFFER_SIZE);

    // 发送指令
    status = viWrite(m_analyzerSession, ViBuf(wrtBuff), BUFFER_SIZE, &retCnt);
    if(status != VI_SUCCESS)
    {
        qDebug() << "sendSetCmd error.";
        return status;
    }
    return status;
}
