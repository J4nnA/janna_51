
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dirPath = QCoreApplication::applicationDirPath() + "/data";
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_btnConnect_clicked()
{
    QString ip = ui->leDeviceIp->text();

    // 连接设备
    qint32 flag = m_server.connectDevice(ip, M_DEVICE_TYPE::devicetype_analyzer);
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
    qint32 flag = m_server.setStartFreq(value, M_DEVICE_TYPE::devicetype_analyzer);
    QString infoStr = QString::number(flag);

    // 打印返回信息
    printInfo(infoStr);

}

void MainWindow::on_btnQueryStartFreq_clicked()
{
    QString infoStr;
    // 查询起始频率
    infoStr = m_server.queryStartFreq(M_DEVICE_TYPE::devicetype_analyzer);

    // 打印返回信息
    printInfo(infoStr);
}

void MainWindow::on_btnSetStopFreq_clicked()
{
    double value = ui->dsbStopFreq->value();
    int index = ui-> cbStopFreqUnit->currentIndex();
    value = value * pow(10.0, index * 3);

    // 设置设备终止频率
    qint32 flag = m_server.setStopFreq(value, M_DEVICE_TYPE::devicetype_analyzer);
    QString infoStr = QString::number(flag);

    // 打印返回信息
    printInfo(infoStr);
}

void MainWindow::on_btnQueryStopFreq_clicked()
{
    QString infoStr;
    // 查询终止频率
    infoStr = m_server.queryStopFreq(M_DEVICE_TYPE::devicetype_analyzer);

    // 打印返回信息
    printInfo(infoStr);
}


void MainWindow::on_btnReadSpecFile_clicked()
{
    QString fileName = dirPath + "/" + ui->leFileName->text();
    QVector<double> data = readFileByName(fileName);

}

void MainWindow::on_btnReadFormatData_clicked()
{
    // 提示信息
    qDebug()<< "MainWindow::on_btnReadFormatData_clicked";

    // 接收数据的数组，以及获取数据个数的变量
    ViReal32 dataArray[Server::DEVICE_MAX_POINT_NUM * 2];
    ViInt32  dataNum = 0;

    // 请求服务
    m_server.queryCurTraceFmtData(dataArray, dataNum, M_DEVICE_TYPE::devicetype_analyzer);
    qDebug() << "MainWindow::totalNum: " << dataNum;

    // 打印操作---日后可替换为其他功能的实现
    ui->textBrowser->append("dataArray: ");

    for(int i = 0; i < dataNum / 2; i++)
    {
        QString str;
        str.sprintf("%d: <%.3f,%.3f>", i / 2, dataArray[i * 2], dataArray[i * 2 + 1]);
        ui->textBrowser->append(str);
    }
}

void MainWindow::on_btnTempTest_clicked()
{
    // 获取当前路径，文件前缀，采集数目
    QString dirPath = QCoreApplication::applicationDirPath() + "/data";
    QString prefix = ui->leFilePrefix->text();
    qint32 collectNum = ui->leCollectNum->text().toInt();
    qint32 timeInterval = ui->leTimeInterval->text().toInt();

    bool flag = saveDataToFile(dirPath, prefix, collectNum, timeInterval);

}

void MainWindow::on_btnReadAllFile_clicked()
{
     QVector<QVector<double>> dataArray = readFilesSameDir(dirPath);

     qDebug() << "dataArray.size: " << dataArray.size();
     qDebug() << "dataArray[0].size: " << dataArray[0].size();

     //for(int i = 0; i < dataArray[4].size();i++){
     //    qDebug() << dataArray[4][i];
     //}
}

void MainWindow::on_btnPlotGraph_clicked()
{
    PlotWindow *plotWindow = new PlotWindow(this);


    QVector<QVector<double>>dataArray = readFilesSameDir(dirPath);

    plotWindow->show();

    plotWindow->plotGraph(dataArray);

}


// 打印信息到文本框中，其他函数调用的部分，以后可以换成其他函数
void MainWindow::printInfo(QString infoStr)
{
    ui->textBrowser->append(infoStr);
}

bool MainWindow::saveDataToFile(const QString &dirPath, const QString &prefix, const qint32 &collectNum, const qint32 &timeInterval)
{
    // 目录与文件前缀
    QDir dir(dirPath);
    QString filePrefix = dirPath + '/' + prefix;
    QString fileType = ".txt";


    // 检测目录是否存在
    if(!dir.exists())
    {
        if(dir.mkpath(dirPath))
        {
            qDebug() << "Directory created successfully.";
        }
        else
        {
            qDebug() << "Failed to create directory.";
        }
    }

    // 存储数据，生成指定数目的文件
    for(int i = 0; i < collectNum; i++)
    {
        QString num = QString::asprintf("%0*d", 3, i);
        QString fileName = filePrefix + num + fileType;

        QFile file(fileName);

        // 打开文件，覆盖模式
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file for writing.";
        }

        QTextStream out(&file);

        ViReal32 dataArray[Server::DEVICE_MAX_POINT_NUM * 2];
        ViInt32  dataNum = 0;                   // 实际采集点数

        // 获取数据
        m_server.queryCurTraceFmtData(dataArray, dataNum, M_DEVICE_TYPE::devicetype_analyzer);

        // 存储数据
        for(int j = 0; j < dataNum / 2; j++)
        {
            qDebug() << QString::number(dataNum);
            QString str;
            str.sprintf("%d:<%.3f,%.3f>", j / 2, dataArray[j * 2], dataArray[j * 2 + 1]);
            out << str << '\n';
            qDebug() << str;
        }
        qDebug() <<"\"" << fileName << "\"";
        // 时间间隔
        QThread::msleep(timeInterval);
    }
    return 1;
}

QVector<double>  MainWindow::readFileByName(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << filename;
        return {};
    }

    QTextStream in(&file);
    QRegularExpression re("<(\\d+\\.?\\d*),(\\d+\\.?\\d*)>");

    QVector<double> data;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QRegularExpressionMatch match = re.match(line);

        if (match.hasMatch()) {
            double x = match.captured(1).toDouble();
            double y = match.captured(2).toDouble();

            data.append(y);
        }
    }

    file.close();

    return data;
}

QVector<QVector<double>> MainWindow::readFilesSameDir(const QString dirctoryPath)
{
    QDir dataDir(dirctoryPath);
    QStringList filters = {"test_data_*.txt"};  //这里相当于qstring，但是为了和日后扩展，使用qstringList
    QStringList fileNames = dataDir.entryList(filters, QDir::Files, QDir::Name);
            // 从QDir下获取文件名称，存为字符串数组，三个参数：筛选条件（字符串数组，可以是0或任意），
                                                        //仅列出文件 不包含目录
                                                        //用名字来排序
    QVector<QVector<double>>dataArray;
    for(int i = 0; i < fileNames.size(); i++)
    {
        QVector<double> data = readFileByName(dirctoryPath + "/" + fileNames[i]);
        dataArray.append(data);
    }

    return dataArray;

}















