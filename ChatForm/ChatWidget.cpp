#include "ChatWidget.h"
#include "ui_ChatWidget.h"
#include <QVBoxLayout>
#include <qevent.h>
#include <qthread.h>
#include <qmutex.h>
ChatWidget::ChatWidget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::ChatWidget)
{
  
    ui->setupUi(this);
    ui->frame->setFixedSize(400, 400);

    this->setAutoFillBackground(true);

    
    
    MessagePosition = QPoint(side_mergin,side_mergin);
    QObject::connect(ui->Send, &QPushButton::clicked, this, &ChatWidget::writeMessage);
    QObject::connect(this, &ChatWidget::incomingMessage, this, &ChatWidget::readMessage);
}
ChatWidget::~ChatWidget()
{
    this->disconnect();
    delete ui;
}

void ChatWidget::waitForMessage()
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

void ChatWidget::readMessage()
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
void ChatWidget::updateMessagePosition(Message<msg_type> msg, MessageWidget* MsgForm)
{
    int points_to_move_down= 10;
    if(!allMessageWidgets.empty())
        points_to_move_down = allMessageWidgets.back()->height();

        
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
void ChatWidget::getNewMessage(Message<msg_type> msg)
{
    
    MessageWidget* messageWidget = new MessageWidget(ui->frame);
    updateMessagePosition(msg, messageWidget);

    messageWidget->setAutoFillBackground(true);
    messageWidget->ResizeForm();
    messageWidget->writeMessageIntoForm(msg);


    allMessageWidgets.push_back(messageWidget);
}

void ChatWidget::writeMessage()
{
    Message<msg_type> msg;
    msg.header.reciever_name = ConvertFromQString(ui->RemoteClientName->text());
    msg.header.sender_name = user_name;
    

    MessageWidget* messageWidget = new MessageWidget(ui->frame);
    
    messageWidget->setAutoFillBackground(true);
    
    user_name = ConvertFromQString(ui->Nickname->text());
    
    
    QString BodyData = ui->MessageText->text();
    msg.body = ConvertFromQString(BodyData);
    messageWidget->writeMessageIntoForm(msg);
    messageWidget->ResizeForm();
    updateMessagePosition(msg, messageWidget);

    allMessageWidgets.push_back(messageWidget);
    send_msg(msg);
    
}




void ChatWidget::Ping_server(Message<msg_type> msg)
{
    //Messanger::Message<msg_type> msg;
    msg.header.id = msg_type::ServerPing;
    //msg.header.sender_name = user_name;
    send(msg);
}

void ChatWidget::Message_all(Message<msg_type> msg)
{
    //Messanger::Message<msg_type> msg;
    msg.header.id = msg_type::MessageAll;
    //msg.header.sender_name = user_name;
    send(msg);
}

void ChatWidget::join_server()
{
    Message<msg_type> msg;
    user_name = ConvertFromQString(ui->Nickname->text());
    
    msg.header.sender_name = user_name;
    msg.header.id = msg_type::JoinServer;
    send(msg);
}

void ChatWidget::setUserLogin(QString login)
{
    ui->Nickname->setText(login);
}

void ChatWidget::send_msg(Message<msg_type> msg)
{
    //Messanger::Message<msg_type> msg;
    msg.header.id = msg_type::PassString;
    //msg.header.sender_name = user_name;
    //for (unsigned int i{}, j{}; j < __data.size(); ++i, ++j)
    //    msg.body[i] = __data[j];

    send(msg);
}

std::array<wchar_t, 256> ChatWidget::ConvertFromQString(QString qstr)
{
    std::string temp_str = qstr.toUtf8().constData();
    std::array<wchar_t, 256> body_content{};
    for (int i = 0; i < temp_str.length(); i++)
    {
        body_content[i] = wchar_t(temp_str[i]);
    }
    return body_content;
}
void ChatWidget::accepted()
{
    QLabel* Connection_status = new QLabel(this);
    Connection_status->setText("Connected");
    Connection_status->move(120, 10);
    Connection_status->setAutoFillBackground(true);
    Connection_status->show();
}