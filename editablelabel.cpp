#include "editablelabel.h"
#include "ui_editablelabel.h"

EditableLabel::EditableLabel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditableLabel)
{
    ui->setupUi(this);
}

EditableLabel::~EditableLabel()
{
    delete ui;
}

void EditableLabel::on_lineEdit_returnPressed()
{
    ui->stackedWidget->setCurrentIndex(0);
    const QString & text = ui->lineEdit->text();
    if ( ui->label->text() != text )
    {
        setText(text);
        emit textChanged(text);
    }
}

void EditableLabel::setText(const QString &text)
{
    ui->lineEdit->setText(text);
    ui->label->setText(text);
}
