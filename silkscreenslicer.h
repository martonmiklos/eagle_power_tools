#ifndef SILKSCREENSLICER_H
#define SILKSCREENSLICER_H

#include "druloader.h"
#include "eagle.h"

class SilkscreenSlicer : public QObject
{
    Q_OBJECT
public:
    SilkscreenSlicer(Eagle *design, DRULoader *dru);

    /**
     * @brief fixSilkscreenWiresStopMaskCollisions
     * Checks all t/bPlace lines of the packages if they are intersect with
     * objects with stop layer (SMD/TH pads, rectangles on stop layers) and
     * slice these wires by converting the overlapping sections to t/bDocu wires
     * @param stopMaskOffsetInMm
     * The amount of distance to avoid the stop layers with the t/bPlace lines
     * Specified in mm, and it is calculated over the stop mask percentage.
     */
    void fixSilkscreenWiresStopMaskCollisions(qreal stopMaskOffsetInMm = 0);

    enum WireSegmentLayer {
        PlaceLayer,
        DocuLayer,
        UnknownLayer
    };
    Q_ENUM(WireSegmentLayer)

private:
    Eagle *m_library;
    DRULoader *m_dru;

    class InterSectionData {
    public:
        void *otherObject = nullptr;
        QPointF intersectionPoint;
        int intersectionCount = 0;
    };
};

#endif // SILKSCREENSLICER_H
