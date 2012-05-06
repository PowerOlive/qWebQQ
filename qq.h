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
#ifndef QQ_H
#define QQ_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QCryptographicHash>
#include <QTreeWidgetItem>
#include <QTimer>
#include <QLineEdit>
#include <QNetworkCookieJar>
#include <QMessageBox>
#include <QMenu>
#include <QDateTime>
#include <QUrl>
#include <QDebug>

#include "util.h"

/*!
    \brief
    Error message level

    \return void
 */
enum ErrorLevel
{
    /*! \brief use qDebug() */
    Normal,
    /*! use MessageBox */
    Pop,
    /*! msgbox + quit */
    Fatal
};

struct Contact
{
private:

public:

    Contact (const QString & uin = QString ()):
        treeItem (NULL),
        recentTreeItem(NULL),
        status (offline),
        category (-1)
    {
        this->uin = uin;
    }

    /*!
        \brief set status by string
    */
    void setStatus (const QString & txt)
    {
        status = util->stringToStatus(txt);
        update ();
    }

    void update()
    {
        if ( treeItem != NULL )
            treeItem->setIcon(0 , displayIcon());
        if ( recentTreeItem != NULL )
            recentTreeItem->setIcon(0 , displayIcon());
    }

    QIcon displayIcon ()
    {
        if ( status == offline )
        {
            return QIcon (":/img/offline.jpg");
        }
        else if ( ! avatar.isEmpty() )
        {
            QPixmap px;
            if ( px.loadFromData(avatar) )
                return QIcon (px);
        }
        return QIcon (":/img/big.png");
    }

    /*!
        \brief display name
     */
    QString displayName ()
    {
        if ( markname.isEmpty() )
            return nick;
        else
            return QString ("%1 (%2)").arg(markname).arg(nick);
        //        return markname.isEmpty() ? nick : markname;
    }

    /*!
        \brief get pixmap of contact head image
    */
    QPixmap pixmap()
    {
        QPixmap px;
        if ( ! px.loadFromData(avatar) )
        {
            //            qDebug() << "Contact: face image damaged.";
            return QPixmap (":/img/small.png");
        }
        return px;
    }

    /*!
        \brief
        corresponding treeItem
    */
    QTreeWidgetItem *treeItem , *recentTreeItem;

    /*!
        \brief
        uinque tencent ID (key item)
    */
    QString uin;

    /*!
        \brief client type
    */
    int client_type;

    /*!
        \brief
        qq number (not key item)
    */
    QString qqNum;

    /*!
        \brief
        nickname
    */
    QString nick;

    /*!
        \brief
        marked name
    */
    QString markname;

    /*!
        \brief
        login status
    */
    ContactStatus status;

    /*!
        \brief
        Avatar raw image data
    */
    QByteArray avatar;

    /*!
        \brief
        contact group index
    */
    int category;
};

class QQ : public QObject
{
    Q_OBJECT
public:
    explicit QQ(QObject *parent = 0);

    /*!
        \brief query personal info from map
    */
    QVariant personalInfo (const QString & field);

    /*!
        \brief
        init login

        \return void
     */
    void login (const QString & qq , const QString & pw , ContactStatus newStatus);

    /*!
        \brief
        logout
    */
    void logout ();

    /*!
        \brief login with verify code
    */
    void setVerifyCode (const QString & code);

    /*!
        \brief send message to contacts
    */
    void sendMessage (const QString & uin , const QString &body);

    /*!
       \brief shake window message
    */
    void shakeHisWindow (const QString & uin);

    /*!
        \brief
        get contact list

        \return QList<Contact>
     */
    const QList<Contact*> & getContacts ()
    {
        return _contacts;
    }

    const QVariantList & getCategories ()
    {
        return categoryList;
    }

    Contact *findContact(const QString & uin)
    {
        return contactsMapping[uin];
    }

    const QList<QPair<int,QString> > & recentContacts()
    {
        return _recentContacts;
    }

    void changeStatus (const ContactStatus & cs);

private:
    QByteArray _ptwebqq;

    /*! \brief contact list */
    QList<Contact*> _contacts;

    /*! \brief QQ num / email */
    QByteArray _uid;

    /*! \brief 3 times encrypted user password */
    QByteArray _userPassword;

    /*! \brief client ID , randomly generated */
    QByteArray _clientid;

    /*! \brief session maps */
    QVariantMap sessionMap , personalInfoMap;
    QVariantList categoryList;

    QMap<QString,Contact*> contactsMapping;
    QList<QPair<int,QString> > _recentContacts;

    /*! \brief network access manager */
    QNetworkAccessManager *nam;

    /*! \brief message id */
    int _messageID;

    /*! \brief status maintained internally */
    ContactStatus _status;

    /*! \brief offline image mapping between imgID and uin */
    QMap<QString,QString> imageCache;

signals:
    /*!
       \brief offline file sent

       \param
       @uin:         unique tencent ID
       @key:         file path
       @name:        file name
       @expire_time: file expire time
    */
    void offlineFileReceived (const QString & uin , const QString & key ,
                              const QString & name , const int & expire_time ,
                              const int & send_time);

    /*! \brief kicked signal */
    void gotKicked (const QString & reason);

    /*! \brief when offline picture download complete */
    void offlinePicReceived (const QString & uin , const QString & identifier , const QByteArray & data);

    /*!
        \brief
        emit error status

        \return void
      */
    void error (const QString & msg , const ErrorLevel & lev = Normal);

    /*!
        \brief login status changed , e.g offline -> online
    */
    void statusChanged (const ContactStatus & status);

    /*!
        \brief face image changed
    */
    void faceChanged (const QString & uin , const QByteArray & data);

    /*!
        \brief mesage received
    */
    void messageReceived (const QString & uin , const QString & body , int rTime);
    void htmlMessageReceived (const QString & uin , const QString & body , int rTime);

    /*!
        \brief poll2
     */
    void infoReceived ();

    /*!
        \brief contact info ready
    */
    void contactsReady ();
    void recentContactsReady ();

    /*!
        \brief fetch long status
    */
    void inputNotify (const QString & uin);

    /*!
        \brief fetch long status
    */
    void statusUpdated (const QString & uin , const QString & line);

    /*!
        \brief show capcha image
    */
    void capchaRequired (const QByteArray & data);
    
private slots:
    void poll2 ();

    void readyReadPoll ();
    void finished (QNetworkReply *reply);
};

#endif // QQ_H
