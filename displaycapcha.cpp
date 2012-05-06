/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Razor - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2012 Razor team
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
