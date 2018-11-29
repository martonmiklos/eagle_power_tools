#ifndef GRAPHICSUTILITIES_H
#define GRAPHICSUTILITIES_H

#include <QPainterPath>

class GraphicsUtilities
{
public:
    GraphicsUtilities();

    static bool painterPathQLineFIntersects(const QPainterPath & path, const QLineF & line);
};

#endif // GRAPHICSUTILITIES_H
