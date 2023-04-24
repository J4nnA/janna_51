#include "Service.h"


qint32 Server::connectDevice(QString ip, DEVICE_TYPE devicetype)
{
    qint32 connectStatus;

    // 根据设备类型，调用不同的连接函数
    switch(devicetype){
    case DEVICE_TYPE::devicetype_analyzer:
        connectStatus = m_analyzer.connectAnalyzer(ip);
        break;
    }
    if(connectStatus != 0)
    {
        qDebug() << "openDevice error.";
        return connectStatus;
    }
    return connectStatus;
}
