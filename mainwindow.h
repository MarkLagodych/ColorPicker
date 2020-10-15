#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QColorDialog>
#include <htmlcolordialog.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{

    Q_OBJECT


public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:

    Ui::MainWindow *ui;
    QColor color = Qt::black; // Current color
    QPalette pal;

    enum ColorSpaces {
        RGB,
        HSV,
        CMYK,
        HSL
    } currentColorSpace = RGB;

    void updateColorPreview();


private slots:

    void toggleBase(int state);
    void changeColorSpace(int index);
    void changeColor();
    void pickColor();
    void pickNamedColor();

};


#endif // MAINWINDOW_H
