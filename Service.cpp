#include "Service.h"


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
