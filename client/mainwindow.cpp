#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (QSslSocket::supportsSsl())
    {
      ui->connectButton->setEnabled(true);
    }
    else
    {
      QString noSslMsg = QString("%1\n%2")
          .arg("*** Your version of Qt does support SSL ***")
          .arg("You must obtain a version of Qt that has SSL"
               "support enabled.  If you believe that your "
               "version of Qt has SSL support enabeld, you may "
               "need to install the OpenSSL run-time libraries.");

      ui->msgBrowser->setText(noSslMsg);
    }

    connect(&sslSocket, SIGNAL(encrypted()), this, SLOT(connectedToServer()));
    connect(&sslSocket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connect(&sslSocket, SIGNAL(disconnected()), this, SLOT(connectionClosed()));

    QShortcut *shortcut = new QShortcut(QKeySequence("Return"), this);
    connect(shortcut, SIGNAL(activated()), ui->sendButton, SLOT(click()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}

void MainWindow::on_connectButton_clicked()
{
    QString hostName = ui->serverIPEdit->text();
    QString port_q = ui->serverPortEdit->text();
    quint16 port = port_q.toInt();

    sslSocket.addCaCertificates("../ca/sslserver.pem");
    sslSocket.connectToHostEncrypted(hostName, port);

    qDebug() << "Connected";
    ui->msgEdit->setEnabled(true);
    ui->msgEdit->setFocus();
    ui->sendButton->setEnabled(true);
    ui->msgBrowser->clear();
}

void MainWindow::receiveMessage()
{
    QByteArray message = sslSocket.readAll();    // Read message
    QString name("");
    QString msg("");

    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(message, &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject obj = parse_doucment.object();
            if(obj.contains("name"))
            {
                QJsonValue name_value = obj.take("name");
                if(name_value.isString())
                {
                    name = name_value.toString();
                }
            }
            if(obj.contains("msg"))
            {
                QJsonValue msg_value = obj.take("msg");
                if(msg_value.isString())
                {
                    msg = msg_value.toString();
                }
            }
        }
    }

    ui->msgBrowser->setTextColor(QColor("blue"));
    ui->msgBrowser->append(QString("[%1] %2 : ")
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                           .arg(name));
    ui->msgBrowser->setTextColor(QColor("black"));
    ui->msgBrowser->append(QString(msg));
}

void MainWindow::connectedToServer()
{
    ui->msgEdit->setEnabled(true);
    ui->msgEdit->setFocus();
    ui->sendButton->setEnabled(true);
    ui->msgBrowser->clear();
}

void MainWindow::on_sendButton_clicked()
{
    QString message = ui->msgEdit->text();
    QString username = ui->userNameEdit->text();
    if (!message.isEmpty())
    {
        QJsonObject send_json;
        send_json.insert("name", username);
        send_json.insert("msg", message.toLocal8Bit().constData());
        QJsonDocument document;
        document.setObject(send_json);
        QByteArray byte_array = document.toJson(QJsonDocument::Compact);

        sslSocket.write(byte_array);
        ui->msgEdit->clear();
        ui->msgEdit->setFocus();
    }
}

void MainWindow::connectionClosed()
{
    qDebug() << "connection close.";
}
