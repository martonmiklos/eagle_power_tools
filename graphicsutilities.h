#ifndef GRAPHICSUTILITIES_H
#define GRAPHICSUTILITIES_H

#include <QLineF>
#include <QPainterPath>
#include <QPointF>

class GraphicsUtilities
{
public:
    static bool lineIntersectsCurve(const QLineF &line,
                                    const QPointF &curvePtA,
                                    const QPointF &curvePtB,
                                    const QPointF &curveCtrlPtA,
                                    const QPointF &curveCtrlPtB,
                                    QPointF *intersectA = nullptr,
                                    QPointF *intersectB = nullptr,
                                    QPointF *intersectC = nullptr,
                                    int *intersectionCnt = nullptr);
private:
    static QList<qreal> cubicRoots(const qreal a, const qreal b, const qreal c, const qreal d);
    static QList<qreal> sortSpecial(QList<qreal> a);
    static QList<qreal> bezierCoeffs(const qreal P0, const qreal P1, const qreal P2, const qreal P3);
    static qreal sgn(qreal n);
};

#endif // GRAPHICSUTILITIES_H
