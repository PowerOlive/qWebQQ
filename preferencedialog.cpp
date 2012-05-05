#include "preferencedialog.h"
#include "ui_preferencedialog.h"

PreferenceDialog::PreferenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog)
{
    ui->setupUi(this);
    setLayout (ui->mainLayout);

    QSettings settings;
    settings.beginGroup("Login");
    ui->autoLogin->setChecked(settings.value("AutoLogin" , false).toBool());
    ui->rememberPass->setChecked(settings.value("RememberPass" , false).toBool());
    settings.endGroup();
}

PreferenceDialog::~PreferenceDialog()
{
    QSettings settings;
    settings.beginGroup("Login");
    settings.setValue("AutoLogin" ,  ui->autoLogin->isChecked());
    settings.setValue("RememberPass" ,  ui->rememberPass->isChecked());
    settings.endGroup();

    delete ui;
}
