#ifndef EDITABLELABEL_H
#define EDITABLELABEL_H

#include <QWidget>

namespace Ui {
class EditableLabel;
}

class EditableLabel : public QWidget
{
    Q_OBJECT
    
public:
    explicit EditableLabel(QWidget *parent = 0);
    ~EditableLabel();

    void setText (const QString & text);

signals:
    void textChanged (const QString & text);
    
private slots:
    void on_lineEdit_returnPressed();

private:
    Ui::EditableLabel *ui;
};

#endif // EDITABLELABEL_H
