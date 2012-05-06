#ifndef ENCRYPTER_H
#define ENCRYPTER_H

#include <QObject>
#include <QDebug>

/*!
    \brief Encryption Provider should not generate characters other than ASCII code
*/
class EncryptionProvider
{
public:
    EncryptionProvider () {}
    /*!
        \brief algorithm name
    */
    virtual QString name() { return ""; }

    /*!
        \brief encrypt it to array
    */
    virtual QByteArray toArray (const QByteArray & data) { return data; }
    virtual QByteArray fromArray (const QByteArray & data) { return data; }
};

class Base64Provider: public EncryptionProvider
{
public:
    QString name () { return "Base64"; }
    QByteArray toArray (const QByteArray & data);
    QByteArray fromArray (const QByteArray & data);
};


class Encrypter : public QObject
{
    Q_OBJECT
public:
    explicit Encrypter(QObject *parent = 0);

    QString encrypt (const QString & data);
    QString decrypt (const QString & data);

    void setALgorithmPrefix (const QString & pf)
    {
        _algorithmPrefix = pf;
    }

    void setProvider (EncryptionProvider *ep)
    {
        this->ep = ep;
    }

private:
    EncryptionProvider *ep;

    /*!
        \brief prefix which indicate the whole message is encrypted
    */
    QString _algorithmPrefix;

signals:
    
public slots:
    
};

#endif // ENCRYPTER_H
