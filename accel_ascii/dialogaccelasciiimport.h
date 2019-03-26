#ifndef DIALOGACCELASCIIIMPORT_H
#define DIALOGACCELASCIIIMPORT_H

#include <QDialog>

namespace Ui {
class DialogAccelAsciiImport;
}

class DialogAccelAsciiImport : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAccelAsciiImport(QWidget *parent = 0);
    ~DialogAccelAsciiImport();

private slots:
    void on_pushButtonConvert_clicked();

private:
    Ui::DialogAccelAsciiImport *ui;
    void loadWidgets();
    void saveWidgets();
};

#endif // DIALOGACCELASCIIIMPORT_H
