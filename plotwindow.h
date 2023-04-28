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
    void plotGraph(QVector<QVector<double>>dataArray);

private:
    Ui::PlotWindow *ui;
};

#endif // PLOTWINDOW_H
