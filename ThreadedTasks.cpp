#include "ThreadedTasks.h"
#include "mainwindow.h"


DataCollector::DataCollector(const MainWindow &mainwindow,
                             const QString& dirPath,
                             const QString& prefix,
                             const qint32& collectNum,
                             const qint32 &timeInterval)
    : m_mainWindow(mainwindow),
      m_dirPath(dirPath),
      m_prefix(prefix),
      m_collectNum(collectNum),
      m_timeInterval(timeInterval)
{
    // 构造函数的实现
}

void DataCollector::run()
{
    // 等待
    QThread::msleep(1000);

    m_mainWindow.saveDataToFile(m_dirPath, m_prefix, m_collectNum, m_timeInterval);

}

SaveAndDraw::SaveAndDraw(const MainWindow &mainwindow,
                         const QString& dirPath,
                         const QString& prefix,
                         const qint32& collectNum,
                         const qint64& startTime,
                         const qint64& intervalTime)
    : m_mainWindow(mainwindow),
      m_dirPath(dirPath),
      m_prefix(prefix),
      m_collectNum(collectNum),
      m_startTime(startTime),
      m_intervalTime(intervalTime)
{
}

void SaveAndDraw::run()
{
    // 等待
    QThread::msleep(m_startTime);
    // 开始采集
    m_mainWindow.saveAndRead(m_dirPath,
                             m_prefix,
                             m_collectNum,
                             m_startTime,
                             m_intervalTime);
}



TestNew::TestNew(qint64 startTime)
    :m_startTime(startTime)
{

}

void TestNew::run()
{
    qDebug() << "wait: " << m_startTime;
    QThread::msleep(m_startTime);
    emit readyCollect();
    // 结束线程
    exit(0);
}

void TestNew::updataStartTime(const qint64 startTime)
{
    m_startTime = startTime;
}
