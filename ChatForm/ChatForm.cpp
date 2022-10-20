#include "ChatForm.h"
#include "ui_chatform.h"
#include <QVBoxLayout>
#include <qevent.h>
#include <qthread.h>
#include <qmutex.h>
ChatForm::ChatForm(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::ChatForm)
{
  
    ui->setupUi(this);
    ui->frame->setFixedSize(400, 400);

    this->setAutoFillBackground(true);
    this->show();
    
    
    MessagePosition = QPoint(side_mergin,side_mergin);
    QObject::connect(ui->Send, &QPushButton::clicked, this, &ChatForm::writeMessage);
    QObject::connect(this, &ChatForm::incomingMessage, this, &ChatForm::readMessage);
}
ChatForm::~ChatForm()
{
    this->disconnect();
    delete ui;
}

void ChatForm::waitForMessage()
{
    bool quit = false;
    while (!quit)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        if (this->is_connected() && !this->get_in_comming().empty())
        {
            emit incomingMessage();
        }
    }
}

void ChatForm::readMessage()
{
    auto msg = this->get_in_comming().pop_front().msg;
    switch (msg.header.id) {
    case msg_type::ServerAccept: {
        this->accepted();
        break;
    }
    case msg_type::PassString: {
        this->getNewMessage(msg);
        break;
    }
    }
}
void ChatForm::updateMessagePosition(Message<msg_type> msg, MessageForm* MsgForm)
{
    int points_to_move_down= 10;
    if(!allMessageForms.empty())
        points_to_move_down = allMessageForms.back()->height();

        
    MessagePosition.setX(side_mergin);
    if (msg.header.sender_name == user_name)
    {
        int msg_form_x_point = ui->frame->width() - MsgForm->width() - 2 * side_mergin;
        MessagePosition += QPoint( msg_form_x_point, points_to_move_down+6);
        
    }
    else
    {
        MessagePosition += QPoint(0, points_to_move_down + 6);
    }
    MsgForm->move(MessagePosition);
}
void ChatForm::getNewMessage(Message<msg_type> msg)
{
    
    MessageForm* messageForm = new MessageForm(ui->frame);
    updateMessagePosition(msg, messageForm);

    messageForm->setAutoFillBackground(true);
    messageForm->ResizeForm();
    messageForm->writeMessageIntoForm(msg);


    allMessageForms.push_back(messageForm);
}

void ChatForm::writeMessage()
{
    Message<msg_type> msg;
    msg.header.reciever_name = ConvertFromQString(ui->RemoteClientName->text());
    msg.header.sender_name = user_name;
    

    MessageForm* messageForm = new MessageForm(ui->frame);
    
    messageForm->setAutoFillBackground(true);
    
    user_name = ConvertFromQString(ui->Nickname->text());
    
    
    QString BodyData = ui->MessageText->text();
    msg.body = ConvertFromQString(BodyData);
    messageForm->writeMessageIntoForm(msg);
    messageForm->ResizeForm();
    updateMessagePosition(msg, messageForm);

    allMessageForms.push_back(messageForm);
    send_msg(msg);
    
}




void ChatForm::Ping_server(Message<msg_type> msg)
{
    //Messanger::Message<msg_type> msg;
    msg.header.id = msg_type::ServerPing;
    //msg.header.sender_name = user_name;
    send(msg);
}

void ChatForm::Message_all(Message<msg_type> msg)
{
    //Messanger::Message<msg_type> msg;
    msg.header.id = msg_type::MessageAll;
    //msg.header.sender_name = user_name;
    send(msg);
}

void ChatForm::join_server()
{
    Message<msg_type> msg;
    user_name = ConvertFromQString(ui->Nickname->text());
    
    msg.header.sender_name = user_name;
    msg.header.id = msg_type::JoinServer;
    send(msg);
}

void ChatForm::send_msg(Message<msg_type> msg)
{
    //Messanger::Message<msg_type> msg;
    msg.header.id = msg_type::PassString;
    //msg.header.sender_name = user_name;
    //for (unsigned int i{}, j{}; j < __data.size(); ++i, ++j)
    //    msg.body[i] = __data[j];

    send(msg);
}

std::array<wchar_t, 256> ChatForm::ConvertFromQString(QString qstr)
{
    std::string temp_str = qstr.toUtf8().constData();
    std::array<wchar_t, 256> body_content{};
    for (int i = 0; i < temp_str.length(); i++)
    {
        body_content[i] = wchar_t(temp_str[i]);
    }
    return body_content;
}
void ChatForm::accepted()
{
    QLabel* Connection_status = new QLabel(this);
    Connection_status->setText("Connected");
    Connection_status->move(120, 10);
    Connection_status->setAutoFillBackground(true);
    Connection_status->show();
}