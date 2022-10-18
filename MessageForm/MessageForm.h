#pragma once
#ifndef MESSAGEFORM_H
#define MESSAGEFORM_H
#include <QtWidgets/QWidget>
#include <Message.h>
#include <Message_types.h>
#include <Queue.h>
#include <QStyle>
#include <QLabel>
namespace Ui { class MessageForm; }
using namespace Messanger;
class MessageForm : public QWidget, public Message<msg_type>
{
    Q_OBJECT

public:
    
    MessageForm(QPoint position, QWidget* parent = nullptr );
    ~MessageForm();

    QRect Rect(QLabel* Body_ptr);

    void ResizeForm();
    void Anchor();
    void writeMessageIntoForm(Message<msg_type> msg);

    
    QString ConvertFromWcharArray(std::array<wchar_t, 256> arr);
private:

    Ui::MessageForm* ui;
    QPoint positionOnForm;
    QPoint BodyAnchor;
    QPoint TimeLabelAnchor;
    
};
#endif 