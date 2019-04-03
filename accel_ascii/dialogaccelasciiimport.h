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
    explicit DialogAccelAsciiImport(QWidget *parent = nullptr);
    ~DialogAccelAsciiImport();

private slots:
    void on_pushButtonConvert_clicked();

private:
    Ui::DialogAccelAsciiImport *ui;
    void loadWidgets();
    void saveWidgets() const;
};

#endif // DIALOGACCELASCIIIMPORT_H
