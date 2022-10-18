#include "Server_interface.h"

#include <Message_types.h>
#define _CRT_SECURE_NO_WARNINGS
using namespace Messanger;

namespace server_detail {

    class Server : public Server_interface<msg_type>
    {
    public:
        Server(uint16_t port)
            : Server_interface<msg_type>(port)
            {
            
            }
    

    protected:
        virtual bool onClientConnect(std::shared_ptr<connection<msg_type>> client) override
        {
            std::cout << "client " << client << std::endl;

            Message<msg_type> msg;
            msg.header.id = msg_type::ServerAccept;
            
            client->Send(msg);
            return true;
        }
        virtual void onClientDisconnect(std::shared_ptr<connection<msg_type>> client) override
        {
            std::cout << "Removing client [" << client->get_id() << "] \n";
        }

        virtual void onMessage(std::shared_ptr<connection<msg_type>> client, Message<msg_type>& msg) override
        {
            
            switch (msg.header.id) {
            case msg_type::ServerPing: {
                std::wcout << "[" << msg.header.sender_name.data() << "]: Ping the server\n";
                client->Send(msg);
                break;
            }

            case msg_type::MessageAll: {
                std::wcout << "[" << msg.header.sender_name.data() << "]: Send the message to all user\n";

                //Construct a new message and send it to all clients
                Messanger::Message<msg_type> __msg;
                __msg.header.id = msg_type::ServerMessage;
                __msg.header.sender_name = msg.header.sender_name;
                message_all_clients(__msg, client);
                break;
            }

            case msg_type::JoinServer: {
                client->set_user_name(msg.header.sender_name);
                std::wcout << "[" << msg.header.sender_name.data() << "] Join the server\n";
                break;
            }

            case msg_type::PassString: {

                std::wcout << "[" << msg.header.sender_name.data() << "]: " << msg.body.data() << '\n';
                auto temp = getRecieverClient(msg);
                messageClient(temp, msg);
                break;
            }
            }
        }
        std::shared_ptr<connection<msg_type>> getRecieverClient(Message<msg_type> msg)// 
         {
            auto connection_queue = get_connection_queue();
            for (int i = 0; i < get_conn_queue_size(); i++)
            {
                if (connection_queue[i]->get_user_name() == msg.header.reciever_name)
                {
                    return connection_queue[i];
                }
            }
            return NULL;
        }
    };










}

int main()
{
    using namespace server_detail;
    Server server(25565);
    server.start_server();

    while (true) {
        server.update(-1, true);
    }

    return 0;
}
