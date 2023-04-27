
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


void MainWindow::on_btnReadFormatData_clicked()
{
    // 提示信息
    qDebug()<< "MainWindow::on_btnReadFormatData_clicked";

    // 接收数据的数组，以及获取数据个数的变量
    ViReal32 dataArray[Server::DEVICE_MAX_POINT_NUM * 2];
    ViInt32  dataNum = 0;

    // 请求服务
    m_server.queryCurTraceFmtData(dataArray, dataNum, DEVICE_TYPE::devicetype_analyzer);
    qDebug() << "MainWindow::totalNum: " << dataNum;

    ui->textBrowser->append("dataArray: ");

    for(int i = 0; i < dataNum / 2; i++)
    {
        QString str;
        str.sprintf("%d: <%.3f,%.3f>", i / 2, dataArray[i * 2], dataArray[i * 2 + 1]);
        ui->textBrowser->append(str);
    }
}


// 打印信息到文本框中，其他函数调用的部分，以后可以换成其他函数
void MainWindow::printInfo(QString infoStr)
{
    ui->textBrowser->append(infoStr);
}


