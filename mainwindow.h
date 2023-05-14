// 获取ui中的数据，只进行简单的处理，不进行数据类型的转换
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QVector>
#include <QThreadPool>
#include "Service.h"
#include "plotwindow.h"
#include "serialport.h"
#include "newserialport.h"
#include "myserialport.h"
#include "ThreadedTasks.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

     bool saveDataToFile(const QString& dirPath,
                         const QString& prefix,
                         const qint32& collectNum,
                         const qint32 &timeInterval)const;

     bool saveAndRead(const QString& dirPath,
                      const QString& prefix,
                      const qint32& collectNum,
                      const qint64& startTime,
                      const qint64& intervalTime)const;
     bool saveDrawTest(const QString& dirPath,
                       const QString& prefix);
signals:
    void dataReady(const QVector<double>& data);
private slots:
    void on_btnConnect_clicked();

    void on_btnSetStartFreq_clicked();

    void on_btnQueryStartFreq_clicked();

    void on_btnSetStopFreq_clicked();

    void on_btnQueryStopFreq_clicked();

    void on_btnReadFormatData_clicked();

    void on_btnTempTest_clicked();

    void on_btnReadSpecFile_clicked();

    void on_btnReadAllFile_clicked();

    void on_btnPlotGraph_clicked();

    void on_btnVnaModeSwitch_clicked();

    void on_btnSetSweepPoint_clicked();

    void on_btnQuerySweepPoint_clicked();

    void on_btnConnectSerialPort_clicked();

    void on_btnStrQuery_clicked();

    void on_btnQueryMaxAngVol_clicked();

    void print_temp(qint32 data);
    // 根据文件名，读取特定文件
    QVector<double> readFileByName(const QString& fileName);

    // 读取同一路径下的所有文件的内容
    QVector<QVector<double>> readFilesSameDir(const QString dirctoryPath);

    void on_btnSetMaxAngVol_clicked();

    void on_btnQueryMinAngVol_clicked();

    void on_btnSetMinAngVol_clicked();

    void on_btnSetCollectPoint_clicked();

    void on_btnSetAngleInterval_clicked();

    void on_btnQueryPointAndInterval_clicked();

    void on_btnQueryIntervalTime_clicked();

    void on_pushButton_4_clicked();

    void onTimeReady();

    void on_btnSaveDrawOne_clicked();

private:
    void printInfo(QString infoStr);

    void setCollectPoint(qint32 collectPoint);

    void setCollectAngInt(float collectAngInt);

    void finalTestSave(const QString dirPath,
                       const QString prefix,
                       const qint32 collectPoint,
                       const qint64 timeInterval)const;
private:
    Ui::MainWindow *ui;

    Server m_server;                // 下层服务

    MySerialPort m_serialPort;

    QString dirPath;                // 数据存放格式

    qint32 m_collectPoint;            // 采集点数---只能通过函数修改

    float m_collectAngInt;            // 采集间隔--只能通过函数修改
public:
    PlotWindow* m_plotWindow;

    TestNew * m_test;
};

#endif // MAINWINDOW_H
