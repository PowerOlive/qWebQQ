#include "relogin.h"
#include "ui_relogin.h"

ReLogin::ReLogin(const QString & reason , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReLogin)
{
    ui->setupUi(this);
    ui->label->setText(reason);
    _decision = Quit;
}

ReLogin::~ReLogin()
{
    delete ui;
}

void ReLogin::on_pushButton_2_clicked()
{
    _decision = Quit;
    close ();
}

void ReLogin::on_pushButton_clicked()
{
    _decision = Login;
    close ();
}
