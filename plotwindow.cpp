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

void PlotWindow::plotGraph(QVector<QVector<double> > dataArray)
{
    qDebug() << "build";

    int numCurves = dataArray[0].size();

    // 设置X轴范围
        ui->customPlot->xAxis->setRange(0, dataArray.size() - 1);

        // 为每条曲线设置不同的颜色
        QVector<QColor> colors = {Qt::red, Qt::blue, Qt::green, Qt::black};

        for (int i = 0; i < numCurves; ++i) {
            QCPGraph *graph = ui->customPlot->addGraph();
            graph->setPen(QPen(colors[i % colors.size()]));

            QVector<double> x(dataArray.size()), y(dataArray.size());

            for (int j = 0; j < dataArray.size(); ++j) {
                x[j] = j;
                y[j] = dataArray[j][i];
            }

            graph->setData(x, y);
        }

        // 调整Y轴范围以适应数据
        ui->customPlot->rescaleAxes();
        ui->customPlot->replot();
}
