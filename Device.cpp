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



ViStatus Analyzer::queryCurFmtTrace(ViReal32 data[], ViInt32 &dataNum)
{
    ViStatus status;
    ViInt32 dataSize;
    // 生成指令
    QString cmd = ":CALC:DATA:FDATA?";

    // 进行查询
    status = queryBlockData(cmd, (ViChar*)data, dataSize);
    if(status != VI_SUCCESS)
    {
        qDebug() << "queryCurFmtTrace error.";
        return status;
    }

    dataNum = dataSize/sizeof(ViReal32);


    return status;

}

ViStatus Analyzer::new_queryCurFmtTrace(ViReal32 data[], ViInt32 &dataNum)
{
    ViStatus status;
    ViInt32 dataSize;
    // 生成指令
    QString cmd = ":CALC:DATA:FDATA?";
    ViChar buf[MAX_POINT_NUM * 2 * 8] = {0};

    status =  queryASCIIData(cmd, buf, dataNum);

    QString str = buf;
    QStringList list = str.split(",");
    dataNum = list.size();
    for (int i = 0;i < dataNum; i++)
    {
        data[i] = list.at(i).toFloat();
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

ViStatus Analyzer::queryBlockData(const QString &cmd, ViChar blockData[], ViInt32 &dataSize)
{
    ViStatus status;
    ViChar tempData[10];                // 读取数据所使用的临时缓冲区
    ViChar tempSize[2] = {0};           // 转换的中间变量
    ViUInt32 totalSize;                 // 接收的有效数据大小
    ViUInt32 recivedSize = 0;           // 已接收的有效数据大小
    ViUInt32 retCnt;

    // 发送查询指令
    sendCmd(cmd);

    // 接收数据并转化
    QThread::msleep(50);

    // 读取前两位字节，获取有效数据的总大小
    status = viRead(m_analyzerSession, (ViBuf)tempData, 2, &retCnt);
    if(status == VI_ERROR_TMO)
    {
        dataSize = 0;
        qDebug() << "get dataSize error.";
        return status;
    }

    // 将获取的有效数据大小进行保存
    tempSize[0] = tempData[1];
    totalSize = atoi(tempSize);
    if(totalSize <= 0)
    {
        dataSize = 0;

        return status;
    }

    // 循环获取有效数据，直到出错或获取到总大小的数据
    while(1)
    {
        status = viRead(m_analyzerSession, (ViBuf)blockData + recivedSize, BUFFER_SIZE_PER_READ, &retCnt);
        if(status == VI_ERROR_TMO)
        {
            break;
        }

        recivedSize += retCnt;
        if(recivedSize >= totalSize)
        {
            break;
        }
    }
    dataSize = recivedSize;


    qDebug() << blockData;
    return status;
}

ViStatus Analyzer::queryASCIIData(const QString &cmd, ViChar fileBuf[], ViInt32 &nRetSize)
{
    /*
    ViStatus status;
    ViUInt32 retCnt;
    ViUInt32 recivedSize = 0;           // 已接收的有效数据大小
    // 发送查询指令
    sendCmd(cmd);

    // 接收数据并转化
    QThread::msleep(100);

    int i = 0;
    while(1)
    {
        status = viRead(m_analyzerSession, (ViBuf)ASCIIData, BUFFER_SIZE_PER_READ, &retCnt);
        if(status == VI_SUCCESS)qDebug() << "success:" << i;
        if(status != VI_SUCCESS)qDebug() << "error: \"i\" is " << i;
        if(status == VI_ERROR_TMO)
        {
            break;
        }
        else if (status == VI_SUCCESS)
        {

            break;
        }
        i++;
        recivedSize += retCnt;
    }

    dataSize = recivedSize;

    return status;
*/
    ViUInt32 actualCount;
    ViStatus CurStatus = VI_SUCCESS;

    // 发送查询指令
    CurStatus =sendCmd(cmd);
    if(CurStatus != VI_SUCCESS)qDebug() << "cmd error";
    // 接收数据并转化
    QThread::msleep(100);

    //分次读取，每次读取SIZE_PER_READ
    ViInt32 nReceived = 0;
    ViUInt32 size = BUFFER_SIZE_PER_READ;
    while(1)
    {
        CurStatus = viRead(m_analyzerSession, (ViBuf)fileBuf + nReceived, size, &actualCount);
        if(CurStatus == VI_SUCCESS)qDebug() << "success";
        if(CurStatus != VI_SUCCESS)qDebug() << "error";
        if(CurStatus == VI_ERROR_TMO)
        {
            break;
        }
        else if (CurStatus == VI_SUCCESS)
        {

            break;
        }

        nReceived += actualCount;


    }


    nRetSize = nReceived;

    return CurStatus;

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













