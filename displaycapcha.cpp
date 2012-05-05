#include "displaycapcha.h"
#include "ui_displaycapcha.h"

DisplayCapcha::DisplayCapcha(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DisplayCapcha)
{
    ui->setupUi(this);
    ui->horizontalLayout->setMargin(3);
    setLayout (ui->horizontalLayout);

    setFixedSize(150 , 100);
}

DisplayCapcha::~DisplayCapcha()
{
    delete ui;
}

void DisplayCapcha::setPixmap(const QByteArray &data)
{
    QPixmap pixmap;
    pixmap.loadFromData(data);
    ui->label->setPixmap( pixmap );
}

QString DisplayCapcha::getCapcha()
{
    return ui->lineEdit->text();
}

void DisplayCapcha::on_lineEdit_returnPressed()
{
    close ();
}
