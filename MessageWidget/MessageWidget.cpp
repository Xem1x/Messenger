#include "MessageWidget.h"
#include "ui_messagewidget.h"
#include <QRegularExpression>
#include <QPainter>
MessageWidget::MessageWidget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::MessageWidget)
{
    ui->setupUi(this);
    
    
    
    this->setAutoFillBackground(true);
    this->show();
    this->setMaximumWidth(300);
    this->setMinimumWidth(50);
    ui->Body->setMaximumWidth(280);
}

MessageWidget::~MessageWidget()
{
    delete ui;
}
qreal MessageWidget::TextWidth(QString& str)
{
    QFontMetricsF FM(ui->Body->font());
    QRectF rect = FM.boundingRect(str);

    return rect.width();
}
int MessageWidget::TextHeight(QString& str)
{
    QFontMetricsF FM(ui->Body->font());
    QRectF rect = FM.boundingRect(str);
    int line_count = str.count("\n") + 2;
    
    return line_count * rect.height() + 10;

}
QString MessageWidget::SplitStrToFitIn(QString str)
{
    QStringList wordList = str.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    QString splitedStr = {};
    QString currentLine = {};
    int lineWidth;
    int nextWordWidth;

    
    for(int elementPos = 0; elementPos < wordList.size(); elementPos++)
    {
        lineWidth = TextWidth(currentLine);
        nextWordWidth = TextWidth(wordList[elementPos]);
        if ((lineWidth + nextWordWidth) > ui->Body->maximumWidth())
        {
            splitedStr += currentLine;
            splitedStr += "\n";
            currentLine = {};

        }
        currentLine += wordList[elementPos];
        currentLine += " ";
    }
    splitedStr += currentLine;
    return splitedStr;
}

void MessageWidget::DrawRoundEdges() {
    const QRect rect(QPoint(0, 0), this->geometry().size());
    QBitmap b(rect.size());
    b.fill(QColor(Qt::color0));
    QPainter painter(&b);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::color1);

    painter.drawRoundedRect(rect, 5, 5);
    painter.end();
    this->setMask(b);
}


void MessageWidget::ResizeForm()
{
    QString BodyContent = ui->Body->text();
    int rectWidth = TextWidth(BodyContent);

    if (rectWidth > ui->Body->width())
    {
        if (rectWidth <= ui->Body->maximumWidth())
        {
            
            ui->Body->resize(rectWidth, ui->Body->height());
            int newFormWidth = ui->Body->width() + 20;
            int newFormHeight = ui->Body->height() + 20 + ui->Time->height();

            this->resize(newFormWidth, newFormHeight);

        }
        else {
            QString splited_to_lines = SplitStrToFitIn(ui->Body->text());
            ui->Body->setText(splited_to_lines);
            ui->Body->resize(TextWidth(BodyContent), TextHeight(BodyContent)+10);
            int newFormWidth = ui->Body->width() + 20;
            int newFormHeight = ui->Body->height() + 20 + ui->Time->height();

            this->resize(newFormWidth, newFormHeight);
        }
    }
    
    DrawRoundEdges();
    
    Anchor();
}


void MessageWidget::Anchor()
{
    BodyAnchor = QPoint(10 , 10);
    ui->Body->move(BodyAnchor);
    TimeLabelAnchor = QPoint(this->width() - 60, this->height() - 20);
    ui->Time->move(TimeLabelAnchor);
}

void MessageWidget::writeMessageIntoForm(Message<msg_type> msg)
{


    QString recivedMessageBody = ConvertFromWcharArray(msg.body);
    ui->Body->setText(recivedMessageBody);
    
    //QString recivedMessageTime = ConvertFromWcharArray(msg.time);
    //ui->Time->setText(recivedMessageTime);
}



QString MessageWidget::ConvertFromWcharArray(std::array<wchar_t, 256> arr)
{
    std::string temp_str;

    for (int i = 0; i < arr.size(); i++)
    {
        temp_str += arr[i];
    }

    QString body_content = QString::fromUtf8(temp_str);
    return body_content;
}



