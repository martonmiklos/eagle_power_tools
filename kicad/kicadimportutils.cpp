#include "kicadimportutils.h"

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

int KicadLayerToEAGLEMapper::mapKicadLayerToEagle(int kicadLayer, bool *ok)
{
    if (ok)
        *ok = true;
    if (0 <= kicadLayer && kicadLayer <= 16) {
        return kicadLayer + 1;
    }

    switch (static_cast<KicadLayer>(kicadLayer)) {
    // FIXME
    case KicadLayerToEAGLEMapper::BAdhes:
        return 36; // bGlue
    case KicadLayerToEAGLEMapper::FAdhes:
        return 35; // tGlue
    case KicadLayerToEAGLEMapper::BPaste:
        return 30; // bStop
    case KicadLayerToEAGLEMapper::FPaste:
        return 29; // tStop
    case KicadLayerToEAGLEMapper::BSilk:
        return 22; // bPlace
    case KicadLayerToEAGLEMapper::FSlik:
        return 21; // tPlace
    case KicadLayerToEAGLEMapper::BMask:
        return 29; // bStop
    case KicadLayerToEAGLEMapper::FMask:
        return 30; // tStop
    case KicadLayerToEAGLEMapper::DwgsUser:
        return 51; // tDocu
    case KicadLayerToEAGLEMapper::CmtsUser:
        return 51; // tDocu
    case KicadLayerToEAGLEMapper::Eco1User:
        break;
    case KicadLayerToEAGLEMapper::Eco2User:
        break;
    case KicadLayerToEAGLEMapper::EdgeCuts:
        return 20; // Dimension
    }
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

