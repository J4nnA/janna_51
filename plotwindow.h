#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QDialog>
#include <QVector>
#include "qcustomplot.h"

namespace Ui {
class PlotWindow;
}

class PlotWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlotWindow(QWidget *parent = nullptr);
    ~PlotWindow();
    // 画图
    void plotGraph(const QVector<QVector<double>>dataArray);

    void incrementalPlot(const QVector<double>& newPoints);

    void plotData(const QVector<double>& data, int xCoord);
private:
    Ui::PlotWindow *ui;
};

#endif // PLOTWINDOW_H
