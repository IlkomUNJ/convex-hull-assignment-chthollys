#ifndef DRAWINGWINDOW_H
#define DRAWINGWINDOW_H

#include "drawingwidget.h"
#include "drawingcanvas.h"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class DrawingWindow : public QMainWindow
{
    Q_OBJECT

public:
    DrawingWindow(QWidget *parent = nullptr);
    ~DrawingWindow();

private:
    DrawingWidget *drawingWidget;

};
#endif // DRAWINGWINDOW_H
