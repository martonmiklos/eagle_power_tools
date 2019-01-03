#ifndef GERBERIMPORTFIXER_H
#define GERBERIMPORTFIXER_H


#include "eagle.h"

class GerberImportFixer
{
public:
    GerberImportFixer(const QString & brdFilePath);

    bool fix();
private:
    Eagle *m_design;

    void findOverlappingWiresRecursive(Signal *signal, Wire *wire, Signal::List signal_s, int layerIndex);
    QString m_brdPath;
    static inline bool qPosCompare(float p1, float p2)
    {
        return (qAbs(p1 - p2) * 1000.f <= qMin(qAbs(p1), qAbs(p2)));
    }
};

#endif // GERBERIMPORTFIXER_H
