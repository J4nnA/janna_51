#include "myserialport.h"

// 声明静态变量
QString MySerialPort::dataBuffer;

Converter::Converter(QObject *parent)
    : QObject{parent}
{
}

int Converter::angleToDelay(const float angleVolecity, const long stepsPerRev)
{
    qint32 delay =(qint32)( (1000000/(angleVolecity*2)) * ((double)360/STEPS_PER_REV) );
    return delay;
}

float Converter::delayToAngle(const int delay, const long stepsPerRev)
{
    float angleVolecity = ( 1000000 / ((double)delay * 2) ) * ( (double)360/STEPS_PER_REV );
    return angleVolecity;
}



MySerialPort::MySerialPort(QObject *parent)
    : QObject{parent}
{
    //串口对象初始化
    m_serialPort.setPortName("COM4");
    m_serialPort.setBaudRate(QSerialPort::Baud9600);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setParity(QSerialPort::NoParity);
    m_serialPort.setStopBits(QSerialPort::OneStop);
    m_serialPort.setFlowControl(QSerialPort::NoFlowControl);

    // 建立收发机制
    connect(&m_serialPort, &QSerialPort::readyRead, this, &MySerialPort::reciveData);
    connect(this, &MySerialPort::firstQueryFinished, this, &MySerialPort::onFirstQueryFinished);
}


void MySerialPort::sendRandomNum(qint32 number)
{

    // 转换数据格式
    //QByteArray data;
    //data.append(QString::number(number));

    // 加上数据转换
    QString temp = QString::number(number);
    QByteArray byteArray = temp.toLatin1();
    const char * dataToSend = byteArray.data();
    m_serialPort.write(dataToSend, byteArray.size());
    // 发送数据
    //m_serialPort.write(data);

    // 清空缓冲区
    m_serialPort.flush();
}

bool MySerialPort::connectSerialPort()
{
    //打开串口
    if (m_serialPort.open(QIODevice::ReadWrite)) {
        qDebug() << "Serial port opened successfully!";
    } else {
        qDebug() << "Failed to open serial port!";
        return 0;
    }

    initQuery();

    return 1;
}

void MySerialPort::startWork()
{
    // 生成指令并转换格式
    QString cmdStr = QString("%1:%2\n").arg(CMD_FUNCTION).arg(FUNC_WORK);
    QByteArray byteArray = cmdStr.toLatin1();
    const char *dataToSend = byteArray.data();

    qDebug() << "cmdStr: " << cmdStr;

    // 发送数据
    m_serialPort.write(dataToSend, byteArray.size());

    // 清空缓冲区
    m_serialPort.flush();
}

void MySerialPort::setMaxAngleVolecity(const float angleVolecity)
{
    qint32 delay = m_converter.angleToDelay(angleVolecity, STEPS_PER_REV);
    m_minDelay = delay;
    // 生成指令并转换格式
    QString cmdStr = QString("%1:%2,%3\n").arg(CMD_FUNCTION_WITH_PARAM)
                                          .arg(FUNC_SET_MIN_DELAY)
                                          .arg(QString::number(delay));
    QByteArray byteArray = cmdStr.toLatin1();
    const char *dataToSend = byteArray.data();

    qDebug() << "cmdStr: " << cmdStr;

    // 设置查询指令格式
    curRequestType = MaxAngleVelocity;

    // 发送数据
    m_serialPort.write(dataToSend, byteArray.size());

    // 清空缓冲区
    m_serialPort.flush();
}

void MySerialPort::setMinAngleVolecity(const float angleVolecity)
{
    qint32 delay = m_converter.angleToDelay(angleVolecity, STEPS_PER_REV);
    m_maxDelay = delay;
    // 生成指令并转换格式
    QString cmdStr = QString("%1:%2,%3\n").arg(CMD_FUNCTION_WITH_PARAM)
                                          .arg(FUNC_SET_MAX_DELAY)
                                          .arg(QString::number(delay));
    QByteArray byteArray = cmdStr.toLatin1();
    const char *dataToSend = byteArray.data();

    qDebug() << "cmdStr: " << cmdStr;

    // 设置查询指令格式
    curRequestType = MinAngleVelocity;

    // 发送数据
    m_serialPort.write(dataToSend, byteArray.size());

    // 清空缓冲区
    m_serialPort.flush();
}

void MySerialPort::queryMaxAngleVolecity()
{
    // 生成指令并转换格式
    QString cmdStr = QString("%1:%2\n").arg(CMD_FUNCTION).arg(FUNC_QUERY_MIN_DELAY);
    QByteArray byteArray = cmdStr.toLatin1();
    const char *dataToSend = byteArray.data();

    qDebug() << "cmdStr: " << cmdStr;

    // 设置查询指令格式
    curRequestType = MaxAngleVelocity;

    // 发送数据
    m_serialPort.write(dataToSend, byteArray.size());

    // 清空缓冲区
    m_serialPort.flush();
}

void MySerialPort::queryMinAngleVolecity()
{

    // 生成指令并转换格式
    QString cmdStr = QString("%1:%2\n").arg(CMD_FUNCTION).arg(FUNC_QUERY_MAX_DELAY);
    QByteArray byteArray = cmdStr.toLatin1();
    const char *dataToSend = byteArray.data();

    qDebug() << "cmdStr: " << cmdStr;

    // 设置查询指令格式
    curRequestType = MinAngleVelocity;

    // 发送数据
    m_serialPort.write(dataToSend, byteArray.size());

    // 清空缓冲区
    m_serialPort.flush();
}

qint32 MySerialPort::queryMaxDelayForUpper()
{
    return m_maxDelay;
}

qint32 MySerialPort::queryMinDelayForUpper()
{
    return m_minDelay;
}

qint64 MySerialPort::queryStartTimeForUpper()
{
    // 更新时间
    startTime = calStartTime();
    return startTime;
}

qint64 MySerialPort::queryWorkTimeForUpper()
{
    workTime = calWorkTime();
    return workTime;
}

void MySerialPort::reciveData()
{
    // 接收数据
    QByteArray data = m_serialPort.readAll();
    dataBuffer.append(data);
    qDebug() << "reciveData: " << data;
    // 检查是否有换行符，如果有，则处理数据并清空缓冲区
    int newLineIndex = dataBuffer.indexOf('\n');
    if (newLineIndex != -1) {
        int dataRet = dataBuffer.left(newLineIndex).toInt();
        dataBuffer.remove(0, newLineIndex + 1);


        // 换算成角速度
        // 根据请求类型，产生不同信号
        float angleVolecity;
        switch(curRequestType){
        case MaxAngleVelocity:
            angleVolecity = m_converter.delayToAngle(dataRet, STEPS_PER_REV);
            m_maxAngVol = angleVolecity;
            emit retMaxAngleVelocity(angleVolecity);

            queryStartTimeForUpper();
            queryWorkTimeForUpper();
            qDebug() <<"angleVolecity: "<< angleVolecity;
            break;
        case MinAngleVelocity:
            angleVolecity = m_converter.delayToAngle(dataRet, STEPS_PER_REV);
            m_minAngVol = angleVolecity;
            emit retMinAngleVelocity(angleVolecity);

            queryStartTimeForUpper();
            queryWorkTimeForUpper();

            qDebug() <<"angleVolecity: "<< angleVolecity;
            break;
        case InitMaxDelay:
            m_maxDelay = dataRet;
            m_minAngVol = m_converter.delayToAngle(dataRet, STEPS_PER_REV);
            qDebug() << "Init maxDelay: "<< m_maxDelay;

            queryStartTimeForUpper();
            queryWorkTimeForUpper();
            break;
        case InitMinDelay:
            m_minDelay = dataRet;
            m_maxAngVol = m_converter.delayToAngle(dataRet, STEPS_PER_REV);
            qDebug() << "Init minDelay: "<< m_minDelay;
            emit firstQueryFinished();
            break;
        }
    }
}

void MySerialPort::initQuery()
{
    queryMinDelay();
       // 不需要QThread::msleep(100);
       // 在第一个查询的响应返回后，会触发firstQueryFinished信号

}

void MySerialPort::onFirstQueryFinished()
{
    queryMaxDelay();
}

void MySerialPort::queryMinDelay()
{
    // 生成指令并转换格式
    QString cmdStr = QString("%1:%2\n").arg(CMD_FUNCTION).arg(FUNC_QUERY_MIN_DELAY);
    QByteArray byteArray = cmdStr.toLatin1();
    const char *dataToSend = byteArray.data();

    qDebug() << "cmdStr: " << cmdStr;

    // 设置查询指令格式
    curRequestType = InitMinDelay;

    // 发送数据
    m_serialPort.write(dataToSend, byteArray.size());

    // 清空缓冲区
    m_serialPort.flush();
}

void MySerialPort::queryMaxDelay()
{
    // 生成指令并转换格式
    QString cmdStr = QString("%1:%2\n").arg(CMD_FUNCTION).arg(FUNC_QUERY_MAX_DELAY);
    QByteArray byteArray = cmdStr.toLatin1();
    const char *dataToSend = byteArray.data();

    qDebug() << "cmdStr: " << cmdStr;

    // 设置查询指令格式
    curRequestType = InitMaxDelay;

    // 发送数据
    m_serialPort.write(dataToSend, byteArray.size());

    // 清空缓冲区
    m_serialPort.flush();
}

qint64 MySerialPort::calAccTime(qint64 startDelay, qint64 stopDelay)
{
    qint64 result = (3 * stopDelay * 2 + 2 * startDelay * 2) * 200 / 1000; //除1000，单位变为毫秒
    qDebug() << "start:" << startDelay <<"\tsotp:" << stopDelay;
    qDebug() << "accTime: " << result;
    return result;      //单位为毫秒
}

qint64 MySerialPort::calStartTime()
{
    qint64 result = 0;
    // 反向加速
    result += calAccTime(m_maxDelay, m_minDelay);
    // 反向半圈
    result += (m_minDelay * 2 * STEPS_PER_REV / 2) / 1000;
    // 反向减速
    result += calAccTime(m_minDelay, m_maxDelay);
    // 正向加速
    result += calAccTime(m_maxDelay, m_minDelay);
    // 正向1000步
    result += m_minDelay * 2 * 1000 / 1000;     // 单位换成毫秒

    // 更新startTime
    startTime = result;
    qDebug() << "start Time: " << startTime;

    return result;
}

qint64 MySerialPort::calWorkTime()
{
    //  正转一圈
    workTime = m_minDelay * 2 * STEPS_PER_REV / 1000;
    qDebug() << "workTime: " << workTime;
    return workTime;
}



