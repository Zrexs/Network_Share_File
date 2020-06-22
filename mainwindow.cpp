#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::myConnected()
{
  ui->fileNameLineEdit->setText("connected");
}


void MainWindow::on_listen_clicked()
{
   setWindowTitle("server");
    _server = new QTcpServer (this);
    _server->listen(QHostAddress::Any, _port);
    connect(_server,SIGNAL(newConnection()), SLOT(myNewConnection()));
}

 void MainWindow::myNewConnection()
 {
    _socket = _server->nextPendingConnection();
    connect(_socket, SIGNAL(readyRead()), SLOT(fileReadyRead()));
    nextBlockSize = 0;
 }

void MainWindow::on_connect_clicked()
{
    setWindowTitle("client");
    _socket = new QTcpSocket(this);
    _socket->connectToHost(ui->line_ip->text(),_port);
    connect(_socket, SIGNAL(connected()),SLOT(myConnected()));
    connect(_socket, SIGNAL(readyRead()), SLOT(fileReadyRead()));
    connect(_socket, SIGNAL(disconnect()),SLOT(deleteLater()));
    nextBlockSize = 0;
}

void MainWindow::on_selectFile_clicked()
{
    _fileAdreessName = QFileDialog::getOpenFileName(this, "Get Any File");
    _fileName = QString(_fileAdreessName.right(_fileAdreessName.size()-_fileAdreessName.lastIndexOf("/")-1));
    ui->fileNameLineEdit->setText(_fileAdreessName);
}

void MainWindow::on_sendFile_clicked()
{
    _fileName = QString(_fileAdreessName.right(_fileAdreessName.size()-_fileAdreessName.lastIndexOf("/")-1));
    QByteArray block;
    QDataStream stream(&block, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_4_7);
    stream  << _fileName;
    QFile file(_fileAdreessName);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray buf = file.readAll();
        stream << quint64(file.size());
        stream << buf;
        _socket->write(block);
        _socket->flush();

    }
    else
    {
       ui->fileNameLineEdit->setText("Error");
    }
}

void MainWindow::fileReadyRead()
{
    QTcpSocket * socket = static_cast<QTcpSocket*>(sender());
    forever
    {
        QDataStream stream(socket);
        stream.setVersion(QDataStream::Qt_4_7);
        if (nextBlockSize == 0)
        {
            if (socket->bytesAvailable() < sizeof(quint64))
                return;
            stream >> _fileName;
            stream >> nextBlockSize;
        }
        if (nextBlockSize > socket->bytesAvailable())
            return;
        QByteArray arrFile;
        stream >> arrFile;
        QString str = QFileDialog::getExistingDirectory(0, "Directory Dialog", "");
        QFile file(str + "/" + _fileName);
        ui->fileNameLineEdit->setText(file.fileName());
        file.open(QIODevice::WriteOnly);
        file.write(arrFile);
        nextBlockSize = 0;
    }
   }

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

