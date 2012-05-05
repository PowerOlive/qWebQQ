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
