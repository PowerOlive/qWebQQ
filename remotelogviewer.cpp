#include "remotelogviewer.h"
#include "ui_remotelogviewer.h"

RemoteLogViewer::RemoteLogViewer(QQ *qq , QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RemoteLogViewer),
    _page (0)
{
    ui->setupUi(this);
    _qq = qq;

    setAttribute(Qt::WA_DeleteOnClose);
}

RemoteLogViewer::~RemoteLogViewer()
{
    delete ui;
}

void RemoteLogViewer::on_pushButton_clicked()
{
    close ();
}
