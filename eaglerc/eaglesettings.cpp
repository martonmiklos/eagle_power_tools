#include "eaglesettings.h"
#include <QDebug>



EAGLESettings::EAGLESettings(QObject *parent) :
    QObject(parent)
{
}

bool EAGLESettings::load(const QString &path)
{
    QSettings eagleRC(path, QSettings::IniFormat);
    int paletteIndex = 0;
    int schPaletteIndex = eagleRC.value("Sch.Palette", "0").toInt();
    int brdPaletteIndex = eagleRC.value("Brd.Palette", "0").toInt();

    while (1) {
        if (eagleRC.value(QString("Palette.%1.0").arg(paletteIndex), "").toString().isEmpty())
            break;

        if (paletteIndex == schPaletteIndex) {
            m_schPalette = new Palette(paletteIndex);
            m_schPalette->parse(&eagleRC);
        }

        if (paletteIndex == brdPaletteIndex) {
            m_brdPalette = new Palette(paletteIndex);
            m_brdPalette->parse(&eagleRC);
        }

        if (m_schPalette && m_brdPalette)
            break;

        paletteIndex++;
    }

    if (paletteIndex == 0) {
        m_error = tr("Unable to load palettes");
        return false;
    }

    if (!m_schPalette) {
        m_error = tr("Sch.Palette index does not points to a valid palette");
        return false;
    }

    if (!m_brdPalette) {
        m_error = tr("Brd.Palette index does not points to a valid palette");
        return false;
    }
    return true;
}

QString EAGLESettings::error() const
{
    return m_error;
}

QColor EAGLESettings::schLayerColor(int layerIndex) const
{
    if (m_schPalette)
        return m_schPalette->layerColor(layerIndex);
    return QColor::Invalid;
}

qreal EAGLESettings::schLayerOpacity(int layerIndex)
{
    if (m_schPalette)
        return m_schPalette->layerOpacity(layerIndex);
    return -1;
}

QColor EAGLESettings::brdLayerColor(int layerIndex) const
{
    if (m_brdPalette)
        return m_brdPalette->layerColor(layerIndex);
    return QColor::Invalid;
}

qreal EAGLESettings::brdLayerOpacity(int layerIndex)
{
    if (m_brdPalette)
        return m_brdPalette->layerOpacity(layerIndex);
    return -1;
}

