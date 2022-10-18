#include "MessageForm.h"
#include "ui_messageform.h"

MessageForm::MessageForm(QPoint position, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::MessageForm)
{
    ui->setupUi(this);
    
    this->move(position);
    this->setAutoFillBackground(true);
    this->show();
    this->setMaximumWidth(300);
    this->setMinimumWidth(50);
}

MessageForm::~MessageForm()
{
    delete ui;
}

void MessageForm::ResizeForm()
{
    //QLabel* Body_ptr = ui->Body;


    //QString text = Body_ptr->text();
    //QRect rectLbl = Rect(Body_ptr);
    //QFont font = Body_ptr->font();


    //int size = font.pointSize();
    //QFontMetrics fontMetrics(font);
    //QRect rect = fontMetrics.boundingRect(rectLbl,
    //    Qt::TextWordWrap, text);


    //// decide whether to increase or decrease
    //int step = rect.height() > rectLbl.height() ? -1 : 1;

    //// iterate until text fits best into rectangle of label


    //for (int i = 0; i < 50; i++) {
    //    font.setPointSize(size + step);
    //    QFontMetrics fontMetrics(font);
    //    rect = fontMetrics.boundingRect(rectLbl,
    //        Qt::TextWordWrap, text);
    //    if (size <= 1) {
    //        
    //        break;
    //    }
    //    if (step < 0) {
    //        size += step;
    //        if (rect.height() < rectLbl.height()) break;
    //    }
    //    else {
    //        if (rect.height() > rectLbl.height()) break;
    //        size += step;
    //    }
    //}
    //// apply result of iteration
    //font.setPointSize(size);
    //setFont(font);



    ///*int newFormWidth = ui->Body->width() + 20;
    //int newFormHeight = ui->Body->height() + 20 + ui->Time->height();

    //this->resize(newFormWidth, newFormHeight);*/

    Anchor();
}
QRect MessageForm::Rect(QLabel* Body_ptr)
{
    
    QRect rect = contentsRect();
    int m = Body_ptr->margin(); 
    rect.adjust(m, m, -m, -m);
    const int align = QStyle::visualAlignment(Body_ptr->layoutDirection(), Body_ptr->QLabel::alignment());
    int i = Body_ptr->indent();
    if (i < 0 && Body_ptr->frameWidth()) { // no indent, but we do have a frame
        m = fontMetrics().horizontalAdvance(QLatin1Char('x')) / 2 - m;
    }
    if (m > 0) {
        if (align & Qt::AlignLeft) rect.setLeft(rect.left() + m);
        if (align & Qt::AlignRight) rect.setRight(rect.right() - m);
        if (align & Qt::AlignTop) rect.setTop(rect.top() + m);
        if (align & Qt::AlignBottom) rect.setBottom(rect.bottom() - m);
    }
    return rect;
}

void MessageForm::Anchor()
{
    BodyAnchor = QPoint(10 , 10);
    ui->Body->move(BodyAnchor);
    TimeLabelAnchor = QPoint(this->width() - 60, this->height() - 20);
    ui->Time->move(TimeLabelAnchor);
}

void MessageForm::writeMessageIntoForm(Message<msg_type> msg)
{


    QString recivedMessageBody = ConvertFromWcharArray(msg.body);
    ui->Body->setText(recivedMessageBody);
    
    //QString recivedMessageTime = ConvertFromWcharArray(msg.time);
    //ui->Time->setText(recivedMessageTime);
}



QString MessageForm::ConvertFromWcharArray(std::array<wchar_t, 256> arr)
{
    std::string temp_str;

    for (int i = 0; i < arr.size(); i++)
    {
        temp_str += arr[i];
    }

    QString body_content = QString::fromUtf8(temp_str);
    return body_content;
}



