#include "htmlcolordialog.h"
#include "ui_htmlcolordialog.h"



HTMLColorDialog::HTMLColorDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::HTMLColorDialog)
{
    ui->setupUi(this);

    setMinimumSize(size());
    setMaximumSize(size());

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->tableWidget->setRowCount(147); // There are 148 colors, but "transparent" can not be added to the table
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("HTML code"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Color"));


    // 1. Create a table of colors
    // 2. Add a hue column
    // 3. Sort by values in hue column
    // 4. Hide hue column

    for (int line = 0, colorIndex = 0; colorIndex < QColor::colorNames().length(); colorIndex++) {

        QString colorName = QColor::colorNames()[colorIndex];
        if (colorName == "transparent") continue; // This color picker is not targeted to have transparency

        QColor color(colorName);
        QString colorCode =
            QString("#%1%2%3")
            .arg(color.red(),   2, 16, (QChar)'0')
            .arg(color.green(), 2, 16, (QChar)'0')
            .arg(color.blue(),  2, 16, (QChar)'0');
        int ihue = color.hue();
        if (ihue == -1) ihue = 361; // If color is grey, put it to the bottom of the table
        QString hue = QString("%1").arg(ihue, 3, 10);

        QTableWidgetItem *item = new QTableWidgetItem(colorName);
        item->setFlags(Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable);
        ui->tableWidget->setItem(line, 0, item);

        item = new QTableWidgetItem(colorCode);
        item->setFlags(Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable);
        ui->tableWidget->setItem(line, 1, item);

        item = new QTableWidgetItem();
        item->setBackgroundColor(color);
        item->setFlags(Qt::ItemFlag::ItemIsEnabled);
        ui->tableWidget->setItem(line, 2, item);

        item = new QTableWidgetItem(hue);
        item->setFlags(Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable);
        ui->tableWidget->setItem(line, 3, item);

        line++;

    }

    ui->tableWidget->sortByColumn(3, Qt::SortOrder::AscendingOrder);
    ui->tableWidget->hideColumn(3);
    ui->tableWidget->resizeColumnToContents(0);
    ui->tableWidget->resizeColumnToContents(1);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}



HTMLColorDialog::~HTMLColorDialog()
{
    delete ui;
}


QColor HTMLColorDialog::getColor() const
{
    QColor invalid = QColor().convertTo(QColor::Spec::Invalid);

   if (ui) {

       int row = ui->tableWidget->currentRow();

       if (row == -1)
           return invalid;

       QTableWidgetItem *item = ui->tableWidget->item(row, 2);

       if (item)
           return item->backgroundColor();
       else
           return invalid;

   } else {
       return invalid;
   }
}
