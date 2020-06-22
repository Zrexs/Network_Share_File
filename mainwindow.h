#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QFile>
#include <QFileDialog>

#include <QDataStream>
#include <QDir>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_listen_clicked();
    void myNewConnection();
    void on_connect_clicked();
    void myConnected();
    void on_selectFile_clicked();
    void on_sendFile_clicked();
    void fileReadyRead();


protected:
    void changeEvent(QEvent *e);


private:
    Ui::MainWindow *ui;
    QTcpServer* _server = nullptr;
    QTcpSocket* _socket = nullptr;
    static constexpr quint16 _port =2121;
    qint64 nextBlockSize;
    QString _fileAdreessName;
    QString _fileName;
    QFile *m_file;
};

#endif // MAINWINDOW_H
