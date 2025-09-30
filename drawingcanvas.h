#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QWidget>
#include <QPen>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>

class DrawingCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingCanvas(QWidget *parent = nullptr);

public slots:
    void clearCanvas();
    void computeConvexHull(int algoIndex);

private slots:
    void animationStep()    ;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    enum Algorithm { None, Slow, Optimized };
    QVector<QPoint> points;
    QVector<QPoint> hull;
    int iterations = 0;

    // QVector<QPoint> slowConvexHull();
    // QVector<QPoint> fastConvexHull();

    QTimer *animationTimer;
    bool isAnimating = false;
    Algorithm currentAlgorithm = None;
    int animationSpeed = 500;

    // State for Slow Algorithm
    int p_idx, q_idx, i_idx;

    // State for Optimized Algorithm
    QVector<QPoint> sortedPoints;
    int graham_idx;
};

#endif // DRAWINGCANVAS_H
