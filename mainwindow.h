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
#include "Service.h"
#include "plotwindow.h"
#include "serialport.h"
#include "newserialport.h"
#include "myserialport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


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

    void on_btnComTest_clicked();

    void on_btnSendSpecialNumber_clicked();

private:
    void printInfo(QString infoStr);

    bool saveDataToFile(const QString& dirPath, const QString& pexfix, const qint32& collectNum, const qint32 &timeInterval);
private slots:
    void print_temp(qint32 data);
    // 根据文件名，读取特定文件
   QVector<double> readFileByName(const QString& fileName);

   // 读取同一路径下的所有文件的内容
   QVector<QVector<double>> readFilesSameDir(const QString dirctoryPath);
   void on_btnSendNumber_clicked();

   void on_btnConnectSerialPort_clicked();

private:
    Ui::MainWindow *ui;

    Server m_server;                // 下层服务

    NewSerialPort m_newSerialPort;

    MySerialPort m_serialPort;

    QString dirPath;                // 数据存放格式
};

#endif // MAINWINDOW_H
