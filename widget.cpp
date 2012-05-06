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
#include "widget.h"
#include "ui_widget.h"
#include "relogin.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi (this);
    connect (&_qq , SIGNAL(error(QString,ErrorLevel)) , SLOT(error(QString,ErrorLevel)));
    connect (&_qq , SIGNAL(statusChanged(ContactStatus)) , SLOT(statusChanged(ContactStatus)));
    connect (&_qq , SIGNAL(faceChanged(QString,QByteArray)) , SLOT(faceChanged(QString,QByteArray)));
    connect (&_qq , SIGNAL(contactsReady()) , SLOT(contactsInfoReady()));
    connect (&_qq , SIGNAL(gotKicked(QString)) , SLOT(gotKicked(QString)));
    connect (&_qq , SIGNAL(recentContactsReady()) , SLOT(recentContactsReady()));
    connect (&_qq , SIGNAL(inputNotify(QString)) , SLOT(inputNotify(QString)));
    connect (&_qq , SIGNAL(longNickFetched(QString,QString)) , SLOT(longNickFetched(QString,QString)));
    connect (&_qq , SIGNAL(offlinePicReceived(QString,QString,QByteArray))
             , SLOT(offlineImageReceived(QString,QString,QByteArray)));
    connect (&_qq , SIGNAL(capchaRequired(QByteArray)) , SLOT(capchaReceived(QByteArray)));
    connect (&_qq , SIGNAL(messageReceived(QString,QString,int)) , SLOT(messageReceived(QString,QString,int)));
    connect (&_qq , SIGNAL(offlineFileReceived(QString,QString,QString,int,int))
             , SLOT(offlineFileReceived(QString,QString,QString,int,int)));

    connect (ui->wFriendsTree , SIGNAL(itemClicked(QTreeWidgetItem*,int)) , SLOT(itemClicked(QTreeWidgetItem* , int)));
    connect (ui->wRecentTree , SIGNAL(itemClicked(QTreeWidgetItem*,int)) , SLOT(itemClicked(QTreeWidgetItem* , int)));

    loadSettings();

    if ( ui->wAutoLogin->isChecked() )
        on_wLogin_clicked();

    ui->stackedWidget->setCurrentIndex(0);
    ui->statusEditStack->setCurrentIndex(0);

    ui->tabWidget->setCurrentIndex(0);
}

Widget::~Widget()
{
    saveSettings();

    delete ui;
}

void Widget::longNickFetched(const QString &uin, const QString &lnick)
{
    TalkDialog *talkDialog = talkDialogMapping[uin];
    if ( talkDialog != NULL )
    {
        talkDialog->setStatusLine(lnick);
    }
}

void Widget::offlineFileReceived(const QString &uin, const QString &key, const QString &name,
                                 const int &expire_time, const int &send_time)
{
    TalkDialog *talkDialog = setupTalkDialog (uin);
    if ( talkDialog != NULL )
    {
        Contact *contact = _qq.findContact(uin);
        if ( contact != NULL )
        {
            talkDialog->appendMessage( contact->displayName() + " " + util->timeStr(send_time),
                                       QString::fromUtf8("离线文件: %2 , 过期时间: %3")
                                       .arg(name)
                                       .arg(util->timeStr(expire_time)));

            const QString & url = "http://113.142.8.163/" + name + "?ver=2173&rkey=" + key;
            if ( question ( QString::fromUtf8("在浏览器中打开这个文件吗: %1").arg(name) , url) )
            {
                util->openUrl(url);
            }
        }
        else
        {
            qDebug() << "No such contact";
        }
    }
    else
        qDebug() << "TalkDlg is null";
}

void Widget::inputNotify(const QString &uin)
{
    TalkDialog *talkDialog = talkDialogMapping[uin];
    if ( talkDialog != NULL )
    {
        talkDialog->isInputting();
    }
}

void Widget::offlineImageReceived(const QString &uin, const QString & identifier , const QByteArray &data)
{
    TalkDialog *talkDialog = setupTalkDialog(uin);
    if ( talkDialog != NULL )
    {
        Contact *contact = _qq.findContact(uin);
        if ( contact != NULL )
        {
            talkDialog->insertPicture(contact->displayName() + "  " + util->timeStr() , identifier , data);
        }
    }
}

void Widget::loadSettings()
{
    QSettings settings;
    settings.beginGroup("Login");
    ui->wQQ->setText(settings.value("QQ").toString());
    ui->wPassword->setText(settings.value("Pass").toString());
    ui->wAutoLogin->setChecked(settings.value("AutoLogin" , false).toBool() );
    settings.endGroup();
}

void Widget::saveSettings()
{
    QSettings settings;
    settings.beginGroup("Login");
    if ( true == settings.value("RememberPass" , false).toBool() )
    {
        settings.setValue("QQ" , ui->wQQ->text());
        settings.setValue("Pass" , ui->wPassword->text());
    }
    settings.endGroup();
}

void Widget::gotKicked(const QString &reason)
{
    ReLogin *reLogin = new ReLogin (reason , this);
    reLogin->exec();

    switch (reLogin->_decision)
    {
    case ReLogin::Quit:
        close ();
        break;
    case ReLogin::Login:
        ui->stackedWidget->setCurrentIndex(0);
        on_wLogin_clicked();
        break;
    default:
        qDebug() << "You didn't handle this after-kick decision";
        break;
    }

    delete reLogin;
}

void Widget::closeEvent(QCloseEvent *)
{
    _qq.logout();
}

TalkDialog* Widget::setupTalkDialog(const QString &uin)
{
    TalkDialog *talkDialog = talkDialogMapping[uin];
    if ( talkDialog == NULL )
    {
        Contact *contact = _qq.findContact(uin);
        if (contact)
        {
            talkDialog = new TalkDialog (&_qq , uin);
            connect (talkDialog , SIGNAL(aboutToClose()) , SLOT(talkDialogClosed()));

            talkDialogMapping[uin] = talkDialog;

            talkDialog->setFaceImage( contact->pixmap() );
            talkDialog->setNick(contact->displayName());
        }
    }

    return talkDialog;
}

void Widget::itemClicked(QTreeWidgetItem * item, int col)
{
    const QString & uin = item->data(col , Qt::UserRole).toString();
    if ( ! uin.isEmpty() )
    {
        TalkDialog *talkDialog = setupTalkDialog(uin);
        if ( talkDialog == NULL )
        {
            qDebug() << "itemClicked: ERROR: talk dialog shouldn't be null , uin: " << uin;
        }
        else
        {
            _qq.fetchLongNick(uin);
            _qq.fetchSingleFaceImg(uin);

            talkDialog->show();
        }
    }
}

void Widget::recentContactsReady()
{
    ui->wRecentTree->clear();
    const QList<QPair<int,QString> > & recentContacts = _qq.recentContacts();

    QList<QPair<int,QString> >::const_iterator it = recentContacts.begin();
    while ( it != recentContacts.end() )
    {
        Contact *contact = _qq.findContact( (*it).second );
        if (contact == NULL)
        {
            qDebug() << "ERROR: contact " << (*it).second << " not found , but in recent contact userlist";
        }
        else
        {
            QTreeWidgetItem *item = new QTreeWidgetItem (*(contact->treeItem));
            contact->recentTreeItem = item;
            ui->wRecentTree->addTopLevelItem(item);
        }

        ++ it;
    }
}

void Widget::capchaReceived(const QByteArray &data)
{
    DisplayCapcha *displayCapcha = new DisplayCapcha (this);

    displayCapcha->setPixmap(data);
    displayCapcha->exec();

    _qq.setVerifyCode( displayCapcha->getCapcha() );

    delete (displayCapcha);
}

void Widget::talkDialogClosed()
{
    TalkDialog *dlg = qobject_cast<TalkDialog*>(sender());

    talkDialogMapping.remove(dlg->_uin);

    dlg->forceClose = true;
    dlg->close();
    dlg->deleteLater();
}

void Widget::contactsInfoReady()
{
    ui->wFriendsTree->clear();

    QList<Contact*> contacts = _qq.getContacts();
    QVariantList categoryList = _qq.getCategories();

#define ADD_CONTACT_ITEM(name,index) do { \
    QTreeWidgetItem *topItem = new QTreeWidgetItem (QStringList() << name); \
    cateMapping.insert(index , topItem); \
    ui->wFriendsTree->addTopLevelItem(topItem);  \
} while (0);

    QMap<int,QTreeWidgetItem*> cateMapping;
    foreach (QVariant cate , categoryList)
    {
        QVariantMap mapA = cate.toMap();
        ADD_CONTACT_ITEM(mapA["name"].toString() , mapA["index"].toInt());
    }

    /*! \brief default group ??? oh fuck tencent  bad deisgn */
    if ( ! cateMapping.contains(0) )
    {
        ADD_CONTACT_ITEM("My Friends" , 0);
    }

    foreach (Contact *contact , contacts)
    {
        if ( ! cateMapping.contains( contact->category) )
        {
            qDebug() << "Interesting: this contact has no visible category , using default one: " << contact->displayName();
            continue;
        }

        QTreeWidgetItem *contactItem = new QTreeWidgetItem ( QStringList() << contact->displayName() );
        contactItem->setData(0 , Qt::UserRole , contact->uin);
        contactItem->setIcon(0 , contact->displayIcon());
        cateMapping[contact->category]->addChild(contactItem);
        contact->treeItem = contactItem;

        _qq.fetchSingleFaceImg(contact->uin);
    }

    _qq.fetchOnlineBuddies();

}

void Widget::on_wLogin_clicked()
{
    if ( ui->wQQ->text().isEmpty() || ui->wPassword->text().isEmpty() )
        return;

    QSettings settings;
    settings.beginGroup("Login");
    if ( ui->wAutoLogin->isChecked() )
    {
        settings.setValue("AutoLogin" , true);
    }
    settings.endGroup();

    _qq.login(ui->wQQ->text() , ui->wPassword->text() , util->stringToStatus(ui->wLoginStatus->currentText()));
}

void Widget::msgbox(const QString &msg, const QString &title)
{
    QMessageBox::information(this , title , msg  , QMessageBox::Ok);
}

void Widget::error(const QString &err, ErrorLevel level)
{
    switch (level)
    {
    case Normal:
        qDebug() << "ERROR: " << err << " Level: " << level;
        break;
    case Pop:
        msgbox (err);
        break;
    case Fatal:
        msgbox (err, QString::fromUtf8("严重错误") );
        break;
    default:
        qDebug() << "You forget to handle this type of error: " << level;
        break;
    }
}

void Widget::messageReceived(const QString &uin, const QString &body, int rTime)
{
    Contact *contact = _qq.findContact(uin);
    if ( contact == NULL )
    {
        qDebug() << "[ Unknonw People ] " << uin;
    }
    else
    {
        TalkDialog *talkDialog = setupTalkDialog(uin);
        if ( talkDialog == NULL )
        {
            qDebug() << "messageReceived: ERROR: talk dialog shouldn't be null , uin: " << uin;
            return;
        }

        //        qDebug() << "Recv: " << rTime << "   but now: " << QDateTime::currentMSecsSinceEpoch();

        talkDialog->appendMessage(contact->displayName() + "  " + util->timeStr(rTime) , body );
        talkDialog->show();
    }
}

void Widget::statusChanged(ContactStatus status)
{
    switch (status)
    {
    case offline:
        break;
    default:
        ui->stackedWidget->setCurrentIndex(1);
        ui->wNicknameLine->setText(_qq.personalInfo("nick").toString());
        ui->wStatusLine->setText("<a href='#'>" +
                                 (_qq.personalInfo("lnick").toString().isEmpty() ? QString::fromUtf8("点击编辑状态") : _qq.personalInfo("lnick").toString())
                                 + "</a>");
        ui->wStatusLineEdit->setText(_qq.personalInfo("lnick").toString());
        break;
    }
}

void Widget::faceChanged(const QString &uin, const QByteArray &data)
{
    if ( uin == ui->wQQ->text() )
    {
        QPixmap px;
        if ( ! px.loadFromData(data) )
            qDebug() << "Warning: failed to load image from data";
        ui->wMyAvatar->setPixmap(px);
    }
    else
    {
        TalkDialog *talkDlg = talkDialogMapping[uin];
        if (talkDlg != NULL)
        {
            QPixmap px;
            if ( ! px.loadFromData(data) )
                qDebug() << "Warning: failed to load image from data";
            talkDialogMapping[uin]->setFaceImage(px);
        }
        //        qDebug() << "ERROR: you shouldn't see this: set face image for other people other than youself";
    }
}

void Widget::on_wQQ_returnPressed()
{
    on_wLogin_clicked();
}

void Widget::on_wPassword_returnPressed()
{
    on_wLogin_clicked();
}

void Widget::on_comboBox_currentIndexChanged(const QString &arg1)
{
    _qq.changeStatus(util->stringToStatus(arg1));
}

void Widget::on_toolButton_clicked()
{
    PreferenceDialog *prefDlg = new PreferenceDialog (this);
    prefDlg->exec();

    delete prefDlg;
}

void Widget::on_wStatusLine_linkActivated(const QString &link)
{
    Q_UNUSED (link)
    ui->statusEditStack->setCurrentIndex(1);
}

void Widget::on_wStatusLineEdit_returnPressed()
{
    const QString & prevStatus = ui->wStatusLine->text();
    const QString & newStatus = ui->wStatusLineEdit->text();

    if ( prevStatus != newStatus )
    {
        ui->wStatusLine->setText("<a href='#'>" + newStatus + "</a>");
        _qq.setLongNick(newStatus);
    }

    ui->statusEditStack->setCurrentIndex(0);
}
