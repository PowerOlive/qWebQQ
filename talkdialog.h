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
#ifndef TALKDIALOG_H
#define TALKDIALOG_H

#include <QWidget>
#include <QKeyEvent>
#include <QProcess>
#include <QTimer>

#include "qq.h"
#include "encrypter.h"

namespace Ui {
class TalkDialog;
}

class TalkDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit TalkDialog(QQ *qq , const QString & uin , QWidget *parent = 0);
    ~TalkDialog();

    void isInputting ()
    {
        setWindowTitle(_title + QString::fromUtf8(" (正在输入...)"));
        inputNotifyTimer.start();
    }

    void insertPicture (const QString & title , const QString & id , const QByteArray & data);

    void appendMessage (const QString & title , QString body , bool notify = true);
    void appendHTML (const QString & title , const QString & body);

    void setNick (const QString & nick);
    void setStatusLine (const QString & line);
    void setFaceImage (const QPixmap & px);

    bool forceClose;
    QString _uin;
    
private slots:
    void on_closeButton_clicked();

    void on_sendButton_clicked();

    void on_toolButton_3_clicked();

    void on_historyButton_clicked();
    void resetWindowTitle ()
    {
        setWindowTitle(_title);
    }

    void on_enableEncryption_toggled(bool checked);

signals:
    void aboutToClose ();

protected:
    void closeEvent(QCloseEvent *e);
    void keyPressEvent(QKeyEvent *e);

private:
    Ui::TalkDialog *ui;
    QTimer inputNotifyTimer;
    QQ *_qq;
    QString _title;
    bool _enableEncryption;

    /*!
        \brief provide encryption
    */
    Encrypter encrypter;
};

#endif // TALKDIALOG_H
