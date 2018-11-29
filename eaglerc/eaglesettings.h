#ifndef EAGLERCLOADER_H
#define EAGLERCLOADER_H

#include "palette.h"
#include "qt_eagle_xml_parser/eagle.h"

#include <QObject>
#include <QSettings>

class EAGLESettings : public QObject
{
    Q_OBJECT
public:
    explicit EAGLESettings(QObject *parent = 0);
    bool load(const QString & path);

    QString error() const;

    QColor schLayerColor(int layerIndex) const;
    qreal schLayerOpacity(int layerIndex);

    QColor brdLayerColor(int layerIndex) const;
    qreal brdLayerOpacity(int layerIndex);

private:
    Palette *m_schPalette = nullptr, *m_brdPalette = nullptr;
    QString m_error;
};

#endif // EAGLERCLOADER_H
