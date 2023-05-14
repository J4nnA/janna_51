#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <QRunnable>
#include <QString>
#include <QThread>
#include <QObject>

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


class SaveAndDraw : public QRunnable
{
public:
    SaveAndDraw(const MainWindow &mainwindow,
                const QString& dirPath,
                const QString& pexfix,
                const qint32& collectNum,
                const qint64 &startTime,
                const qint64 &intervalTime);
    void run() override;
private:
    const MainWindow &m_mainWindow;
    QString m_dirPath;
    QString m_prefix;
    qint32 m_collectNum;
    qint64 m_startTime;
    qint32 m_intervalTime;
};


class TestNew :  public QThread
{
    Q_OBJECT
public:
    TestNew(qint64 startTime);
    void run() override;

    void updataStartTime(const qint64 startTime);

signals:
    void readyCollect();
private:
    qint64 m_startTime;
};

#endif // DATACOLLECTOR_H
