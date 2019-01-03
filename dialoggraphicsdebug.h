#ifndef DIALOGGRAPHICSDEBUG_H
#define DIALOGGRAPHICSDEBUG_H

#include <QDialog>

namespace Ui {
class DialogGraphicsDebug;
}

class DialogGraphicsDebug : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGraphicsDebug(QWidget *parent = 0);
    ~DialogGraphicsDebug();

    QPainter *painter() const;

private:
    Ui::DialogGraphicsDebug *ui;
    QPainter *m_painter = nullptr;
};

#endif // DIALOGGRAPHICSDEBUG_H
