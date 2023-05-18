// 因更换serialPort对象，故转台控制功能暂时失效，记得使用该功能前进行对象替换

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dirPath = QCoreApplication::applicationDirPath() + "/data";

    m_collectPoint = 12;

    m_collectAngInt = 30.0;

    m_plotWindow = new PlotWindow(this);

    m_test = new TestNew(5000);
    // 等待接收返回数据并打印
    connect(&m_serialPort, &MySerialPort::retData, this, [=](qint32 data){
        printInfo(QString::number(data));
    });

    // 查询最大角速度后，处理数据
    connect(&m_serialPort, &MySerialPort::retMaxAngleVelocity, this,[=](float angleVolecity){
       printInfo("MaxAngleVelocity: " + QString::number(angleVolecity));
    });

    // 查询最小角速度后，处理数据
    connect(&m_serialPort, &MySerialPort::retMinAngleVelocity, this,[=](float angleVolecity){
       printInfo("MinAngleVelocity: " + QString::number(angleVolecity));
    });

    // 启动采集绘图功能
    connect(m_test, &TestNew::readyCollect, this, &MainWindow::onTimeReady);
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
    qDebug() << "FileName: " << fileName;
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
    qint32 collectNum = ui->leCollectPoint->text().toInt();
    qint32 timeInterval = ui->leTimeInterval->text().toInt();

    // 启动转台
    //m_serialPort.start();

    // 等待16.4s开始采集
    //
    //QThread::msleep(16400);
    //bool flag = saveDataToFile(dirPath, prefix, collectNum, timeInterval);

    // 使用多线程来调用该功能
    // 任务实例化
    DataCollector * dataCollector = new DataCollector(*this, dirPath, prefix, collectNum, timeInterval);
    // 启动
    QThreadPool::globalInstance()->start(dataCollector);
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

void MainWindow::setCollectPoint(qint32 collectPoint)
{
    // 设置采集点数
    m_collectPoint = collectPoint;
    // 采集间隔也同步修改
    m_collectAngInt = 360.0 / m_collectPoint;
}

void MainWindow::setCollectAngInt(float collectAngInt)
{
    m_collectAngInt = collectAngInt;
    m_collectPoint = 360 / m_collectAngInt;
}




bool MainWindow::saveDataToFile(const QString &dirPath,
                                const QString &prefix,
                                const qint32 &collectNum,
                                const qint32 &timeInterval)const
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
        //QString num = QString::asprintf("%0*d", 3, i);
        QString num;
        num.sprintf("%0*d", 3, i);
        QString fileName = filePrefix + num + fileType;

        QFile file(fileName);

        // 打开文件，覆盖模式
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            //qDebug() << "Failed to open file for writing.";
        }

        QTextStream out(&file);

        ViReal32 dataArray[Server::DEVICE_MAX_POINT_NUM * 2];
        ViInt32  dataNum = 0;                   // 实际采集点数

        // 获取数据
        m_server.queryCurTraceFmtData(dataArray, dataNum, M_DEVICE_TYPE::devicetype_analyzer);

        // 存储数据
        for(int j = 0; j < dataNum / 2; j++)
        {
            //qDebug() << QString::number(dataNum);
            QString str;
            str.sprintf("%d:<%.3f,%.3f>", j / 2, dataArray[j * 2], dataArray[j * 2 + 1]);
            out << str << '\n';
            //qDebug() << str;
        }
        //qDebug() <<"\"" << fileName << "\"";
        // 时间间隔
        QThread::msleep(timeInterval);
    }

    return 1;
}

bool MainWindow::saveAndRead(const QString &dirPath,
                             const QString &prefix,
                             const qint32 &collectNum,
                             const qint64 &startTime,
                             const qint64 &intervalTime) const
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
            return false; // Failed to create the directory, return from the function
        }
    }

    // 生成绘图窗口对象
    PlotWindow *plotWindow = new PlotWindow();
    plotWindow->show();

    // 存储数据，生成指定数目的文件
    for(int i = 0; i < collectNum; i++)
    {
        QString num;
        num.sprintf("%0*d", 3, i);
        QString fileName = filePrefix + num + fileType;

        QFile file(fileName);

        // 打开文件，覆盖模式
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file for writing.";
            continue; // Skip this iteration if failed to open the file
        }

        QTextStream out(&file);

        ViReal32 dataArray[Server::DEVICE_MAX_POINT_NUM * 2];
        ViInt32  dataNum = 0;                   // 实际采集点数

        // 使用qvector存储数据
        QVector<double> dataVector;

        // 获取数据
        m_server.queryCurTraceFmtData(dataArray, dataNum, M_DEVICE_TYPE::devicetype_analyzer);

        // 存储数据
        for(int j = 0; j < dataNum / 2; j++)
        {
            double y = dataArray[j * 2 + 1];
            dataVector.append(y);

            QString str;
            str.sprintf("%d:<%.3f,%.3f>", j / 2, dataArray[j * 2], dataArray[j * 2 + 1]);
            out << str << '\n';
        }

        // 调用绘图函数
        plotWindow->incrementalPlot(dataVector);

        // 时间间隔
        QThread::msleep(intervalTime);

    }

    return true;
}

bool MainWindow::saveDrawTest(const QString &dirPath, const QString &prefix)
{
    // 目录与文件前缀
    QDir dir(dirPath);
    QString filePrefix = dirPath + '/' + prefix;
    QString fileType = ".txt";

    qDebug() << "dirPath:" << dirPath;
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
            return false; // Failed to create the directory, return from the function
        }
    }


    QVector<double> temp;
    temp.append(1.0);
    temp.append(2.0);
    temp.append(2.0);
    temp.append(3.0);

    // 绘图

    emit dataReady(temp);
}



void MainWindow::print_temp(qint32 data)
{
    printInfo(QString::number(data));
}

QVector<double>  MainWindow::readFileByName(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << filename;
        return {};
    }

    QTextStream in(&file);
    QRegularExpression re("<(-?\\d+\\.?\\d*),(-?\\d+\\.?\\d*)>");
    //   < ( d + . d*),(d + . d*) >
    QVector<double> data;
        qDebug() << "before: " << data.size();
    while (!in.atEnd()) {
        QString line = in.readLine();
        QRegularExpressionMatch match = re.match(line);

        if (match.hasMatch()) {
            //double x = match.captured(1).toDouble();
            double y = match.captured(2).toDouble();

            data.append(y);
        }
    }
    //qDebug() <<
    qDebug() << "after: " << data.size();
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



void MainWindow::on_btnVnaModeSwitch_clicked()
{
    qint32 flag = m_server.setMeasMode(SCPI_MEAS_MODE::s_measmode_VNA, M_DEVICE_TYPE::devicetype_analyzer);
    printInfo(QString::number(flag));
}


void MainWindow::on_btnSetSweepPoint_clicked()
{
    qint32 value = ui->leSweepPoint->text().toInt();

    qint32 flag = m_server.setSweepPoint(value, M_DEVICE_TYPE::devicetype_analyzer);

    printInfo(QString::number(flag));
}


void MainWindow::on_btnQuerySweepPoint_clicked()
{
    QString infoStr;

    infoStr = m_server.querySweepPoint(M_DEVICE_TYPE::devicetype_analyzer);

    printInfo(infoStr);
}



void MainWindow::on_btnConnectSerialPort_clicked()
{
    bool flag = m_serialPort.connectSerialPort();

    printInfo(QString::number(flag));
}


void MainWindow::on_btnStrQuery_clicked()
{
    m_serialPort.queryMaxAngleVolecity();
}


void MainWindow::on_btnQueryMaxAngVol_clicked()
{
    // 调用下层函数，其实就是发信号，但注意查询这次要分类了
    m_serialPort.queryMaxAngleVolecity();

}


void MainWindow::on_btnSetMaxAngVol_clicked()
{
    // 获取设置值
    float angleVolecity = ui->leMaxAngVol->text().toFloat();
    m_serialPort.setMaxAngleVolecity(angleVolecity);
}


void MainWindow::on_btnQueryMinAngVol_clicked()
{
    m_serialPort.queryMinAngleVolecity();
}

void MainWindow::on_btnSetMinAngVol_clicked()
{
    // 获取设置值
    float angleVolecity = ui->leMinAngVol->text().toFloat();
    m_serialPort.setMinAngleVolecity(angleVolecity);
}


void MainWindow::on_btnSetCollectPoint_clicked()
{
    qint32 collectPoint = ui->leCollectPoint->text().toInt();
    qDebug() << "setCollectPoint: " << collectPoint;
    setCollectPoint(collectPoint);
}

void MainWindow::on_btnSetAngleInterval_clicked()
{
    float collectAngInt = ui->leCollectAngInterval->text().toFloat();
    setCollectAngInt(collectAngInt);
}

void MainWindow::on_btnQueryPointAndInterval_clicked()
{
    printInfo("m_collectPoint: " + QString::number(m_collectPoint));
    printInfo("m_collectAngInt: " + QString::number(m_collectAngInt));
}


void MainWindow::on_btnQueryIntervalTime_clicked()
{
    float intervalTime = m_serialPort.queryWorkTimeForUpper() / m_collectPoint;
    printInfo("IntervalTime: " + QString::number(intervalTime));
    printInfo("startTime: " + QString::number(m_serialPort.queryStartTimeForUpper()));

}

void MainWindow::finalTestSave(const QString dirPath,
                               const QString prefix,
                               const qint32 collectPoint,
                               const qint64 timeInterval)const
{
    qDebug() << "MainWindow::finalTestSave.";
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
            return; // Failed to create the directory, return from the function
        }
    }

    // 打开窗口
    m_plotWindow->show();

    for(int i = 0; i < collectPoint; i++)
    {
        QString num;
        num.sprintf("%0*d", 3, i);
        // 生成保存文件名
        QString fileName = filePrefix + num + fileType;

        ViReal32 dataArray[Server::DEVICE_MAX_POINT_NUM * 2];
        ViInt32  dataNum = 0;                   // 实际采集点数

        // 使用qvector存储数据
        QVector<double> dataVector;  // 用于保存y值的向量
        QVector<double> idValueVector;  // 用于保存编号和对应值的向量

        // 获取数据
        m_server.queryCurTraceFmtData(dataArray, dataNum, M_DEVICE_TYPE::devicetype_analyzer);

        // 存储数据
        for(int j = 0; j < dataNum / 2; j++)
        {
            double y = dataArray[j * 2 + 1];
            dataVector.append(y);

            // 追加存储编号和对应值的代码
            idValueVector.append(dataArray[j * 2]);
            idValueVector.append(dataArray[j * 2 + 1]);
        }

        qDebug() << "idValueVector: " << idValueVector.size();

        // 创建保存到文件的操作并将其放入新的线程中
        FileSaver* fileSaver = new FileSaver(fileName, idValueVector);
        QThread* thread = new QThread;
        fileSaver->moveToThread(thread);
        connect(thread, &QThread::started, fileSaver, &FileSaver::saveToFile);
        connect(fileSaver, &FileSaver::finished, thread, &QThread::quit);
        connect(fileSaver, &FileSaver::finished, fileSaver, &FileSaver::deleteLater);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        thread->start();

        // 调用绘图函数
        m_plotWindow->plotData(idValueVector, i);
        QCoreApplication::processEvents();  // 添加这行代码
        // 时间间隔
        QThread::msleep(timeInterval);
    }
}



void MainWindow::on_pushButton_4_clicked()
{
    // 获取当前路径，文件前缀，采集数目
    QString dirPath = QCoreApplication::applicationDirPath() + "/data";
    QString prefix = ui->leFilePrefix->text();
    qint32 collectNum = ui->leCollectPoint->text().toInt();
    qint64 startTime = m_serialPort.queryStartTimeForUpper();
    qint64 timeInterval = m_serialPort.queryWorkTimeForUpper() / m_collectPoint;

    qDebug() << "start";
    SaveAndDraw * saveAndDraw = new SaveAndDraw(*this, dirPath, prefix, collectNum, startTime, timeInterval);
    QThreadPool::globalInstance()->start(saveAndDraw);
}

void MainWindow::onTimeReady()
{
    qDebug() << "ready";
    // 准备绘图函数
    // 获取当前路径，文件前缀，采集数目
    QString dirPath = QCoreApplication::applicationDirPath() + "/data";
    QString prefix = ui->leFilePrefix->text();
    qint64 timeInterval = m_serialPort.queryWorkTimeForUpper() / m_collectPoint;

    // 存储数据，绘制图像
    finalTestSave(dirPath, prefix, m_collectPoint, timeInterval);
}




void MainWindow::on_btnSaveDrawOne_clicked()
{

    qDebug() << "MainWindow::on_btnSaveDrawOne_clicked";
    // 启动转台
    m_serialPort.startWork();
    // 启动计时
    qint64 temp = m_serialPort.queryStartTimeForUpper();
    m_test->updataStartTime(temp);
    m_test->start();
}


void MainWindow::on_btnPlot_clicked()
{
    QVector<double> a = {1.0,120.0,2.0,110,3.0,100};
    QVector<double> b = {1.0,100.0,2.0,110,3.0,120};
    PlotWindow* plotWindow = new PlotWindow();
    plotWindow->show();
    plotWindow->plotData(a, 1);
    plotWindow->plotData(b, 2);
}


void MainWindow::on_pushButton_clicked()
{

}

