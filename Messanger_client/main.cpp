#include "MessengerForm.h"
#include "ChatWidget.h"
#include <QtWidgets/QApplication>
#define _CRT_SECURE_NO_WARNINGS




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    
    MessengerForm* ms = new MessengerForm();
    
    
    ms->show();

    std::thread th([&]() { ms->chat->waitForMessage(); });
    return a.exec();
    
    
}
