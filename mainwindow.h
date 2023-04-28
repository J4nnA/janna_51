﻿// 获取ui中的数据，只进行简单的处理，不进行数据类型的转换
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include "Service.h"

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

private:
    void printInfo(QString infoStr);

    bool saveDataToFile(const QString& dirPath, const QString& pexfix, const qint32& collectNum, const qint32 &timeInterval);

private:
    Ui::MainWindow *ui;

    Server m_server;                // 下层服务
};

#endif // MAINWINDOW_H
