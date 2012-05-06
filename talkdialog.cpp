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
#include "talkdialog.h"
#include "ui_talkdialog.h"
#include "util.h"
#include "remotelogviewer.h"

TalkDialog::TalkDialog(QQ *qq, const QString &uin, QWidget *parent) :
    QWidget(parent),
    forceClose (false),
    _uin (uin),
    ui(new Ui::TalkDialog)
{
    ui->setupUi(this);
    _qq = qq;

    inputNotifyTimer.setInterval(3 * 1000);
    connect (&inputNotifyTimer , SIGNAL(timeout())  , SLOT(resetWindowTitle()));
}

TalkDialog::~TalkDialog()
{
    delete ui;
}

void TalkDialog::insertPicture (const QString &title, const QString & id , const QByteArray &data)
{
    ui->textEdit->append("<font color='blue'>" + title + "</font>");

    QImage image = QImage::fromData(data);

    QTextDocument * textDocument = ui->textEdit->document();
    textDocument->addResource( QTextDocument::ImageResource, id , QVariant ( image ) );

    QTextCursor cursor = ui->textEdit->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth( image.width() );
    imageFormat.setHeight( image.height() );
    imageFormat.setName( id) ;
    cursor.insertImage(imageFormat);
}

void TalkDialog::keyPressEvent(QKeyEvent *e)
{
    switch (e->modifiers())
    {
    case Qt::CTRL:
        switch (e->key())
        {
        case Qt::Key_Return:
            on_sendButton_clicked();
            break;
        }

        break;
    default:
        break;
    }
}

void TalkDialog::appendMessage(const QString &title, const QString &body)
{
    ui->textEdit->append("<font color='blue'>" + title + "</font>");
    ui->textEdit->append(body);
}

void TalkDialog::setFaceImage(const QPixmap &px)
{
    ui->headLabel->setPixmap(px);
}

void TalkDialog::setNick(const QString &nick)
{
    ui->nick->setText(nick);
    setWindowTitle(nick);
    _title = nick;
}

void TalkDialog::setStatusLine(const QString &line)
{
    ui->statusLine->setText(line);
}

void TalkDialog::closeEvent(QCloseEvent *e)
{
    if ( ! forceClose )
    {
        emit aboutToClose();
        e->ignore();
    }
}

void TalkDialog::on_closeButton_clicked()
{
    emit aboutToClose();
}

void TalkDialog::on_sendButton_clicked()
{
    const QString & body = ui->myTextBox->toPlainText();
    appendMessage( _qq->personalInfo("nick").toString() + " " + util->timeStr() , body );

    _qq->sendMessage(_uin , body);
    ui->myTextBox->clear();
}

void TalkDialog::on_toolButton_3_clicked()
{
    _qq->shakeHisWindow(_uin);
    appendMessage(_qq->personalInfo("nick").toString() + " " + util->timeStr() ,
                  QString::fromUtf8("您发送了一个窗口震动"));
}

void TalkDialog::on_historyButton_clicked()
{
    RemoteLogViewer *logViewer = new RemoteLogViewer (_qq);
    logViewer->show();
}
