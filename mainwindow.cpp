
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

// 打印信息到文本框中，其他函数调用的部分，以后可以换成其他函数
void MainWindow::printInfo(QString infoStr)
{
    ui->textBrowser->append(infoStr);
}

