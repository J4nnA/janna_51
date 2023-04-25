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

ViStatus Analyzer::setStartFreq(QString startFreqStr)
{
    ViStatus status;

    // 生成操作字符串
    QString opStr = ":SENS:FREQ:STAR";

    // 发送指令
    status = sendSetCmd(m_analyzerSession, opStr, startFreqStr);
    if(status != VI_SUCCESS)
    {
        qDebug() << "setStartFreq error.";
        return status;
    }

    return status;
}

ViStatus Analyzer::queryStartFreq(ViReal64 &retFreq)
{
    ViStatus status;

    // 生成操作字符串
    QString opStr = ":SENS:FREQ:STAR";

    // 缓冲区
    ViChar WR_Buff[BUFFER_SIZE];

    // 发送查询指令并接收查询结果
    status = Analyzer::sendQueryCmd(m_analyzerSession, opStr, WR_Buff);
    if(status != VI_SUCCESS)
    {
        qDebug() << "queryStartFreq error.";
        return status;
    }
    // 根据查询返回数据类型进行对应转换操作
    switch(Analyzer::queryDataFmt)
    {
    case SCPI_DATA_FMT::s_datafmt_HEX:
        std::memcpy(&retFreq, WR_Buff, sizeof(ViReal64));
        break;
    case SCPI_DATA_FMT::s_datafmt_ASC:
        QString tempStr = WR_Buff;
        retFreq = tempStr.toDouble();
        break;
    }

    return status;
}

ViStatus Analyzer::setQueryDataFmt(SCPI_DATA_FMT queryDataFmt)
{
    ViStatus status;

    // 根据不同参数，发送不同的指令
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

ViStatus Analyzer::sendQueryCmd(const ViSession &analyzerSession, QString opStr, ViChar WR_Buff[])
{
    ViStatus status;
    ViUInt32 retCnt;

    // 添加查询符号，构建指令
    QString cmdStr = opStr + "?";


    // 转换数据类型
    QByteArray tempArray = cmdStr.toLocal8Bit();

    // 写入缓冲区
    std::memcpy(WR_Buff, tempArray, BUFFER_SIZE);

    // 发送指令
    status = viWrite(m_analyzerSession, ViBuf(WR_Buff), BUFFER_SIZE, &retCnt);
    if(status != VI_SUCCESS)
    {
        qDebug() << "sendSetCmd error.";
        return status;
    }

    // 等待并接收返回数据
    QThread::msleep(50);
    memset(WR_Buff, 0, BUFFER_SIZE);
    status = viRead(m_analyzerSession, (ViBuf)WR_Buff, BUFFER_SIZE, &retCnt);
    if(status != VI_SUCCESS)
    {
        qDebug() << "reciveData error.";
        return status;
    }

    return status;
}













