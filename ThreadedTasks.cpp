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
