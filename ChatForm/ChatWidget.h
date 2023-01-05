#pragma once
#ifndef ChatWidget_H
#define ChatWidget_H
#include <QtWidgets/QWidget>

#include <Message.h>
#include <Message_types.h>
#include "../MessageWidget/MessageWidget.h"
#include "Client_interface.h"

using namespace Messanger;
QT_BEGIN_NAMESPACE
namespace Ui { class ChatWidget; }
QT_END_NAMESPACE

class ChatWidget : public QWidget, public client_interface<msg_type>
{
    Q_OBJECT

public:
    ChatWidget(QWidget *parent = nullptr);
    ~ChatWidget();
    void waitForMessage();
    void readMessage(/*Message<msg_type> msg*/);
    void writeMessage();
    void join_server();
    void setUserLogin(QString login);
    void updateMessagePosition(Message<msg_type> msg, MessageWidget* MsgForm);
    void Ping_server(Message<msg_type> msg);
    void Message_all(Message<msg_type> msg);
    void getNewMessage(Message<msg_type> msg);
    void send_msg(Message<msg_type> msg);
    std::array<wchar_t, 256> ConvertFromQString(QString qstr);
    
    void accepted();
signals:
    void incomingMessage();
    
private:
    Ui::ChatWidget* ui;
    std::vector<MessageWidget *> allMessageWidgets;
    std::array<wchar_t, 256> user_name = {};
    QPoint MessagePosition;
    int side_mergin = 10;
};
#endif
