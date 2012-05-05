#ifndef REMOTELOGVIEWER_H
#define REMOTELOGVIEWER_H

#include <QWidget>
#include "qq.h"

namespace Ui {
class RemoteLogViewer;
}

class RemoteLogViewer : public QWidget
{
    Q_OBJECT
    
public:
    explicit RemoteLogViewer(QQ *qq , QWidget *parent = 0);
    ~RemoteLogViewer();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::RemoteLogViewer *ui;
    QQ *_qq;
    int _page;
};

#endif // REMOTELOGVIEWER_H
