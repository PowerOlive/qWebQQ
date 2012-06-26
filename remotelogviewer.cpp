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
#include "remotelogviewer.h"
#include "ui_remotelogviewer.h"

RemoteLogViewer::RemoteLogViewer(QQ *qq, const QString &title, const QString &uin, QWidget *parent):
    QWidget(parent),
    ui(new Ui::RemoteLogViewer)
{
    ui->setupUi(this);
    _qq = qq;
    _uin = uin;
    _title = title;

    justLoaded = true;

    _qq->fetchLog(_uin);

    connect (_qq , SIGNAL(logReady(QByteArray)) , this , SLOT(processLog(QByteArray)));

    setAttribute(Qt::WA_DeleteOnClose);
}

RemoteLogViewer::~RemoteLogViewer()
{
    disconnect (_qq , SIGNAL(logReady(QByteArray)) , this , SLOT(processLog(QByteArray)));

    delete ui;
}

void RemoteLogViewer::on_pushButton_clicked()
{
    close ();
}

void RemoteLogViewer::processLog(const QByteArray &log)
{
    QJson::Parser parser;
    bool ok = false;

    ui->textEdit->clear();

    QVariantMap mapA = parser.parse(log , &ok).toMap();
    if ( ! ok )
    {
        qDebug() << "Syntax error of log";
        return;
    }

    if ( mapA.value("ret").toInt() != 0 || mapA.value("tuin").toString() != _uin )
    {
        qDebug() << "Retcode: " << mapA.value("ret").toInt();
        return;
    }

    ui->logPage->setMaximum(mapA.value("total").toInt());
    if ( justLoaded )
    {
        justLoaded = false;
        ui->logPage->setValue(ui->logPage->maximum());
    }

    foreach (const QVariant & v , mapA.value("chatlogs").toList())
    {
        const QVariantMap & vMap = v.toMap();
        // 16: what you say ; 17: what he says
        const int & cmd = vMap.value("cmd").toInt();
        const QString & time = util->timeStr(vMap.value("time").toInt());

        QString msg;
        foreach (const QVariant & m , vMap.value("msg").toList())
        {
            if ( ! msg.isEmpty() )
                msg.append("\n");
            msg.append(m.toString());
        }

        msg = msg.trimmed().replace("&","&amp;").replace(">","&gt;").replace("<","&lt;");

        if ( msg.isEmpty() )
            continue;

        if ( cmd == 16 )
        {
            ui->textEdit->append(QString::fromUtf8("<font color='green'>我 %2</font>").arg(time));
        }
        else
        {
            ui->textEdit->append(QString("<font color='blue'>%1 %2</font>").arg(_title).arg(time));
        }

        ui->textEdit->append(msg);
    }
}

void RemoteLogViewer::on_logPage_valueChanged(int arg1)
{
    if ( arg1 == ui->logPage->minimum() )
        ui->prevPage->setEnabled(false);
    else
        ui->prevPage->setEnabled(true);

    if ( arg1 == ui->logPage->maximum() )
        ui->nextPage->setEnabled(false);
    else
        ui->nextPage->setEnabled(true);

    _qq->fetchLog(_uin , arg1);
}

void RemoteLogViewer::on_prevPage_clicked()
{
    ui->logPage->setValue(ui->logPage->value() - 1);
}

void RemoteLogViewer::on_nextPage_clicked()
{
    ui->logPage->setValue(ui->logPage->value() + 1);
}

void RemoteLogViewer::on_clearLogs_clicked()
{
    if ( question("删除聊天记录吗?"))
    {
        _qq->clearLogs(_uin);
        ui->textEdit->setText(QString::fromUtf8("暂无记录"));
    }
}
