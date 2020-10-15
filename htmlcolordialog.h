#ifndef HTMLCOLORDIALOG_H
#define HTMLCOLORDIALOG_H

#include <QDialog>
#include <QFile>
#include <QTextStream>

namespace Ui {
class HTMLColorDialog;
}

class HTMLColorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HTMLColorDialog(QWidget *parent = nullptr);
    QColor getColor() const;
    ~HTMLColorDialog();

private:
    Ui::HTMLColorDialog *ui;
};

#endif // HTMLCOLORDIALOG_H
