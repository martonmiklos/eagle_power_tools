#include "kicadimportutils.h"

#include <QDebug>

#include "../qt_eagle_xml_parser/unitutilities.h"

KicadUnitConverter::Mode KicadUnitConverter::mode() const
{
    return m_mode;
}

void KicadUnitConverter::setMode(const Mode &mode)
{
    m_mode = mode;
}

double KicadUnitConverter::convert(const QString &kicadData)
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
    {BAdhes, 36}, // bGlue
    {FAdhes, 35}, // tGlue
    {BPaste, 30}, // bStop
    {FPaste, 29}, // tStop
    {BSilkS, 22}, // bPlace
    {FSilkS, 21}, // tPlace
    {BMask, 29}, // bStop
    {FMask, 30}, // tStop
    {DwgsUser, 51}, // tDocu
    {CmtsUser, 51}, // tDocu
    {EdgeCuts, 20}, // Dimension
};

QMap<QString, int> KicadLayerToEAGLEMapper::kicadStringToEagleLayerMap =
{
    {"F.Cu", 1}, // Top
    {"B.Cu", 16}, // Bottom
    {"B.Adhes", 36}, // bGlue
    {"F.Adhes", 35}, // tGlue
    {"B.Paste", 30}, // bStop
    {"F.Paste", 29}, // tStop
    {"B.SilkS", 22}, // bPlace
    {"F.SilkS", 21}, // tPlace
    {"B.Mask", 29}, // bStop
    {"F.Mask", 30}, // tStop
    {"DwgsUser", 51}, // tDocu
    {"CmtsUser", 51}, // tDocu
    {"F.CrtYd", 39}, // tKeepOut
    {"B.CrtYd", 40}, // bKeepOut
    {"F.Fab", 51}, // tDocu
    {"B.Fab", 52} // bDocu
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
    }

    if (kicadStringToEagleLayerMap.contains(kicadLayer))
        return  kicadStringToEagleLayerMap.value(kicadLayer);
    if (ok)
        *ok = false;
    qWarning() << "Unknown kicad layer" << kicadLayer;
    return -1;
}


const char *emsg(void)
{
    return errno ? strerror(errno) : "unknown reason";
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
    fprintf(stderr, "slurp failed: %s", emsg());
    return NULL;
}

