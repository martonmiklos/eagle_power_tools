#include "gerberimportfixer.h"

#include "qt_eagle_xml_parser/utils.h"

#include <QDebug>
#include <QUuid>

GerberImportFixer::GerberImportFixer(const QString &brdFilePath) :
    m_brdPath(brdFilePath)
{
    bool ok = false;
    m_design = Eagle::parseFile(brdFilePath, &ok);
    if (!ok) {
        delete(m_design);
        m_design = nullptr;
    }
}

bool GerberImportFixer::fix()
{
    if (!m_design)
        return false;

    int layers[2] = {1, 16};
    int signalIndex = 0;

    // gerber import will name the bottom/top wire segments similarly
    /*    for (Signal *signal : *m_design->drawing()->board()->signals_()->signalList()) {
        if (signal->name().contains("$")) {
            if (signal->wireList()->count() == 2
                    && signal->wireList()->at(0)->layer() == 1
                    && signal->wireList()->at(1)->layer() == 16) {
                // two distinct layer segments at opposite layers

                // create a new layer suffixed with "_bot"
                Signal *bottomSignal = new Signal();
                bottomSignal->setName(signal->name() + "_bot");
                if (signal->classSet())
                    bottomSignal->setClass(signal->class_());

                // remove the bottom wire from the original layer
                Wire *bottomWire = signal->wireList()->at(1);
                signal->wireList()->removeAll(bottomWire);

                // add it to the new signal and append the signal
                bottomSignal->addWire(bottomWire);
                m_design->drawing()->board()->signals_()->addSignal(bottomSignal);
            }
        }
    }

    // connect together the wire segments with overlapping ends on top/bottom layer
    for (int i = 0; i<2;i++) {
        int layerIndex = layers[i];
        for (Signal *signal : *m_design->drawing()->board()->signals_()->signalList()) {
            if (signal->name().contains("$")) {
                for (int i = 0; i<signal->wireList()->count(); i++) {
                    Wire *wire = signal->wireList()->at(i);
                    if (wire->layer() == layerIndex) {
                        signal->setName(QString("SGN%1").arg(signalIndex));
                        findOverlappingWiresRecursive(signal,
                                                      wire,
                                                      *m_design->drawing()->board()->signals_()->signalList(),
                                                      layerIndex);
                    }
                }
            }
            signalIndex++;
        }
    }
*/
    for (auto layerIndex : layers) {

        /*        for (Circle *circle : *m_design->drawing()->board()->plain()->circleList()) {
            if (circle->layer() == layerIndex) {
                bool viaPut = false;
                for (Circle *otherCircle : *m_design->drawing()->board()->plain()->circleList()) {
                    if (otherCircle->layer() == layers[i == 0 ? 1 : 0]
                            && qFuzzyCompare(otherCircle->x(), circle->x())
                            && qFuzzyCompare(otherCircle->y(), circle->y())
                            && qFuzzyCompare(otherCircle->radius(), circle->radius())) {
                        // circle found on the opposite side
                        qWarning() << "circle found on the opposite side";
                        // now let's look for a drill
                        for (Circle *drill : *m_design->drawing()->board()->plain()->circleList()) {
                            if (drill->layer() == 44
                                    && qFuzzyCompare(drill->x(), circle->x())
                                    && qFuzzyCompare(drill->y(), circle->y())) {
                                qWarning() << "drill found";
                                // circle found on the drill layer -> it's a via -> let's look for signal name
                                Via *via = new Via();
                                via->setDiameter(circle->radius() * 2);
                                via->setX(circle->x());
                                via->setY(circle->y());
                                via->setDrill(drill->radius() * 2);
                                via->setExtent("1-16"); // TODO maybe parametrize
                                via->setShape(Via::Shape_round);

                                for (Signal *signal : *m_design->drawing()->board()->signals_()->signalList()) {
                                    for (int i = 0; i<signal->wireList()->count(); i++) {
                                        Wire *wire = signal->wireList()->at(i);
                                        if ((
                                                    wire->layer() == layers[0]
                                                 || wire->layer() == layers[1]
                                            )
                                            && (
                                                (qFuzzyCompare(wire->x1(), circle->x())
                                                 && qFuzzyCompare(wire->y1(), circle->y())
                                                 )
                                             || (qFuzzyCompare(wire->x2(), circle->x())
                                                 && qFuzzyCompare(wire->y2(), circle->y()))
                                            )) {
                                            m_design->drawing()->board()->plain()->circleList()->removeAll(circle);
                                            m_design->drawing()->board()->plain()->circleList()->removeAll(otherCircle);
                                            m_design->drawing()->board()->plain()->circleList()->removeAll(drill);
                                            signal->addVia(via);
                                            viaPut = true;
                                            break;
                                        }
                                    }
                                }

                                if (!viaPut) {
                                    m_design->drawing()->board()->plain()->circleList()->removeAll(circle);
                                    m_design->drawing()->board()->plain()->circleList()->removeAll(otherCircle);
                                    m_design->drawing()->board()->plain()->circleList()->removeAll(drill);

                                    Signal *viaSignal = new Signal();
                                    viaSignal->setName("VIA_"+ QUuid::createUuid().toString());
                                    viaSignal->addVia(via);
                                    m_design->drawing()->board()->signals_()->signalList()->append(viaSignal);
                                }

                                if (viaPut)
                                    break;
                            } // drill position matches
                            if (viaPut)
                                break;
                        } // for on all drill circles
                    } // top - bottom circle pair found
                    if (viaPut)
                        break;
                } // all circles on the opposite layer
            } // proper layer
        } // all circles on the top/bottom layer
*/
        /*for (Rectangle *rectangle : *m_design->drawing()->board()->plain()->rectangleList()) {
            if (rectangle->layer() == layerIndex
                    && qPosCompare(rectangle->x2() - rectangle->x1(), rectangle->y2() - rectangle->y1())) {
                bool viaPut = false;
                for (Rectangle *otherRectangle : *m_design->drawing()->board()->plain()->rectangleList()) {
                    if (otherRectangle->layer() == layers[i == 0 ? 1 : 0]
                            && qPosCompare(otherRectangle->x1(), rectangle->x1())
                            && qPosCompare(otherRectangle->y2(), rectangle->y2())
                            ) {
                        // rectangle found on the opposite side
                        qWarning() << "rectangle found on the opposite side";
                        // now let's look for a drill
                        for (Circle *drill : *m_design->drawing()->board()->plain()->circleList()) {
                            if (drill->layer() == 44
                                    && qPosCompare((float)drill->x(), (float)(((rectangle->x2() - rectangle->x1()) / 2.0) + rectangle->x1()))
                                    && qPosCompare((float)drill->y(), (float)(((rectangle->y2() - rectangle->y1()) / 2.0) + rectangle->y1()))) {
                                // circle found on the drill layer -> it's a  -> let's look for signal name
                                qWarning() << "drill found";

                                Via *via = new Via();
                                via->setDiameter(rectangle->x2()  - rectangle->x1());
                                via->setX(drill->x());
                                via->setY(drill->y());
                                via->setDrill(drill->radius() * 2);
                                via->setExtent("1-16"); // TODO maybe parametrize
                                via->setShape(Via::Shape_square);

                                for (Signal *signal : *m_design->drawing()->board()->signals_()->signalList()) {
                                    for (int i = 0; i<signal->wireList()->count(); i++) {
                                        Wire *wire = signal->wireList()->at(i);
                                        if ((
                                                    wire->layer() == layers[0]
                                                 || wire->layer() == layers[1]
                                            )
                                            && (
                                                (wire->x1() == drill->x() && wire->y1() == drill->y())
                                             || (wire->x2() == drill->x() && wire->y2() == drill->y())
                                            )) {
                                            signal->addVia(via);
                                            m_design->drawing()->board()->plain()->rectangleList()->removeAll(rectangle);
                                            m_design->drawing()->board()->plain()->rectangleList()->removeAll(otherRectangle);
                                            m_design->drawing()->board()->plain()->circleList()->removeAll(drill);
                                            viaPut = true;
                                            break;
                                        }
                                    }
                                }

                                if (!viaPut) {
                                    m_design->drawing()->board()->plain()->rectangleList()->removeAll(rectangle);
                                    m_design->drawing()->board()->plain()->rectangleList()->removeAll(otherRectangle);
                                    m_design->drawing()->board()->plain()->circleList()->removeAll(drill);

                                    Signal *viaSignal = new Signal();
                                    viaSignal->setName("VIA_"+ QUuid::createUuid().toString());
                                    viaSignal->addVia(via);
                                    m_design->drawing()->board()->signals_()->signalList()->append(viaSignal);
                                }

                                if (viaPut)
                                    break;
                            } // drill position matches
                            if (viaPut)
                                break;
                        } // for on all drill circles
                    } // top - bottom rectangle pair found
                    if (viaPut)
                        break;
                } // all rectangles on the opposite layer
            } // proper layer
        } // all rectangles on the top/bottom layer*/
    } // top - bottom layer

    /*for (Signal *viaSignal : *m_design->drawing()->board()->signals_()->signalList()) {
        for (int i = 0; i< viaSignal->viaList()->count(); i++) {
            Via *via = viaSignal->viaList()->at(i);
            for (Signal *otherSignal : *m_design->drawing()->board()->signals_()->signalList()) {
                bool moved = false;
                if (otherSignal != viaSignal) {
                    for (Wire *wire : *otherSignal->wireList()) {
                        if (
                                (qFuzzyCompare(via->x(), wire->x1()) && qFuzzyCompare(via->y(), wire->y1())) ||
                                (qFuzzyCompare(via->x(), wire->x2()) && qFuzzyCompare(via->y(), wire->y2()))
                                ) {
                            qWarning() << "VIA match move " << otherSignal->wireList()->count() << viaSignal->wireList()->count();
                            for (Wire *wireToMove : *otherSignal->wireList()) {
                                viaSignal->wireList()->append(wireToMove);
                            }

                            for (Via *viaToMove : *otherSignal->viaList()) {
                                viaSignal->viaList()->append(viaToMove);
                            }
                            otherSignal->wireList()->clear();
                            otherSignal->viaList()->clear();
                            qWarning() << "after move " << otherSignal->wireList()->count() << viaSignal->wireList()->count();
                            moved = true;
                            break;
                        }
                    }
                }
                if (moved)
                    break;
            }
        }
    }*/
    int counter = 0;
    // merge together overlapping wires on top-bottom layer
    // TODO check for parallel lines as well
    /*for (Signal *signal_ : *m_design->drawing()->board()->signals_()->signalList()) {
        qWarning() << counter << "/" << m_design->drawing()->board()->signals_()->signalList()->count();
        counter++;
        for (int i = 0; i< signal_->wireList()->count(); i++) {
            Wire *wire = signal_->wireList()->at(i);
            if (wire->layer() == 1 || wire->layer() == 16) {
                for (Signal *otherSignal : *m_design->drawing()->board()->signals_()->signalList()) {
                    bool moved = false;
                    if (otherSignal != signal_) {
                        for (Wire *otherWire : *otherSignal->wireList()) {
                            if (otherWire->layer() != wire->layer())
                                continue;

                            QLineF l1 = EAGLE_Utils::wire2QLine(*wire);
                            QLineF l2 = EAGLE_Utils::wire2QLine(*otherWire);

                            if (l1.intersect(l2, nullptr) == QLineF::BoundedIntersection) {
                                for (Wire *wireToMove : *otherSignal->wireList()) {
                                    signal_->wireList()->append(wireToMove);
                                }

                                for (Via *viaToMove : *otherSignal->viaList()) {
                                    signal_->viaList()->append(viaToMove);
                                }
                                otherSignal->wireList()->clear();
                                otherSignal->viaList()->clear();
                                moved = true;
                                break;
                            }
                        }
                    }
                    if (moved)
                        break;
                }
            }
        }
    }*/

    // remove orphaned signals
    for (int i = 0; i<m_design->drawing()->board()->signals_()->signalList()->count(); i++) {
        Signal *signal_ = m_design->drawing()->board()->signals_()->signalList()->at(i);
        if (signal_->wireList()->isEmpty()
                && signal_->viaList()->isEmpty()
                && signal_->polygonList()->isEmpty()
                && signal_->contactrefList()->isEmpty()) {
            m_design->drawing()->board()->signals_()->signalList()->removeAt(i);
            i--;
            qWarning() << "empty signal " << signal_->name() << " removed";
        }
    }
    m_design->writeFile(m_brdPath);
    return true;
}

void GerberImportFixer::findOverlappingWiresRecursive(Signal *signal, Wire *wire, Signal::List signal_s, int layerIndex)
{
    for (Signal *otherSignal : signal_s) {
        if (otherSignal->name().contains("$")) {
            for (Wire *otherWire : *otherSignal->wireList()) {
                if (otherWire->layer() == layerIndex) {
                    QLineF line1 = EAGLE_Utils::wire2QLine(*wire);
                    QLineF line2 = EAGLE_Utils::wire2QLine(*otherWire);

                    if (line1.p1() == line2.p1()
                            || line1.p1() == line2.p2()
                            || line1.p2() == line2.p2()
                            || line1.p2() == line2.p1()) {
                        otherSignal->wireList()->removeAll(otherWire);
                        signal->wireList()->append(otherWire);
                        findOverlappingWiresRecursive(signal, otherWire, signal_s, layerIndex);
                    }
                }
            }
        }
    }
}
