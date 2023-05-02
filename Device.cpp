#include "Device.h"

// 声明全局变量（const的不用）
SCPI_DATA_FMT Analyzer::queryDataFmt;

#define SIZE_PER_READ      2048  //每次读取的最大长度

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
    QString cmd = ":SENS:FREQ:STAR " + startFreqStr;

    // 发送指令
    status = sendCmd(cmd);
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
    QString cmd = ":SENS:FREQ:STAR?";


    // 发送查询指令并接收查询结果
    status = querySingleData(cmd, retFreq);
    if(status != VI_SUCCESS)
    {
        qDebug() << "queryStartFreq error.";
        return status;
    }

    return status;
}

ViStatus Analyzer::setStopFreq(QString stopFreqStr)
{
    ViStatus status;

    // 生成操作字符串
    QString cmd = ":SENS:FREQ:STOP " + stopFreqStr;

    // 发送指令
    status = sendCmd(cmd);
    if(status != VI_SUCCESS)
    {
        qDebug() << "setStopFreq error.";
        return status;
    }
    return status;
}

ViStatus Analyzer::queryStopFreq(ViReal64 &retFreq)
{
    ViStatus status;

    // 生成指令
    QString cmd = ":SENS:FREQ:STOP?";

    // 查询数据
    status = querySingleData(cmd, retFreq);
    if(status != VI_SUCCESS)
    {
        qDebug() << "queryStopFreq error.";
        return status;
    }
    return status;
}

ViStatus Analyzer::setSweepPoint(QString sweepPoint)
{
    ViStatus status;

    // 生成操作字符串
    QString cmd = ":SENS:SWE:POIN " + sweepPoint;

    // 发送指令
    status = sendCmd(cmd);
    if(status != VI_SUCCESS)
    {
        qDebug() << "setSweepPoint error.";
        return status;
    }
    return status;
}

ViStatus Analyzer::querySweepPoint(ViInt32 &sweepPoint)
{
    ViStatus status;

    // 生成指令
    QString cmd = ":SENS:SWE:POIN?";

    // 查询数据
    status = querySingleData(cmd, sweepPoint);
    if(status != VI_SUCCESS)
    {
        qDebug() << "querySweepPoint error.";
        return status;
    }
    return status;
}


ViStatus Analyzer::queryCurTraceFmtData(ViChar charDataArray[], ViInt32 &dataNum)
{
    qDebug() << "queryCurTraceFmtData";
    ViStatus status;


    QString cmd = ":CALC:DATA:FDATA?";

    // 查询数组数据
    status = queryArrayData(cmd, charDataArray, dataNum);
    return status;
}

ViStatus Analyzer::setMeasMode(SCPI_MEAS_MODE s_measmode)
{
    ViStatus status;

    QString opStr = ":INSTrument:SELect ";
    QString cmd;

    switch(s_measmode)
    {
    case SCPI_MEAS_MODE::s_measmode_CAT:
        break;
    case SCPI_MEAS_MODE::s_measmode_SA:
        break;
    case SCPI_MEAS_MODE::s_measmode_VNA:
        cmd = opStr + "VNA";
        break;
    case SCPI_MEAS_MODE::s_measmode_USBPM:
        break;
    case SCPI_MEAS_MODE::s_measmode_VVM:
        break;
    default:
        break;
    }

    status = sendCmd(cmd);
    return status;
}



ViStatus Analyzer::setQueryDataFmt(SCPI_DATA_FMT queryDataFmt)
{
    ViStatus status;

    // 根据不同参数，发送不同的指令
    switch(queryDataFmt)
    {
    case SCPI_DATA_FMT::s_datafmt_ASC:
        status = sendCmd(QString(":FORM:ASC"));
        break;
    case SCPI_DATA_FMT::s_datafmt_HEX:
        status = sendCmd(QString(":FORM:HEX"));
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

ViStatus Analyzer::querySingleData(const QString &cmd, ViInt32 &longValue)
{
    ViStatus status;
    ViUInt32 retCnt;
    ViChar readBuff[BUFFER_SIZE];

    // 发送指令
    sendCmd(cmd);

    // 接收数据并转化
    QThread::msleep(50);
    status = viRead(m_analyzerSession, (ViBuf)readBuff, BUFFER_SIZE, &retCnt);
    // 根据查询返回数据类型进行对应转换操作
    switch(Analyzer::queryDataFmt)
    {
    case SCPI_DATA_FMT::s_datafmt_HEX:
        std::memcpy(&longValue, readBuff, sizeof(ViInt32));
        break;
    case SCPI_DATA_FMT::s_datafmt_ASC:
        QString tempStr = readBuff;
        longValue = tempStr.toInt();
        break;
    }
    return status;
}


ViStatus Analyzer::querySingleData(const QString& cmd, ViReal64 &doubleValue)
{
    ViStatus status;
    ViUInt32 retCnt;
    ViChar readBuff[BUFFER_SIZE];

    // 发送指令
    sendCmd(cmd);

    // 接收数据并转化
    QThread::msleep(50);
    status = viRead(m_analyzerSession, (ViBuf)readBuff, BUFFER_SIZE, &retCnt);
    // 根据查询返回数据类型进行对应转换操作
    switch(Analyzer::queryDataFmt)
    {
    case SCPI_DATA_FMT::s_datafmt_HEX:
        std::memcpy(&doubleValue, readBuff, sizeof(ViReal64));
        break;
    case SCPI_DATA_FMT::s_datafmt_ASC:
        QString tempStr = readBuff;
        doubleValue = tempStr.toDouble();
        break;
    }
    return status;
}


ViStatus Analyzer::queryArrayData(const QString &cmd, ViChar charDataArray[], ViInt32 &dataNum)
{
    // 提示信息
    qDebug() << "Analyzer::queryArrayData";

    ViStatus status = VI_SUCCESS;
    ViUInt32 retCnt;
    ViInt32 recivedSize = 0;

    // 发送查询指令
    status = sendCmd(cmd);
    // 等待
    QThread::msleep(50);
    // 根据查询返回数据类型进行对应的读取操作
    switch(Analyzer::queryDataFmt)
    {
    case SCPI_DATA_FMT::s_datafmt_ASC:
        // 读取数据
        while(1)
        {
            status = viRead(m_analyzerSession, (ViBuf)charDataArray + recivedSize, SIZE_PER_READ, &retCnt);
            if(status == VI_ERROR_TMO)
            {
                break;
            }
            else if (status == VI_SUCCESS)
            {

                break;
            }
            recivedSize += retCnt;
        }
        dataNum = recivedSize;
        break;
    }


    return status;
}



ViStatus Analyzer::sendCmd(const QString& cmd)
{
    ViStatus status;
    ViUInt32 retCnt;
    ViChar wrtBuff[BUFFER_SIZE];

    // 转换数据类型
    QByteArray tempArray = cmd.toLocal8Bit();

    // 写入缓冲区
    std::memcpy(wrtBuff, tempArray, BUFFER_SIZE);

    // 发送指令
    status = viWrite(m_analyzerSession, ViBuf(wrtBuff), BUFFER_SIZE, &retCnt);
    if(status != VI_SUCCESS)
    {
        qDebug() << "sendCmd error.";
        return status;
    }
    return status;

}













