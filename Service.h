// 接受上层数据，处理后，转换成qstring
// 日后可维护多个设备
// 返回————————
#ifndef SERVICE_H
#define SERVICE_H

#include <QString>
#include <QDebug>
#include "visa.h"
#include "visatype.h"
#include "Device.h"

class Server{
public:
    //Server();

private:
    Analyzer m_analyzer;            // 矢网分析仪
};


#endif // SERVICE_H
