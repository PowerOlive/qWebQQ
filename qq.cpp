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
#include "qq.h"

/*!
    \brief non-variable urls
*/
//#define QQ_LOGIN_URL "http://ptlogin2.qq.com/login?remember_uin=1&aid=1003903&h=1&ptlang=2052&from_ui=1&dummy&fp=loginerroralert&pttype=1"
#define QQ_LOGIN_URL "http://ptlogin2.qq.com/login?webqq_type=10&remember_uin=1&login2qq=1&aid=1003903&u1=http%3A%2F%2Fweb.qq.com%2Floginproxy.html%3Flogin2qq%3D1%26webqq_type%3D10&h=1&ptredirect=0&ptlang=2052&from_ui=1&pttype=1&dumy=&fp=loginerroralert&action=2-7-10801&mibao_css=m_webqq&t=1&g=1"
#define QQ_VERIFY_CODE "http://captcha.qq.com/getimage?aid=1003903&uin="
#define QQ_LOGIN_STATUS "http://d.web2.qq.com/channel/login2"
#define QQ_POLL2_URL "http://d.web2.qq.com/channel/poll2"
#define QQ_PERSONAL_INFO "http://s.web2.qq.com/api/get_self_info2"
#define GET_GROUP_INFO "http://s.web2.qq.com/api/get_group_name_list_mask2"
#define GET_USER_FRIENDS "http://s.web2.qq.com/api/get_user_friends2"
#define RECENT_CONTACTS "http://d.web2.qq.com/channel/get_recent_list2"
#define SEND_BUDDY_MSG "http://d.web2.qq.com/channel/send_buddy_msg2"
#define SET_LONG_NICK "http://s.web2.qq.com/api/set_long_nick2"

#define INIT_LOGIN_URL "http://ui.ptlogin2.qq.com/cgi-bin/login?target=self&style=5&mibao_css=m_webqq&appid=1003903&enable_qlogin=0&no_verifyimg=1&s_url=http%3A%2F%2Fweb.qq.com%2Floginproxy.html&f_url=loginerroralert&strong_login=1&login_state=10&t=20120619001"

// tuin , vfwebqq , page=0
#define LOG_URL "http://web.qq.com/cgi-bin/webqq_chat/?cmd=1&tuin=%1&vfwebqq=%2&page=%3&row=10&callback=cLog"

// tuin , vfwebqq
#define CLEARLOG_URL "http://web.qq.com/cgi-bin/webqq_chat/?cmd=2&tuin=%1&vfwebqq=%2&callback=showDeleteResult"

#define QQ_LOGOUT "http://d.web2.qq.com/channel/logout2?ids=&clientid=%1&psessionid=%2"

// send file: upload to server , get a file path in return , and post to user
// "r=%7B%22to%22%3A%22" + uin + "%22%2C%22file_path%22%3A%22%2F" + file_path + "%22%2C%22filename%22%3A%22" + filename + "%22%2C%22to_uin%22%3A%22" + to_uin + "%22%2C%22clientid%22%3A%22" + _clientid + "%22%2C%22psessionid%22%3A%22" + sessionMap["psessionid"].toByteArray() + "%22%7D"
// + "&clientid=" + _clientid
// + "&psessionid=" + sessionMap["psessionid"].toByteArray()
#define UPLOAD_FILE ""
#define POST_SEND_FILE "http://d.web2.qq.com/channel/send_offfile2"

#define GROUP_LIST_URL "http://s.web2.qq.com/api/get_group_name_list_mask2"

// vfwebqq + [tuin]
#define FETCH_LONG_NICK "http://s.web2.qq.com/api/get_long_nick?tuin=[%1]&vfwebqq=%2"

// newstatus (en) , clientid , psessionid
#define CHANGE_STATUS "http://d.web2.qq.com/channel/change_status2?newstatus=%1&clientid=%2&psessionid=%3"

// file_path , f_uin , clientid , psessionid
#define GET_OFFLINE_PIC "http://d.web2.qq.com/channel/get_offpic2?file_path=%1&f_uin=%2&clientid=%3&psessionid=%4"

// qq num
#define CLEAR_LOG "http://tj.qstatic.com/getlog?app2=%1$chatlog$c2c$clear"

// uin , client id , psessionid
#define SHAKE_WINDOW "http://d.web2.qq.com/channel/shake2?to_uin=%1&clientid=%2&psessionid=%3"

#define GET_ONLINE_BUDDIES "http://d.web2.qq.com/channel/get_online_buddies2?psessionid=%1&clientid=%2"

#define USER_AGENT "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/534.30 (KHTML, like Gecko) Chrome/12.0.742.9 Safari/534.30"

#define GET_REQUEST(a) do { \
    QNetworkRequest req (a);  \
    req.setRawHeader("Accept-Encoding" , "gzip"); \
    req.setRawHeader("User-Agent" , USER_AGENT); \
    req.setRawHeader("Referer" , "http://ui.ptlogin2.qq.com/cgi-bin/login?target=self&style=5&mibao_css=m_webqq&appid=1003903&enable_qlogin=0&no_verifyimg=1&s_url=http%3A%2F%2Fweb.qq.com%2Floginproxy.html&f_url=loginerroralert&strong_login=1&login_state=10&t=20120504001"); \
    nam->get(req); \
    } while (0);

#define GET_REQUEST2(a) do { \
    QNetworkRequest req (a);  \
    req.setRawHeader("User-Agent" , USER_AGENT); \
    req.setRawHeader("Accept-Encoding" , "identity"); \
    req.setRawHeader("Connection" , "close"); \
    req.setRawHeader("Referer" , "http://s.web2.qq.com/proxy.html?v=20110412001&callback=1&id=2"); \
    nam->get(req); \
    } while (0);

#define GET_REQUEST3(a) do { \
    QNetworkRequest req (a);  \
    req.setRawHeader("User-Agent" , USER_AGENT); \
    req.setRawHeader("Accept-Encoding" , "identity"); \
    req.setRawHeader("Connection" , "close"); \
    req.setRawHeader("Referer" , "http://d.web2.qq.com/proxy.html?v=20110331002&callback=2"); \
    nam->get(req); \
    } while (0);

#define POST_REQUEST(a,b) do {\
    QNetworkRequest req (a); \
    req.setRawHeader("User-Agent" , USER_AGENT); \
    req.setRawHeader("Accept-Encoding" , "identity"); \
    req.setRawHeader("Content-Type" , "application/x-www-form-urlencoded"); \
    req.setRawHeader("Referer" , "http://s.web2.qq.com/proxy.html?v=20110412001&callback=1&id=2"); \
    nam->post(req,b); \
    } while (0);

#define GET_FACE_RAND(u) do { \
    srand (time(NULL)); \
    int r = 1 + rand() % 9; \
    GET_REQUEST2( QString("http://face%1.qun.qq.com/cgi/svr/face/getface?cache=0&type=1&fid=0&uin=%2&vfwebqq=%3").arg(r).arg(u).arg(sessionMap["vfwebqq"].toString()) ); \
    } while (0);

QQ::QQ(QObject *parent) :
    QObject(parent),
    _clientid ( QString::number( rand() % 999999 + 1234 ).toAscii() ),
    //    _clientid ( "21628014" ),
    nam (new QNetworkAccessManager (this)),
    _messageID ( 38001234 ),
    _status (online)
{
    QNetworkDiskCache *cache = new QNetworkDiskCache (this);
    cache->setCacheDirectory("/dev/shm/qq.cache/");
    nam->setCache(cache);

    const char *http_proxy = getenv("http_proxy");
    if ( http_proxy != NULL )
    {
        /// http://ip:port
        QStringList info = QString (http_proxy).remove("http://").split(":");
        if ( info.length() == 2 )
        {
            QNetworkProxy proxy;
            proxy.setType(QNetworkProxy::HttpProxy);
            proxy.setHostName(info.at(0));
            proxy.setPort(info.at(1).toInt());
            nam->setProxy(proxy);

            qDebug() << "Using proxy: " << proxy.hostName() << " : " << proxy.port();

            nam->get(QNetworkRequest (QString("http://checkip.dyndns.com/")));
        }
    }

    nam->setCookieJar(new QNetworkCookieJar(this));
    connect (nam , SIGNAL(finished(QNetworkReply*)) , SLOT(finished(QNetworkReply*)));
}

void QQ::setLongNick(const QString &lnick)
{
    QByteArray postData ("r=%7B%22nlk%22%3A%22" + lnick.toUtf8() + "%22%2C%22vfwebqq%22%3A%22" +
                         sessionMap["vfwebqq"].toByteArray() + "%22%7D");

    POST_REQUEST ( QString(SET_LONG_NICK) , postData);
}

void QQ::clearLogs(const QString &tuin)
{
    GET_REQUEST3(QString(CLEARLOG_URL).arg(tuin).arg(sessionMap["vfwebqq"].toString()));
}

void QQ::fetchLog(const QString &tuin, int page)
{
    GET_REQUEST3(QString(LOG_URL).arg(tuin).arg(sessionMap["vfwebqq"].toString()).arg(page));
}

QVariant QQ::personalInfo(const QString &field)
{
    return personalInfoMap[field];
}

void QQ::fetchSingleFaceImg(const QString &uin)
{
    GET_FACE_RAND ( uin )
}

void QQ::fetchLongNick(const QString &uin)
{
    GET_REQUEST3 (QString (FETCH_LONG_NICK).arg(uin).arg(sessionMap["vfwebqq"].toString()));
}

void QQ::logout()
{
    if ( _status == offline )
    {
        GET_REQUEST3 ( QString (QQ_LOGOUT)
                       .arg(QString(_clientid))
                       .arg(sessionMap["psessionid"].toString()));
    }
}

void QQ::sendMessage (const QString &uin, const QString & body)
{

    QByteArray postData = "r=%7B%22to%22%3A" + uin.toAscii() + "%2C%22face%22%3A594%2C%22content%22%3A%22%5B%5C%22"
            + body.toUtf8() + "%5C%5Cr%5C%5Cn%5C%22%2C%5B%5C%22font%5C%22%2C%7B%5C%22name%5C%22%3A%5C%22%5C%5C%5C%22%E5%AE%8B%E4%BD%93%5C%5C%5C%22%5C%22%2C%5C%22size%5C%22%3A%5C%2210%5C%22%2C%5C%22style%5C%22%3A%5B0%2C0%2C0%5D%2C%5C%22color%5C%22%3A%5C%22000000%5C%22%7D%5D%5D%22%2C%22msg_id%22%3A"
            + QString::number(_messageID).toAscii()
            + "%2C%22clientid%22%3A%22" + _clientid
            + "%22%2C%22psessionid%22%3A%22" + sessionMap["psessionid"].toByteArray() + "%22%7D"
            + "&clientid=" + _clientid
            + "&psessionid=" + sessionMap["psessionid"].toByteArray();

    POST_REQUEST (QString (SEND_BUDDY_MSG) , postData);

    ++ _messageID;
}

void QQ::poll2()
{
    if ( _status == offline )
    {
        qDebug() << "You're offline , poll pending.";
        return;
    }

    QByteArray data = "{\"clientid\":" + _clientid + ",\"psessionid\":\"" + sessionMap["psessionid"].toByteArray() + "\",\"key\":0,\"ids\":[]}";
    data = "t=" + util->currentTimeInSeconds().toAscii() + "&r=" + data.toPercentEncoding() + "&clientid=" + _clientid + "&psessionid=" + sessionMap["psessionid"].toByteArray();

    QNetworkRequest req (QString(QQ_POLL2_URL) );
    req.setRawHeader("Accept-Encoding" , "gzip,deflate,sdch");
    req.setRawHeader("Content-Type" , "application/x-www-form-urlencoded");
    req.setRawHeader("Referer" , "http://d.web2.qq.com/proxy.html?v=20110331002&callback=2");

    //    nam->post(req , data);
    QNetworkReply *reply = nam->post(req,data);
    connect (reply , SIGNAL(readyRead()) , SLOT(readyReadPoll()));
}

void QQ::shakeHisWindow(const QString &uin)
{
    GET_REQUEST3 ( QString (SHAKE_WINDOW).arg(uin).arg( QString(_clientid)).arg(sessionMap["psessionid"].toString()) );
}

void QQ::readyReadPoll()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    const QByteArray & data = reply->readAll();

    QVariantList messageList = util->getGeneralResult(data , "poll2").toList();
    foreach (QVariant m , messageList)
    {
        QVariantMap mMap = m.toMap();

        const QString & pollType =  mMap["poll_type"].toString();
        /*! \brief single message */
        if ( pollType == "message" )
        {
            QVariantMap valueMap = mMap["value"].toMap();
            QVariantList content = valueMap["content"].toList();

            const QString & from_uin = valueMap["from_uin"].toString();

            QString message;
            foreach (QVariant cont , content)
            {
                QVariantList contList = cont.toList();
                if ( contList.isEmpty() )
                {
                    /* decrypt message */
                    message.append( cont.toString() );
                }
                else if ( contList[0].toString() == "offpic" )
                {
                    // file_path , f_uin , clientid , psessionid
                    GET_REQUEST3 ( QString(GET_OFFLINE_PIC)
                                   .arg( contList[1].toMap()["file_path"].toString() )
                                   .arg( valueMap["from_uin"].toString() )
                                   .arg( QString(_clientid))
                                   .arg( sessionMap["psessionid"].toString()) )
                }
            }

            message = message.trimmed().remove(QRegExp ("\n+$"));
            if ( ! message.isEmpty() )
            {
                //                if (message.startsWith("^EN^:"))
                //                {
                //                    message = QString::fromUtf8( QByteArray::fromBase64(message.remove("^EN^:").toAscii()) );
                //                    qDebug() << "Encrypted Message: " << message;
                //                }
                //                else
                //                {
                //                    qDebug() << "Normal Message: " << message;
                //                }

                emit messageReceived(from_uin, message , valueMap["time"].toInt());
            }

        }
        /*! status change */
        else if ( pollType == "buddies_status_change" )
        {
            QVariantMap infoMap = mMap["value"].toMap();
            Contact *contact = findContact(infoMap["uin"].toString());
            if ( contact != NULL )
            {
                contact->setStatus(infoMap["status"].toString());
            }
        }
        /*! input notify */
        else if (pollType == "input_notify" )
        {
            emit inputNotify(mMap["value"].toMap()["from_uin"].toString());
        }
        /*! \brief kicked from server */
        else if ( pollType == "kick_message" )
        {
            const QVariantMap & kMap = mMap["value"].toMap();
            _status = offline;
            emit gotKicked(kMap["reason"].toString());

            /* kill all rest conn */
            nam->setNetworkAccessible(QNetworkAccessManager::NotAccessible);
        }
        /*! \brief shake message */
        else if ( pollType == "shake_message" )
        {
            QVariantMap valueMap = mMap["value"].toMap();

            emit messageReceived(valueMap["from_uin"].toString() , QString::fromUtf8("对方给您发送了一个震动")
                                 , QDateTime::currentMSecsSinceEpoch());
        }
        /*! \brief tips ignored */
        else if ( pollType == "tips" )
        {

        }
        /*! \brief offline file sent */
        else if ( pollType == "push_offfile")
        {
            QVariantMap fileMap = mMap["value"].toMap();

            /// ignored size
            emit offlineFileReceived( fileMap["from_uin"].toString() ,
                                      fileMap["rkey"].toString() ,
                                      fileMap["name"].toString() ,
                                      fileMap["expire_time"].toInt() ,
                                      fileMap["time"].toInt() );
        }
        else
        {
            qDebug() << "Message: " << mMap["poll_type"];
            qDebug() << mMap["value"].toString();
        }
    }
}

void QQ::login(const QString &qq, const QString &pw, ContactStatus newStatus)
{
    _status = newStatus;

    /// re-enable connections
    nam->setNetworkAccessible(QNetworkAccessManager::Accessible);

    _uid = qq.toUtf8();

    /// 3-times md5 encryption
    _userPassword = pw.toUtf8();
    for ( int i = 0 ; i < 3 ; ++ i )
        _userPassword = QCryptographicHash::hash( _userPassword , QCryptographicHash::Md5 );
    _userPassword = _userPassword.toHex().toUpper();

    GET_REQUEST ( QString (INIT_LOGIN_URL) );
}

void QQ::setVerifyCode(const QString &code)
{
    nam->setNetworkAccessible(QNetworkAccessManager::Accessible);

    QUrl qqLoginUrl (QString(QQ_LOGIN_URL));
    qqLoginUrl.addQueryItem("verifycode" , code);
    qqLoginUrl.addQueryItem("u" , _uid);
    qqLoginUrl.addQueryItem("p" , QCryptographicHash::hash(_userPassword + code.toAscii() , QCryptographicHash::Md5).toHex().toUpper());

    GET_REQUEST(qqLoginUrl)
}

void QQ::changeStatus(const ContactStatus &cs)
{
    GET_REQUEST3 (QString (CHANGE_STATUS)
                  .arg(util->statusToString(cs))
                  .arg(QString(_clientid))
                  .arg(sessionMap["psessionid"].toString()));

    _status = cs;
}

void QQ::fetchOnlineBuddies()
{
    GET_REQUEST3 ( QString(GET_ONLINE_BUDDIES)
                   .arg(sessionMap["psessionid"].toString())
                   .arg(QString(_clientid)));
}

void QQ::finished(QNetworkReply *reply)
{
    int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QString & url = reply->url().toString();

    if ( httpStatusCode < 200 || httpStatusCode >= 400 )
    {
        qDebug() << "Network failure: " << httpStatusCode << " " << reply->url().toString() << " : " << reply->errorString();
        if ( url.startsWith("http://ui.ptlogin2.qq.com/cgi-bin/login" ) )
        {
            error ( QString::fromUtf8("无法登录: 网络链接失败") , Pop );
        }
        else
            qDebug() << "Network failure: " << url << " : " << reply->errorString();

        return;
    }

    const QByteArray & data = reply->readAll();

    qDebug() << "URL: " << url;
    qDebug() << "Data: " << data;
    qDebug() << "Headers: " << reply->request().rawHeader("Cookie");
    qDebug() << "=---------=";

    if ( url.startsWith("http://ui.ptlogin2.qq.com/cgi-bin/login") )
    {
        GET_REQUEST( QString("http://check.ptlogin2.qq.com/check?uin=" + _uid + "&appid=1003903&r=0.5419327881638609") );
    }
    /*! \brief Request of auth code */
    else if ( url.startsWith("http://check.ptlogin2.qq.com/check") )
    {
        QRegExp ptui_checkVC ("ptui_checkVC\\(.*'(![A-Z0-9]{3}).*\\);");
        if ( ptui_checkVC.indexIn(data) != -1 )
        {
            setVerifyCode(ptui_checkVC.cap(1));
        }
        else
        {
            error ("Capcha required: " + data);
            GET_REQUEST ( QString (QQ_VERIFY_CODE) + _uid )
        }
    }
    /*! \brief login complete */
    else if ( url.startsWith("http://ptlogin2.qq.com/login") )
    {
        const QString & decodedMsg = QString::fromUtf8(data);
        if ( decodedMsg.contains( QString::fromUtf8("登录成功" )) )
        {
            QByteArray _cookies = reply->rawHeader("Set-Cookie");
            int idxA = _cookies.indexOf("ptwebqq=");
            if ( idxA != -1 )
            {
                idxA = _cookies.indexOf("=" , idxA);
                int idxB = _cookies.indexOf(";" , idxA);
                _ptwebqq = _cookies.mid(idxA + 1 , idxB - idxA - 1);
            }

            if ( _ptwebqq.isEmpty() )
            {
                error ("ptwebqq was empty ?");
                return;
            }

            /*! \brief post login status */
            POST_REQUEST(QString(QQ_LOGIN_STATUS) ,
                         "psessionid=null&r=%7B%22status%22%3A%22"
                         + QByteArray("offline") + /*util->statusToString(_status).toAscii()*/
                         + "%22%2C%22psessionid%22%3A%22null%22%2C%22passwd_sig%22%3A%22%22%2C%22clientid%22%3A" + _clientid + "%2C%22ptwebqq%22%3A%22"
                         + _ptwebqq
                         + "%22%7D&clientid="
                         + _clientid );

        }
        else if ( decodedMsg.contains( QString::fromUtf8("正确")) )
        {
            emit error ( QString::fromUtf8("密码或验证码不对") );
        }
    }
    /*! \brief offline pic download */
    else if (url.startsWith("http://d.web2.qq.com/channel/get_offpic2"))
    {
        static QRegExp f_uinReg ("f_uin=([0-9]+)");
        if (f_uinReg.indexIn(url) != -1)
        {
            QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            if ( ! redirectionTarget.isNull() )
            {
                imageCache.insert( redirectionTarget.toString() , f_uinReg.cap(1));
                GET_REQUEST (redirectionTarget.toString());
            }
        }
    }
    /*! \brief download complete */
    else if ( imageCache.contains(url) )
    {
        static QRegExp rKeyReg ("rkey=([0-9a-zA-Z]+)");
        if ( rKeyReg.indexIn(url) != -1 )
            emit offlinePicReceived( imageCache[url] , rKeyReg.cap(1) , data);
        else
            qDebug() << "image redirect url: protocol changed: " << url;
    }
    /*! \brief login complete phrase 2 , receive session ID */
    else if ( url.startsWith("http://d.web2.qq.com/channel/login2") )
    {
        sessionMap = util->getGeneralResult(data , "login2").toMap();

        /*! \brief change status */
        changeStatus(_status);

        /*! \brief retrieve personal info */
        GET_REQUEST ( QString(QQ_PERSONAL_INFO) );

        /*! \brief get groups */
        GET_REQUEST3 ( QString(GROUP_LIST_URL) );

        /*! face image */
        GET_FACE_RAND( sessionMap["uin"].toString() );

        /*! \brief retrieve contacts info */
        GET_REQUEST2 ( QString("http://s.web2.qq.com/api/get_friend_info2?tuin=" + _uid + "&verifysession=&code=&vfwebqq=" + sessionMap["vfwebqq"].toByteArray()));

    }
    /*! \brief personal info retrieved */
    else if ( url.startsWith("http://s.web2.qq.com/api/get_self_info2") )
    {
        personalInfoMap = util->getGeneralResult(data , "login2").toMap();

        _status = online;
        emit statusChanged(online);

        poll2 ();
    }
    /*! \brief recent contacts */
    else if (url.startsWith("http://d.web2.qq.com/channel/get_recent_list2"))
    {
        _recentContacts.clear();
        QVariantList recentContactList = util->getGeneralResult(data , "get_recent_list2").toList();

        foreach (const QVariant & rc , recentContactList)
        {
            QVariantMap rMap = rc.toMap();
            const int & cType = rMap["type"].toInt();
            const QString & uin = rMap["uin"].toString();

            _recentContacts.push_back( QPair<int,QString> (cType , uin));
        }

        emit recentContactsReady();
    }
    /*! \brief face image data received */
    else if (url.contains("qun.qq.com/cgi/svr/face"))
    {
        static QRegExp uinReg ("t?uin=([0-9]+)");
        if (uinReg.indexIn(url) != -1)
        {
            const QString & uin = uinReg.cap(1);

            if ( uin != _uid )
            {
                Contact *contact = contactsMapping[uin];
                if ( contact == NULL )
                {
                    qDebug() << "ERROR: contact doesn't exist: " << uin;
                    return;
                }
                contact->avatar = data;
                contact->update();
            }

            emit faceChanged (uin , data);
        }
    }
    /*! \brief poll2 message */
    else if (url.startsWith("http://d.web2.qq.com/channel/poll2"))
    {
        poll2 ();
    }
    /*! \brief personal data */
    else if (url.startsWith("http://s.web2.qq.com/api/get_friend_info2"))
    {
        POST_REQUEST(QString (GET_USER_FRIENDS) , "r=%7B%22h%22%3A+%22hello%22%2C+%22vfwebqq%22%3A+%22" + sessionMap["vfwebqq"].toByteArray() + "%22%7D")
    }
    /*! \brief user friend list */
    else if (url.startsWith("http://s.web2.qq.com/api/get_user_friends2"))
    {
        QVariantMap mapA = util->getGeneralResult(data , "get_user_friends2").toMap();

        _contacts.clear();
        categoryList = mapA.value("categories").toList();

        QList<QVariant> friendIDs = mapA.value("friends").toList();
        foreach (QVariant friendID , friendIDs)
        {
            QVariantMap fMap = friendID.toMap();
            if ( ! fMap.contains("categories") )
                continue;

            const QString & uin = fMap["uin"].toString();

            Contact *contact = new Contact (uin);
            contact->category = fMap["categories"].toInt();

            contactsMapping[uin] = contact;
            _contacts.push_back(contact);

            //            GET_FACE_RAND(uin);
        }

        /// display names and status
        QList<QVariant> friendNames = mapA.value("info").toList();
        foreach (QVariant friendName , friendNames)
        {
            QVariantMap fMap = friendName.toMap();
            const QString & uin = fMap["uin"].toString();

            Contact *contact = contactsMapping[uin];
            if ( contact == NULL )
            {
                //                qDebug() << "ERROR: " << uin << " not found";
            }
            else
            {
                contact->nick =  fMap["nick"].toString().trimmed();
            }
        }

        /// display marked names
        QList<QVariant> markedNames = mapA.value("marknames").toList();
        foreach (QVariant friendName , markedNames)
        {
            QVariantMap fMap = friendName.toMap();
            const QString & uin = fMap["uin"].toString();

            Contact *contact = contactsMapping[uin];
            if ( contact == NULL )
            {
                //                qDebug() << "ERROR: " << uin << " not found";
            }
            else
            {
                contact->markname = fMap.value("markname").toString().trimmed();
            }
        }

        emit contactsReady();

        POST_REQUEST(QString(RECENT_CONTACTS) ,
                     "psessionid=" + sessionMap["psessionid"].toByteArray()
                     + "&clientid=" + _clientid
                     + "&r=%7B%22vfwebqq%22%3A%22" + sessionMap["vfwebqq"].toByteArray()
                     + "%22%2C%22clientid%22%3A%22" + _clientid
                     + "%22%2C%22psessionid%22%3A%22" + sessionMap["psessionid"].toByteArray() + "%22%7D");
    }
    /*! \brief capcha */
    else if (url.startsWith("http://captcha.qq.com/getimage"))
    {
        emit capchaRequired(data);
    }
    /*! \brief online buddies */
    else if (url.startsWith("http://d.web2.qq.com/channel/get_online_buddies2"))
    {
        QVariantList mapA = util->getGeneralResult(data , "online_buddies2").toList();

        foreach (QVariant buddy , mapA)
        {
            QVariantMap bMap = buddy.toMap();
            Contact *contact = contactsMapping[ bMap["uin"].toString() ];
            if ( contact != NULL )
            {
                contact->setStatus(bMap["status"].toString());
                fetchSingleFaceImg(contact->uin);
            }
        }

    }
    /*! \brief message sent status */
    else if (url.startsWith("http://d.web2.qq.com/channel/send_buddy_msg2"))
    {
        if ( ! data.contains("ok") )
        {
            error ("message sent: failed: " + data);
        }
    }
    /*! \brief shake message */
    else if (url.startsWith("http://d.web2.qq.com/channel/shake2"))
    {

    }
    /*! \brief statuc change (my) */
    else if (url.startsWith("http://d.web2.qq.com/channel/change_status2"))
    {
        if ( ! data.contains("ok") )
        {
            error ("status change: failed: " + data);
        }
    }
    /*! \brief logout */
    else if (url.startsWith("http://d.web2.qq.com/channel/logout2"))
    {
        if ( ! data.contains("ok") )
        {
            error ("logout failed: " + data);
        }
    }
    /*! \brief long nick */
    else if (url.startsWith("http://s.web2.qq.com/api/get_long_nick"))
    {
        QVariantList listA = util->getGeneralResult(data , "get_long_nick").toList();
        foreach (QVariant l , listA)
        {
            QVariantMap lMap = l.toMap();
            /// WARNING: choose between to_uin and uin ??
            emit longNickFetched(lMap["uin"].toString() , lMap["lnick"].toString());
        }
    }
    /*! \brief group list */
    else if (url.startsWith(GROUP_LIST_URL) )
    {
        QVariantMap mapA = util->getGeneralResult(data , "get_long_nick").toMap();

        qDebug() << "Not handling group info now";
//        qDebug() << mapA.value("gnamelist") << mapA.value("gmarklist");
    }
    /*! set long nick */
    else if (url.startsWith("http://s.web2.qq.com/api/set_long_nick2"))
    {
        if ( ! data.contains(":0}") )
        {
            error ("set long nick failed: " + data);
        }
    }
    /*! \brief logs */
    else if ( url.startsWith("http://web.qq.com/cgi-bin/webqq_chat/") )
    {
        QString data2 (data);
        data2 = data2.remove(QRegExp ("^cLog\\("));
        data2.chop(2);

        data2.replace ("chatlogs:" , "\"chatlogs\":");
        data2.replace ("cmd:" , "\"cmd\":");
        data2.replace ("msg:" , "\"msg\":");
        data2.replace ("page:" , "\"page\":");
        data2.replace ("ret:" , "\"ret\":");
        data2.replace ("seq:" , "\"seq\":");
        data2.replace ("time:" , "\"time\":");
        data2.replace ("total:" , "\"total\":");
        data2.replace ("tuin:" , "\"tuin\":");
        data2.replace ("type:" , "\"type\":");
        data2.replace ("ver:" , "\"ver\":");

        emit logReady(data2.toAscii());

    }
    /*! \brief clear log*/
    else if ( url.startsWith("http://web.qq.com/cgi-bin/webqq_chat/") )
    {
        if ( data.indexOf("ret:0") == -1 )
        {
            qDebug() << "Error deleting logs: " << data;
        }
    }
    else
    {
        qDebug() << "You forget to handle: " << url;
        qDebug() << "Data were: " << data;
    }
}
