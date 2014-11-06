#include "chatrecordform.h"
#include "ui_chatrecordform.h"

chatRecordForm::chatRecordForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatRecordForm)
{
    ui->setupUi(this);

    QShortcut *shortcut = new QShortcut(QKeySequence("Return"), this);
    connect(shortcut, SIGNAL(activated()), ui->passwdButton, SLOT(click()));
}

chatRecordForm::~chatRecordForm()
{
    delete ui;
}

void chatRecordForm::on_exitButton_clicked()
{
    this->close();
}

void chatRecordForm::on_passwdButton_clicked()
{
    QString passwd = ui->passwdEdit->text();
    if(passwd != QString("123456")){
        QMessageBox msgBox;
        msgBox.setText("密码错误");
        msgBox.exec();
        return;
    }
    QFile file("chatRecord.dat");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    QString str;
    while(in.atEnd() == 0)
    {
        in >> str;
        ui->msgBrowser->append(str);
    }
}
