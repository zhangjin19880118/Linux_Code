#include "MyServer.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#if 0
{
    cmd:"register",
    result:"ok"
}

{
    cmd:"login",
    result:"ok"
}
#endif

MyServer::MyServer(QObject *parent) : QObject(parent)
{
    this->server.listen(QHostAddress::Any, 8888);

    this->connect(&this->server, &HttpServer::requestReady,
                  this, &MyServer::onRequestReady);
}

void MyServer::onRequestReady(HttpServerRequest &request, HttpServerResponse &response)
{
#if 0
    qDebug() << request.url().path();
    //qDebug() << request.httpVersion();
    qDebug() << request.method();

    response.writeHead(HttpResponseStatus::OK);//200
    //response.writeHead(HttpResponseStatus::NOT_FOUND);//404
    //response.write("hello");
    //response.write("world");
    response.end("<h1>hello world</h1>");
#endif

    if("POST" == request.method())
    {
        this->connect(&request, &HttpServerRequest::end, [&](){
            this->handlePostData(request, response);
        });
    }
}

void MyServer::handlePostData(HttpServerRequest &request, HttpServerResponse &response)
{
    //qDebug() << request.readBody();
    QByteArray body = request.readBody();
    qDebug() << body;

    QJsonDocument doc = QJsonDocument::fromJson(body);
    QJsonObject root = doc.object();
    QString cmd = root.value("cmd").toString();
    QString username = root.value("username").toString();
    QString password = root.value("password").toString();

    response.writeHead(HttpResponseStatus::OK);//200

    if(cmd == "register")
    {
        QJsonObject obj;
        obj.insert("cmd", "register");
        obj.insert("result", "ok");

        QJsonDocument doc(obj);
        QByteArray respBuf = doc.toJson();

        response.end(respBuf);
        return;
    }
    else if(cmd == "login")
    {
        QJsonObject obj;
        obj.insert("cmd", "login");
        obj.insert("result", "ok");

        QJsonDocument doc(obj);
        QByteArray respBuf = doc.toJson();

        response.end(respBuf);
        return;
    }

    response.end("cmd not found!\n");
}
