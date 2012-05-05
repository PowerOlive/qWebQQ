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
