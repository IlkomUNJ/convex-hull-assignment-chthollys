#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include "drawingcanvas.h"

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>


class DrawingWidget : public QWidget
{
    Q_OBJECT
public:
    DrawingWidget(QWidget *parent = nullptr);

    void clearCanvas();
    void runConvexHull();

private:
    DrawingCanvas *canvas;
    QPushButton *runButton;
    QPushButton *clearButton;
    QComboBox *algoSelector;
};

#endif // DRAWINGWIDGET_H
