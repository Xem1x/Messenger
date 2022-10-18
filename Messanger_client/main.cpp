#include <ChatForm.h>
#include <qthread.h>

#include <QtWidgets/QApplication>
#define _CRT_SECURE_NO_WARNINGS




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    

    ChatForm* chat = new ChatForm();
    
    
    chat->show();
    
    chat->connect_to("127.0.0.1", 25565);
    chat->join_server();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    if (chat->is_connected() && !chat->get_in_comming().empty()) {
        auto msg = chat->get_in_comming().pop_front().msg;
        
        switch (msg.header.id) {
        case msg_type::ServerAccept: {
            
            chat->accepted();
            break;
        }
        }
    }
    std::thread th([&]() { chat->waitForMessage(); });
    
    return a.exec();
    
    
}
