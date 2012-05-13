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
#ifndef REMOTELOGVIEWER_H
#define REMOTELOGVIEWER_H

#include <QWidget>
#include "qq.h"
#include "qjson/parser.h"

namespace Ui {
class RemoteLogViewer;
}

class RemoteLogViewer : public QWidget
{
    Q_OBJECT
    
public:
    explicit RemoteLogViewer(QQ *qq , const QString & title , const QString & uin , QWidget *parent = 0);
    ~RemoteLogViewer();

    bool question (const char *msg)
    {
        return question (QString::fromUtf8(msg));
    }

    bool question (const QString & msg)
    {
        return QMessageBox::Yes == QMessageBox::question(this , QString::fromUtf8("问题") ,
                                                         msg , QMessageBox::Yes , QMessageBox::No);
    }
    
private slots:
    void on_pushButton_clicked();

    void processLog (const QByteArray & log);

    void on_logPage_valueChanged(int arg1);

    void on_prevPage_clicked();

    void on_nextPage_clicked();

    void on_clearLogs_clicked();

private:
    Ui::RemoteLogViewer *ui;
    QQ *_qq;
    bool justLoaded;
    QString _uin , _title;
};

#endif // REMOTELOGVIEWER_H
