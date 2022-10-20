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
    
    MessageForm(QWidget* parent = nullptr );
    ~MessageForm();

    QString SplitStrToFitIn(QString str);
    qreal TextWidth(QString& str);
    int TextHeight(QString& str);
    void ResizeForm();
    void Anchor();
    void writeMessageIntoForm(Message<msg_type> msg);
    void DrawRoundEdges();
    
    QString ConvertFromWcharArray(std::array<wchar_t, 256> arr);
private:

    Ui::MessageForm* ui;
    QPoint positionOnForm;
    QPoint BodyAnchor;
    QPoint TimeLabelAnchor;
    
};
#endif 