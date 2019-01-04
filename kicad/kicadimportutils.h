#ifndef KICADIMPORTUTILS_H
#define KICADIMPORTUTILS_H

#include <QString>

// TODO get rid of these functions
const char *emsg(void);
char *slurp(FILE *f);

class KicadUnitConverter
{
public:
    enum Mode {
        DeciMil,
        Mil,
        Mm
    };
    KicadUnitConverter(Mode = DeciMil);

    Mode mode() const;
    void setMode(const Mode &mode);

    double convert(const QString & kicadData);

private:
    Mode m_mode;
};


class KicadLayerToEAGLEMapper
{
public:
    static int mapKicadLayerToEagle(int kicadLayer, bool *ok = nullptr);

    enum KicadLayer {
        BAdhes = 16,
        FAdhes = 17,
        BPaste = 18,
        FPaste = 19,
        BSilk = 20,
        FSlik = 21,
        BMask = 22,
        FMask = 23,
        DwgsUser = 24,
        CmtsUser = 25,
        Eco1User = 26,
        Eco2User = 27,
        EdgeCuts = 28
        // TODO extend it
    };
};

#endif // KICADIMPORTUTILS_H
