
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_btnConnect_clicked()
{
    QString ip = ui->leDeviceIp->text();

    // 连接设备
    qint32 flag = m_server.connectDevice(ip, DEVICE_TYPE::devicetype_analyzer);
    QString infoStr =   QString::number(flag);

    // 打印返回信息
    printInfo(infoStr);
}

void MainWindow::on_btnSetStartFreq_clicked()
{
    double value = ui->dsbStartFreq->value();
    int index = ui-> cbStartFreqUnit->currentIndex();
    value = value * pow(10.0, index * 3);

    // 设置设备起始频率
    qint32 flag = m_server.setStartFreq(value, DEVICE_TYPE::devicetype_analyzer);
    QString infoStr = QString::number(flag);

    // 打印返回信息
    printInfo(infoStr);

}

void MainWindow::on_btnQueryStartFreq_clicked()
{
    QString infoStr;
    // 查询起始频率
    infoStr = m_server.queryStartFreq(DEVICE_TYPE::devicetype_analyzer);

    // 打印返回信息
    printInfo(infoStr);
}

void MainWindow::on_btnSetStopFreq_clicked()
{
    double value = ui->dsbStopFreq->value();
    int index = ui-> cbStopFreqUnit->currentIndex();
    value = value * pow(10.0, index * 3);

    // 设置设备终止频率
    qint32 flag = m_server.setStopFreq(value, DEVICE_TYPE::devicetype_analyzer);
    QString infoStr = QString::number(flag);

    // 打印返回信息
    printInfo(infoStr);
}

void MainWindow::on_btnQueryStopFreq_clicked()
{
    QString infoStr;
    // 查询终止频率
    infoStr = m_server.queryStopFreq(DEVICE_TYPE::devicetype_analyzer);

    // 打印返回信息
    printInfo(infoStr);
}

// 打印信息到文本框中，其他函数调用的部分，以后可以换成其他函数
void MainWindow::printInfo(QString infoStr)
{
    ui->textBrowser->append(infoStr);
}




void MainWindow::on_btnReadFormatData_clicked()
{
    ViReal32 data[ANALYZER_MAX_POINT_NUM];
    long dataNum = m_server.queryCurFmtTrace(data, DEVICE_TYPE::devicetype_analyzer);
    for(int i = 0; i < dataNum; i++)
    {
        QString str;
        str.sprintf("%d:<%.3f,%.3f>", i / 2, data[i * 2], data[i * 2 + 1]);
        ui->textBrowser->append(str);
    }
}

void MainWindow::testBlockData()
{
    ViSession temp = m_server.m_analyzer.m_analyzerSession;

    ViReal32 pData[MAX_POINT_NUM * 2];
    ViInt32  nSize = 0;

    // 测试底层函数
    qDebug() << "nSize" << nSize;
    m_server.low2Device_readCurTraceFormatData(pData, nSize);
    qDebug() << "_ nSize" << nSize;

    //-------------
    ui->textBrowser->append("new_data");
    for(int i = 0; i < nSize/2; i++)
    {
        QString str;
        str.sprintf("%d:<%.3f,%.3f>", i / 2, pData[i * 2], pData[i * 2 + 1]);
        ui->textBrowser->append(str);
    }
}



void MainWindow::on_pushButton_clicked()
{
    trans_testBlockData();
}

// 完成，等待替换
void MainWindow::trans_testBlockData()
{
    qDebug()<< "MainWindow::trans_testBlockData";
    ViReal32 dataArray[MAX_POINT_NUM * 2];
    ViInt32  dataNum = 0;

    qDebug() << "nSize" << dataNum;

    m_server.low2Device_readCurTraceFormatData(dataArray, dataNum);
    qDebug() << "_ nSize" << dataNum;

    ui->textBrowser->append("new_data");
    for(int i = 0; i < dataNum / 2; i++)
    {
        QString str;
        str.sprintf("%d:<%.3f,%.3f>", i / 2, dataArray[i * 2], dataArray[i * 2 + 1]);
        ui->textBrowser->append(str);
    }
}

