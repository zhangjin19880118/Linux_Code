#ifndef MAINWND_H
#define MAINWND_H

#include <QWidget>
#include <QTreeWidget>
#include <QListWidget>
#include <QTextBrowser>
#include <QTextEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QHostAddress>

class MainWnd : public QWidget
{
    Q_OBJECT
public:
    explicit MainWnd(QWidget *parent = 0);
    void onReadyRead();
    void onSend();
    void readData();
    QListWidget *left;
    QWidget *right;
    QTextBrowser *output;
    QTextEdit *input;
    QPushButton *send;
    QTcpSocket *socket;

    int packetSize;
    QByteArray buf;

signals:

public slots:
};

#endif // MAINWND_H
