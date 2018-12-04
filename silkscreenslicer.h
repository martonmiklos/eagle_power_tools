#ifndef SILKSCREENSLICER_H
#define SILKSCREENSLICER_H

#include "qt_eagle_xml_parser/eagle.h"

class SilkscreenSlicer
{
public:
    SilkscreenSlicer(Eagle *design);

    void checkSilkscreenWiresStopCollisions();
    double stopMaskPercentage() const;
    void setStopMaskPercentage(double stopMaskPercentage);

private:
    Eagle *m_eagle;

    /**
     * @brief m_stopMaskPercentage
     * The stopmask size will be increased with the following percentage.
     * See DRC/Mask/Stop max
     */
    double m_stopMaskPercentage;
};

#endif // SILKSCREENSLICER_H
