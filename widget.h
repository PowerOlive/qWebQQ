/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 
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
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>

#include "qq.h"
#include "talkdialog.h"
#include "displaycapcha.h"
#include "preferencedialog.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void msgbox (const QString & msg , const QString & title = QString::fromUtf8 ("错误") );
    bool question (const char* msg)
    {
        return question (QString::fromUtf8(msg));
    }

    bool question (const QString & msg , const QString & detailedText = QString ())
    {
        QMessageBox msgBox;
//        msgBox.setParent(this);
        msgBox.setWindowTitle(QString::fromUtf8("问题"));
        msgBox.setDetailedText(detailedText);
        msgBox.setText(msg);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setIcon(QMessageBox::Question);
        return msgBox.exec() == QMessageBox::Yes;
    }
    
protected:
    void closeEvent(QCloseEvent *);

private slots:
    /*!
        \brief contact inputting
    */
    void inputNotify (const QString & uin);

    /*!
        \brief login status changed
    */
    void statusChanged (ContactStatus status);

    /*!
        \brief do login stuff
    */
    void on_wLogin_clicked();

    /*!
        \brief image received
    */
    void offlineImageReceived (const QString & uin , const QString &identifier, const QByteArray &data);

    /*!
        \brief handle error message
    */
    void error(const QString & err , ErrorLevel level);

    /*!
        \brief handle offline file downloading
    */
    void offlineFileReceived (const QString & uin , const QString & key ,
                              const QString & name , const int & expire_time ,
                              const int & send_time);

    /*!
        \brief face image changed
    */
    void faceChanged(const QString & uin , const QByteArray & data);

    /*!
        \brief
        display long nick on talkdialogs
    */
    void longNickFetched (const QString & uin , const QString & lnick);

    /*!
        \brief add contacts to widget
    */
    void contactsInfoReady ();
    void recentContactsReady ();

    /*!
        \brief init talk
    */
    void itemClicked(QTreeWidgetItem *item, int col);

    /*!
        \brief handle talking dialog clsoe event
    */
    void talkDialogClosed ();

    /*!
        \brief input capcha
    */
    void capchaReceived (const QByteArray & data);

    /*! \brief handle kick message */
    void gotKicked (const QString & reason);

    /*!
        \brief message received
    */
    void messageReceived (const QString & uin , const QString & body , int rTime);

    void on_wQQ_returnPressed();

    void on_wPassword_returnPressed();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_toolButton_clicked();

    void on_wStatusLine_linkActivated(const QString &link);

    void on_wStatusLineEdit_returnPressed();

private:
    Ui::Widget *ui;
    QQ _qq;

    void loadSettings ();
    void saveSettings ();

    /*!
        \brief initial a new talk dialog
    */
    TalkDialog *setupTalkDialog (const QString & uin);

    /*!
        \brief mapping between uin and talk dialog
    */
    QMap<QString,TalkDialog*> talkDialogMapping;
};

#endif // WIDGET_H
