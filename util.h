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
#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <qjson/parser.h>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>

#include <QUrl>
#include <ctime>

class Util;
extern Util *util;

/*!
    \brief
    Contact login status enum

    \return void
 */
enum ContactStatus
{
    online , away , callme , busy , silent , hidden , offline
};

class Util : public QObject
{
    Q_OBJECT
public:

    static void init()
    {
        util = new Util ();
    }

    explicit Util(QObject *parent = 0);

    void openUrl (const QString & url , bool local = false)
    {
        QDesktopServices::openUrl ( local ? QUrl::fromLocalFile(url) : QUrl (url) );
    }

    QVariant getGeneralResult (const QByteArray & data , const QString & debugInfo = QString());
    QString timeStr (int secs = QDateTime::currentMSecsSinceEpoch())
    {
        QDateTime dt;
        dt.setMSecsSinceEpoch(secs);
        return dt.toString("hh:mm:ss");
    }

    ContactStatus stringToStatus (const QString & qs)
    {
        if ( statusMap.contains(qs) )
        {
            return statusMap[qs];
        }
        else
        {
            qDebug() << "str2enum: error converting: " << qs;
            return online;
        }
    }

    QString statusToString (ContactStatus cs)
    {
        if ( statusRevMap.contains(cs) )
        {
            return statusRevMap[cs];
        }
        else
        {
            qDebug() << "enum2str: error converting: " << cs;
            return "online";
        }
    }

private:
    QMap<QString,ContactStatus> statusMap;
    QMap<ContactStatus,QString> statusRevMap;

signals:
    
public slots:
    
};

#endif // UTIL_H
