#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QHostAddress address = QHostAddress::Any;
    QString port_q = ui->serverPortEdit->text();
    quint16 port = port_q.toInt();

    sslServer.setSslLocalCertificate("../ca/sslserver.pem");
    sslServer.setSslPrivateKey("../ca/sslserver.key");
    sslServer.setSslProtocol(QSsl::SslV3);
    if (sslServer.listen(address, port))
        qDebug().nospace() << "Now listening on " << qPrintable(address.toString()) << ":" << port;
    else
        qDebug().nospace() << "ERROR: could not bind to " << qPrintable(address.toString()) << ":" << port;

    connect(&sslServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

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

void MainWindow::acceptConnection()
{
    qDebug() << "New connection";
    QSslSocket *sslSocket = dynamic_cast<QSslSocket*>(sslServer.nextPendingConnection());
    assert(sslSocket);

    connect(sslSocket, SIGNAL(encrypted()), this, SLOT(handshakeComplete()));
    connect(sslSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionFailure()));
}

void MainWindow::handshakeComplete()
{
    QSslSocket *socket = dynamic_cast<QSslSocket *>(sender());
    assert(socket);

    connect(socket, SIGNAL(disconnected()), this, SLOT(connectionClosed()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));

    sockets.push_back(socket);

    ui->msgBrowser->append(QString("[%1] Accepted connection from %2:%3")
                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                            .arg(socket->peerAddress().toString())
                            .arg(socket->peerPort()));
    ui->msgEdit->setEnabled(true);
    ui->sendButton->setEnabled(true);
}

void MainWindow::connectionClosed()
{
    QSslSocket *socket = dynamic_cast<QSslSocket *>(sender());
    assert(socket);

    ui->msgBrowser->append(QString("[%1] Connection from %2:%3 closed: %4")
                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                            .arg(socket->peerAddress().toString())
                            .arg(socket->peerPort())
                            .arg(socket->errorString()));
    sockets.removeOne(socket);
    socket->disconnect();
    socket->deleteLater();
}

void MainWindow::receiveMessage()
{
    QSslSocket *socket = dynamic_cast<QSslSocket *>(sender());
    assert(socket);

    QByteArray message = socket->readAll();
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

    QString info = QString("[%1] %2 : ")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
            .arg(name);

    ui->msgBrowser->setTextColor(QColor("blue"));
    ui->msgBrowser->append(info);
    ui->msgBrowser->setTextColor(QColor("black"));
    ui->msgBrowser->append(msg);

    foreach (QSslSocket *s, sockets)
    {
      s->write(message);
    }

    recordChat(info+"\n"+msg);
}

void MainWindow::connectionFailure()
{
    QSslSocket *socket = dynamic_cast<QSslSocket *>(sender());
    assert(socket);

    ui->msgBrowser->append(QString("[%1] Connection from %2:%3 failed: %4")
                            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                            .arg(socket->peerAddress().toString())
                            .arg(socket->peerPort())
                            .arg(socket->errorString()));
    sockets.removeOne(socket);
    socket->disconnect();
    socket->deleteLater();
}

void MainWindow::on_sendButton_clicked()
{
    QString message = ui->msgEdit->text();
    QString username("Server");
    if (!message.isEmpty())
    {
        QJsonObject send_json;
        send_json.insert("name", username);
        send_json.insert("msg", message.toLocal8Bit().constData());
        QJsonDocument document;
        document.setObject(send_json);
        QByteArray byte_array = document.toJson(QJsonDocument::Compact);

        foreach (QSslSocket *s, sockets)
        {
          s->write(byte_array);
        }
        QString info = QString("[%1] %2 : ")
                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                .arg(username);
        ui->msgBrowser->setTextColor(QColor("green"));
        ui->msgBrowser->append(info);
        ui->msgBrowser->setTextColor(QColor("black"));
        ui->msgBrowser->append(message);

        ui->msgEdit->clear();
        ui->msgEdit->setFocus();

        recordChat(info+"\n"+message);
    }
}

void MainWindow::recordChat(QString data)
{
    QFile file("chatRecord.dat");
    file.open(QIODevice::WriteOnly|QFile::Append);
    QDataStream out(&file);
    out << data;
    file.close();
}

void MainWindow::on_chatRecordButton_clicked()
{
    chat = new chatRecordForm();
    chat->show();
}
