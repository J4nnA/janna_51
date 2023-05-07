#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <QRunnable>
#include <QString>
#include <QThread>

class MainWindow;

class DataCollector : public QRunnable
{
public:
    DataCollector(const MainWindow &mainwindow,
                  const QString& dirPath,
                  const QString& pexfix,
                  const qint32& collectNum,
                  const qint32 &timeInterval);
    void run() override;
private:
    const MainWindow &m_mainWindow;
    QString m_dirPath;
    QString m_prefix;
    qint32 m_collectNum;
    qint32 m_timeInterval;
};


#endif // DATACOLLECTOR_H
