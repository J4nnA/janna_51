#include "myserialport.h"

// 声明静态变量
QString MySerialPort::dataBuffer;

Converter::Converter(QObject *parent)
    : QObject{parent}
{
}

int Converter::angleToDelay(const float angleVolecity, const long stepsPerRev)
{
    return 1;
}

float Converter::delayToAngle(const int delay, const long stepsPerRev)
{
    return 1;
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
    return 1;
}

void MySerialPort::startWork()
{
}

void MySerialPort::setMaxAngleVolecity(const float angleVolecity)
{

}

void MySerialPort::setMinAngleVolecity(const float angleVolecity)
{

}

float MySerialPort::queryMaxAngleVolecity()
{
    // 生成指令并转换格式
    QString cmdStr = QString("%1:%2\n").arg(CMD_FUNCTION).arg(FUNC_QUERY_MIN_DELAY);
    QByteArray byteArray = cmdStr.toLatin1();
    const char *dataToSend = byteArray.data();

    qDebug() << "cmdStr: " << cmdStr;
    // 发送数据
    m_serialPort.write(dataToSend, byteArray.size());

    // 清空缓冲区
    m_serialPort.flush();
    return 1;
}

float MySerialPort::queryMinAngleVolecity()
{
    return 1;
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

        // 发射信号
        emit retData(dataRet);
    }
}



