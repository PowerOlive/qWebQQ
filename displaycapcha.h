#ifndef DISPLAYCAPCHA_H
#define DISPLAYCAPCHA_H

#include <QDialog>

namespace Ui {
class DisplayCapcha;
}

class DisplayCapcha : public QDialog
{
    Q_OBJECT
    
public:
    explicit DisplayCapcha(QWidget *parent = 0);
    ~DisplayCapcha();

    void setPixmap (const QByteArray & data);
    QString getCapcha ();

private:
    Ui::DisplayCapcha *ui;

signals:
    void capchaInputted (const QString & txt);

private slots:
    void on_lineEdit_returnPressed();
};

#endif // DISPLAYCAPCHA_H
