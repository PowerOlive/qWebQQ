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
#ifndef RELOGIN_H
#define RELOGIN_H

#include <QDialog>

namespace Ui {
class ReLogin;
}

class ReLogin : public QDialog
{
    Q_OBJECT

public:
    enum Decision
    {
        Login,
        Quit
    };

    explicit ReLogin(const QString &reason, QWidget *parent);
    ~ReLogin();

    Decision _decision;
    
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::ReLogin *ui;
};

#endif // RELOGIN_H
