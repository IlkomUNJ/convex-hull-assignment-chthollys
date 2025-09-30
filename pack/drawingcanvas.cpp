#include "drawingcanvas.h"
#include "utils.h"


DrawingCanvas::DrawingCanvas(QWidget *parent)
    : QWidget{parent}
{
    setMinimumSize(600, 400);
    setStyleSheet("background-color: white; border: 1px solid gray");

    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &DrawingCanvas::animationStep);
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event)
{
    points.push_back(event->pos());
    update();
}

void DrawingCanvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint((QPainter::Antialiasing));

    // Draw points
    painter.setBrush(Qt::blue);
    for (const QPoint &p : points) {
        painter.drawEllipse(p, 3, 3);
    }

    if (isAnimating) {
        if (currentAlgorithm == Slow) {
            // Draw lines from pivot to all sorted points
            // Check against invalid index access
            if (i_idx < points.size()) {
                painter.setPen(QPen(Qt::lightGray, 1, Qt::DashLine));
                painter.drawLine(points[p_idx], points[i_idx]);
            }
            // The green line is for valid index
            painter.setPen(QPen(Qt::green, 1.5));
            painter.drawLine(points[p_idx], points[q_idx]);
        }
        else if (currentAlgorithm == Optimized) {
            // Draw a line to the point being considered
            painter.setPen(QPen(Qt::green, 1.5));
            if (graham_idx < sortedPoints.size()) {
                painter.drawLine(hull.last(), sortedPoints[graham_idx]);
            }
        }

    }

    // Draw convex hull edges
    for (int i = 0; i < hull.size(); i++) {
        QPoint a = hull[i];
        QPoint b = hull[(i + 1) % hull.size()];
        painter.drawLine(a, b);
    }

    // Draw iteration count
    painter.setPen(Qt::red);
    QRect textArea(0, height() - 30, width(), 30);
    painter.drawText(textArea, Qt::AlignCenter, QString("Iterations: %1").arg(iterations));
}

void DrawingCanvas::clearCanvas()
{
    animationTimer->stop();
    isAnimating = false;
    points.clear();
    hull.clear();
    iterations = 0;
    update();
}

void DrawingCanvas::computeConvexHull(int algoIndex)
{
    if (isAnimating || points.size() < 3) return;

    // Reset
    hull.clear();
    iterations = 0;

    currentAlgorithm = static_cast<Algorithm>(algoIndex + 1);
    if (currentAlgorithm == Slow) {
        // --- Initialize SLow Algorithm ---
        int start_idx = 0;
        for (int i = 1; i < points.size(); i++) {
            if (points[i].x() < points[start_idx].x()) start_idx = i;
        }
        p_idx = start_idx;
        i_idx = 0;
        hull.push_back(points[p_idx]);
    }
    else if (currentAlgorithm == Optimized) {
        // ---  Initialize Optimized Algorithm ---
        // Find pivot (lowest y, then lowest x))
        int pivot_idx = 0;
        for (int i = 1; i < points.size(); i++) {
            if (points[i].y() < points[pivot_idx].y() ||
                (points[i].y() == points[pivot_idx].y() &&
                                                          points[i].x() < points[pivot_idx].x()))
            {
                pivot_idx = i;
            }
        }

        QPoint pivot = points[pivot_idx];

        // Sort points by polar angle around the pivot
        sortedPoints = points;
        std::sort(sortedPoints.begin(), sortedPoints.end(),
                  [&](const QPoint& a, const QPoint& b) {
            int o = orientation(pivot, a, b);
            // Collinear: closer one first
            if (o == 0) return distSq(pivot, a) < distSq(pivot, b);
            // Counter-clockwise
            return (o == 2);
        });

        // Initialize hull and animation index
        hull.push_back(sortedPoints[0]);
        hull.push_back(sortedPoints[1]);
        graham_idx = 2;
    }

    isAnimating = true;
    animationTimer->start(animationSpeed);
}

void DrawingCanvas::animationStep()
{
    if (currentAlgorithm == Slow) {
        // --- Slow Animation Step ---
        if (i_idx >= points.size()) {
            if (hull.size() > 1 && points[p_idx] == hull.first()) {
                animationTimer->stop();
                isAnimating = false;
                update();
                return;
            }
            hull.push_back(points[p_idx]);
            i_idx = 0;
        }
        if (i_idx == 0) q_idx = (p_idx + 1) % points.size();

        int o = orientation(points[p_idx], points[i_idx], points[q_idx]);
        if (o == 2 || (o == 0 && distSq(points[p_idx], points[i_idx]) > distSq(points[p_idx], points[q_idx])))
        {
            q_idx = i_idx;
        }
        iterations++;
        i_idx++;
        if (i_idx >= points.size()) p_idx = q_idx;
    }

    else if (currentAlgorithm == Optimized) {
        // --- Optimized Animation Step ---
        if (graham_idx >= sortedPoints.size()) {
            animationTimer->stop();
            isAnimating = false;
            update();
            return;
        }

        QPoint nextPoint = sortedPoints[graham_idx];
        while (hull.size() >= 2 && orientation(hull[hull.size() - 2], hull.last(), nextPoint) != 2)
        {
            hull.pop_back(); // Pop if not a left turn
            iterations++;
        }
        hull.push_back(nextPoint);
        graham_idx++;
        iterations++;
    }

    update();
}

// QVector<QPoint> DrawingCanvas::slowConvexHull()
// {
//     if (points.size() < 3) {
//         return points;
//     }

//     QVector<QPoint> hull;

//     // Find the leftmost point
//     int l = 0;
//     for (int i = 1; i < points.size(); i++) {
//         if (points[i].x() < points[l].x()) {
//             l = 1;
//         }
//     }


//     // Start from the leftmost point and move counter-clockwise
//     // until reach the start point again.
//     int p = l, q;
//     this->iterations = 0;

//     do {
//         // Add current point to the hull
//         hull.push_back(points[p]);

//         // Find the next point 'q' that is most counter-clockwise
//         q = (p + 1) % points.size();

//         for (int i = 0; i < points.size(); i++) {
//             this->iterations++;
//             int o = orientation(points[p], points[i], points[q]);
//             if (o == 2) {
//                 q = i;
//             }
//             else if (o == 0 && distSq(points[p], points[i]) > distSq(points[p], points[q])) {
//                 q = i;
//             }
//         }

//         p = q;

//     } while (p != l);

//     return hull;
// }
