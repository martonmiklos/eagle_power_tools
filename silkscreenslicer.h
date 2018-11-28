#ifndef SILKSCREENSLICER_H
#define SILKSCREENSLICER_H

#include "qt_eagle_xml_parser/eagle.h"

class SilkscreenSlicer
{
public:
    SilkscreenSlicer(Eagle *design);

    void checkSilkscreenWiresStopCollisions();
private:
    Eagle *m_eagle;
};

#endif // SILKSCREENSLICER_H
