#pragma once
#ifndef MessageWidget_H
#define MessageWidget_H
#include <QtWidgets/QWidget>
#include <Message.h>
#include <Message_types.h>
#include <Queue.h>
#include <QStyle>
#include <QLabel>
namespace Ui { class MessageWidget; }
using namespace Messanger;
class MessageWidget : public QWidget, public Message<msg_type>
{
    Q_OBJECT

public:
    
    MessageWidget(QWidget* parent = nullptr );
    ~MessageWidget();

    QString SplitStrToFitIn(QString str);
    qreal TextWidth(QString& str);
    int TextHeight(QString& str);
    void ResizeForm();
    void Anchor();
    void writeMessageIntoForm(Message<msg_type> msg);
    void DrawRoundEdges();
    
    QString ConvertFromWcharArray(std::array<wchar_t, 256> arr);
private:

    Ui::MessageWidget* ui;
    QPoint positionOnForm;
    QPoint BodyAnchor;
    QPoint TimeLabelAnchor;
    
};
#endif 