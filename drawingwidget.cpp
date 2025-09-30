#include "drawingwidget.h"

DrawingWidget::DrawingWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Canvas
    canvas = new DrawingCanvas(this);
    mainLayout->addWidget(canvas, 1);

    // --- Control Panel Setup ---
    runButton = new QPushButton("Run Convex Hull", this);
    clearButton = new QPushButton("Clear", this);
    algoSelector = new QComboBox(this);
    algoSelector->addItem("Slow");
    algoSelector->addItem("Optimized");

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(algoSelector);
    btnLayout->addWidget(runButton);
    btnLayout->addWidget(clearButton);

    mainLayout->addLayout(btnLayout);


    // Connect signals
    connect(runButton, &QPushButton::clicked, [this]() {
        canvas->computeConvexHull(algoSelector->currentIndex());
    });
    connect(clearButton, &QPushButton::clicked, canvas, &DrawingCanvas::clearCanvas);
}

// void DrawingWidget::clearCanvas()
// {
//     canvas->clearCanvas();
// }
