#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QSslSocket>
#include <QThread>
#include <QShortcut>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_exitButton_clicked();
    void on_connectButton_clicked();
    void receiveMessage();
    void connectedToServer();
    void on_sendButton_clicked();
    void connectionClosed();

private:
    Ui::MainWindow *ui;
    QSslSocket sslSocket;
};

#endif // MAINWINDOW_H
