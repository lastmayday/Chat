#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "SslServer.h"
#include "chatrecordform.h"

#include <QCoreApplication>
#include <QHostAddress>
#include <QSslSocket>
#include <QThread>
#include <cassert>
#include <QColor>
#include <QShortcut>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

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
    void acceptConnection();
    void handshakeComplete();
    void connectionClosed();
    void receiveMessage();
    void connectionFailure();
    void recordChat(QString data);
    void on_sendButton_clicked();
    void on_chatRecordButton_clicked();

private:
    Ui::MainWindow *ui;
    SslServer sslServer;
    QList<QSslSocket *> sockets;
    chatRecordForm *chat;
};

#endif // MAINWINDOW_H
