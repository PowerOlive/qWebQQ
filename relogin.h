#ifndef RELOGIN_H
#define RELOGIN_H

#include <QDialog>

namespace Ui {
class ReLogin;
}

class ReLogin : public QDialog
{
    Q_OBJECT

public:
    enum Decision
    {
        Login,
        Quit
    };

    explicit ReLogin(const QString &reason, QWidget *parent);
    ~ReLogin();

    Decision _decision;
    
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::ReLogin *ui;
};

#endif // RELOGIN_H
