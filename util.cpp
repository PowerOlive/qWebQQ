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
#include "util.h"

Util *util = 0;

Util::Util(QObject *parent) :
    QObject(parent)
{
    statusMap[QString::fromUtf8("在线")] = online;
    statusMap[QString::fromUtf8("忙碌")] = busy;
    statusMap[QString::fromUtf8("离线")] = offline;
    statusMap[QString::fromUtf8("静音")] = silent;
    statusMap[QString::fromUtf8("隐身")] = hidden;
    statusMap[QString::fromUtf8("Q我吧")] = callme;
    statusMap[QString::fromUtf8("离开")] = away;

    statusRevMapCN[online] = QString::fromUtf8("在线");
    statusRevMapCN[busy] = QString::fromUtf8("忙碌");
    statusRevMapCN[offline] = QString::fromUtf8("离线");
    statusRevMapCN[silent] = QString::fromUtf8("静音");
    statusRevMapCN[hidden] = QString::fromUtf8("隐身");
    statusRevMapCN[callme] = QString::fromUtf8("Q我吧");
    statusRevMapCN[away] = QString::fromUtf8("离开");

    statusMap["online"] = online;
    statusMap["busy"] = busy;
    statusMap["offline"] = offline;
    statusMap["silent"] = silent;
    statusMap["hidden"] = hidden;
    statusMap["callme"] = callme;
    statusMap["away"] = away;

    statusRevMap[online] = "online";
    statusRevMap[busy] = "busy";
    statusRevMap[offline] = "offline";
    statusRevMap[silent] = "silent";
    statusRevMap[hidden] = "hidden";
    statusRevMap[callme] = "callme";
    statusRevMap[away] = "away";
}

QVariant Util::getGeneralResult (const QByteArray & data , const QString & debugInfo )
{
    QJson::Parser parser;
    bool ok = false;

    QVariant friendInfo2 = parser.parse(data , &ok);
    if ( !ok)
    {
        qDebug() << "[ " << debugInfo << " ] Json parsing error";
        return QVariantMap();
    }

    QVariantMap mapA = friendInfo2.toMap();
    if (mapA["retcode"].toString() != "0")
    {
        qDebug() << "[ get_friend_info2 ] unable to fetch info";
        return QVariantMap();
    }

    return mapA.value("result");
}
