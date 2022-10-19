#pragma once
#ifndef CHATFORM_H
#define CHATFORM_H
#include <QtWidgets/QWidget>
#include <qwaitcondition.h>
#include <Message.h>
#include <Message_types.h>
#include "../MessageForm/MessageForm.h"
#include "Client_interface.h"

using namespace Messanger;
namespace Ui { class ChatForm; }
class ChatForm : public QWidget, public client_interface<msg_type>
{
    Q_OBJECT

public:
    ChatForm(QWidget *parent = nullptr);
    ~ChatForm();
    void waitForMessage();
    void readMessage(/*Message<msg_type> msg*/);
    void writeMessage();
    void join_server();

    void updateMessagePosition(Message<msg_type> msg, MessageForm* MsgForm);
    void Ping_server(Message<msg_type> msg);
    void Message_all(Message<msg_type> msg);
    void getNewMessage(Message<msg_type> msg);
    void send_msg(Message<msg_type> msg);
    std::array<wchar_t, 256> ConvertFromQString(QString qstr);
    
    void accepted();
signals:
    void incomingMessage();
    
private:
    Ui::ChatForm* ui;
    std::vector<MessageForm *> allMessageForms;
    std::array<wchar_t, 256> user_name = {};
    QPoint MessagePosition;
    int side_mergin = 10;
};
#endif
