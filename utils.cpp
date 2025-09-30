#include "utils.h"

// Helper function to find orientation of ordered triplet (p, q, r).
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// 2 --> Counter-clockwise
int orientation(const QPoint &p, const QPoint &q, const QPoint &r)
{
    long long val = (long long)(q.y() - p.y()) * (r.x() - q.x()) -
                    (long long)(q.x() - p.x()) * (r.y() - q.y());

    if (val == 0) return 0; // Collinear
    return (val > 0) ? 1 : 2; // Clockwise (1) or Counter-clockwise (2)
}

qint64 distSq(const QPoint &p1, const QPoint &p2)
{
    return (long long)(p1.x() - p2.x()) * (p1.x() - p2.x()) +
           (long long)(p1.y() - p2.y()) * (p1.y() - p2.y());
}
