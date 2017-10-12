#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <tufao-1/Tufao/httpserver.h>
#include <tufao-1/Tufao/httpserverrequest.h>
#include <tufao-1/Tufao/httpserverresponse.h>
#include <QUrl>
#include <QDebug>
using namespace Tufao;

class MyServer : public QObject
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = 0);

    HttpServer server;
    void onRequestReady(Tufao::HttpServerRequest &request, Tufao::HttpServerResponse &response);
    void handlePostData(Tufao::HttpServerRequest &request, Tufao::HttpServerResponse &response);
signals:

public slots:
};

#endif // MYSERVER_H
