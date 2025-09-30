#include "drawingwindow.h"

DrawingWindow::DrawingWindow(QWidget *parent)
    : QMainWindow(parent)
{
    drawingWidget = new DrawingWidget(this);
    setCentralWidget(drawingWidget);
}

DrawingWindow::~DrawingWindow(){}
