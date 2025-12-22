#include "chatclient.h"
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>

ChatClient::ChatClient(QObject *parent)
    : QObject{parent}
{
    m_ClientSocket=new QTcpSocket(this);
    connect(m_ClientSocket,&QTcpSocket::connected,this,&ChatClient::connected);
    connect(m_ClientSocket,&QTcpSocket::readyRead,this,&ChatClient::onReadyRead);
}

void ChatClient::onReadyRead()
{
    QByteArray jsonData;
    QDataStream socketStream(m_ClientSocket);
    socketStream.setVersion(QDataStream::Qt_5_12);
    for(;;){
        socketStream.startTransaction();
        socketStream>>jsonData;
        if(socketStream.commitTransaction()){
            //emit messageReceived(QString::fromUtf8(jsonData));
            //sendMessage("I recieved messge");

            QJsonParseError parseError;
            const QJsonDocument jsonDoc=QJsonDocument::fromJson(jsonData,&parseError);
            if(parseError.error==QJsonParseError::NoError){
                if(jsonDoc.isObject()){
                    //emit logMessage(QJsonDocument(jsonDoc).toJson(QJsonDocument::Compact));
                    emit jsonReceived(jsonDoc.object());
                }
            }
        }
        else{
            break;
        }
    }
}

void ChatClient::sendMessage(const QString &text, const QString &type)
{
    if(m_ClientSocket->state()!=QAbstractSocket::ConnectedState)
        return;
    if(!text.isEmpty()){
        QDataStream serverStream(m_ClientSocket);
        serverStream.setVersion(QDataStream::Qt_5_12);

        QJsonObject message;
        message["type"]=type;
        message["text"]=text;

        serverStream<<QJsonDocument(message).toJson();
    }
}

void ChatClient::connectToServer(const QHostAddress &address, quint16 port)
{
    m_ClientSocket->connectToHost(address,port);
}

void ChatClient::disconnectFromHost()
{
    m_ClientSocket->disconnectFromHost();
}
