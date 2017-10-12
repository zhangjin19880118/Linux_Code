#include "MainWnd.h"
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHostAddress>
#include <QDebug>
#include <QtEndian>

MainWnd::MainWnd(QWidget *parent) : QWidget(parent)
{
    this->left = new QListWidget;
    this->right = new QWidget;

    QSplitter *splitter = new QSplitter;
    splitter->addWidget(this->left);
    splitter->addWidget(this->right);

    QHBoxLayout *hBox = new QHBoxLayout(this);
    hBox->addWidget(splitter);

    QVBoxLayout *vBox = new QVBoxLayout(right);
    vBox->setMargin(0);
    vBox->addWidget(output = new QTextBrowser, 2);
    vBox->addWidget(input = new QTextEdit, 1);
    vBox->addWidget(send = new QPushButton("send"));

    this->socket = new QTcpSocket;
    this->socket->bind(QHostAddress("192.168.213.141"));
    this->socket->connectToHost(QHostAddress("192.168.213.141"), 8888);

    this->connect(this->socket, &QTcpSocket::readyRead, this, &MainWnd::onReadyRead);
    this->connect(this->send, &QPushButton::clicked, this, &MainWnd::onSend);
}

void MainWnd::onReadyRead()
{
    while (this->socket->bytesAvailable() > 0)
    {
        if(this->buf.size() < 4)
        {
            this->buf += this->socket->read(4 - this->buf.size());
            if(4 == this->buf.size())
            {
                this->packetSize = qFromBigEndian(*(uint32_t *)buf.data());
                qDebug()<<"packet size is"<<this->packetSize;
                this->readData();
            }
        }
        else
            this->readData();

        if(buf.size() == 4 + this->packetSize)
        {
            this->buf = this->buf.mid(4);
            qDebug()<<this->buf;

            int index = buf.indexOf("|");
            QString ip;
            if(-1 == index)
            {
                ip = this->buf;
                this->left->addItem(ip);
            }
            else
            {
                QString msg;
                ip = this->buf.left(index);
                msg = this->buf.mid(index + 1);
                this->output->append(ip + "say:" + msg);
            }
        }
        this->buf.clear();
    }
}

void MainWnd::onSend()
{
    if(0 == this->input->toPlainText())
        return;
    if(NULL == this->left->currentItem())
        return;
    QListWidgetItem *item = this->left->currentItem();
    QString ip = item->text();

    QByteArray buf = ip.toUtf8() + this->input->toPlainText().toUtf8();
    uint32_t size = qToBigEndian(buf.size());

    this->socket->write((const char *)&size, 4);
    this->socket->write(buf);

    this->output->append("I say:" + this->input->toPlainText());
    this->input->clear();
}

void MainWnd::readData()
{
    this->buf += this->socket->read(4 + this->packetSize - buf.size());
}
