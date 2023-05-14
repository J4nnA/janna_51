#include "plotwindow.h"
#include "ui_plotwindow.h"

PlotWindow::PlotWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotWindow)
{
    ui->setupUi(this);
}

PlotWindow::~PlotWindow()
{
    delete ui;
}

void PlotWindow::plotGraph(const QVector<QVector<double> > dataArray)
{
    qDebug() << "build";
    qDebug() << dataArray.size();
    qDebug() << dataArray[0].size();

    int numCurves = dataArray[0].size();

    // 设置X轴范围
        ui->customPlot->xAxis->setRange(0, dataArray.size() - 1);       // 设置x轴范围

        // 为每条曲线设置不同的颜色
        QVector<QColor> colors = {Qt::red, Qt::blue, Qt::green, Qt::black}; // 颜色数组，固定大小，但是很多

        for (int i = 0; i < numCurves; ++i) {
            QCPGraph *graph = ui->customPlot->addGraph();                   //
            graph->setPen(QPen(colors[i % colors.size()]));

            QVector<double> x(dataArray.size()), y(dataArray.size());

            for (int j = 0; j < dataArray.size(); ++j) {
                x[j] = j;
                y[j] = dataArray[j][i];
            }

            graph->setData(x, y);
        }

        // 调整Y轴范围以适应数据
        //ui->customPlot->yAxis->setRange(-100,0);
        ui->customPlot->rescaleAxes();
        ui->customPlot->replot();
}

void PlotWindow::incrementalPlot(const QVector<double>& newPoints)
{
    static QVector<double> dataVector; // 用于存储所有数据的静态向量
    static QCPGraph* graph = nullptr;  // 用于存储图形的静态指针

    // 将新的数据点添加到数据向量中
    for (double point : newPoints) {
        dataVector.push_back(point);
    }

    // 如果这是第一次调用函数，创建新的图形
    if (graph == nullptr) {
        graph = ui->customPlot->addGraph();
        graph->setPen(QPen(Qt::red));
    }

    // 创建x和y的向量
    QVector<double> x(dataVector.size() / 2), y(dataVector.size() / 2);
    for (int j = 0; j < dataVector.size() / 2; ++j) {
        x[j] = j;
        y[j] = dataVector[2*j+1];  // 假设数据向量中的y值在每对点的第二个位置
    }

    // 更新图形的数据并重新绘制
    graph->setData(x, y);
    ui->customPlot->xAxis->setRange(0, x.size() - 1);  // 更新x轴的范围
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
    qDebug() << "draw";
}

void PlotWindow::plotData(const QVector<double>& data, int xCoord) {
    // 创建图形映射表
    static QMap<double, QCPGraph*> graphMap;
    static QVector<QColor> colors = {Qt::red, Qt::blue, Qt::green, Qt::black};

    for (int i = 0; i < data.size(); i += 2) {
        double id = data[i];
        double value = data[i + 1];

        // 查看给定的编号是否已经有对应的图形
        QCPGraph* graph;
        if (graphMap.contains(id)) {
            // 如果有，就使用已有的图形
            graph = graphMap[id];
        } else {
            // 如果没有，就创建一个新的图形
            graph = ui->customPlot->addGraph();
            graph->setPen(QPen(colors[graphMap.size() % colors.size()]));

            // 把新的图形添加到图形映射表中
            graphMap[id] = graph;
        }

        // 在图形上添加新的数据点
        graph->addData(xCoord, value);
    }

    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
}

