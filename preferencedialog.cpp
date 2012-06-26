/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * 
 * 
 *
 * Copyright: 2012 Labo A.L
 * Authors:
 *   Aaron Lewis <the.warl0ck.1989@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */
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
