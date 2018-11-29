#include "palette.h"


void Palette::parse(const QSettings *settings)
{
    int layerIndex = 0;
    while (1) {
        QString colorString = settings->value(QString("Palette.%1.%2").arg(m_paletteIndex).arg(layerIndex), "").toString();
        if (colorString.length() == 10) {
            QColor color;
            color.setNamedColor("#" + colorString.mid(2, colorString.length() - 2));
            m_colors.insert(layerIndex, color);
        } else {
            break;
        }
        layerIndex++;
    }
}

QColor Palette::layerColor(int layerIndex) const
{
    return m_colors.value(layerIndex);
}

qreal Palette::layerOpacity(int layerIndex) const
{
    return m_colors.value(layerIndex).alpha() / 255.0f;
}

int Palette::paletteIndex() const
{
    return m_paletteIndex;
}

void Palette::setPaletteIndex(int paletteIndex)
{
    m_paletteIndex = paletteIndex;
}
