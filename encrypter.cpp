#include "encrypter.h"

QByteArray Base64Provider::fromArray(const QByteArray &data)
{
    QByteArray data_ = data;
    /* fuck tencent and JSON */
//    qDebug() << "Covert: " << data_ << " to: " << data_.replace(" " , "+");
    return QByteArray::fromBase64(data_.replace(" " , "+"));
}

QByteArray Base64Provider::toArray(const QByteArray &data)
{
    return data.toBase64();
}

Encrypter::Encrypter(QObject *parent) :
    QObject(parent)
{
}

QString Encrypter::encrypt(const QString &data)
{
    QString result (data);
    if ( ep != NULL )
    {
        result = _algorithmPrefix + ep->toArray(data.toUtf8());
    }
    else
    {
        qDebug() << "ERROR:  no encryption provider set , nothing deployed";
    }
//    qDebug() << "Encryption: " << data << "   To: " << result;

    return result;
}

QString Encrypter::decrypt(const QString &data)
{
    QString result (data);
    if ( ep != NULL )
    {
        if ( ! _algorithmPrefix.isEmpty() && result.startsWith(_algorithmPrefix) )
        {
            result = result.remove(0 , _algorithmPrefix.length());
            result = QString::fromUtf8 ( ep->fromArray(result.toAscii()) );
        }
    }
    else
    {
        qDebug() << "ERROR:  no encryption provider set , nothing deployed";
    }

//    qDebug() << "Decryption: " << result << " To:  " << result;

    return result;
}
