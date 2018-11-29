#ifndef PALETTE_H
#define PALETTE_H

#include <QColor>
#include <QMultiMap>
#include <QSettings>


/**
 * @brief The Palette class
 * This class represents a palette loaded from the .eaglerc
 *
 * The palette is basically acts as a layerindex - color lookup
 */
class Palette
{
public:
    Palette(int index) : m_paletteIndex(index) {}
    void parse(const QSettings *settings);
    QColor layerColor(int layerIndex) const;
    /**
     * @brief layerOpacity
     * @param layerIndex
     * @return
     * The opacity of the layer identified by the layerIndex in the 0-1 range
     * (0: totally transparent)
     */
    qreal layerOpacity(int layerIndex) const;

    int paletteIndex() const;
    void setPaletteIndex(int paletteIndex);

private:
    int m_paletteIndex;
    QMultiMap<int, QColor> m_colors;
};

#endif // PALETTE_H
