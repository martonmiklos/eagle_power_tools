#include "kicadimportutils.h"

#include <QDebug>

#include "../qt_eagle_xml_parser/unitutilities.h"
#include "../qt_eagle_xml_parser/eaglelayers.h"

KicadUnitConverter::Mode KicadUnitConverter::mode() const
{
    return m_mode;
}

void KicadUnitConverter::setMode(const Mode &mode)
{
    m_mode = mode;
}

double KicadUnitConverter::convertTo_mm(const QString &kicadData)
{
    if (m_mode == DeciMil) {
        return UnitUtilities::milToMm(kicadData.toDouble() / 10.0);
    } else if (m_mode == Mil) {
        return UnitUtilities::milToMm(kicadData.toDouble());
    }
    return kicadData.toDouble();
}

KicadUnitConverter::KicadUnitConverter(KicadUnitConverter::Mode mode) :
    m_mode(mode)
{

}

QMap<KicadLayerToEAGLEMapper::KicadLayer, int> KicadLayerToEAGLEMapper::kicadToEagleLayerMap =
{
    {BAdhes, EagleLayers::bGlue},
    {FAdhes, EagleLayers::tGlue},
    {BPaste, EagleLayers::bStop},
    {FPaste, EagleLayers::tStop},
    {BSilkS, EagleLayers::bPlace},
    {FSilkS, EagleLayers::tPlace},
    {BMask, EagleLayers::bStop},
    {FMask, EagleLayers::tStop},
    {DwgsUser, EagleLayers::tDocu},
    {CmtsUser, EagleLayers::tDocu},
    {EdgeCuts, EagleLayers::Dimension},
};

QMap<QString, int> KicadLayerToEAGLEMapper::kicadStringToEagleLayerMap =
{
    {"F.Cu", EagleLayers::Top},
    {"B.Cu", EagleLayers::Bottom},
    {"B.Adhes", EagleLayers::bGlue},
    {"F.Adhes", EagleLayers::tGlue},
    {"B.Paste", EagleLayers::bStop},
    {"F.Paste", EagleLayers::tStop},
    {"B.SilkS", EagleLayers::bPlace},
    {"F.SilkS", EagleLayers::tPlace},
    {"B.Mask", EagleLayers::bStop},
    {"F.Mask", EagleLayers::tStop},
    {"DwgsUser", EagleLayers::tDocu},
    {"CmtsUser", EagleLayers::tDocu},
    {"F.CrtYd", EagleLayers::tKeepout},
    {"B.CrtYd", EagleLayers::bKeepout},
    {"F.Fab", EagleLayers::tDocu},
    {"B.Fab", EagleLayers::bDocu}
};


int KicadLayerToEAGLEMapper::mapKicadLayerToEagle(int kicadLayer, bool *ok)
{
    if (ok)
        *ok = true;
    if (0 <= kicadLayer && kicadLayer <= 16)
        return kicadLayer + 1;

    if (kicadToEagleLayerMap.contains(static_cast<KicadLayer>(kicadLayer)))
        return  kicadToEagleLayerMap.value(static_cast<KicadLayer>(kicadLayer));
    if (ok)
        *ok = false;
    return -1;
}

int KicadLayerToEAGLEMapper::mapKicadStringLayerToEagle(const QString & kicadLayer, bool *ok)
{
    if (ok)
        *ok = true;
    if (kicadLayer.toLower().startsWith("In")) {
        // kicad internal layers range 1-30
        // while EAGLE offers 14 internal layer: 2-15
        int internalnumber = kicadLayer.mid(2).toInt();
        if (internalnumber < 8 || internalnumber > 23)
            return internalnumber + 1;
        else
            return -1;
    }

    if (kicadStringToEagleLayerMap.contains(kicadLayer))
        return  kicadStringToEagleLayerMap.value(kicadLayer);
    if (ok)
        *ok = false;
    qWarning() << "Unknown kicad layer" << kicadLayer;
    return -1;
}

/**@warning does not work for large file >4GB */
char *slurp(FILE *f)
{
    Q_ASSERT(f);
    long length = 0, r = 0;
    char *b = NULL;
    errno = 0;
    if((r = fseek(f, 0, SEEK_END)) < 0)
        goto fail;
    errno = 0;
    if((r = length = ftell(f)) < 0)
        goto fail;
    errno = 0;
    if((r = fseek(f, 0, SEEK_SET)) < 0)
        goto fail;
    b = static_cast<char*>(malloc(length + 1));
    errno = 0;
    if((unsigned long)length != fread(b, 1, length, f))
        goto fail;
    return b;
fail:
    free(b);
    return NULL;
}

