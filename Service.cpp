#include "Service.h"
#define SIZE_PER_READ 2048
#define BUFFER_SENDSIZE    64

qint32 Server::connectDevice(QString ip, DEVICE_TYPE devicetype)
{
    qint32 connectStatus;

    // 生成完整的设备名称
    QString nameStr = "TCPIP0::" + ip + "::5000::SOCKET";

    // 根据设备类型，调用不同的连接函数
    switch(devicetype){
    case DEVICE_TYPE::devicetype_analyzer:
        connectStatus = m_analyzer.connectAnalyzer(nameStr);
        break;
    }
    if(connectStatus != 0)
    {
        qDebug() << "openDevice error.";
        return connectStatus;
    }
    return connectStatus;
}

qint32 Server::setStartFreq(double startFreq, DEVICE_TYPE deviceType)
{
    qint32 flag;

    // 转换数据类型
    QString startFreqStr = QString::number(startFreq);
    switch(deviceType)
    {
    case DEVICE_TYPE::devicetype_analyzer:
        flag = m_analyzer.setStartFreq(startFreqStr);
        break;
    }
    if(flag != 0)
    {
        qDebug() << "setDeviceStartFreq error";
        return flag;
    }

    return flag;
}

QString Server::queryStartFreq(DEVICE_TYPE deviceType)
{
    QString startFreqStr;
    qint32 flag;
    ViReal64 startFreq;

    // 转换数据类型
    switch(deviceType)
    {
    case DEVICE_TYPE::devicetype_analyzer:

        flag = m_analyzer.queryStartFreq(startFreq);
        if(flag != 0)
        {
            qDebug() << "ananlyzer's queryStartFreq error.";
        }
        startFreqStr.sprintf("%.0f", startFreq);
        break;
    }
    return startFreqStr;
}

qint32 Server::setStopFreq(double stopFreq, DEVICE_TYPE deviceType)
{
    qint32 flag;

    // 转换数据类型
    QString stopFreqStr = QString::number(stopFreq);
    switch(deviceType)
    {
    case DEVICE_TYPE::devicetype_analyzer:
        flag = m_analyzer.setStopFreq(stopFreqStr);
        break;
    }
    if(flag != 0)
    {
        qDebug() << "setDeviceStopFreq error";
        return flag;
    }

    return flag;
}

QString Server::queryStopFreq(DEVICE_TYPE deviceType)
{
    QString stopFreqStr;
    qint32 flag;
    ViReal64 stopFreq;

    // 转换数据类型
    switch(deviceType)
    {
    case DEVICE_TYPE::devicetype_analyzer:

        flag = m_analyzer.queryStopFreq(stopFreq);
        if(flag != 0)
        {
            qDebug() << "ananlyzer's queryStopFreq error.";
        }
        stopFreqStr.sprintf("%.0f", stopFreq);
        break;
    }
    return stopFreqStr;
}

long Server::queryCurFmtTrace(ViReal32 data[], DEVICE_TYPE deviceType)
{
    long dataNum;

    // 对不同仪器进行不同的读取操作
    switch(deviceType)
    {
    case DEVICE_TYPE::devicetype_analyzer:
        //m_analyzer.new_queryCurFmtTrace(data, dataNum);
        m_analyzer.queryCurFmtTrace_ASCII(data, dataNum);
        break;
    }

    return dataNum;
}

ViStatus test_readASCIIDataBuff(ViSession handle, ViChar fileBuf[], ViInt32& nRetSize)
{

    ViUInt32 actualCount;
    ViStatus CurStatus = VI_SUCCESS;


    //分次读取，每次读取SIZE_PER_READ
    ViInt32 nReceived = 0;
    ViUInt32 size = SIZE_PER_READ;
    int i = 0;
    while(1)
    {
        i++;
        qDebug() << i;
        CurStatus = viRead(handle, (ViBuf)fileBuf + nReceived, size, &actualCount);
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
ViStatus test_settingCommand(ViSession handle,
                        ViChar* pCmd,
                        ViChar* pVal)
{
    if (nullptr == pCmd)
    {
        return -1;
    }
    ViChar wrtBuf[BUFFER_SIZE];
    ViUInt32 retCnt;
    if (nullptr == pVal)//无参数的情况
    {
        sprintf(wrtBuf,"%s", pCmd);
    }
    else
    {
        sprintf(wrtBuf,"%s %s", pCmd, pVal);
    }
    QString cmd = wrtBuf;
    viClear(handle);
    ViStatus status = viWrite(handle, (ViBuf)wrtBuf, BUFFER_SENDSIZE, &retCnt);
    QThread::msleep(300);
    return  status;
}


ViStatus Server::test_readCurTraceFormatData(ViReal32 pData[], ViInt32 bufsz, ViInt32 &nSize)
{
    // setQueryResultDataFormat(s_datafmt_HEX);
    //1.发送命令
    QString cmd = ":CALC:DATA:FDATA?";
    m_analyzer.sendCmd(cmd);

    // 读数据
    ViChar   buf[MAX_POINT_NUM * 2 * 8] = {0};
    ViStatus curStatus =  test_readASCIIDataBuff(m_analyzer.m_analyzerSession, buf, nSize);
    qDebug() << "here";
    QString str = buf;
    QStringList list = str.split(",");
    nSize = list.size();
    for (int i = 0;i<nSize;i++)
    {
        pData[i] = list.at(i).toFloat();
    }
    return curStatus;
}

ViStatus Server::trans_test_readCurTraceFormatData(ViReal32 dataArray[], ViInt32 &dataNum)
{
    ViStatus status;
    ViChar   charArray[MAX_POINT_NUM * 2 * 8] = {0};

    qDebug() << "Server::trans_test_readCurTraceFormatData";

    // 读数据
    status = m_analyzer.low2service_queryCurFmtTrace(charArray, dataNum);


    QString str = charArray;
    QStringList list = str.split(",");

    dataNum = list.size();
    for (int i = 0; i < dataNum; i++)
    {
        dataArray[i] = list.at(i).toFloat();
    }
    return status;
}

// 潜入深海
ViStatus Server::low2Device_readCurTraceFormatData(ViReal32 dataArray[], ViInt32 &dataNum)
{
    ViStatus status;

    qDebug() << "Server::low2Device_readCurTraceFormatData";
    qDebug() << "right server.";


    // 读数据
    ViChar   charArray[MAX_POINT_NUM * 2 * 8] = {0};
    status = m_analyzer.low2service_queryCurFmtTrace(charArray, dataNum);

    QString str = charArray;
    QStringList list = str.split(",");

    dataNum = list.size();
    for (int i = 0; i < dataNum; i++)
    {
        dataArray[i] = list.at(i).toFloat();
    }
    return status;
}
