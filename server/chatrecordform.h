#ifndef CHATRECORDFORM_H
#define CHATRECORDFORM_H

#include <QWidget>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <QMessageBox>
#include <QShortcut>

namespace Ui {
class chatRecordForm;
}

class chatRecordForm : public QWidget
{
    Q_OBJECT

public:
    explicit chatRecordForm(QWidget *parent = 0);
    ~chatRecordForm();

private slots:
    void on_exitButton_clicked();
    void on_passwdButton_clicked();

private:
    Ui::chatRecordForm *ui;
};

#endif // CHATRECORDFORM_H
