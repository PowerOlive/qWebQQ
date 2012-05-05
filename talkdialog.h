#ifndef TALKDIALOG_H
#define TALKDIALOG_H

#include <QWidget>
#include <QKeyEvent>
#include "qq.h"
#include <QTimer>

namespace Ui {
class TalkDialog;
}

class TalkDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit TalkDialog(QQ *qq , const QString & uin , QWidget *parent = 0);
    ~TalkDialog();

    void isInputting ()
    {
        setWindowTitle(_title + QString::fromUtf8(" (正在输入...)"));
        inputNotifyTimer.start();
    }

    void insertPicture (const QString & title , const QString & id , const QByteArray & data);

    void appendMessage (const QString & title , const QString & body);
    void appendHTML (const QString & title , const QString & body);

    void setNick (const QString & nick);
    void setStatusLine (const QString & line);
    void setFaceImage (const QPixmap & px);

    bool forceClose;
    QString _uin;
    
private slots:
    void on_closeButton_clicked();

    void on_sendButton_clicked();

    void on_toolButton_3_clicked();

    void on_historyButton_clicked();
    void resetWindowTitle ()
    {
        setWindowTitle(_title);
    }

signals:
    void aboutToClose ();

protected:
    void closeEvent(QCloseEvent *e);
    void keyPressEvent(QKeyEvent *e);

private:
    Ui::TalkDialog *ui;
    QTimer inputNotifyTimer;
    QQ *_qq;
    QString _title;
};

#endif // TALKDIALOG_H
