#include "graphicsutilities.h"

#include <qmath.h>
#include <QDebug>


bool GraphicsUtilities::lineIntersectsCurve(const QLineF &line,
                                            const QPointF &curvePtA,
                                            const QPointF &curvePtB,
                                            const QPointF &curveCtrlPtA,
                                            const QPointF &curveCtrlPtB,
                                            QPointF *intersectA,
                                            QPointF *intersectB,
                                            QPointF *intersectC,
                                            int *intersectionCnt)
{
    bool ret = false;
    // px,py,lx,ly
    // px/y0 : curve ptA
    // px/y1 : curve ctrlPtA
    // px/y2 : curve ctrlPtB
    // px/y3 : curve ptB
    // lx/y0 : wire.x1/y1
    // lx/y1 : wire.x2/y2

    qreal A = line.y2()-line.y1();	    //A=y2-y1
    qreal B = line.x1()-line.x2();	    //B=x1-x2
    qreal C = line.x1()*(line.y1()-line.y2()) +
            line.y1()*(line.x2()-line.x1());	//C=x1*(y1-y2)+y1*(x2-x1)

    QList<qreal> bx = bezierCoeffs(curvePtA.x(),curveCtrlPtA.x(),curveCtrlPtB.x(),curvePtB.x());
    QList<qreal> by = bezierCoeffs(curvePtA.y(),curveCtrlPtA.y(),curveCtrlPtB.y(),curvePtB.y());


    qreal P[4];
    P[0] = A*bx[0]+B*by[0];		/*t^3*/
    P[1] = A*bx[1]+B*by[1];		/*t^2*/
    P[2] = A*bx[2]+B*by[2];		/*t*/
    P[3] = A*bx[3]+B*by[3] + C;	/*1*/


    /*verify the roots are in bounds of the linear segment*/
    int i = 0;
    if (intersectionCnt)
        *intersectionCnt = 0;
    for (qreal t : cubicRoots(P[0], P[1], P[2], P[3])) {
        if (0.0 < t && t < 1.0) {
            qreal x = bx[0]*t*t*t+bx[1]*t*t+bx[2]*t+bx[3];
            qreal y = by[0]*t*t*t+by[1]*t*t+by[2]*t+by[3];

            /*above is intersection point assuming infinitely long line segment,
                  make sure we are also in bounds of the line*/
            qreal s;
            if (line.x1() == line.x2()) {
                // it is a vertical line
                s = (y-line.y1())/(line.y2()-line.y1());
            } else {
                s = (x-line.x1())/(line.x2()-line.x1());
            }

            /*in bounds?*/
            if (0.0 <= s && s <= 1.0) {
                QPointF *intersectPoint = nullptr;
                // move intersection point
                switch (i) {
                case 0:
                    intersectPoint = intersectA;
                    break;
                case 1:
                    intersectPoint = intersectB;
                    break;
                case 2:
                    intersectPoint = intersectC;
                    break;
                }

                if (intersectPoint) {
                    intersectPoint->setX(x);
                    intersectPoint->setY(y);
                } else {
                    // shortcut: user does not care about the current
                    // and the further intersection points return true and call it a day
                    return true;
                }
                if (intersectionCnt)
                    *intersectionCnt = (*intersectionCnt + 1);
                ret = true;
            }
        }
        i++;
    }
    return ret;
}

QList<qreal> GraphicsUtilities::cubicRoots(const qreal a, const qreal b, const qreal c, const qreal d)
{
    qreal A=b/a;
    qreal B=c/a;
    qreal C=d/a;

    qreal Q, R, D, Im;

    Q = (3.0*B - qPow(A, 2.0))/9.0;
    R = (9.0*A*B - 27.0 *C - 2.0*qPow(A, 3.0))/54.0;
    D = qPow(Q, 3.0) + qPow(R, 2.0);    // polynomial discriminant

    QList<qreal> t;
    t << 0.0;
    t << 0.0;
    t << 0.0;

    if (D >= 0.0) {
        // complex or duplicate roots
        qreal S = sgn(R + qSqrt(D))*qPow(qAbs(R + qSqrt(D)),(1.0/3.0));
        qreal T = sgn(R - qSqrt(D))*qPow(qAbs(R - qSqrt(D)),(1.0/3.0));

        t[0] = -A/3.0 + (S + T);                    // real root
        t[1] = -A/3.0 - (S + T)/2.0;                  // real part of complex root
        t[2] = -A/3.0 - (S + T)/2.0;                  // real part of complex root
        Im = qAbs(qSqrt(3.0)*(S - T)/2.0);    // complex part of root pair

        /*discard complex roots*/
        if (Im!=0) {
            t[1]=-1;
            t[2]=-1;
        }

    } else {
        // distinct real roots
        qreal th = qAcos(R/qSqrt(-qPow(Q, 3.0)));

        t[0] = 2.0 * qSqrt(-Q)*qCos(th/3.0) - A/3.0;
        t[1] = 2.0 * qSqrt(-Q)*qCos((th + 2.0*M_PI)/3.0) - A/3.0;
        t[2] = 2.0 * qSqrt(-Q)*qCos((th + 4.0*M_PI)/3.0) - A/3.0;
        Im = 0.0;
    }

    /*discard out of spec roots*/
    for (int i=0;i<3;i++) {
        if (t[i]<0 || t[i]>1.0)
            t[i]=-1;
    }

    /*sort but place -1 at the end*/
    t = sortSpecial(t);

    return t;
}

QList<qreal> GraphicsUtilities::sortSpecial(QList<qreal> a)
{
    bool flip;
    qreal temp;
    do {
        flip=false;
        for (int i=0;i<a.length()-1;i++) {
            if ((a[i+1]>=0 && a[i]>a[i+1]) ||
                    (a[i]<0 && a[i+1]>=0)) {
                flip=true;
                temp=a[i];
                a[i]=a[i+1];
                a[i+1]=temp;
            }
        }
    } while (flip);
    return a;
}

QList<qreal> GraphicsUtilities::bezierCoeffs(const qreal P0, const qreal P1, const qreal P2, const qreal P3)
{
    QList<qreal> Z;
    Z << -P0 + 3*P1 + -3*P2 + P3;
    Z << 3*P0 - 6*P1 + 3*P2;
    Z << -3*P0 + 3*P1;
    Z << P0;
    return Z;
}

qreal GraphicsUtilities::sgn(qreal n)
{
    return n < 0.0 ? 1 : -1;
}

