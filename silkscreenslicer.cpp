#include "silkscreenslicer.h"

#include "qt_eagle_xml_parser/utils.h"

#include <QLabel>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QTransform>

SilkscreenSlicer::SilkscreenSlicer(Eagle *design, DRULoader *dru) :
    QObject(),
    m_library(design),
    m_dru(dru)
{

}

void SilkscreenSlicer::fixSilkscreenWiresStopMaskCollisions(qreal stopMaskOffsetInMm)
{
    enum {
        WireLayerIndex,
        MaskIndex
    };
    int tPlaceLayerIndex = EAGLE_Utils::layerIndexFromName("tPlace", m_library);
    int bPlaceLayerIndex = EAGLE_Utils::layerIndexFromName("bPlace", m_library);
    int tDocuLayerIndex = EAGLE_Utils::layerIndexFromName("tDocu", m_library);
    int bDocuLayerIndex = EAGLE_Utils::layerIndexFromName("bDocu", m_library);
    int tStopLayerIndex = EAGLE_Utils::layerIndexFromName("tDocu", m_library);
    int bStopLayerIndex = EAGLE_Utils::layerIndexFromName("bDocu", m_library);
    int layerCrossCheck[2][2] = {
        {
            tPlaceLayerIndex,
            EAGLE_Utils::layerIndexFromName("Top", m_library)
        },
        {
            bPlaceLayerIndex,
            EAGLE_Utils::layerIndexFromName("Bottom", m_library)
        }
    };

    for (Package *pkg : *m_library->drawing()->library()->packages()->packageList()) {
        QList<Wire*> newWireSegments;
        for (Wire *wire : *pkg->wireList()) {
            if (wire->layer() != tPlaceLayerIndex && wire->layer() != bPlaceLayerIndex)
                continue;

            QLineF wireLine = EAGLE_Utils::wire2QLine(*wire);
            bool p1Internal = false;
            bool removeOriginalWire = false;
            QPointF pt1, pt2, internalPt;

            QList<InterSectionData> intersections;

            // check for t/bStop rectangles - wire overlaps
            for (Rectangle *rect : *pkg->rectangleList()) {
                if ((rect->layer() != tStopLayerIndex && wire->layer() == tPlaceLayerIndex) ||
                    (rect->layer() != bStopLayerIndex && wire->layer() == bPlaceLayerIndex)) {
                    int intersectionCount = EAGLE_Utils::rectWireIntersections(*rect,
                                                                                      *wire,
                                                                                      &internalPt,
                                                                                      &pt1,
                                                                                      &pt2);
                    if (intersectionCount) {
                        if (intersectionCount >= 1) {
                            InterSectionData i;
                            i.otherObject = rect;
                            i.intersectionPoint = pt1;
                            i.intersectionCount = intersectionCount;
                            intersections.append(i);
                        }

                        if (intersectionCount == 2) {
                            InterSectionData i;
                            i.otherObject = rect;
                            i.intersectionPoint = pt2;
                            i.intersectionCount = intersectionCount;
                            intersections.append(i);
                        }

                        if (wireLine.p1() == internalPt)
                            p1Internal = true;

                        removeOriginalWire = true;
                    }
                }
            }
            // END of t/bStop rectangles - wire check

            // TODO check for t/bStop wire - t/bPlaceWire overlaps

            //
            for (Polygon *polygon : *pkg->polygonList()) {

            }
            //

            // Check for wire - TH pads overlaps
            for (Pad *pad : *pkg->padList()) {
                int padIntersectionCount = EAGLE_Utils::padStopMaskWireIntersections(*pad,
                                                                                  *wire,
                                                                                  &internalPt,
                                                                                  &pt1,
                                                                                  &pt2,
                                                                                  m_dru);
                if (padIntersectionCount) {
                    if (padIntersectionCount >= 1) {
                        InterSectionData i;
                        i.otherObject = pad;
                        i.intersectionPoint = pt1;
                        i.intersectionCount = padIntersectionCount;
                        intersections.append(i);
                    }

                    if (padIntersectionCount == 2) {
                        InterSectionData i;
                        i.otherObject = pad;
                        i.intersectionPoint = pt2;
                        i.intersectionCount = padIntersectionCount;
                        intersections.append(i);
                    }

                    if (wireLine.p1() == internalPt)
                        p1Internal = true;

                    removeOriginalWire = true;
                }
            }
            // END of pads - wire check

            // find overlapping the t/bPlace wires - Top/Bottom SMD pad pairs
            for (auto i : layerCrossCheck) {
                if (wire->layer() == i[WireLayerIndex]) {
                    for (Smd *smd : *pkg->smdList()) {
                        if (smd->layer() == i[MaskIndex] && smd->stop() != Smd::Stop_no) {
                            int intersectionCount = EAGLE_Utils::smdStopMaskWireIntersections(*smd,
                                                                                              *wire,
                                                                                              &internalPt,
                                                                                              &pt1,
                                                                                              &pt2,
                                                                                              m_dru);
                            if (intersectionCount) {
                                if (intersectionCount >= 1) {
                                    InterSectionData i;
                                    i.otherObject = smd;
                                    i.intersectionPoint = pt1;
                                    i.intersectionCount = intersectionCount;
                                    intersections.append(i);
                                }

                                if (intersectionCount == 2) {
                                    InterSectionData i;
                                    i.otherObject = smd;
                                    i.intersectionPoint = pt2;
                                    i.intersectionCount = intersectionCount;
                                    intersections.append(i);
                                }

                                if (wireLine.p1() == internalPt)
                                    p1Internal = true;

                                removeOriginalWire = true;
                            }
                        }
                    }
                } // endif wire is t/bPlace
            } // for (auto i : layerCrossCheck)
            // END of SMDs - wire check

            // start slicing of the wire from P1
            WireSegmentLayer currentSegmentLayer = PlaceLayer;
            QPointF startingPoint = wireLine.p1();

            // determine layer shall the slicing started
            if (p1Internal)
                currentSegmentLayer = DocuLayer;

            // sort the intersection points in the order
            // of distance from the P1 of the original wire
            std::sort(intersections.begin(),
                  intersections.end(),
                  [startingPoint](const InterSectionData &a, const InterSectionData &b) ->
                  bool {
                    QLineF line1(startingPoint, a.intersectionPoint), line2(startingPoint, b.intersectionPoint);
                    return line1.length() < line2.length();
                  }
            );

            QLineF currentSegmentLine;
            Wire *newWireSegment = nullptr;
            // P2 will be moved to P1 in the very first iteration of the loop below
            currentSegmentLine.setP2(startingPoint);

            QList<void*> openObjects;
            bool first = true;
            for (const InterSectionData & dt : intersections) {
                bool add = false;
                if (openObjects.contains(dt.otherObject)) {
                    openObjects.removeOne(dt.otherObject);
                } else {
                    add = true;
                }

                if (p1Internal && first)
                    add = false;
                first = false;

                if (openObjects.count() == 0) {
                    currentSegmentLine.setP1(currentSegmentLine.p2());
                    currentSegmentLine.setP2(dt.intersectionPoint);

                    newWireSegment = new Wire(*wire);
                    newWireSegment->setLayer(
                                currentSegmentLayer == DocuLayer
                                ? (wire->layer() == tPlaceLayerIndex ? tDocuLayerIndex : bDocuLayerIndex)
                                : (wire->layer() == tPlaceLayerIndex ? tPlaceLayerIndex : bPlaceLayerIndex)
                                );

                    newWireSegments.append(newWireSegment);
                    // we need to move only the P2
                    if (currentSegmentLayer == PlaceLayer) {
                        // move P2 away from the stop mask
                        currentSegmentLine.setLength(currentSegmentLine.length() - wire->width() / 2.0 - stopMaskOffsetInMm);
                    } else if (currentSegmentLayer == DocuLayer) {
                        // started from inside a pad -> increase the length of the docu layer
                        // to draw over the pad
                        currentSegmentLine.setLength(currentSegmentLine.length() + wire->width() / 2.0 + stopMaskOffsetInMm);
                    }
                    EAGLE_Utils::QLineF_ToWire(currentSegmentLine, newWireSegment);
                    qWarning() << currentSegmentLine.x1() << currentSegmentLine.x2() << currentSegmentLayer;

                    currentSegmentLayer = currentSegmentLayer == DocuLayer ?
                              PlaceLayer : DocuLayer;
                }

                if (add) {
                    openObjects.append(dt.otherObject);
                }
            }

            currentSegmentLine.setP1(currentSegmentLine.p2());
            currentSegmentLine.setP2(wireLine.p2());
            newWireSegment = new Wire(*wire);
            newWireSegment->setLayer(
                        currentSegmentLayer == DocuLayer
                        ? (wire->layer() == tPlaceLayerIndex ? tDocuLayerIndex : bDocuLayerIndex)
                        : (wire->layer() == tPlaceLayerIndex ? tPlaceLayerIndex : bPlaceLayerIndex)
                        );

            EAGLE_Utils::QLineF_ToWire(currentSegmentLine, newWireSegment);
            newWireSegments.append(newWireSegment);

            if (removeOriginalWire)
                pkg->wireList()->removeOne(wire);
            delete wire;
        } // loop on the wires of the package

        if (newWireSegments.count())
            pkg->wireList()->append(newWireSegments);
    } // loop in the packages of the library
}

