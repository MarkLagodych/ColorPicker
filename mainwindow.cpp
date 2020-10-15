#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pal = palette();
    updateColorPreview();

    ui->colorPreview_frame->setAutoFillBackground(true);

    // Make the window size fixed
    setMinimumSize(size());
    setMaximumSize(size());

    connect(ui->base_checkBox, SIGNAL(stateChanged(int)), this, SLOT(toggleBase(int)));
    connect(ui->colorSpace_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeColorSpace(int)));
    connect(ui->component1_spinBox, SIGNAL(valueChanged(int)), this, SLOT(changeColor()));
    connect(ui->component2_spinBox, SIGNAL(valueChanged(int)), this, SLOT(changeColor()));
    connect(ui->component3_spinBox, SIGNAL(valueChanged(int)), this, SLOT(changeColor()));
    connect(ui->component4_spinBox, SIGNAL(valueChanged(int)), this, SLOT(changeColor()));

    connect(ui->pickNamed_button, SIGNAL(clicked()), this, SLOT(pickNamedColor()));
    connect(ui->pick_button, SIGNAL(clicked()), this, SLOT(pickColor()));

    changeColorSpace(ColorSpaces::RGB);

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateColorPreview()
{
    pal.setColor(QPalette::Background, color);
    ui->colorPreview_frame->setPalette(pal);
}


void MainWindow::toggleBase(int state)
{
    int base = state == Qt::CheckState::Checked ? 16 : 10;

    ui->component1_spinBox->setDisplayIntegerBase(base);
    ui->component2_spinBox->setDisplayIntegerBase(base);
    ui->component3_spinBox->setDisplayIntegerBase(base);
    ui->component4_spinBox->setDisplayIntegerBase(base);
}


void MainWindow::changeColorSpace(int index)
{
    QColor clr = color.toRgb();
    currentColorSpace = (ColorSpaces) index;

    #define IMPLEMENT_COLOR_SPACE(                           \
        space, isCMYK,                                       \
        component1, component2, component3,                  \
        max1, max2, max3,                                    \
        name1, name2, name3                                  \
    )                                                        \
        ui->component1_spinBox->setValue(clr.component1());  \
        ui->component2_spinBox->setValue(clr.component2());  \
        ui->component3_spinBox->setValue(clr.component3());  \
        ui->component4_label->setEnabled(isCMYK);            \
        ui->component4_spinBox->setEnabled(isCMYK);          \
        ui->component1_spinBox->setRange(0, max1);           \
        ui->component2_spinBox->setRange(0, max2);           \
        ui->component3_spinBox->setRange(0, max3);           \
        ui->component1_label->setText(name1);                \
        ui->component2_label->setText(name2);                \
        ui->component3_label->setText(name3);                \

    switch (index) {

    case ColorSpaces::RGB:
        IMPLEMENT_COLOR_SPACE(Rgb, false, red, green, blue, 255, 255, 255, "Red", "Green", "Blue")
        break;

    case ColorSpaces::HSV:
        IMPLEMENT_COLOR_SPACE(Hsv, false, hue, saturation, value, 359, 255, 255, "Hue", "Saturation", "Value")
        break;

    case ColorSpaces::CMYK:
        IMPLEMENT_COLOR_SPACE(Cmyk, true, cyan, magenta, yellow, 255, 255, 255, "Cyan", "Magenta", "Yellow")
        ui->component4_spinBox->setRange(0, 255);
        ui->component4_spinBox->setValue(clr.black());
        break;

    case ColorSpaces::HSL:
        IMPLEMENT_COLOR_SPACE(Hsl, false, hslHue, hslSaturation, lightness, 359, 255, 255, "Hue", "Saturation", "Lightness")
        break;

    default:
        return;

    }

    #undef IMPLEMENT_COLOR_SPACE

}


void MainWindow::changeColor()
{
    int a = ui->component1_spinBox->value();
    int b = ui->component2_spinBox->value();
    int c = ui->component3_spinBox->value();
    int d = ui->component4_spinBox->value();

    switch (currentColorSpace) {
    case RGB:
        color.setRgb(a, b, c);
        break;

    case HSV:
        color.setHsv(a, b, c);
        break;

    case CMYK:
        color.setCmyk(a, b, c, d);
        break;

    case HSL:
        color.setHsl(a, b, c);
        break;
    }

    updateColorPreview();
}



void MainWindow::pickColor()
{
    color = QColorDialog::getColor(color, this, "Pick a color", QColorDialog::DontUseNativeDialog);
    if (!color.isValid()) return;
    changeColorSpace(currentColorSpace);
    updateColorPreview();
}



void MainWindow::pickNamedColor()
{
    HTMLColorDialog HTMLColor(this);
    if (HTMLColor.exec() == QDialog::DialogCode::Accepted) {
        QColor tmpColor = HTMLColor.getColor();
        if (tmpColor.isValid()) {
            color = tmpColor;
            changeColorSpace(currentColorSpace);
            updateColorPreview();
        }
    }
}

